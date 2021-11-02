#version 300 es
precision mediump float;

in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0f);
    //FragColor = vec4(0,0,1, 1.0f);
}
