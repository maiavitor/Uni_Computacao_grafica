#pragma once
#include "loadObj.h"
#include "Shader.h"
#include "loadMaterial.h"
#include "bezier.h"

class modelo {
private:
	GLuint VAO, textID;
	glm::mat4 model = glm::mat4(1.0f);
	string texturePath = "";
	Shader& shader;	
	int nVertices;
	
	
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 ka = glm::vec3(1.0f);
    glm::vec3 kd = glm::vec3(1.0f);
    glm::vec3 ks = glm::vec3(1.0f);
    GLfloat specular = 1.0f; 
	Bezier *curve;
    	
public:	
	glm::vec3 lightPos = glm::vec3(10.0f, 5.0f, 7.0f);
	int points, steps;
	float radius;	

	modelo(Shader& s, int points, int steps, float radius, string obj, string mtl ) : shader(s), points(points) , steps(steps), radius(radius) {
			
		VAO = loadSimpleOBJ(obj, nVertices);				
		
		int w,h;
		
		loadMTL(mtl, texturePath, ka, kd, ks, specular);
		textID = loadTexture("../assets/Modelos3D/"+texturePath, w, h);
			
		setRotation(180.0f, 'y');
		setScale(0.5f);

		
		curve = new Bezier();
		curve->generateCircleControlPointsSet(points, radius);
		curve->generateCurve(steps);
	}

	modelo(Shader& s, string obj, string mtl ) : shader(s)  {
				
			VAO = loadSimpleOBJ(obj, nVertices);				
			
			int w,h;
			
			loadMTL(mtl, texturePath, ka, kd, ks, specular);
			textID = loadTexture("../assets/Modelos3D/"+texturePath, w, h);
				
			setRotation(0.0f, 'y');
			//setScale(0.5f);
	
		}
	
	void setTransf(glm::mat4 m){ 
		model = m;
	}

	void move(const GLfloat a, const GLfloat d, const GLfloat s,const GLfloat w, const GLfloat i,const GLfloat k) {
        model = glm::translate(model, glm::vec3(a, i, w));
        model = glm::translate(model, glm::vec3(d, k, s));
        }

	void drawCurve(int i)  {
		
		int mod = i % (curve->getPoint() - 1) ;
						
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textID);
		
		glBindVertexArray(VAO);
		shader.setVec3("ka", ka.x, ka.y, ka.z);
		shader.setVec3("kd", kd.x, kd.y, kd.z);
		shader.setVec3("ks", ks.x, ks.y, ks.z);
		shader.setFloat("q", specular / 2.0f );
		shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);

		
		setTransf(glm::translate(glm::mat4(1.0f),curve->getPointsCurve(mod)));

		shader.setMat4("model", glm::value_ptr(model));
		shader.setInt("texBuff",0);

		glDrawArrays(GL_TRIANGLES, 0 , nVertices);
		glBindVertexArray(0);
		
	}

	void draw()  {
		
						
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textID);
		
		glBindVertexArray(VAO);

		shader.setVec3("ka", ka.x, ka.y, ka.z);
		shader.setVec3("kd", kd.x, kd.y, kd.z);
		shader.setVec3("ks", ks.x, ks.y, ks.z);
		shader.setFloat("q", specular * 10);
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

    void moveVec(glm::vec3 posi){
    	model = glm::translate(model, posi);
    }
        
};

