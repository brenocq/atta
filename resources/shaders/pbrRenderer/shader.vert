#version 300 es
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

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
    vec4 coordImage = view * coord;

    //float z = coordImage.z;
    //coordImage /= z;
    //float r = sqrt(coordImage.x*coordImage.x + coordImage.y*coordImage.y);
    //float L = 1 + r*0.103689 + r*r*0.00487908 + r*r*r*0.00116894 + r*r*r*r*0.000941614;
    //coordImage.x *= L;
    //coordImage.y *= L;
    //coordImage *= z;

    gl_Position = projection * coordImage;

    worldPos = coord.xyz;
    normal = mat3(transpose(invModel))*inNormal;
    texCoord = inTexCoord;
}
