#ifndef MODELO_H
#define MODELO_H


class modelo {
private:
	GLuint VAO, IBO;
	glm::mat4 model;

public:
	modelo(GLuint vao, GLuint ibo, glm::mat4 mod):
		VAO(vao), IBO(ibo), model(mod){} 
	~modelo();

		void setTransf(const glm:mat4& transform);

		void draw() const {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ELEMNT_ARRAY_BUFFER, IBO);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				
		}

		
};
#endif
