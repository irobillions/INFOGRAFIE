#version 330 core


layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
out vec4 v_color;;

void main() {
	v_color = vec4(vertex_color, 1.0f);
	gl_Position = vec4(vertex_pos, 1.0f);
}