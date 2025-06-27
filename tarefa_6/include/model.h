#pragma once
#include "loadObj.h"
#include "Shader.h"
#include "loadMaterial.h"

class modelo {
private:
	GLuint VAO, textID;
	glm::mat4 model;
	Shader& shader;	
	int nVertices;
	int w,h;
	
public:	
	glm::vec3 lightPos = glm::vec3(1.0f, 0.5f, -1.0f);	
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 ka = glm::vec3(1.0f);
    glm::vec3 kd = glm::vec3(1.0f);
    glm::vec3 ks = glm::vec3(1.0f);
    GLfloat specular = 1.0f; 


	modelo( Shader& s):shader(s){
			
			VAO = loadSimpleOBJ("../assets/Modelos3D/Suzanne.obj", nVertices);				
			textID = loadTexture("../assets/Modelos3D/Suzanne.png", w, h);
			loadMTL("../assets/Modelos3D/Suzanne.mtl", ka, kd, ks, specular);
			model = glm::mat4(1.0f);
			setRotation(180.0f, 'y');
			setScale(0.5f);
		}
	void setTransf(glm::mat4 m){
		model = m;
	}

	void move(const GLfloat a, const GLfloat d, const GLfloat s,const GLfloat w, const GLfloat i,const GLfloat k) {
	        model = glm::translate(model, glm::vec3(a, i, w));
	        model = glm::translate(model, glm::vec3(d, k, s));
	        }

	void draw()  {
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textID);
		
		glBindVertexArray(VAO);

		shader.setVec3("ka", ka.x, ka.y, ka.z);
		shader.setVec3("kd", kd.x, kd.y, kd.z);
		shader.setVec3("ks", ks.x, ks.y, ks.z);
		shader.setFloat("q", specular);
		shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		
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

        
};

