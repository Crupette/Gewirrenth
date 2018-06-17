#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

uniform mat4 cameraMatrix;

void main(){
    gl_Position = cameraMatrix * vec4(position.xy, 0, 1.0);
}
