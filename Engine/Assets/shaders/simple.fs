#version 330 core

in vec3 frag_color;

out vec4 color;


void main(){
	color = vec4(frag_color.x, frag_color.y, frag_color.z, 0);
}