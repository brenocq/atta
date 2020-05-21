#----------- Folders ------------
BIN	   = bin/
LIB    = lib/
SRC	   = src/
OBJ    = obj/
SHA    = shaders/
#------------ Files -------------
FILES = deviceMemory commandBuffers image depthBuffer swapChain imageView commandPool device surface instance stbImage window simulator 
SHADERS = shader
#------------ Helpers -------------
CC = g++
VULKAN_SDK_PATH = /home/breno/Programs/VulkanSDK/1.2.135.0/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Wall
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan -I ${LIB} 

#---------- Text style ----------
RED    = \033[0;31m
GREEN  = \033[0;32m
NC     = \033[0m
BOLD   = \033[1m


SOURCES=$(patsubst %, ${SRC}%.cpp, ${FILES})
HEADERS=$(patsubst %, ${SRC}%.h, ${FILES})
OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})

SHADERS_VERT=$(patsubst %, ${SHA}%.vert, ${SHADERS})
SHADERS_FRAG=$(patsubst %, ${SHA}%.frag, ${SHADERS})

DEPENDENCIES=../assembler/lib/defines.h

EXECUTABLE=${BIN}simulator

#------------ Build -------------
# Compile project files
${OBJ}%.o: ${SRC}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

build: ${OBJECTS} ${SHADERS_VERT} ${SHADERS_FRAG} ${SRC}main.cpp
	@/bin/echo -e "${GREEN}${BOLD}---------- Building ----------${NC}"
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_VERT} -o shaders/vert.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHADERS_FRAG} -o shaders/frag.spv
	${CC} ${CFLAGS} ${SRC}main.cpp ${OBJECTS} -o ${EXECUTABLE} ${LDFLAGS}

#------------ Init -------------
init:
	mkdir bin obj

#------------ Run -------------
run: build
	@/bin/echo -e "${GREEN}${BOLD}---------- Running ----------${NC}"
	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./${EXECUTABLE}	

#------------ Clean -------------
clean:
	@/bin/echo -e "${GREEN}${BOLD}---------- Cleaning ----------${NC}"
	rm -rf ${OBJECTS}
	rm -rf ${EXECUTABLE}
