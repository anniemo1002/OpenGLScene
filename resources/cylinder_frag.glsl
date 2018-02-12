#version 330 core
out vec4 frag_Color;
in vec3 cy_color;
void main()
{
	frag_Color = vec4(cy_color,1.0);
}
