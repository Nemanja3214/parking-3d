#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
	gl_Position = uP * uV * uM * vec4(position, 1.0); //Zbog nekomutativnosti mnozenja matrica, moramo mnoziti MVP matrice i tjemena "unazad"
}