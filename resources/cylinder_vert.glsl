#version 330 core
layout(location = 3) in vec3 cylinder_vertex;
layout(location = 4) in vec3 cylinder_color;
uniform mat4 P;
uniform mat4 MV;
out vec3 cy_color;
void main()
{
	cy_color = cylinder_color;
	gl_Position = P * MV * vec4(cylinder_vertex, 1.0);
}
