#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

uniform float u_Mix;

void main()
{
    vec4 texColor = mix(texture(u_Texture0, v_TexCoord), texture(u_Texture1, v_TexCoord), u_Mix);
    FragColor = texColor;
}