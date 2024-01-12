#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

in vec2 chUV2;

in vec3 TexCoords; // direction vector representing a 3D texture coordinate

uniform vec4 u_Color;

uniform int uIsLight;
uniform int uIsEmission;


struct Light{ //Svjetlosni izvor
	vec3 pos; //Pozicija
	vec3 lightDir;
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

uniform Light uSpotlight1;
uniform Light uSpotlight2;
uniform Material uMaterial;
uniform vec3 uViewPos;	//Pozicija kamere (za racun spekularne komponente)

vec3 getLight(Light l){
vec3 resA = l.kA * uMaterial.kA;

	vec3 normal = normalize(chNormal);
	vec3 lightToFrag = normalize(l.pos - chFragPos);
	float nD = max(dot(normal, lightToFrag), 0.0);
	vec3 resD = l.kD * ( nD * uMaterial.kD);

	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightToFrag, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS = l.kS * (s * uMaterial.kS);

	float dp = dot(-lightToFrag, normalize(l.lightDir));
	if (dp <= 0.95) {
	   resD = vec3(0.0);
	   resS = vec3(0);
	}

	float distance    = length(l.pos - chFragPos);
	float attenuation = 1.0 / (l.constant + l.linear * distance + 
    				l.quadratic * (distance * distance));   

	resA  *= attenuation; 
	resD  *= attenuation;
	resS *= attenuation;

    return vec3(resA + resD + resS);
	}

void main()
{    
	vec3 light = getLight(uSpotlight1) + getLight(uSpotlight2);
	vec3 phong = (light[0] + light[1]) * vec3(u_Color) + light[2];
	FragColor = uIsLight == 1 ? vec4(phong, u_Color[3]) : u_Color;
}

