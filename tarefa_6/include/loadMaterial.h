#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

 
using namespace std;


    GLuint loadTexture(string filePath,int w, int h )
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
    
    	return texID;
    }

	
	void loadMTL(string filePath, glm::vec3	ka, glm::vec3 kd, glm::vec3 ks, GLfloat specular){
	
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
