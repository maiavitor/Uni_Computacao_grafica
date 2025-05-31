#pragma once
#include "loadObj.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
 

class modelo {
private:
	GLuint VAO, textID;
	glm::mat4 model;
	int nVertices;
	int w,h;

	

public:
	modelo(){
			VAO = loadSimpleOBJ("../assets/Modelos3D/Suzanne.obj", nVertices);				
			model = glm::mat4(1.0f);
		} 

	void setTransf(const GLfloat a, const GLfloat d, const GLfloat s,const GLfloat w, const GLfloat i,const GLfloat k) {
	        model = glm::translate(model, glm::vec3(a, i, w));
	        model = glm::translate(model, glm::vec3(d, k, s));
	        }

	void draw(const GLint modelloc) const {
			
			glBindVertexArray(VAO);
			glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
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
    
};

