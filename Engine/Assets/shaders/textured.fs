#version 330 core

in vec2 frag_uv;
out vec4 color;
uniform sampler2D txtSampler;
void main(){
	color = texture(txtSampler, frag_uv).rgba;
}