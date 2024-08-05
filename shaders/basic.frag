#version 410 core

in vec2 v_texCoords;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_texCoords);
}
