//--------------------------------------------------
// Atta Graphics Module
// shaderGroup.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SHADER_GROUP_H
#define ATTA_GRAPHICS_SHADER_GROUP_H
#include <atta/graphics/shader.h>
#include <atta/core/math/math.h>

namespace atta
{
    class Image;
    class ShaderGroup
    {
    public:
        struct CreateInfo
        {
            std::vector<fs::path> shaderPaths;

            StringId debugName = StringId("Unnamed Shader Group");
        };
        ShaderGroup(const CreateInfo& info);

        virtual ~ShaderGroup() = default;

        virtual void recompile() = 0;
        virtual void bind() = 0;

        const std::vector<std::shared_ptr<Shader>>& getShaders() const { return _shaders; };

        virtual void setBool(const char* name, bool b) = 0;
        virtual void setInt(const char* name, int i) = 0;
        virtual void setFloat(const char* name, float f) = 0;
        virtual void setVec2(const char* name, vec2 v) = 0;
        virtual void setVec3(const char* name, vec3 v) = 0;
        virtual void setVec4(const char* name, vec4 v) = 0;
        virtual void setMat3(const char* name, mat3 m) = 0;
        virtual void setMat4(const char* name, mat4 m) = 0;
        virtual void setTexture(const char* name, StringId sid) = 0;
        virtual void setTexture(const char* name, std::shared_ptr<Image> image) = 0;
        virtual void setCubemap(const char* name, StringId sid) = 0;
        virtual void setCubemap(const char* name, std::shared_ptr<Image> image) = 0;

    protected:
        std::vector<std::shared_ptr<Shader>> _shaders;

        const StringId _debugName;
    };
}

#endif// ATTA_GRAPHICS_SHADER_GROUP_H
