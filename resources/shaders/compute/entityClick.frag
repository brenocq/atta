#version 300 es
precision mediump float;
uniform int entityId;

out int FragColor;

void main()
{
    FragColor = entityId;
}
