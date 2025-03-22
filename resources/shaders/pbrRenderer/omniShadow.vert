#version 320 es
//--------------------------------------------------
// Atta Shaders
// omniShadow.vert
// Date: 2022-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(inPosition, 1.0);
}
