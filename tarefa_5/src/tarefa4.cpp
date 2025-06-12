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

//internal includes
#include "model.h"
#include "loadObj.h"
#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Dimensões da janela
const GLuint WIDTH = 1000, HEIGHT = 1000;

GLfloat rotateX=0.0, rotateY=0.0, rotateZ=0.0, dir_a=0.0, dir_d=0.0 ,dir_w=0.0, dir_s=0.0;
GLfloat dir_i=0.0, dir_k=0.0, escala=1.0f;

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
	Shader shader("../shader/vertex.glsl","../shader/fragment.glsl"); 
	
	shader.Use();

	//inicializando os objetos com buffer, indices, e matriz
	modelo su = modelo();

	glm::vec3 camPos = glm::vec3(0.0f,0.0f,2.0f);

	su.loadTexture("../assets/Modelos3D/Suzanne.png");
	
	su.loadMTL("../assets/Modelos3D/Suzanne.mtl");
	
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	shader.setVec3("ka", su.ka.x,su.ka.y,su.ka.z);
	shader.setVec3("kd", su.kd.x, su.kd.y, su.kd.z);
	shader.setVec3("ks", su.ks.x,su.ks.y,su.ks.z);
	shader.setFloat("q", su.specular);
	
	shader.setVec3("lightPos", 1.0f, 0.5f, -1.0f);
	shader.setVec3("camPos", camPos.x,camPos.y,camPos.z);

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
		
		su.draw(shader);							

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
}

