#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

uniform sampler2D uDiffMap1;
uniform vec4 u_Color;

void main()
{    
    FragColor = texture(uDiffMap1, chUV) * u_Color;
}

