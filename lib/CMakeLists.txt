add_library(robotSimLib INTERFACE)
target_include_directories(robotSimLib INTERFACE include/)

#add_library(robotSimLibImgui 
#				imgui-sim/imgui_impl_glfw.cpp
#				imgui-sim/imgui_impl_vulkan.cpp)
#target_link_libraries(robotSimLibImgui PRIVATE glfw imgui::imgui)
