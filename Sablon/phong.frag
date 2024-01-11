#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

uniform vec4 u_Color;
uniform int uIsLight;
uniform int uIsLightbulbOn;

struct Light{ //Svjetlosni izvor
	vec3 pos; //Pozicija
	vec3 kA; //Ambijentalna komponenta (Indirektno svjetlo)
	vec3 kD; //Difuzna komponenta (Direktno svjetlo)
	vec3 kS; //Spekularna komponenta (Odsjaj)

	  float constant;
    float linear;
    float quadratic;
};
struct Material{ //Materijal objekta
	vec3 kA;
	vec3 kD;
	vec3 kS;
	float shine; //Uglancanost
};

uniform Light uLight;
uniform Material uMaterial;
uniform vec3 uViewPos;	//Pozicija kamere (za racun spekularne komponente)

vec3 getLight(Light l){
	vec3 resA = l.kA * uMaterial.kA;
	if(uIsLightbulbOn == 1){
		return resA * vec3(u_Color);
	}

	vec3 normal = normalize(chNormal);
	vec3 lightToFrag = normalize(l.pos - chFragPos);
	float nD = max(dot(normal, lightToFrag), 0.0);
	vec3 resD = l.kD * ( nD * uMaterial.kD);

	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightToFrag, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS = l.kS * (s * uMaterial.kS);

	float distance    = length(l.pos - chFragPos);
	float attenuation = 1.0 / (l.constant + l.linear * distance + 
    				l.quadratic * (distance * distance));   

	resA  *= attenuation; 
	resD  *= attenuation;
	resS *= attenuation;

	return (resA + resD) * vec3(u_Color) + resS;
	}

void main()
{    
    FragColor = uIsLight == 1 ? vec4(getLight(uLight), u_Color[3]) : u_Color;
}

