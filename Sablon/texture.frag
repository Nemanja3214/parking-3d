#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

uniform sampler2D uDiffMap1;
uniform vec4 u_Color;

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

uniform Light uLight;
uniform Material uMaterial;
uniform vec3 uViewPos;	//Pozicija kamere (za racun spekularne komponente)

void main()
{    
    vec3 resA = uLight.kA * uMaterial.kA;

	vec3 normal = normalize(chNormal);
	vec3 lightToFrag = normalize(uLight.pos - chFragPos);
	float nD = max(dot(normal, lightToFrag), 0.0);
	vec3 resD = uLight.kD * ( nD * uMaterial.kD);

	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightToFrag, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS = uLight.kS * (s * uMaterial.kS);

	float dp = dot(-lightToFrag, normalize(uLight.lightDir));
	if (dp <= 0.95) {
	   resD = vec3(0.0);
	   resS = vec3(0);
	}

	float distance    = length(uLight.pos - chFragPos);
	float attenuation = 1.0 / (uLight.constant + uLight.linear * distance + 
    				uLight.quadratic * (distance * distance));   

	resA  *= attenuation; 
	resD  *= attenuation;
	resS *= attenuation;

    FragColor = texture(uDiffMap1, chUV) * vec4(resA + resD + resS, 1.0);
    //FragColor = vec4(phong, 1.0);
}

