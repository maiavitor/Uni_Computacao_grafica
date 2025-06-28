#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <string>
#include <assert.h>
#include <cstdio>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

//internal includes
#include "model.h"
#include "Shader.h"
#include "camera.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

GLfloat  rotateX=0.0, rotateY=0.0, rotateZ=0.0, dir_a=0.0, dir_d=0.0 ,dir_w=0.0, dir_s=0.0;
GLfloat dir_i=0.0, dir_k=0.0, escala=1.0f;
int selectmodel = 0;


//objetos globais

Camera camera = Camera();

//variaveis globais

float lastX = 0.0f;
float lastY = 0.0f;

bool firstmouse = true;


// Função MAIN
int main()
{
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Vitor Maia!", NULL, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW

	glfwSetKeyCallback(window, key_callback);
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("../shader/vertex.glsl","../shader/fragment.glsl"); 		
	
	shader.Use();

	modelo modelo1 = modelo(shader,  "../assets/Modelos3D/hill.obj", "../assets/Modelos3D/hill.mtl");
	modelo modelo2 = modelo(shader,  "../assets/Modelos3D/wizard.obj", "../assets/Modelos3D/wizard.mtl");
	modelo modelo3 = modelo(shader, 120,70, 4.0f, "../assets/Modelos3D/fary.obj", "../assets/Modelos3D/fary.mtl");
	modelo listModel[3]= {modelo1, modelo2, modelo3};

	glm::vec3 mod = glm::vec3(0.0f,3.10f,1.0f);

	shader.setMat4("projection", glm::value_ptr(camera.projection));
	
	glEnable(GL_DEPTH_TEST);
	int i = 0;
	listModel[1].moveVec(mod);

	listModel[2].setScale(1.5f);
	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.setVec3("camPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

		glBindVertexArray(0);
		
		shader.setMat4("view",glm::value_ptr(camera.getView()));
		
		listModel[selectmodel].move(dir_a, dir_d, dir_s, dir_w, dir_i, dir_k);

		listModel[selectmodel].setRotation(rotateX, 'x' );
		listModel[selectmodel].setRotation(rotateY, 'y' );
		listModel[selectmodel].setRotation(rotateZ, 'z' );
		
		listModel[selectmodel].move(dir_a, dir_d, dir_s, dir_w, dir_i, dir_k);

		dir_a=dir_d=dir_s=dir_w=dir_i=dir_k = 0;
		rotateZ=rotateX=rotateY = 0;
		escala = 1;
		
		
		for (int c = 0; c < 2; c++){
			listModel[c].draw();			
		}

		listModel[2].drawCurve(i);		
		
		glfwSwapBuffers(window);

		i++;
	}

	
	
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

	if (key == GLFW_KEY_M && action == GLFW_PRESS )
	{
		
		selectmodel += 1;
		selectmodel = selectmodel % 2;		
	}

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
	
	if (key == GLFW_KEY_A && (action == GLFW_PRESS))
	{
		
		camera.keyMovement("direita");
	}

	if (key == GLFW_KEY_D && (action == GLFW_PRESS ))
	{
		camera.keyMovement("esquerda");
	}
	
	if (key == GLFW_KEY_W && (action == GLFW_PRESS ))
	{
		camera.keyMovement("frente");		
	}
	
	if (key == GLFW_KEY_S && (action == GLFW_PRESS ))
	{
		camera.keyMovement("tras");	
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
		
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS))
	{
		dir_a += 0.1;
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS ))
	{
		dir_d -= 0.1;
	}
	
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS ))
	{
		dir_w += 0.1;
	}
	
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS ))
	{
		dir_s -= 0.1;
	}	
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	
	if (firstmouse) {
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
		std::cout << firstmouse << std::endl;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	
	camera.processMouse(xoffset, yoffset);


}
