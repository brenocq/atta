#version 450 core

in vec3 color;
out vec4 FragColor;
uniform vec3 lightColor;

void main()
{
	FragColor = vec4(color*lightColor, 1.0f);
}
