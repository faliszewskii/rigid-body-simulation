#version 400 core

uniform mat4 view;
uniform mat4 projection;
uniform vec3 position;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
}