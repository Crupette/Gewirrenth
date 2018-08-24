#version 330 core

in vec2 frag_Uv;
in vec4 frag_Color;

out vec4 glColor;

uniform sampler2D tex;
//uniform vec4 color;

void main(){
    glColor = texture(tex, frag_Uv) * frag_Color;
}
