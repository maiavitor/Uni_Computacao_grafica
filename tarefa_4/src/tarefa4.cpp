/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para as disciplinas de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 07/03/2025
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cstdio>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"
#include "loadObj.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;


// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texc;


uniform mat4 model;


out vec3 vNormal;
out vec4 fragPos; 

out vec2 texcoord;

void main()
{
   	gl_Position = model * vec4(position.x, position.y, position.z, 1.0);
	fragPos = model * vec4(position.x, position.y, position.z, 1.0);
	vNormal = normal;
	texcoord = vec2(texc.x, 1 - texc.y);
})";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = R"(
#version 400

uniform sampler2D texBuff;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float q;

out vec4 color;

in vec4 fragPos;
in vec3 vNormal;
in vec4 vColor;
in vec2 texcoord;

void main()
{

	vec4 objectColor = vec4(texture(texBuff,texcoord).rgb,1.0);

	//Coeficiente de luz ambiente
	vec3 ambient =  lightColor * ka ;

	//Coeficiente de reflexão difusa
	vec3 N = normalize(vNormal);

	vec3 specular = ks;
	vec3 L;
	float diff, spec;
	vec3 diffuse = kd;
	vec3 R, V;

	L = normalize(lightPos - vec3(fragPos));
	diff = max(dot(N, L),0.0);
	diffuse += diff * lightColor;

	//Coeficiente de reflexão especular
	R = normalize(reflect(-L,N));
	V = normalize(camPos - vec3(fragPos));
	spec = max(dot(R,V),0.0);
	spec = pow(spec,q);
	specular += (ks * spec * lightColor); 



	vec3 result = (ambient + diffuse) * vec3(objectColor) + specular;
	color = vec4(result,1.0);

})";

GLfloat  rotateX=0.0, rotateY=0.0, rotateZ=0.0, dir_a=0.0, dir_d=0.0 ,dir_w=0.0, dir_s=0.0;
GLfloat dir_i=0.0, dir_k=0.0, escala=1.0f;

int imodelo=0;
glm::vec3 light[3] = {
		glm::vec3(0.9, 0.8, 0.9), // Key 0.7, 0.0, 0.9
		glm::vec3(-0.9, -0.8, 0.0), // Fill
		glm::vec3(1.0, -0.5, -5.0) // Back
	};
// Função MAIN
int main()
{
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Vitor Maia!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();
	
	glUseProgram(shaderID);

	//inicializando os objetos com buffer, indices, e matriz
	modelo su = modelo();

	glm::vec3 camPos = glm::vec3(0.0,0.0,-3.0);

	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	su.loadTexture("../assets/Modelos3D/Suzanne.png");
	su.loadMTL("../assets/Modelos3D/Suzanne.mtl");


	std::cout << su.ks.x;


	glUniform3f(glGetUniformLocation(shaderID, "lightColor"), su.ka.x,su.ka.y,su.ka.z);
	glUniform3f(glGetUniformLocation(shaderID, "ka"), su.ka.x,su.ka.y,su.ka.z);
	glUniform3f(glGetUniformLocation(shaderID, "kd"), su.kd.x,su.kd.y,su.kd.z);
	glUniform3f(glGetUniformLocation(shaderID, "ks"), su.ks.x,su.ks.y,su.ks.z);
	glUniform1f(glGetUniformLocation(shaderID, "q"), su.specular);
	/*for (int x = 0; x < 3; x++)
		glUniform3f(glGetUniformLocation(shaderID, ("light[" + std::to_string(x) + "]").c_str()), light[x].x,light[x].y,light[x].z);*/

	glUniform3f(glGetUniformLocation(shaderID, "camPos"), camPos.x,camPos.y,camPos.z);

	GLint textloc  = glGetUniformLocation(shaderID, "texBuff");

	glEnable(GL_DEPTH_TEST);

	su.setScale(0.5f);
	su.setRotation(180.0f, 'y');

	//parametros texturas
	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(0);

		su.setScale(escala);

		su.setRotation(rotateX, 'x');
		su.setRotation(rotateY, 'y');
		su.setRotation(rotateZ, 'z');

		su.setTransf(dir_a,dir_d,dir_s,dir_w,dir_i,dir_k);

		dir_a=dir_d=dir_s=dir_w=dir_i=dir_k = 0;
		rotateZ=rotateX=rotateY = 0;
		escala = 1;
		
		su.draw(modelLoc, textloc);							

		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		
		rotateX += 1.7f;		
	}

	if (key == GLFW_KEY_Y && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		
		rotateY += 1.7f;		
	}

	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		rotateZ += 1.7f;
	}
	
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		
		dir_a -= 0.02f;
	}

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		dir_d += 0.02f;
	}
	
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		dir_w += 0.02f;		
	}
	
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		dir_s -= 0.02f;	
	}

	if (key == GLFW_KEY_I && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		dir_i += 0.02f;		
	}
	
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		dir_k -= 0.02f;
	}
	
	if (key == GLFW_KEY_LEFT_BRACKET && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		escala += 0.02f;		
	}
	
	if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		escala -= 0.02f;		
	}
	
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		imodelo =  (1+imodelo) % 2;		
	}

}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
