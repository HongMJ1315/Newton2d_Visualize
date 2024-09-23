// point_vert.glsl

#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

void main(){

    float x = aPos.x / 5.0;
    float y = aPos.y / 4.0;

    gl_Position = vec4(float(x), float(y), 0.0, 1.0);
    vertexColor = aColor;
}