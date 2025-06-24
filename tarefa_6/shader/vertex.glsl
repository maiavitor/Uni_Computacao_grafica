#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec4 fragPos; 
out vec2 texcoord;

void main()
{
   	gl_Position = projection * view *  model * vec4(position.x, position.y, position.z, 1.0);
	fragPos =  vec4(position.x, position.y, position.z, 1.0);
	vNormal =  normal;
	texcoord = vec2(texc.x, 1 - texc.y);
}

