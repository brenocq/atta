//--------------------------------------------------
// Atta Graphics Module
// pbrRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/pbrRenderer.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderPass.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/components/directionalLight.h>
#include <atta/component/components/environmentLight.h>
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/pointLight.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

PbrRenderer::PbrRenderer() : Renderer("PbrRenderer"), _firstRender(true), _lastEnvironmentMap(StringId("Not defined")) {
    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGB});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
    framebufferInfo.width = 500;
    framebufferInfo.height = 500;
    framebufferInfo.debugName = StringId("PbrRenderer Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    //---------- Create geometry pipeline ----------//
    Pipeline::CreateInfo geometryPipelineInfo{};
    std::shared_ptr<RenderPass> geometryRenderPass;
    {
        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo{};
        shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shader.vert", "shaders/pbrRenderer/shader.frag"};
        shaderGroupInfo.debugName = StringId("PbrRenderer Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("PbrRenderer Render Pass");
        geometryRenderPass = graphics::create<RenderPass>(renderPassInfo);

        // Vertex input layout
        geometryPipelineInfo.shaderGroup = shaderGroup;
        geometryPipelineInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                                       {"inNormal", VertexBufferElement::Type::VEC3},
                                       {"inTexCoord", VertexBufferElement::Type::VEC2}};
        geometryPipelineInfo.renderPass = geometryRenderPass;
        _geometryPipeline = graphics::create<Pipeline>(geometryPipelineInfo);
    }

    //---------- Common pipelines ----------//
    _selectedPipeline = std::make_unique<SelectedPipeline>(geometryRenderPass, geometryPipelineInfo.layout);
    _drawerPipeline = std::make_unique<DrawerPipeline>(geometryRenderPass);

    //---------- Create background shader ----------//
    {
        ShaderGroup::CreateInfo bgShaderGroupInfo{};
        bgShaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/background.vert", "shaders/pbrRenderer/background.frag"};
        bgShaderGroupInfo.debugName = StringId("PBR background Shader Group");
        _backgroundShader = graphics::create<ShaderGroup>(bgShaderGroupInfo);
    }

    //---------- Directional shadow mapping ----------//
    {
        // Framebuffer
        Image::CreateInfo imageInfo;
        imageInfo.format = Image::Format::DEPTH32F;
        imageInfo.samplerWrap = Image::Wrap::BORDER;
        imageInfo.borderColor = vec4(1.0f);
        imageInfo.width = 1024;
        imageInfo.height = 1024;
        imageInfo.debugName = StringId("PbrRenderer::dirShadowMap::image");
        std::shared_ptr<Image> depthImage = graphics::create<Image>(imageInfo);

        Framebuffer::CreateInfo framebufferInfo{};
        framebufferInfo.attachments.push_back({Image::Format::NONE, depthImage});
        framebufferInfo.width = 1024;
        framebufferInfo.height = 1024;
        framebufferInfo.debugName = StringId("PbrRenderer::shadowMap::framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo{};
        shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shadow.vert", "shaders/pbrRenderer/shadow.frag"};
        shaderGroupInfo.debugName = StringId("PbrRenderer::shadowMap::shaderGroup");
        std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("PbrRenderer::shadowMap::renderPass");
        std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

        // Vertex input layout
        Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                               {"inNormal", VertexBufferElement::Type::VEC3},
                               {"inTexCoord", VertexBufferElement::Type::VEC2}};
        _shadowMapPipeline = graphics::create<Pipeline>(pipelineInfo);
    }

    //---------- Omnidirectional shadow mapping ----------//
    {
        // Framebuffer
        Image::CreateInfo imageInfo;
        imageInfo.format = Image::Format::DEPTH32F;
        imageInfo.width = 1024;
        imageInfo.height = 1024;
        imageInfo.isCubemap = true;
        imageInfo.debugName = StringId("PbrRenderer::omniShadowMap::image");
        _omnidirectionalShadowMap = graphics::create<Image>(imageInfo);

        Framebuffer::CreateInfo framebufferInfo{};
        framebufferInfo.attachments.push_back({Image::Format::NONE, _omnidirectionalShadowMap});
        framebufferInfo.width = 1024;
        framebufferInfo.height = 1024;
        framebufferInfo.debugName = StringId("PbrRenderer::omniShadowMap::framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo{};
        shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/omniShadow.vert", "shaders/pbrRenderer/omniShadow.geom",
                                       "shaders/pbrRenderer/omniShadow.frag"};
        shaderGroupInfo.debugName = StringId("PbrRenderer::omniShadowMap::shaderGroup");
        std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("PbrRenderer::omniShadowMap::renderPass");
        std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

        // Vertex input layout
        Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                               {"inNormal", VertexBufferElement::Type::VEC3},
                               {"inTexCoord", VertexBufferElement::Type::VEC2}};
        _omniShadowMapPipeline = graphics::create<Pipeline>(pipelineInfo);
    }
}

PbrRenderer::~PbrRenderer() {}

void PbrRenderer::render(std::shared_ptr<Camera> camera) {
    if (_firstRender) {
        brdfLUT();
    }

    std::vector<component::Entity> entities = component::getNoPrototypeView();

    // Check current envinroment map
    StringId currEnvironmentMap = StringId("textures/white.jpg"); // Default environment map is white texture
    for (auto entity : entities) {
        component::EnvironmentLight* el = entity.get<component::EnvironmentLight>();

        if (el) {
            currEnvironmentMap = el->sid;
            break;
        }
    }

    // Recreate environment map if it is different from last one
    if (currEnvironmentMap != _lastEnvironmentMap) {
        _lastEnvironmentMap = currEnvironmentMap;
        graphics::getRendererAPI()->generateCubemap(_lastEnvironmentMap);
        irradianceCubemap();
        prefilterCubemap();
    }

    shadowPass();
    geometryPass(camera);

    if (_renderSelected)
        _selectedPipeline->render(camera);
    if (_renderDrawer)
        _drawerPipeline->render(camera);

    _firstRender = false;
}

void PbrRenderer::resize(uint32_t width, uint32_t height) {
    if (width != getWidth() || height != getHeight())
        _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
}

void PbrRenderer::shadowPass() {
    std::vector<component::Entity> entities = component::getNoPrototypeView();

    //----- Directional shadow mapping -----//
    component::Entity directionalLightEntity = -1;
    for (auto entity : entities) {
        component::DirectionalLight* dl = entity.get<component::DirectionalLight>();
        component::Transform* t = entity.get<component::Transform>();
        if (dl && t) {
            directionalLightEntity = entity;
            break;
        }
    }

    if (directionalLightEntity != -1) {
        _shadowMapPipeline->begin();
        {
            std::shared_ptr<ShaderGroup> shader = _shadowMapPipeline->getShaderGroup();
            shader->bind();

            // Create light matrix
            component::Transform* t = directionalLightEntity.get<component::Transform>();

            float height = 10.0f;
            float ratio = 1.0f;
            float far = 25.0f;
            mat4 proj = orthographic(height, ratio, far);
            mat4 view;
            view.setPosOri(vec3(), t->orientation);
            _directionalLightMatrix = proj * view;
            shader->setMat4("lightSpaceMatrix", transpose(_directionalLightMatrix));

            // Fill shadow map rendering the scene
            for (auto entity : entities) {
                component::Mesh* mesh = entity.get<component::Mesh>();
                component::Transform* transform = entity.get<component::Transform>();

                if (mesh && transform) {
                    shader->setMat4("model", transpose(transform->getWorldTransformMatrix(entity)));
                    graphics::getRendererAPI()->renderMesh(mesh->sid);
                }
            }
        }
        _shadowMapPipeline->end();
    }

    //----- Omnidirectional shadow mapping -----//
    component::Entity pointLightEntity = -1;
    for (auto entity : entities) {
        component::PointLight* pl = entity.get<component::PointLight>();
        component::Transform* t = entity.get<component::Transform>();
        if (pl && t) {
            pointLightEntity = entity;
            break;
        }
    }

    if (pointLightEntity != -1) {
        _omniShadowMapPipeline->begin();
        {
            std::shared_ptr<ShaderGroup> shader = _omniShadowMapPipeline->getShaderGroup();
            shader->bind();

            // Create light matrix
            component::Transform* t = pointLightEntity.get<component::Transform>();

            // TODO world position
            float fov = radians(90.0f);
            float ratio = 1.0f;
            float near = 0.01f;
            float far = 25.0f;
            mat4 proj = perspective(fov, ratio, near, far);
            std::vector<mat4> shadowMatrices = {proj * lookAt(t->position, t->position + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
                                                proj * lookAt(t->position, t->position + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
                                                proj * lookAt(t->position, t->position + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
                                                proj * lookAt(t->position, t->position + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
                                                proj * lookAt(t->position, t->position + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
                                                proj * lookAt(t->position, t->position + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))};

            shader->setMat4("shadowMatrices[0]", transpose(shadowMatrices[0]));
            shader->setMat4("shadowMatrices[1]", transpose(shadowMatrices[1]));
            shader->setMat4("shadowMatrices[2]", transpose(shadowMatrices[2]));
            shader->setMat4("shadowMatrices[3]", transpose(shadowMatrices[3]));
            shader->setMat4("shadowMatrices[4]", transpose(shadowMatrices[4]));
            shader->setMat4("shadowMatrices[5]", transpose(shadowMatrices[5]));
            shader->setMat4("model", transpose(t->getWorldTransformMatrix(pointLightEntity)));
            shader->setVec3("lightPos", t->position);
            shader->setFloat("far_plane", far);

            // Fill shadow map rendering the scene
            for (auto entity : entities) {
                component::Mesh* mesh = entity.get<component::Mesh>();
                component::Transform* transform = entity.get<component::Transform>();

                if (mesh && transform) {
                    shader->setMat4("model", transpose(transform->getWorldTransformMatrix(entity)));
                    graphics::getRendererAPI()->renderMesh(mesh->sid);
                }
            }
        }
        _omniShadowMapPipeline->end();
    }
}

void PbrRenderer::geometryPass(std::shared_ptr<Camera> camera) {
    std::vector<component::Entity> entities = component::getNoPrototypeView();
    _geometryPipeline->begin();
    {
        //---------- PBR shader ----------//
        std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();
        shader->bind();

        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        shader->setVec3("camPos", camera->getPosition());
        shader->setMat4("directionalLightMatrix", transpose(_directionalLightMatrix));
        shader->setImage("directionalShadowMap", _shadowMapPipeline->getRenderPass()->getFramebuffer()->getImage());
        shader->setCubemap("omniShadowMap", _omnidirectionalShadowMap);
        shader->setFloat("omniFarPlane", 25.0f);

        // Always set environment textures (if there is no environment map, use white texture)
        shader->setCubemap("irradianceMap", "PbrRenderer::irradiance");
        shader->setCubemap("prefilterMap", "PbrRenderer::prefilter");
        shader->setImage("brdfLUT", "PbrRenderer::brdfLUT");

        if (_lastEnvironmentMap != "Not defined"_sid)
            shader->setInt("numEnvironmentLights", 1);
        else {
            LOG_WARN("graphics::PbrRenderer", "Number of environment light should always be 1 (white texture if not defined)");
            shader->setInt("numEnvironmentLights", 0);
        }
        shader->setMat3("environmentLightOri", mat3(1.0f));

        //----- Lighting -----//
        _environmentMapOri = mat3(1.0f);
        int numPointLights = 0;
        int numDirectionalLights = 0;
        for (auto entity : entities) {
            component::Transform* transform = entity.get<component::Transform>();
            component::PointLight* pl = entity.get<component::PointLight>();
            component::DirectionalLight* dl = entity.get<component::DirectionalLight>();
            component::EnvironmentLight* el = entity.get<component::EnvironmentLight>();

            if (transform && (pl || dl || el)) {
                if (pl && numPointLights < 10) {
                    vec3 position = transform->getWorldTransformMatrix(entity).getPosition();
                    int i = numPointLights++;
                    shader->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), position);
                    shader->setVec3(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
                }
                if (dl) {
                    vec3 base = {0.0f, 0.0f, -1.0f};
                    mat4 rotation;
                    rotation.setPosOri({0, 0, 0}, transform->orientation);
                    base = transpose(mat3(rotation)) * base;
                    shader->setVec3("directionalLight.direction", base);
                    shader->setVec3("directionalLight.intensity", dl->intensity);
                    numDirectionalLights++;
                }
                if (el) {
                    mat4 ori;
                    ori.setPosOri(vec3(0.0f), transform->orientation);
                    _environmentMapOri = mat3(ori);
                    shader->setMat3("environmentLightOri", transpose(_environmentMapOri));
                }
                if (numPointLights++ == 10)
                    LOG_WARN("graphics::PbrRenderer", "Maximum number of point lights reached, 10 lights");
            }
        }
        shader->setInt("numPointLights", numPointLights);
        shader->setInt("numDirectionalLights", numDirectionalLights ? 1 : 0);

        //----- Entities -----//
        for (auto entity : entities) {
            component::Mesh* mesh = entity.get<component::Mesh>();
            component::Transform* transform = entity.get<component::Transform>();
            component::Material* compMat = entity.get<component::Material>();
            resource::Material* material = compMat ? compMat->getResource() : nullptr;

            if (mesh && transform) {
                mat4 model = transpose(transform->getWorldTransformMatrix(entity));
                mat4 invModel = inverse(model);
                shader->setMat4("model", model);
                shader->setMat4("invModel", invModel);

                if (material) {
                    if (material->colorIsImage()) {
                        shader->setImage("albedoTexture", material->colorImage);
                        shader->setVec3("material.albedo", {-1, -1, -1});
                    } else
                        shader->setVec3("material.albedo", material->color);

                    if (material->metallicIsImage()) {
                        shader->setImage("metallicTexture", material->metallicImage);
                        shader->setFloat("material.metallic", -1);
                    } else
                        shader->setFloat("material.metallic", material->metallic);

                    if (material->roughnessIsImage()) {
                        shader->setImage("roughnessTexture", material->roughnessImage);
                        shader->setFloat("material.roughness", -1);
                    } else
                        shader->setFloat("material.roughness", material->roughness);

                    if (material->aoIsImage()) {
                        shader->setImage("aoTexture", material->aoImage);
                        shader->setFloat("material.ao", -1);
                    } else
                        shader->setFloat("material.ao", material->ao);

                    if (material->hasNormalImage()) {
                        shader->setImage("normalTexture", material->normalImage);
                        shader->setInt("material.hasNormalTexture", 1);
                    } else
                        shader->setInt("material.hasNormalTexture", 0);
                } else {
                    resource::Material::CreateInfo defaultMaterial{};
                    shader->setVec3("material.albedo", defaultMaterial.color);
                    shader->setFloat("material.metallic", defaultMaterial.metallic);
                    shader->setFloat("material.roughness", defaultMaterial.roughness);
                    shader->setFloat("material.ao", defaultMaterial.ao);
                    shader->setInt("material.hasNormalTexture", 0);
                }

                graphics::getRendererAPI()->renderMesh(mesh->sid);
            }
        }

        //---------- Background shader ----------//
        if (_lastEnvironmentMap != "Not defined"_sid) {
            _backgroundShader->bind();
            _backgroundShader->setMat4("projection", transpose(camera->getProj()));
            _backgroundShader->setMat4("view", transpose(camera->getView()));
            _backgroundShader->setCubemap("environmentMap", _lastEnvironmentMap);
            //_backgroundShader->setCubemap("environmentMap", _omnidirectionalShadowMap);
            //_backgroundShader->setCubemap("environmentMap", "PbrRenderer::irradiance");
            _backgroundShader->setMat3("environmentMapOri", _environmentMapOri);

            graphics::getRendererAPI()->renderCube();
        }
    }
    _geometryPipeline->end();
}

void PbrRenderer::irradianceCubemap() {
    // Create shader
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/compute/irradiance.vert", "shaders/compute/irradiance.frag"};
    shaderGroupInfo.debugName = StringId("Irradiance Shader Group");
    std::shared_ptr<ShaderGroup> shader = graphics::create<ShaderGroup>(shaderGroupInfo);

    // Generate irradiance cubemap
    RendererAPI::GenerateProcessedCubemapInfo info;
    info.cubemapSid = StringId("PbrRenderer::irradiance");
    info.shader = shader;
    info.width = 128;
    info.height = 128;
    info.numMipLevels = 1;
    info.func = [&](std::shared_ptr<ShaderGroup> shader, mat4 proj, mat4 view, int face, int mipLevel) {
        if (mipLevel == 0 && face == 0) {
            shader->setCubemap("environmentMap", _lastEnvironmentMap);
            shader->setMat4("projection", transpose(proj));
        }
        shader->setMat4("view", transpose(view));
    };
    graphics::getRendererAPI()->generateProcessedCubemap(info);
}

void PbrRenderer::prefilterCubemap() {
    // Create shader
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/compute/prefilter.vert", "shaders/compute/prefilter.frag"};
    shaderGroupInfo.debugName = StringId("Prefilter Shader Group");
    std::shared_ptr<ShaderGroup> shader = graphics::create<ShaderGroup>(shaderGroupInfo);

    // Generate prefilter cubemap
    RendererAPI::GenerateProcessedCubemapInfo info;
    info.cubemapSid = StringId("PbrRenderer::prefilter");
    info.shader = shader;
    info.width = 512;
    info.height = 512;
    info.numMipLevels = 5;
    info.func = [&](std::shared_ptr<ShaderGroup> shader, mat4 proj, mat4 view, int face, int mipLevel) {
        if (mipLevel == 0 && face == 0) {
            shader->setCubemap("environmentMap", _lastEnvironmentMap);
            shader->setMat4("projection", transpose(proj));
        }

        shader->setMat4("view", transpose(view));

        float roughness = (float)mipLevel / (float)(info.numMipLevels - 1);
        shader->setFloat("roughness", roughness);
    };
    graphics::getRendererAPI()->generateProcessedCubemap(info);
}

void PbrRenderer::brdfLUT() {
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/compute/brdf.vert", "shaders/compute/brdf.frag"};
    shaderGroupInfo.debugName = StringId("BRDF LUT Shader Group");
    std::shared_ptr<ShaderGroup> shader = graphics::create<ShaderGroup>(shaderGroupInfo);

    // Generate brdf LUT
    RendererAPI::GenerateProcessedTextureInfo info;
    info.textureSid = StringId("PbrRenderer::brdfLUT");
    info.shader = shader;
    info.imageInfo.format = Image::Format::RG16F;
    info.imageInfo.samplerWrap = Image::Wrap::REPEAT;
    info.imageInfo.width = 512;
    info.imageInfo.height = 512;
    info.imageInfo.mipLevels = 1;
    info.imageInfo.debugName = StringId("PbrRenderer brdfLUT image");

    graphics::getRendererAPI()->generateProcessedTexture(info);
}

} // namespace atta::graphics
