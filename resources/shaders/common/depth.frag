#version 300 es
precision mediump float;

uniform sampler2D depthMap;
in vec2 TexCoords;
in vec3 Position;

out vec4 FragColor;

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0f);
}
