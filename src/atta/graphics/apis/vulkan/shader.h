// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/pipelineLayout.h>
#include <atta/graphics/apis/vulkan/pushConstant.h>
#include <atta/graphics/apis/vulkan/uniformBuffer.h>
#include <atta/graphics/shader.h>

namespace atta::graphics::vk {

class Shader final : public gfx::Shader {
  public:
    Shader(const fs::path& file);
    ~Shader();

    std::string generateApiCode(ShaderType type, std::string iCode) override;
    void compile() override;
    void bind() override;
    void unbind() override;

    void setBool(const char* name, bool b) override;
    void setInt(const char* name, int i) override;
    void setFloat(const char* name, float f) override;
    void setVec2(const char* name, vec2 v) override;
    void setVec3(const char* name, vec3 v) override;
    void setVec4(const char* name, vec4 v) override;
    void setMat3(const char* name, mat3 m) override;
    void setMat4(const char* name, mat4 m) override;

    std::vector<VkPipelineShaderStageCreateInfo> getShaderStages() const;
    std::shared_ptr<UniformBuffer> getUniformBuffer() const;
    std::shared_ptr<PushConstant> getPushConstant() const;
    std::vector<std::shared_ptr<gfx::Image>> getUniformImages() const;
    void pushUniformBuffer();
    void pushConstants(VkCommandBuffer commandBuffer, std::shared_ptr<PipelineLayout> pipelineLayout);

  private:
    static VkShaderStageFlagBits convertFileToShaderStage(const fs::path& filepath);

    void updateVariable(const char* name, uint8_t* data, size_t size);
    void updateImage(const char* name, std::shared_ptr<gfx::Image> image);

    bool runCommand(std::string cmd);
    std::string readFile(const fs::path& file);

    std::shared_ptr<Device> _device;
    std::map<ShaderType, VkShaderModule> _shaders;
    std::vector<uint8_t> _uniformBufferData;
    std::shared_ptr<UniformBuffer> _uniformBuffer;
    std::vector<uint8_t> _pushConstantData;
    std::shared_ptr<PushConstant> _pushConstant;
    std::vector<std::shared_ptr<gfx::Image>> _uniformImages;
};

} // namespace atta::graphics::vk
