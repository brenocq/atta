#version 450 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 invModel;

out vec3 worldPos;
out vec3 normal;
out vec2 texCoord;


void main()
{
	vec4 coord = model * vec4(inPosition, 1.0f);
	gl_Position = projection * view * coord;

	worldPos = coord.xyz;
	normal = mat3(transpose(invModel))*inNormal;
	texCoord = inTexCoord;
}
