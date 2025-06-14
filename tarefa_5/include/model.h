#pragma once
#include "loadObj.h"
#include "Shader.h"
#include "loadMaterial.h"

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
			textID = loadTexture("../assets/Modelos3D/Suzanne.png", w, h);
			loadMTL("../assets/Modelos3D/Suzanne.mtl", ka, kd, ks, specular);
			model = glm::mat4(1.0f);
			setRotation(180.0f, 'y');
			setScale(0.5f);
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
    
};

