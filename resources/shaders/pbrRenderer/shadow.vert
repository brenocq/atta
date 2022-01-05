#version 300 es
//--------------------------------------------------
// Atta Shaders
// shadow.vert
// Date: 2022-01-04
// By Breno Cunha Queiroz
//--------------------------------------------------

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(inPosition, 1.0);
}
