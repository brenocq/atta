//--------------------------------------------------
// Atta Graphics System
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/pipeline.h>

namespace atta
{
	Pipeline::Pipeline(const CreateInfo& info):
		_backfaceCulling(info.backfaceCulling), _wireframe(info.wireframe),
		_lineWidth(info.lineWidth), _debugName(info.debugName)
	{

	}
}
