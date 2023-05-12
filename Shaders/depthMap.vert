#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 spaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = spaceMatrix * model * vec4(position, 1.0f);
}