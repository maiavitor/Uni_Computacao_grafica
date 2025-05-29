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

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;


GLuint ibo;
// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

GLfloat  rotateX=0.0, rotateY=0.0, rotateZ=0.0, dir_a=0.0, dir_d=0.0 ,dir_w=0.0, dir_s=0.0;
GLfloat dir_i=0.0, dir_k=0.0, escala=1.0f;

int imodelo=0;

// Função MAIN
int main()
{
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tarefa 2 -- Cubos", nullptr, nullptr);
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
	 
	//inicialização do buffer dos obejetos
	GLuint VAO1 = setupGeometry();
	GLuint VAO2 = setupGeometry();

	//inicializando e aplicando as transformações
	glm::mat4 mod1 = glm::translate(glm::mat4(1),glm::vec3(0.5f,0.0f,0.0f));
	glm::mat4 mod2 = glm::translate(glm::mat4(1),glm::vec3(-0.5f,0.0f,0.0f));
	
	glUseProgram(shaderID);

	//inicializando os objetos com buffer, indices, e matriz
	modelo cubo0(VAO1, ibo, mod1);
	modelo cubo1(VAO2, ibo, mod2);

	modelo modelos[2] = {cubo0,cubo1};


	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	
	// model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(0);
		
		for (int i = 0; i < 2 ;i++){
			// gerenciamento dos movimentos dos cubos
			if (imodelo == 1) {
				modelos[1].setRotation(rotateX, 'x');
				modelos[1].setRotation(rotateY, 'y');
				modelos[1].setRotation(rotateZ, 'z');
				modelos[1].setTransf(dir_a,dir_d,dir_s,dir_w,dir_i,dir_k);
				modelos[1].setScale(escala);
			}
			else {
				modelos[0].setRotation(rotateX, 'x');
				modelos[0].setRotation(rotateY, 'y');
				modelos[0].setRotation(rotateZ, 'z');
				modelos[0].setTransf(dir_a,dir_d,dir_s,dir_w,dir_i,dir_k);
				modelos[0].setScale(escala);
			}
			
			dir_a=dir_d=dir_w=dir_s=dir_i=dir_k = 0.0f;
			rotateX=rotateY=rotateZ = 0.0f;
			escala = 1.0f;

			modelos[i].draw(modelLoc);					
			
		}

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
		rotateX += 1.0f;		
	}

	if (key == GLFW_KEY_Y && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		
		rotateY += 1.0f;		
	}

	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || GLFW_REPEAT))
	{
		rotateZ += 1.0f;
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

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
 
	GLfloat vertices[] = {

		//Base da pirâmide: 2 triângulos
		//x    y    z    r    g    b
		-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,   // 2 inferior - +z 
		 0.5,  0.5, 0.5, 0.0, 1.0, 0.0,   // 1 superior - +z
		 0.5, -0.5, 0.5, 1.0, 0.0, 1.0,   // 2 superior - +z
		-0.5,  0.5, 0.5, 1.0, 1.0, 0.0,   // 1 inferior - +z

		-0.5, -0.5,  -0.5, 1.0, 0.0, 0.2,   // 2 inferior - z
		 0.5,  0.5,  -0.5, 0.0, 1.0, 0.2,   // 1 superior - z
		 0.5, -0.5,  -0.5, 0.2, 0.0, 1.0,   // 2 superior - z
		-0.5,  0.5,  -0.5, 1.0, 0.5, 1.0,   // 1 inferior - z


	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	unsigned int indices[] = {
								0, 1, 2, 
								3, 1, 0,

								2, 6, 4,
								0, 2, 4,

								7, 6, 5,
								4, 6, 7,

								1, 5, 7,
								3, 1, 7,

								1, 5, 2,
								2, 5, 6,

								3, 7, 0,
								0, 7, 4
									
		
	};

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return VAO;
}

