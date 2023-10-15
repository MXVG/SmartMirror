#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos * vec3(0.4, 0.4, 0.4), 1.0f);
    Color = aColor;
}