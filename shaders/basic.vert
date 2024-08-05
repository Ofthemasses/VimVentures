#version 410 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoords;

out vec2 v_texCoords;

void main()
{
    v_texCoords = texCoords;
    gl_Position = vec4(position.x, position.y, 0.0, 0.5);
}
