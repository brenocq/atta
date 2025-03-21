//--------------------------------------------------
// Atta Graphics Module
// base.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_BASE_H
#define ATTA_GRAPHICS_APIS_OPENGL_BASE_H

#ifdef ATTA_OS_WEB
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

namespace atta::graphics::gl {

using OpenGLId = uint32_t;

}

#endif // ATTA_GRAPHICS_APIS_OPENGL_BASE_H
