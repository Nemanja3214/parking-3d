#version 330 core

layout(location = 0) in vec3 inPos;
out vec3 chCol;
uniform float width;
uniform float progress;
uniform float minX;
uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
	mat4 mvp = uP * uV * uM;
	gl_Position = (inPos.x > minX) ? vec4(minX+ progress * width, inPos.y, 0.0, 1.0) : vec4(inPos, 1.0);
	gl_Position = mvp * gl_Position; 
	chCol = vec3(0.0, 1.0, 0.0);
}