#pragma once
#include "loadObj.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

 
using namespace std;
 

class modelo {
private:
	GLuint VAO, textID;
	glm::mat4 model;
	int nVertices;
	int w,h;
	
public:	

	glm::vec3 ka = glm::vec3(1.0f);
    glm::vec3 kd = glm::vec3(1.0f);
    glm::vec3 ks = glm::vec3(1.0f);
    GLfloat specular = 1.0f; 


	modelo(){
			VAO = loadSimpleOBJ("../assets/Modelos3D/Suzanne.obj", nVertices);				
			model = glm::mat4(1.0f);
		}

	

	void setTransf(const GLfloat a, const GLfloat d, const GLfloat s,const GLfloat w, const GLfloat i,const GLfloat k) {
	        model = glm::translate(model, glm::vec3(a, i, w));
	        model = glm::translate(model, glm::vec3(d, k, s));
	        }

	void draw(const Shader shader) const {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textID);
			
			glBindVertexArray(VAO);
			shader.setMat4("model", glm::value_ptr(model));
			shader.setInt("texBuff",0);

			glDrawArrays(GL_TRIANGLES, 0 , nVertices);
			glBindVertexArray(0);
			
	}

	void setRotation(const GLfloat angle, char axis){
		const float radians = glm::radians(angle);
        switch (axis) {
            case 'x':			        
                model = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f));
                break;
            case 'y':
                model = glm::rotate(model, radians, glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            case 'z':
                model = glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));
                break;           
        }
	}	

	void setScale(const GLfloat size){
		model = glm::scale(model, glm::vec3(size,size,size));
	}
	 
	const glm::mat4& getModelMatrix() const noexcept {
	        return model;
	    }

    void loadTexture(string filePath)
    {
    	GLuint texID; // id da textura a ser carregada
    
    	// Gera o identificador da textura na memória
    	glGenTextures(1, &texID);
    	glBindTexture(GL_TEXTURE_2D, texID);
    
    	// Ajuste dos parâmetros de wrapping e filtering
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    	// Carregamento da imagem usando a função stbi_load da biblioteca stb_image
    	int nrChannels;
    
    	unsigned char *data = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);
    
    	if (data)
    	{
    		if (nrChannels == 3) // jpg, bmp
    		{
    			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    		}
    		else // assume que é 4 canais png
    		{
    			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		}
    		
    		glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	else
    	{
    		std::cout << "Failed to load texture " << filePath << std::endl;
    	}
    
    	stbi_image_free(data);
    
    	glBindTexture(GL_TEXTURE_2D, 0);
    
    	textID = texID;
    }

	
	void loadMTL(string filePath){
	
		std::ifstream arqEntrada(filePath.c_str());
		    if (!arqEntrada.is_open()) 
			{
		        std::cerr << "Erro ao tentar ler o arquivo " << filePath << std::endl;
		    }
		
	    std::string line;
	    while (std::getline(arqEntrada, line)) 
		{
	        std::istringstream ssline(line);
	        std::string word;
	        ssline >> word;

	        if (word == "Ka") 
        		ssline >> ka.r >> ka.g >> ka.b;
	        		
        	else if (word == "Ke")
        		ssline >> kd.r >> kd.g >> kd.b;
	        	
        	else if (word == "Ks")
        		ssline >> ks.r >> ks.g >> ks.b;
	     	
	        else if (word == "Ns")
        		ssline >> specular;
		}
	}
  
    
};

