#version 450 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{		
    //vec3 envColor = texture(environmentMap, WorldPos).rgb;
    vec3 envColor = textureLod(environmentMap, WorldPos, 2.2).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}
