#----------- Folders ------------
BIN	   	   = bin/
LIB    	   = lib/
SRC	   	   = src/
SRC_SIM    = ${SRC}simulator/
SRC_VULKAN = ${SRC_SIM}vulkan/
SRC_ASSETS = ${SRC}assets/
OBJ    	   = obj/
SHA    	   = ${SRC}shaders/

#------------ Files -------------
FILES = 
FILES_SIM = simulator
FILES_VULKAN = application window instance debugMessenger debugCommon physicalDevice device surface swapChain helpers imageView graphicsPipeline shaderModule pipelineLayout renderPass frameBuffer commandPool commandBuffers semaphore fence vertex buffer vertexBuffer stagingBuffer indexBuffer descriptorSetLayout uniformBuffer
#debugUtilsMessenger camera sampler tinyObjLoader frameBuffer shaderModule descriptorPool descriptorSetLayout descriptorSetManager descriptorSets renderPass pipelineLayout graphicsPipeline fence semaphore buffer deviceMemory commandBuffers image depthBuffer swapChain imageView commandPool physicalDevice device surface instance stbImage window simulator 
FILES_ASSETS = #cornellBox scene model texture textureImage uniformBuffer
SHADERS = shader

#------------ Helpers -------------
CC = g++
VULKAN_SDK_PATH = /home/breno/Programs/VulkanSDK/1.2.135.0/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Wall
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lstdc++fs -lvulkan -I ${LIB}

#---------- Text style ----------
RED    = \033[0;31m
GREEN  = \033[0;32m
NC     = \033[0m
BOLD   = \033[1m


SOURCES=$(patsubst %, ${SRC}%.cpp, ${FILES})
SOURCES+=$(patsubst %, ${SRC_SIM}%.cpp, ${FILES_SIM})
SOURCES+=$(patsubst %, ${SRC_VULKAN}%.cpp, ${FILES_VULKAN})
SOURCES+=$(patsubst %, ${SRC_ASSETS}%.cpp, ${FILES_ASSETS})

HEADERS=$(patsubst %, ${SRC}%.h, ${FILES})
HEADERS+=$(patsubst %, ${SRC_SIM}%.h, ${FILES_SIM})
HEADERS+=$(patsubst %, ${SRC_VULKAN}%.h, ${FILES_VULKAN})
HEADERS+=$(patsubst %, ${SRC_ASSETS}%.h, ${FILES_ASSETS})

OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_SIM})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_VULKAN})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_ASSETS})

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

${OBJ}%.o: ${SRC_ASSETS}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}
	
build: ${OBJECTS} ${SHADERS_VERT} ${SHADERS_FRAG} ${SRC}main.cpp
	@/bin/echo -e "${GREEN}${BOLD}---------- Building ----------${NC}"
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_VERT} -o ${SRC}shaders/vert.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_FRAG} -o ${SRC}shaders/frag.spv
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
