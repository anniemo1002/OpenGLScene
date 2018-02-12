#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec4 vertNor;

uniform mat4 P;
uniform mat4 MV;
void main()
{
        vec4 temp = 1* vertNor;
	gl_Position = P * MV * vertPos;
}
