directxtex = {
	source = path.join(dependencies.basePath, "DirectXTex/DirectXTex"),
}

function directxtex.import()
	links { "DirectXTex" }
	directxtex.includes()
end

function directxtex.includes()
	includedirs {
		directxtex.source,
		path.join(dependencies.basePath, "DirectXTex/Common"),
	}
end

function directxtex.project()
	project "DirectXTex"
		language "C++"
		kind "StaticLib"

		directxtex.includes()

		local shader_dir = path.getabsolute(path.join(directxtex.source, "Shaders"))
		local compile_shaders = path.join(shader_dir, "CompileShaders.cmd")
		local generated_shader = path.join(directxtex.source, "BC7Encode_EncodeBlockCS.inc")
		local tools_dir = path.getabsolute(path.join(dependencies.basePath, "../tools"))
		local fxc = path.join(tools_dir, "fxc.exe")

		if not os.isfile(generated_shader) then
			print("Compiling DirectXTex DirectCompute shaders...")

			if not os.isdir(shader_dir) then
				error("DirectXTex shader directory not found: " .. shader_dir)
			end

			if not os.isfile(compile_shaders) then
				error("CompileShaders.cmd not found: " .. compile_shaders)
			end

			if not os.isfile(fxc) then
				error("fxc.exe not found: " .. fxc)
			end

			os.execute(
				'cmd /c "cd /d "' .. shader_dir .. '" && ' ..
					'set "PATH=' .. tools_dir .. ';%PATH%" && ' ..
					'set "CompileShadersOutput=.." && ' ..
					'call "' .. compile_shaders .. '""'
			)

			if not os.isfile(generated_shader) then
				error("Failed to compile DirectXTex DirectCompute shaders")
			end
		end

		files {
			path.join(directxtex.source, "*.h"),
			path.join(directxtex.source, "*.cpp"),
			path.join(directxtex.source, "*.inl"),
		}

		warnings "Off"
end

table.insert(dependencies, directxtex)
