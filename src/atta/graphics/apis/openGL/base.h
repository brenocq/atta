// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#ifdef ATTA_OS_WEB
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

namespace atta::graphics::gl {

using OpenGLId = uint32_t;

}
