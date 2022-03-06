#version 300 es
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 invModel;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    vec4 coord = model * vec4(inPosition, 1.0f);
    gl_Position = projection * view * coord;

    fragPos = coord.xyz;
    normal = mat3(transpose(invModel))*inNormal;
    texCoord = inTexCoord;
}
