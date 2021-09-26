//--------------------------------------------------
// Atta Graphics System
// pipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_PIPELINE_H
#define ATTA_GRAPHICS_SYSTEM_PIPELINE_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/shaderGroup.h>
#include <atta/graphicsSystem/renderPass.h>
#include <atta/graphicsSystem/vertexBuffer.h>

namespace atta
{
	class Pipeline
	{
	public:
		enum PrimitiveTopology
		{
			NONE,
			POINT,
			LINE,
			TRIANGLE,
			LINE_STRIP,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		struct CreateInfo
		{
			std::shared_ptr<ShaderGroup> shaderGroup;
			std::shared_ptr<RenderPass> renderPass;
			VertexBufferLayout layout;
			PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLE;
			bool backfaceCulling = true;
			bool wireframe = false;
			bool lineWidth = 1.0f;

			StringId debugName = StringId("Unnamed Pipeline");
		};

		Pipeline(const CreateInfo& info);
		virtual ~Pipeline() = default;

		virtual void begin(bool clear = true) = 0;
		virtual void end() = 0;

		virtual void* getImGuiTexture() const = 0;

		std::shared_ptr<ShaderGroup> getShaderGroup() const { return _shaderGroup; };
		std::shared_ptr<RenderPass> getRenderPass() const { return _renderPass; };
		StringId getDebugName() const { return _debugName; }

	protected:
		std::shared_ptr<ShaderGroup> _shaderGroup;
		std::shared_ptr<RenderPass> _renderPass;
		VertexBufferLayout _layout;
		PrimitiveTopology _primitiveTopology;

		const bool _backfaceCulling;
		const bool _wireframe;
		const bool _lineWidth;

		const StringId _debugName;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_PIPELINE_H
