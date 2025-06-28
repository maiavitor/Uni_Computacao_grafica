#version 400

in vec4 fragPos;
in vec3 vNormal;
in vec2 texcoord;

uniform sampler2D texBuff;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float q;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;


out vec4 color;


void main()
{

	vec3 ambient = lightColor * ka ;

	//Coeficiente de luz ambiente

	//difusa
	vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - vec3(fragPos));
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * kd;

	//espucluar

	vec3 R = reflect(-L, N);
    vec3 V = normalize(camPos - vec3(fragPos));
    float spec = pow(max(dot(R, V), 0.0), q);
    vec3 specular = spec * ks * lightColor;

	vec3 texColor = texture(texBuff,texcoord).rgb;

	vec3 result = (ambient + diffuse) * texColor + specular;
	color = vec4(result, 1.0f);
	
}
