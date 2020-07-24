#----------- Folders ------------
BIN	   	   = bin/
LIB    	   = lib/
SRC	   	   = src/
SRC_SIM    = ${SRC}simulator/
SRC_VULKAN = ${SRC_SIM}vulkan/
SRC_IMGUI  =  ${LIB}imgui/
SRC_UI     =  ${SRC_VULKAN}ui/
SRC_RT     =  ${SRC_VULKAN}rayTracing/
SRC_ASSETS = ${SRC}assets/
OBJ    	   = obj/
SHA    	   = ${SRC}shaders/
SHA_RT     = ${SHA}rayTracing/

#------------ Files -------------
FILES = 
FILES_SIM = simulator scene
FILES_VULKAN = application window instance debugMessenger debugCommon physicalDevice device surface swapChain helpers imageView graphicsPipeline shaderModule pipelineLayout renderPass frameBuffer commandPool commandBuffers semaphore fence vertex buffer vertexBuffer stagingBuffer indexBuffer descriptorSetLayout uniformBuffer descriptorPool descriptorSets stbImage texture image sampler depthBuffer tinyObjLoader model colorBuffer descriptorBinding vulkan descriptorSetManager material imageMemoryBarrier procedural sphere
FILES_IMGUI = imgui imgui_demo imgui_widgets imgui_draw imgui_impl_glfw imgui_impl_vulkan
FILES_UI = userInterface uiRenderPass uiFrameBuffer
FILES_RT = rayTracing deviceProcedures accelerationStructure bottomLevelAccelerationStructure topLevelAccelerationStructure rayTracingPipeline shaderBindingTable
SHADERS = shader
#SHADERS_RT = rayTracing.rchit rayTracing.rgen rayTracing.rmiss rayTracing.procedural.rchit rayTracing.procedural.rint

#------------ Helpers -------------
CC = g++
VULKAN_SDK_PATH = /home/breno/Programs/VulkanSDK/1.2.141.2/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Wall -O3
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lstdc++fs -lvulkan -I${LIB}

#---------- Text style ----------
RED    = \033[0;31m
GREEN  = \033[0;32m
NC     = \033[0m
BOLD   = \033[1m


SOURCES=$(patsubst %, ${SRC}%.cpp, ${FILES})
SOURCES+=$(patsubst %, ${SRC_SIM}%.cpp, ${FILES_SIM})
SOURCES+=$(patsubst %, ${SRC_VULKAN}%.cpp, ${FILES_VULKAN})
SOURCES+=$(patsubst %, ${SRC_IMGUI}%.cpp, ${FILES_IMGUI})
SOURCES+=$(patsubst %, ${SRC_UI}%.cpp, ${FILES_UI})
SOURCES+=$(patsubst %, ${SRC_RT}%.cpp, ${FILES_RT})

OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_SIM})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_VULKAN})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_IMGUI})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_UI})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_RT})

SHADERS_VERT=$(patsubst %, ${SHA}%.vert, ${SHADERS})
SHADERS_FRAG=$(patsubst %, ${SHA}%.frag, ${SHADERS})

EXECUTABLE=${BIN}simulator

#------------ Build -------------
# Compile project files
${OBJ}%.o: ${SRC}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_SIM}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_VULKAN}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_IMGUI}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}
	
${OBJ}%.o: ${SRC_UI}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_RT}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

build: ${OBJECTS} ${SHADERS_VERT} ${SHADERS_FRAG} ${SRC}main.cpp
	@/bin/echo -e "${GREEN}${BOLD}---------- Building ----------${NC}"
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_VERT} -o ${SHA}vert.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_FRAG} -o ${SHA}frag.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA_RT}rayTracing.procedural.rchit -o ${SHA_RT}rayTracing.procedural.rchit.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA_RT}rayTracing.procedural.rint -o ${SHA_RT}rayTracing.procedural.rint.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA_RT}rayTracing.rchit -o ${SHA_RT}rayTracing.rchit.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA_RT}rayTracing.rgen -o ${SHA_RT}rayTracing.rgen.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA_RT}rayTracing.rmiss -o ${SHA_RT}rayTracing.rmiss.spv
	${CC} ${CFLAGS} ${SRC}main.cpp ${OBJECTS} -o ${EXECUTABLE} ${LDFLAGS}

#------------ Run -------------
run: build
	@/bin/echo -e "${GREEN}${BOLD}---------- Running ----------${NC}"
	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./${EXECUTABLE}	

#------------ Clean -------------
clean:
	@/bin/echo -e "${GREEN}${BOLD}---------- Cleaning ----------${NC}"
	rm -rf ${OBJECTS}
	rm -rf ${EXECUTABLE}
