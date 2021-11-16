#version 300 es
in vec3 inPos;
in vec4 inColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
    gl_PointSize = 5.0;
    gl_Position = projection * view * vec4(inPos, 1.0); 
    color = inColor;
}  
