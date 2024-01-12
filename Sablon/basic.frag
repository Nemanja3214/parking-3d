#version 330 core

out vec4 outCol;
uniform vec4 u_Color = vec4(-1.0);;


void main()
{
	outCol = all(equal(u_Color, vec4(-1.0))) ? vec4(1.0, 0.0, 0.0, 1.0) : u_Color;
}