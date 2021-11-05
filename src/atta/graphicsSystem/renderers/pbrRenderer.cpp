//--------------------------------------------------
// Atta Graphics System
// pbrRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/relationshipComponent.h>
#include <atta/componentSystem/components/materialComponent.h>
#include <atta/componentSystem/components/pointLightComponent.h>
#include <atta/componentSystem/components/directionalLightComponent.h>
#include <atta/componentSystem/factory.h>

#include <atta/graphicsSystem/drawer.h>

namespace atta
{
    PbrRenderer::PbrRenderer():
        Renderer("PbrRenderer")
    {
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo {};
        framebufferInfo.attachments.push_back({Image::Format::RGB});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
        framebufferInfo.width = 500;
        framebufferInfo.height = 500;
        framebufferInfo.debugName = StringId("PbrRenderer Framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

        //---------- Create geometry pipeline ----------//
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shader.vert", "shaders/pbrRenderer/shader.frag"};
            shaderGroupInfo.debugName = StringId("PbrRenderer Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            // Render Pass
            RenderPass::CreateInfo renderPassInfo {};
            renderPassInfo.framebuffer = framebuffer;
            renderPassInfo.debugName = StringId("PbrRenderer Render Pass");
            std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

            Pipeline::CreateInfo pipelineInfo {};
            // Vertex input layout
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.layout = {
                { "inPosition", VertexBufferElement::Type::VEC3 },
                { "inNormal", VertexBufferElement::Type::VEC3 },
                { "inTexCoord", VertexBufferElement::Type::VEC2 }
            };
            pipelineInfo.renderPass = renderPass;
            _geometryPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }

        //---------- Create line pipeline ----------//
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/line/shader.vert", "shaders/line/shader.frag"};
            shaderGroupInfo.debugName = StringId("PbrRenderer Line Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            // Render Pass
            RenderPass::CreateInfo renderPassInfo {};
            renderPassInfo.framebuffer = framebuffer;
            renderPassInfo.debugName = StringId("PbrRenderer Line Render Pass");
            std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

            Pipeline::CreateInfo pipelineInfo {};
            // Vertex input layout
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.layout = {
                { "inPos", VertexBufferElement::Type::VEC3 },
                { "inColor", VertexBufferElement::Type::VEC4 }
            };
            pipelineInfo.renderPass = renderPass;
            _linePipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }

        //---------- Create point pipeline ----------//
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/point/shader.vert", "shaders/point/shader.frag"};
            shaderGroupInfo.debugName = StringId("PbrRenderer Point Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            // Render Pass
            RenderPass::CreateInfo renderPassInfo {};
            renderPassInfo.framebuffer = framebuffer;
            renderPassInfo.debugName = StringId("PbrRenderer Point Render Pass");
            std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

            Pipeline::CreateInfo pipelineInfo {};
            // Vertex input layout
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.layout = {
                { "inPos", VertexBufferElement::Type::VEC3 },
                { "inColor", VertexBufferElement::Type::VEC4 }
            };
            pipelineInfo.renderPass = renderPass;
            _pointPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }

        ////---------- Create background shader ----------//
        //{
        //    ShaderGroup::CreateInfo bgShaderGroupInfo {};
        //    bgShaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/background.vert", "shaders/pbrRenderer/background.frag"};
        //    bgShaderGroupInfo.debugName = StringId("PBR background Shader Group");
        //    _backgroundShader = GraphicsManager::create<ShaderGroup>(bgShaderGroupInfo);
        //}

    }

    PbrRenderer::~PbrRenderer()
    {

    }

    void PbrRenderer::render(std::shared_ptr<Camera> camera)
    {
        static bool first = true;
        if(first)
        {
            generateCubemap();
            convoluteCubemap();
            prefilterCubemap();
            brdfLUT();
            first = false;
        }

        std::vector<EntityId> entities = ComponentManager::getEntities();

        _geometryPipeline->begin();
        {
            std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();
            shader->bind();
            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));
            shader->setVec3("camPos", camera->getPosition());

            shader->setInt("irradianceMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _irradianceMap);

            shader->setInt("prefilterMap", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _prefilterMap);

            shader->setInt("brdfLUT", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            glBindTexture(GL_TEXTURE_2D, _brdfLUT);

            //----- Lighting -----//
            int numPointLights = 0;
            for(auto entity : entities)
            {
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                PointLightComponent* pl = ComponentManager::getEntityComponent<PointLightComponent>(entity);
                DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);

                if(transform && (pl || dl))
                {
                    if(pl && numPointLights < 10)
                    {
                        int i = numPointLights++;
                        shader->setVec3(("pointLights["+std::to_string(i)+"].position").c_str(), transform->position);
                        shader->setVec3(("pointLights["+std::to_string(i)+"].intensity").c_str(), pl->intensity);
                    }
                    if(dl)
                    {
                        vec3 before = { 0.0f, -1.0f, 0.0f };
                        //vec3 direction;
                        //transform->orientation.transformVector(before, direction);
                        shader->setVec3("directionalLight.direction", before);
                        shader->setVec3("directionalLight.intensity", dl->intensity);
                    }
                    if(numPointLights++ == 10)
                        LOG_WARN("PhongRenderer", "Maximum number of point lights reached, 10 lights");
                }
            }
            shader->setInt("numPointLights", numPointLights);

            for(auto entity : entities)
            {
                MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(entity);
                RelationshipComponent* relationship = ComponentManager::getEntityComponent<RelationshipComponent>(entity);

                if(mesh && transform)
                {
                    mat4 model; 
                    model.setPosOriScale(transform->position, transform->orientation, transform->scale);

                    while(relationship && relationship->parent >= 0)
                    {
                        TransformComponent* ptransform = ComponentManager::getEntityComponent<TransformComponent>(relationship->parent);
                        if(ptransform)
                        {
                            mat4 pmodel; 
                            pmodel.setPosOriScale(ptransform->position, ptransform->orientation, ptransform->scale);
                            model = pmodel * model;
                        }
                        relationship = ComponentManager::getEntityComponent<RelationshipComponent>(relationship->parent);
                    }

                    model.transpose();
                    mat4 invModel = inverse(model);
                    shader->setMat4("model", model);
                    shader->setMat4("invModel", invModel);

                    if(material)
                    {
                        if(material->albedoTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("albedoTexture", material->albedoTexture);
                            shader->setVec3("material.albedo", {-1, -1, -1});
                        }
                        else
                            shader->setVec3("material.albedo", material->albedo);

                        if(material->metallicTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("metallicTexture", material->metallicTexture);
                            shader->setFloat("material.metallic", -1);
                        }
                        else
                            shader->setFloat("material.metallic", material->metallic);

                        if(material->roughnessTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("roughnessTexture", material->roughnessTexture);
                            shader->setFloat("material.roughness", -1);
                        }
                        else
                            shader->setFloat("material.roughness", material->roughness);

                        if(material->aoTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("aoTexture", material->aoTexture);
                            shader->setFloat("material.ao", -1);
                        }
                        else
                            shader->setFloat("material.ao", material->ao);

                        if(material->normalTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("normalTexture", material->normalTexture);
                            shader->setInt("material.hasNormalTexture", 1);
                        }
                        else
                            shader->setInt("material.hasNormalTexture", 0);
                    }
                    else
                    {
                        MaterialComponent material {};
                        shader->setVec3("material.albedo", material.albedo);
                        shader->setFloat("material.metallic", material.metallic);
                        shader->setFloat("material.roughness", material.roughness);
                        shader->setFloat("material.ao", material.ao);
                        shader->setFloat("material.hasNormalTexture", 0);
                    }

                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }

            //_backgroundShader->bind();
            //_backgroundShader->setMat4("projection", transpose(camera->getProj()));
            //_backgroundShader->setMat4("view", transpose(camera->getView()));
            //_backgroundShader->setInt("environmentMap", 0);
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, _envCubemap);
            //renderCube();

        }
        _geometryPipeline->end();

        _linePipeline->begin(false);
        {
            std::shared_ptr<ShaderGroup> shader = _linePipeline->getShaderGroup();
            shader->bind();
            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));
            Drawer::draw<Drawer::Line>();
        }
        _linePipeline->end();

        _pointPipeline->begin(false);
        {
            std::shared_ptr<ShaderGroup> shader = _pointPipeline->getShaderGroup();
            shader->bind();
            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));
            Drawer::draw<Drawer::Point>();
        }
        _pointPipeline->end();
    }

    void PbrRenderer::resize(uint32_t width, uint32_t height)
    {
        if(width != _width || height != _height)
        {
            _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
            _width = width;
            _height = height;
        }
    }

    // renderCube() renders a 1x1 3D cube in NDC.
    // -------------------------------------------------
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    void PbrRenderer::renderCube()
    {
        // initialize (if necessary)
        if (cubeVAO == 0)
        {
            float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
            };
            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(cubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void PbrRenderer::renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    void PbrRenderer::generateCubemap()
    {
        StringId sid("textures/white.jpg");
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
        //---------- Equirectangular to cubemap ----------//
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/equiToCubemap.vert", "shaders/compute/equiToCubemap.frag"};
        shaderGroupInfo.debugName = StringId("EquiToCubemap Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        ResourceManager::get<Texture>(fs::path(sid.getString()));// Load
        glGenTextures(1, &_envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _envCubemap);
        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Projection and view matrices
        mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
        //mat4 captureViews[] =
        //{
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)),
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)),
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)),
        //    lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f))
        //};

        // Convert HDR equirectangular environment map to cubemap equivalent
        shader->bind();
        shader->setMat4("projection", transpose(captureProjection));
        shader->setTexture("equirectangularMap", sid);
        //glViewport(0, 0, 512, 512);
        //glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        //for(unsigned int i = 0; i < 6; ++i)
        //{
        //    shader->setMat4("view", transpose(captureViews[i]));
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _envCubemap, 0);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    renderCube();
        //}
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PbrRenderer::convoluteCubemap()
    {
        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


        // Projection and view matrices
        mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
        mat4 captureViews[] =
        {
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f))
        };

        glGenTextures(1, &_irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/irradiance.vert", "shaders/compute/irradiance.frag"};
        shaderGroupInfo.debugName = StringId("Irradiance Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        shader->bind();
        shader->setMat4("projection", transpose(captureProjection));
        shader->setInt("equirectangularMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _envCubemap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            shader->setMat4("view", transpose(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PbrRenderer::prefilterCubemap()
    {
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        // Projection and view matrices
        mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
        mat4 captureViews[] =
        {
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)),
            lookAt(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f))
        };


        glGenTextures(1, &_prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/prefilter.vert", "shaders/compute/prefilter.frag"};
        shaderGroupInfo.debugName = StringId("Prefilter Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        shader->bind();
        shader->setMat4("projection", transpose(captureProjection));
        shader->setInt("equirectangularMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth  = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            shader->setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                shader->setMat4("view", transpose(captureViews[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PbrRenderer::brdfLUT()
    {
        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


        glGenTextures(1, &_brdfLUT);
        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, _brdfLUT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _brdfLUT, 0);

        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/brdf.vert", "shaders/compute/brdf.frag"};
        shaderGroupInfo.debugName = StringId("BRDF LUT Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        glViewport(0, 0, 512, 512);
        shader->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
