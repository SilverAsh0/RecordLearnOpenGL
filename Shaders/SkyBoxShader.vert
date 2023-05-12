#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    //保证天空盒的深度绝对是1
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}