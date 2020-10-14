#----------- Folders ------------
BIN	   	   = bin/
LIB    	   = lib/
SRC	   	   = src/
SRC_SIM    = ${SRC}simulator/
SRC_OBJS   = ${SRC_SIM}objects/
SRC_OBJS_BSC= ${SRC_OBJS}basic/
SRC_VULKAN = ${SRC_SIM}vulkan/
SRC_PHY    =  ${SRC_SIM}physics/
SRC_CNST   =  ${SRC_PHY}constraints/
SRC_FOR    =  ${SRC_PHY}forces/
SRC_HLP    = ${SRC_SIM}helpers/
SRC_IMGUI  =  ${LIB}imgui/
SRC_UI     =  ${SRC_VULKAN}ui/
SRC_RT     =  ${SRC_VULKAN}rayTracing/
SRC_PIP    =  ${SRC_VULKAN}pipeline/
SRC_ASSETS = ${SRC}assets/
SRC_DEMO = ${SRC}demo/
OBJ    	   = obj/
SHA    	   = ${SRC}shaders/
SHA_RT     = ${SHA}rayTracing/

#------------ Files -------------
FILES = 
FILES_SIM = simulator scene object
FILES_VULKAN = application window instance debugMessenger debugCommon physicalDevice device surface swapChain helpers imageView shaderModule renderPass frameBuffer commandPool commandBuffers semaphore fence vertex buffer vertexBuffer stagingBuffer indexBuffer descriptorSetLayout uniformBuffer descriptorPool descriptorSets stbImage texture image sampler depthBuffer tinyObjLoader model colorBuffer descriptorBinding vulkan descriptorSetManager material imageMemoryBarrier procedural modelViewController 
FILES_PHY = physicsEngine objectPhysics
FILES_CNST = constraint fixedConstraint hingeConstraint
FILES_FOR = force forceGenerator
FILES_HLP = log drawHelper debugDrawer
FILES_IMGUI = imgui imgui_demo imgui_widgets imgui_draw imgui_impl_glfw imgui_impl_vulkan
FILES_UI = userInterface uiRenderPass uiFrameBuffer
FILES_RT = rayTracing deviceProcedures accelerationStructure bottomLevelAccelerationStructure topLevelAccelerationStructure rayTracingPipeline shaderBindingTable
FILES_PIP = pipeline pipelineLayout graphicsPipeline linePipeline
FILES_OBJS = others/displays/displayTFT144
FILES_OBJS_BSC = importedObject plane box sphere cylinder
FILES_DEMO = ttzinho/ttzinho
#SHADERS = graphics/graphicsShader line/lineShader
#SHADERS_RT = rayTracing.rchit rayTracing.rgen rayTracing.rmiss rayTracing.procedural.rchit rayTracing.procedural.rint

#------------ Helpers -------------
CC = g++
VULKAN_SDK_PATH = /home/breno/Programs/VulkanSDK/1.2.141.2/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Wall -O3
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lstdc++fs -lvulkan -I${LIB} -I${SRC} 

#---------- Text style ----------
RED    = \033[0;31m
GREEN  = \033[0;32m
NC     = \033[0m
BOLD   = \033[1m

OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_SIM})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_VULKAN})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_PHY})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_CNST})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_FOR})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_HLP})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_IMGUI})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_UI})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_RT})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_PIP})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_OBJS})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_OBJS_BSC})
OBJECTS+=$(patsubst %, ${OBJ}%.o, ${FILES_DEMO})

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

${OBJ}%.o: ${SRC_PHY}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_CNST}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_FOR}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_HLP}%.cpp
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

${OBJ}%.o: ${SRC_PIP}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_OBJS}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_OBJS_BSC}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

${OBJ}%.o: ${SRC_DEMO}%.cpp
	@/bin/echo -e "${GREEN}Compiling $<${NC}"
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

build: ${OBJECTS} ${SHADERS_VERT} ${SHADERS_FRAG} ${SRC}main.cpp
	@/bin/echo -e "${GREEN}${BOLD}---------- Building ----------${NC}"
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA}graphics/graphicsShader.vert -o ${SHA}graphics/graphicsShader.vert.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA}graphics/graphicsShader.frag -o ${SHA}graphics/graphicsShader.frag.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA}line/lineShader.vert -o ${SHA}line/lineShader.vert.spv
	$(VULKAN_SDK_PATH)/bin/glslc ${SHA}line/lineShader.frag -o ${SHA}line/lineShader.frag.spv
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
