#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"
 
using namespace std;
 

class Camera {

private:

	glm::vec3 Front, Up, Right; 

	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float Yaw = -90.0f;
	float Pitch = 0.0f;

	float MovementSpeed = 1.2f;
	
	
public:	

	glm::mat4 view, projection = glm::mat4(1);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);

	Camera(){

		updateCamera();
							
		projection = glm::perspective(glm::radians(45.0f), 1000.0f / 1000.0f, 0.1f, 100.0f);
		

	}

	void keyMovement(const char* mov){

		if (mov == "frente")
			cameraPos += Front * MovementSpeed;
		if (mov == "tras")
			cameraPos -= Front * MovementSpeed;
		if (mov == "direita")
			cameraPos -= Right * MovementSpeed;
		if (mov == "esquerda")
			cameraPos += Right * MovementSpeed;
	}

	void updateCamera(){
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		Front   = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up    = glm::normalize(glm::cross(Right, Front));
	}	

	glm::mat4 getView(){
		return glm::lookAt(cameraPos, cameraPos + Front, Up);
	}
    
};

