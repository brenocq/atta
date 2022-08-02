#version 300 es
precision mediump float;

in vec2 texCoord;
out vec4 FragColor;

uniform vec3 albedo;
uniform sampler2D albedoTexture;

void main()
{
    if(albedo.x < 0.0f)
        FragColor = vec4(texture(albedoTexture, texCoord).rgb, 1.0f);
    else
        FragColor = vec4(albedo, 1.0f);
}
