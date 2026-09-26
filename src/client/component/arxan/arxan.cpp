#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "component/game_module.hpp"
#include "component/scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#define PRECOMPUTED_INTEGRITY_CHECKS
#define PRECOMPUTED_BREAKPOINTS
#define PRECOMPUTED_ILLEGAL_INSTRUCTIONS

#ifdef PRECOMPUTED_INTEGRITY_CHECKS
#include "integrity.hpp"
#endif
#ifdef PRECOMPUTED_BREAKPOINTS
#include "breakpoints.hpp"
#endif
#ifdef PRECOMPUTED_ILLEGAL_INSTRUCTIONS
#include "illegal_instructions.hpp"
#endif

#define ProcessDebugPort 7
#define ProcessDebugObjectHandle 30
#define ProcessDebugFlags 31

namespace arxan
{
	namespace integrity
	{
		const std::vector<std::pair<uint8_t*, size_t>>& get_text_sections()
		{
			static const std::vector<std::pair<uint8_t*, size_t>> text = []
			{
				std::vector<std::pair<uint8_t*, size_t>> texts{};

				const utils::nt::library game{ game_module::get_game_module() };
				for (const auto& section : game.get_section_headers())
				{
					if (section->Characteristics & IMAGE_SCN_MEM_EXECUTE)
					{
						texts.emplace_back(game.get_ptr() + section->VirtualAddress, section->Misc.VirtualSize);
					}
				}

				return texts;
			}();

			return text;
		}

		bool is_in_texts(const uint64_t addr)
		{
			const auto& texts = get_text_sections();
			for (const auto& text : texts)
			{
				const auto start = reinterpret_cast<ULONG_PTR>(text.first);
				if (addr >= start && addr <= (start + text.second))
				{
					return true;
				}
			}

			return false;
		}

		bool is_in_texts(const void* addr)
		{
			return is_in_texts(reinterpret_cast<uint64_t>(addr));
		}

		struct integrity_handler_context
		{
			uint32_t* computed_checksum;
			uint32_t* original_checksum;
		};

		bool is_on_stack(uint8_t* stack_frame, const void* pointer)
		{
			const auto stack_value = reinterpret_cast<uint64_t>(stack_frame);
			const auto pointer_value = reinterpret_cast<uint64_t>(pointer);

			const auto diff = static_cast<int64_t>(stack_value - pointer_value);
			return std::abs(diff) < 0x1000;
		}

		// Pretty trashy, but working, heuristic to search the integrity handler context
		bool is_handler_context(uint8_t* stack_frame, const uint32_t computed_checksum, const uint32_t frame_offset)
		{
			const auto* potential_context = reinterpret_cast<integrity_handler_context*>(stack_frame + frame_offset);
			return is_on_stack(stack_frame, potential_context->computed_checksum)
				&& *potential_context->computed_checksum == computed_checksum
				&& is_in_texts(potential_context->original_checksum);
		}

		integrity_handler_context* search_handler_context(uint8_t* stack_frame, const uint32_t computed_checksum)
		{
			for (uint32_t frame_offset = 0; frame_offset < 0x90; frame_offset += 8)
			{
				if (is_handler_context(stack_frame, computed_checksum, frame_offset))
				{
					return reinterpret_cast<integrity_handler_context*>(stack_frame + frame_offset);
				}
			}

			return nullptr;
		}

		uint32_t adjust_integrity_checksum(const uint64_t return_address, uint8_t* stack_frame,
			const uint32_t current_checksum)
		{
			[[maybe_unused]] const auto handler_address = return_address - 5;
			const auto* context = search_handler_context(stack_frame, current_checksum);

			if (!context)
			{
				//OutputDebugStringA(utils::string::va("Unable to find frame offset for: %llX", return_address));
				return current_checksum;
			}

			const auto correct_checksum = *context->original_checksum;
			*context->computed_checksum = correct_checksum;

			/*
			if (current_checksum != correct_checksum)
			{
				OutputDebugStringA(utils::string::va("Adjusting checksum (%llX): %X -> %X", handler_address,
					current_checksum, correct_checksum));
			}
			*/

			return correct_checksum;
		}

		void patch_intact_basic_block_integrity_check(void* address)
		{
			const auto game_address = reinterpret_cast<uint64_t>(address);
			constexpr auto inst_len = 3;

			const auto next_inst_addr = game_address + inst_len;
			const auto next_inst = *reinterpret_cast<uint32_t*>(next_inst_addr);

			if ((next_inst & 0xFF00FFFF) != 0xFF004583)
			{
				throw std::runtime_error(utils::string::va("Unable to patch intact basic block: %llX", game_address));
			}

			const auto other_frame_offset = static_cast<uint8_t>(next_inst >> 16);
			static const auto stub = utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.push(rax);

				a.mov(rax, qword_ptr(rsp, 8));
				a.sub(rax, 2); // Skip the push we inserted

				a.push(rax);
				a.pushad64();

				a.mov(r8, qword_ptr(rsp, 0x88));
				a.mov(rcx, rax);
				a.mov(rdx, rbp);
				a.call_aligned(adjust_integrity_checksum);

				a.mov(qword_ptr(rsp, 0x80), rax);

				a.popad64();
				a.pop(rax);

				a.add(rsp, 8);

				a.mov(dword_ptr(rdx, rcx, 4), eax);

				a.pop(rax); // return addr
				a.xchg(rax, qword_ptr(rsp)); // switch with push

				a.add(dword_ptr(rbp, rax), 0xFFFFFFFF);

				a.mov(rax, dword_ptr(rdx, rcx, 4)); // restore rax

				a.ret();
			});

			// push other_frame_offset
			utils::hook::set<uint16_t>(game_address, static_cast<uint16_t>(0x6A | (other_frame_offset << 8)));
			utils::hook::call(game_address + 2, stub);
		}

		void patch_split_basic_block_integrity_check(void* address)
		{
			const auto game_address = reinterpret_cast<uint64_t>(address);
			constexpr auto inst_len = 3;

			const auto next_inst_addr = game_address + inst_len;

			if (*reinterpret_cast<uint8_t*>(next_inst_addr) != 0xE9)
			{
				throw std::runtime_error(utils::string::va("Unable to patch split basic block: %llX", game_address));
			}

			const auto jump_target = utils::hook::extract<void*>(reinterpret_cast<void*>(next_inst_addr + 1));
			const auto stub = utils::hook::assemble([jump_target](utils::hook::assembler& a)
			{
				a.push(rax);

				a.mov(rax, qword_ptr(rsp, 8));
				a.push(rax);

				a.pushad64();

				a.mov(r8, qword_ptr(rsp, 0x88));
				a.mov(rcx, rax);
				a.mov(rdx, rbp);
				a.call_aligned(adjust_integrity_checksum);

				a.mov(qword_ptr(rsp, 0x80), rax);

				a.popad64();
				a.pop(rax);

				a.add(rsp, 8);

				a.mov(dword_ptr(rdx, rcx, 4), eax);

				a.add(rsp, 8);

				a.jmp(jump_target);
			});

			utils::hook::call(game_address, stub);
		}

#ifdef PRECOMPUTED_INTEGRITY_CHECKS
		void search_and_patch_integrity_checks_precomputed()
		{
			for (const auto i : intact_integrity_check_blocks)
			{
				patch_intact_basic_block_integrity_check(reinterpret_cast<void*>(i));
			}

			for (const auto i : split_integrity_check_blocks)
			{
				patch_split_basic_block_integrity_check(reinterpret_cast<void*>(i));
			}
		}
#endif

		void search_and_patch_integrity_checks()
		{
#ifdef PRECOMPUTED_INTEGRITY_CHECKS
			assert(game::base_address == 0x140000000);
			search_and_patch_integrity_checks_precomputed();
#else
			const auto intact_results = "89 04 8A 83 45 ? FF"_sig;
			const auto split_results = "89 04 8A E9"_sig;

			for (auto* i : intact_results)
			{
				patch_intact_basic_block_integrity_check(i);
			}

			for (auto* i : split_results)
			{
				patch_split_basic_block_integrity_check(i);
			}
#endif
		}
	}
	using namespace integrity;

	namespace anti_debug
	{
		utils::hook::detour nt_close_hook;
		utils::hook::detour nt_query_information_process_hook;

		NTSTATUS WINAPI nt_query_information_process_stub(const HANDLE handle, const PROCESSINFOCLASS info_class,
			const PVOID info,
			const ULONG info_length, const PULONG ret_length)
		{
			auto* orig = static_cast<decltype(NtQueryInformationProcess)*>(nt_query_information_process_hook.get_original());
			auto status = orig(handle, info_class, info, info_length, ret_length);

			if (NT_SUCCESS(status))
			{
				if (info_class == ProcessDebugObjectHandle)
				{
					*static_cast<HANDLE*>(info) = nullptr;
					return static_cast<LONG>(0xC0000353);
				}
				else if (info_class == ProcessDebugPort)
				{
					*static_cast<HANDLE*>(info) = nullptr;
				}
				else if (info_class == ProcessDebugFlags)
				{
					*static_cast<ULONG*>(info) = 1;
				}
			}

			return status;
		}

		NTSTATUS NTAPI nt_close_stub(const HANDLE handle)
		{
			char info[16]{};
			if (NtQueryObject(handle, OBJECT_INFORMATION_CLASS(4), &info, 2, nullptr) >= 0 && size_t(handle) != 0x12345)
			{
				auto* orig = static_cast<decltype(NtClose)*>(nt_close_hook.get_original());
				return orig(handle);
			}

			return STATUS_INVALID_HANDLE;
		}

		void hide_being_debugged()
		{
			auto* const peb = PPEB(__readgsqword(0x60));
			peb->BeingDebugged = false;
			*reinterpret_cast<PDWORD>(LPSTR(peb) + 0xBC) &= ~0x70; // NtGlobalFlag

			auto* const heap = *reinterpret_cast<PVOID*>(LPSTR(peb) + 0x30);
			if (heap != nullptr)
			{
				*reinterpret_cast<PDWORD>(LPSTR(heap) + 0x70) = 2;   // Force HEAP_GROWABLE only
				*reinterpret_cast<PDWORD>(LPSTR(heap) + 0x74) = 0;   // ForceFlags = 0
			}
		}

		void remove_hardware_breakpoints()
		{
			CONTEXT context;
			ZeroMemory(&context, sizeof(context));
			context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			auto* const thread = GetCurrentThread();
			GetThreadContext(thread, &context);

			context.Dr0 = 0;
			context.Dr1 = 0;
			context.Dr2 = 0;
			context.Dr3 = 0;
			context.Dr6 = 0;
			context.Dr7 = 0;

			SetThreadContext(thread, &context);
		}

		LONG WINAPI exception_filter(const LPEXCEPTION_POINTERS info)
		{
			if (info->ExceptionRecord->ExceptionCode == STATUS_INVALID_HANDLE)
			{
				return EXCEPTION_CONTINUE_EXECUTION;
			}

			return EXCEPTION_CONTINUE_SEARCH;
		}

		BOOL WINAPI set_thread_context_stub(const HANDLE thread, CONTEXT* context)
		{
			if (context->ContextFlags == CONTEXT_DEBUG_REGISTERS)
			{
				return TRUE;
			}

			return SetThreadContext(thread, context);
		}

		enum dbg_funcs_e
		{
			DbgBreakPoint,
			DbgUserBreakPoint,
			DbgUiConnectToDbg,
			DbgUiContinue,
			DbgUiConvertStateChangeStructure,
			DbgUiDebugActiveProcess,
			DbgUiGetThreadDebugObject,
			DbgUiIssueRemoteBreakin,
			DbgUiRemoteBreakin,
			DbgUiSetThreadDebugObject,
			DbgUiStopDebugging,
			DbgUiWaitStateChange,
			DbgPrintReturnControlC,
			DbgPrompt,
			DBG_FUNCS_COUNT,
		};
		const char* dbg_funcs_names[] =
		{
			"DbgBreakPoint",
			"DbgUserBreakPoint",
			"DbgUiConnectToDbg",
			"DbgUiContinue",
			"DbgUiConvertStateChangeStructure",
			"DbgUiDebugActiveProcess",
			"DbgUiGetThreadDebugObject",
			"DbgUiIssueRemoteBreakin",
			"DbgUiRemoteBreakin",
			"DbgUiSetThreadDebugObject",
			"DbgUiStopDebugging",
			"DbgUiWaitStateChange",
			"DbgPrintReturnControlC",
			"DbgPrompt",
		};
		struct dbg_func_bytes_s
		{
			std::uint8_t buffer[15];
		};
		dbg_func_bytes_s dbg_func_bytes[DBG_FUNCS_COUNT];
		void* dbg_func_procs[DBG_FUNCS_COUNT]{};

		void store_debug_functions()
		{
			const utils::nt::library ntdll("ntdll.dll");

			for (auto i = 0; i < DBG_FUNCS_COUNT; i++)
			{
				dbg_func_procs[i] = ntdll.get_proc<void*>(dbg_funcs_names[i]);
				memcpy(dbg_func_bytes[i].buffer, dbg_func_procs[i], sizeof(dbg_func_bytes[i].buffer));
			}
		}

		void restore_debug_functions()
		{
			for (auto i = 0; i < DBG_FUNCS_COUNT; i++)
			{
				utils::hook::copy(dbg_func_procs[i], dbg_func_bytes[i].buffer, sizeof(dbg_func_bytes[i].buffer));
			}
		}

		namespace exceptions
		{
			void patch_int2d_trap(const std::uint64_t address)
			{
				// int 2Dh / jmp continuation
				const auto continuation = utils::hook::extract<void*>(reinterpret_cast<void*>(address + 3));
				const auto stub = utils::hook::assemble([continuation](utils::hook::assembler& a)
				{
					a.mov(qword_ptr(rax), 1);
					a.jmp(continuation);
				});

				utils::hook::jump(address, stub);
			}

			void patch_illegal_instruction(const std::uint64_t address, void* continuation)
			{
				// lea rax, [rbp+x] / ud2, either directly or through a jmp
				const auto frame_offset = *reinterpret_cast<std::int8_t*>(address + 3);
				const auto stub = utils::hook::assemble([frame_offset, continuation](utils::hook::assembler& a)
				{
					a.lea(rax, ptr(rbp, frame_offset));
					a.mov(qword_ptr(rax), 1);
					a.jmp(continuation);
				});

				utils::hook::jump(address, stub);
			}

			void patch_illegal_instruction_intact(const std::uint64_t address)
			{
				patch_illegal_instruction(address, reinterpret_cast<void*>(address + 6));
			}

			void patch_illegal_instruction_split(const std::uint64_t address)
			{
				const auto ud2 = utils::hook::extract<std::uint8_t*>(reinterpret_cast<void*>(address + 5));
				if (*reinterpret_cast<std::uint16_t*>(ud2) != 0x0B0F)
				{
					return; // false positive
				}

				patch_illegal_instruction(address, ud2 + 2);
			}

			void patch_traps()
			{
#if defined(PRECOMPUTED_BREAKPOINTS) && defined(PRECOMPUTED_ILLEGAL_INSTRUCTIONS)
				assert(game::base_address == 0x140000000);

				for (const auto i : int2d_breakpoint_addresses)
				{
					patch_int2d_trap(i);
				}

				for (const auto i : illegal_instructions_intact)
				{
					patch_illegal_instruction_intact(i);
				}

				for (const auto i : illegal_instructions_split)
				{
					patch_illegal_instruction_split(i);
				}
#else
				const auto game = game_module::get_game_module();

				for (auto* i : utils::hook::signature("CD 2D E9 ? ? ? ?", game).process())
				{
					patch_int2d_trap(reinterpret_cast<std::uint64_t>(i));
				}

				for (auto* i : utils::hook::signature("48 8D 45 ? 0F 0B", game).process())
				{
					patch_illegal_instruction_intact(reinterpret_cast<std::uint64_t>(i));
				}

				for (auto* i : utils::hook::signature("48 8D 45 ? E9 ? ? ? ?", game).process())
				{
					patch_illegal_instruction_split(reinterpret_cast<std::uint64_t>(i));
				}
#endif
			}
		}
	}
	using namespace anti_debug;

	std::once_flag patch_once;

	bool should_patch_traps()
	{
		return !utils::nt::is_wine() || utils::nt::is_sogen();
	}

	void patch_code()
	{
		std::call_once(patch_once, []
		{
			search_and_patch_integrity_checks();

			if (should_patch_traps())
			{
				exceptions::patch_traps();
			}
		});
	}

	PVOID WINAPI add_vectored_exception_handler_stub(const ULONG first, const PVECTORED_EXCEPTION_HANDLER handler)
	{
		// a guard registers its handler right before executing its trap, which happens before post_unpack. patching here is intentional
		patch_code();
		return AddVectoredExceptionHandler(first, handler);
	}

	class component final : public component_interface
	{
	public:
		void* load_import(const std::string& library, const std::string& function) override
		{
			if (function == "SetThreadContext")
			{
				return set_thread_context_stub;
			}

			if (function == "AddVectoredExceptionHandler" && should_patch_traps())
			{
				return add_vectored_exception_handler_stub;
			}

			return nullptr;
		}

		void post_load() override
		{
			remove_hardware_breakpoints();
			hide_being_debugged();
			scheduler::loop(hide_being_debugged, scheduler::pipeline::async);
			if (!utils::nt::is_wine() || utils::nt::is_sogen())
			{
				store_debug_functions();
			}

			const utils::nt::library ntdll("ntdll.dll");
			nt_close_hook.create(ntdll.get_proc<void*>("NtClose"), nt_close_stub);

			const auto nt_query_information_process = ntdll.get_proc<void*>("NtQueryInformationProcess");
			nt_query_information_process_hook.create(nt_query_information_process, nt_query_information_process_stub);
			nt_query_information_process_hook.enable();
			nt_query_information_process_hook.move();

			AddVectoredExceptionHandler(1, exception_filter);
		}

		void post_unpack() override
		{
			remove_hardware_breakpoints();
			patch_code();

			if (!utils::nt::is_wine() || utils::nt::is_sogen())
			{
				restore_debug_functions();
			}
		}

		component_priority priority() override
		{
			return component_priority::arxan;
		}
	};
}

REGISTER_COMPONENT(arxan::component)
