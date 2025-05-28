#pragma once



class modelo {
private:
	GLuint VAO, IBO;
	glm::mat4 model;

public:
	modelo(GLuint vao, GLuint ibo, glm::mat4 mod):
		VAO(vao), IBO(ibo), model(mod){} 
	~modelo();

		void setTransf(const glm::mat4& transform) noexcept {
		        model_ = transform;
		        }

		void draw() const {
				glBindVertexArray(VAO_);
				glBindBuffer(GL_ELEMNT_ARRAY_BUFFER, IBO_);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				
		}

		void setRotation(const GLfloat angle, char axis){
			switch (axis){
				const float radians = glm::radians(angleDegrees);
		        switch (axis) {
		            case 'x':			        
		                model_ = glm::rotate(model_, radians, glm::vec3(1.0f, 0.0f, 0.0f));
		                break;
		            case 'y':
		                model_ = glm::rotate(model_, radians, glm::vec3(0.0f, 1.0f, 0.0f));
		                break;
		            case 'z':
		                model_ = glm::rotate(model_, radians, glm::vec3(0.0f, 0.0f, 1.0f));
		                break;           
		        }
			}	
		}
		 
		const glm::mat4& getModelMatrix() const noexcept {
		        return modelMatrix_;
		    }
};

