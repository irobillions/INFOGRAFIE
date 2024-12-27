#version 330 core


layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
out vec4 v_color;

uniform mat4 model; // model = matrixce pour les mouvements local de l'objet bouger ou rotation ou scale
uniform mat4 view; // view = angle dans lequel on voit notre objet cest a dire position de la camera
uniform mat4 projection; // projection = calculer la perspective correct des objets

void main() {
	v_color = vec4(vertex_color, 1.0f);
	gl_Position = projection * view * model * vec4(vertex_pos, 1.0f);
}