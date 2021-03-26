file(GLOB_RECURSE shader_files *.vert *.frag *.rgen *.rchit *.rint *.rmiss *.vert *.frag *.rgen *.rchit *.rint *.rmiss)

#message(${shader_files})

file(GLOB_RECURSE shader_extra_files *.glsl)
set_source_files_properties(${shader_extra_files} PROPERTIES HEADER_FILE_ONLY TRUE)

# Shader compilation
foreach(shader ${shader_files})
	#message("SHADER: ${shader}")
	get_filename_component(dir ${shader} DIRECTORY)
	get_filename_component(file_name ${shader} NAME)
	string(REPLACE "/src/" "/include/atta/assets/" output_dir ${dir})
	set(output_file ${output_dir}/${file_name}.spv)
	#message("OUTPUT DIR ${output_dir}")
	#message("OUTPUT FILE ${output_file}")
	#set(output_file ${dir})
	#message(${output_file})

	set(compiled_shaders ${compiled_shaders} ${output_file})
	set(compiled_shaders ${compiled_shaders} PARENT_SCOPE)
	set_source_files_properties(${shader} PROPERTIES HEADER_FILE_ONLY TRUE)
	add_custom_command(
		OUTPUT ${output_file}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
		COMMAND ${Vulkan_GLSLANG_VALIDATOR} --target-env vulkan1.2 -V ${shader} -o ${output_file}
		DEPENDS ${shader} ${shader_extra_files}
	)
endforeach()

add_custom_target(
	shaders
	DEPENDS ${compiled_shaders}
	SOURCES ${shader_files})
