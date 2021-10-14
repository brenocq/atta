#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;

    mat4 rotView = mat4(mat3(view));
    vec4 coordImage = rotView * vec4(WorldPos, 1.0);

    //float z = coordImage.z;
    //coordImage /= z;
    //float r = sqrt(coordImage.x*coordImage.x + coordImage.y*coordImage.y);
    //float L = 1 + r*0.103689 + r*r*0.00487908 + r*r*r*0.00116894 + r*r*r*r*0.000941614;
    //coordImage.x *= L;
    //coordImage.y *= L;
    //coordImage *= z;

    vec4 clipPos = projection * coordImage;

    gl_Position = clipPos.xyww;
}
