#pragma once



class modelo {
private:
	GLuint VAO, IBO;
	glm::mat4 model;

public:
	modelo(GLuint vao, GLuint ibo, glm::mat4 mod):
		VAO(vao), IBO(ibo), model(mod){} 

		void setTransf(const glm::mat4& transform) noexcept {
		        model = transform;
		        }

		void draw() const {
				
				glBindVertexArray(VAO);
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
		 
		const glm::mat4& getModelMatrix() const noexcept {
		        return model;
		    }
};

