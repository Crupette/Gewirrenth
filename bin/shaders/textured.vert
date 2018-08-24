#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

out vec2 frag_Uv;
out vec4 frag_Color;

uniform mat4 cameraMatrix;

void main(){
    gl_Position = cameraMatrix * vec4(position.xy, 0, 1.0);
	
	frag_Uv = vec2(uv.x, -uv.y);
	frag_Color = color;
}
