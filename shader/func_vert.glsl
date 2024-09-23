// func_vert.glsl

#version 410 core

layout(location = 0) in vec2 aPos;

out vec2 TexCoord;

void main(){
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aPos;
}
