#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"
 
using namespace std;
 

class Camera {
private:

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::mat4 view, projection = glm::mat4(1);
	
	
public:	

	Camera(Shader shader){
	
			view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(glm::radians(45.0f), 1000.0f / 1000.0f, 0.1f, 100.0f);
			shader.setMat4("view", glm::value_ptr(view));
			shader.setMat4("projection", glm::value_ptr(projection));
			shader.setVec3("camPos", cameraPos.x, cameraPos.y, cameraPos.z);

		}
  
    
};

