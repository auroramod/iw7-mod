#pragma once

#define BINARY_PAYLOAD_SIZE 0x14000000

// Decide whether to load the game as lib or to inject it
#define INJECT_HOST_AS_LIB

#pragma warning(push)
#pragma warning(disable: 4100)
#pragma warning(disable: 4127)
#pragma warning(disable: 4244)
#pragma warning(disable: 4458)
#pragma warning(disable: 4702)
#pragma warning(disable: 4996)
#pragma warning(disable: 5054)
#pragma warning(disable: 5056)
#pragma warning(disable: 6011)
#pragma warning(disable: 6297)
#pragma warning(disable: 6385)
#pragma warning(disable: 6386)
#pragma warning(disable: 6387)
#pragma warning(disable: 26110)
#pragma warning(disable: 26451)
#pragma warning(disable: 26444)
#pragma warning(disable: 26451)
#pragma warning(disable: 26489)
#pragma warning(disable: 26495)
#pragma warning(disable: 26498)
#pragma warning(disable: 26812)
#pragma warning(disable: 28020)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <MsHTML.h>
#include <MsHtmHst.h>
#include <ExDisp.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <corecrt_io.h>
#include <fcntl.h>
#include <shellapi.h>
#include <csetjmp>
#include <ShlObj.h>
#include <winternl.h>
#include <VersionHelpers.h>
#include <Psapi.h>
#include <urlmon.h>
#include <atlbase.h>
#include <iphlpapi.h>
#include <wincrypt.h>

// min and max is required by gdi, therefore NOMINMAX won't work
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <map>
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include <regex>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <utility>
#include <filesystem>
#include <functional>
#include <sstream>
#include <optional>
#include <set>
#include <unordered_set>
#include <variant>
#include <random>
#include <numeric>

#include <gsl/gsl>
#include <udis86.h>
#include <MinHook.h>
#include <tomcrypt.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <asmjit/core/jitruntime.h>
#include <asmjit/x86/x86assembler.h>

#pragma warning(pop)
#pragma warning(disable: 4100)

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "urlmon.lib" )
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Crypt32.lib")

#include "resource.hpp"

using namespace std::literals;

#define assert_sizeof(__ASSET__, __SIZE__) static_assert(sizeof(__ASSET__) == __SIZE__)
#define assert_offsetof(__ASSET__, __VARIABLE__, __OFFSET__) static_assert(offsetof(__ASSET__, __VARIABLE__) == __OFFSET__)

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if defined(_WIN32) && defined(_WIN64)
#define TARGET_ARCHITECTURE "win64"
#elif defined(_WIN32)
#define TARGET_ARCHITECTURE "win32"
#elif defined(__linux__) && defined(__x86_64__)
#define TARGET_ARCHITECTURE "linux64"
#elif defined(__linux__) && defined(__i386__)
#define TARGET_ARCHITECTURE "linux32"
#elif defined(__APPLE__) && defined(__MACH__) && defined(__x86_64__)
#define TARGET_ARCHITECTURE "macos64"
#elif defined(__APPLE__) && defined(__MACH__) && defined(__i386__)
#define TARGET_ARCHITECTURE "macos32"
#else
#define TARGET_ARCHITECTURE "unknown"
#endif