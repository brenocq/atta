file(GLOB model_files models/*/*.obj models/*/*.mtl models/*/*.png models/*/*.jpg)
file(GLOB texture_files textures/**/*.jpg textures/**/*.png textures/**/*.hdr textures/*.jpg textures/*.png textures/*.hdr)
file(GLOB font_files fonts/**/*.ttf)

macro(copy_models asset_files dir_name copied_files)
	#message("asset: ${${asset_files}}")
	foreach(asset ${${asset_files}})
		#message("asset: ${asset}")
		get_filename_component(file_name ${asset} NAME)
		get_filename_component(full_path ${asset} ABSOLUTE)
		string(REGEX REPLACE "\\.[^.]*$" "" output_name ${file_name})# File name without extension
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${dir_name}/${output_name})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${asset} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				#COMMAND mklink \"${output_file}\" \"${full_path}\"
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()

macro(copy_textures asset_files dir_name copied_files)
	#message("asset: ${${asset_files}}")
	foreach(asset ${${asset_files}})
		#message("asset: ${asset}")
		get_filename_component(file_name ${asset} NAME)
		get_filename_component(full_path ${asset} ABSOLUTE)
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${dir_name})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${asset} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				#COMMAND mklink \"${output_file}\" \"${full_path}\"
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()

# TODO improve to use same function for all three
macro(copy_fonts asset_files dir_name copied_files)
	#message("asset: ${${asset_files}}")
	foreach(asset ${${asset_files}})
		#message("asset: ${asset}")
		get_filename_component(file_name ${asset} NAME)
		get_filename_component(full_path ${asset} ABSOLUTE)
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${dir_name})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${asset} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				#COMMAND mklink \"${output_file}\" \"${full_path}\"
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()


copy_models(model_files models copied_models)
copy_textures(texture_files textures copied_textures)
copy_fonts(font_files fonts copied_fonts)
	
source_group("Models" FILES ${model_files})
source_group("Textures" FILES ${texture_files})
source_group("Fonts" FILES ${font_files})

add_custom_target(
	assets 
	DEPENDS ${copied_models} ${copied_textures} ${copied_fonts} 
	SOURCES ${model_files} ${texture_files} ${font_files})
