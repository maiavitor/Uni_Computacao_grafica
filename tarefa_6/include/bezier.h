#pragma once

#include <string>
#include <vector>
#include <math.h>
#include "Shader.h"


class Bezier {
	private:
		vector<glm::vec3> controlPoints;
		vector<glm::vec3> curvePoints;
		
		glm::mat4 matrix = glm::mat4(-1, 3, -3, 1,
									 3, -6, 3, 0,
									-3, 3, 0, 0,
									1, 0, 0,0 );
		
	public:
		Bezier(){ }

		void generateCurve(int points){
			float step = 1.0/ (float)points;
			float t = 0;
			for(int i = 0; i < controlPoints.size() - 3; i+= 3){
				for (float t = 0.0; t <= 1.0; t += step){
					glm::vec3 p;

					glm::vec4 T(t*t*t, t*t, t, 1);

					glm::vec3 P0 = controlPoints[i];
					glm::vec3 P1 = controlPoints[i+1];
					glm::vec3 P2 = controlPoints[i+2];
					glm::vec3 P3 = controlPoints[i+3];

					glm::mat4x3 G(P0,P1,P2,P3);

					p = G * matrix * T;

					curvePoints.push_back(p);
					
				}
			}
		}

		void generateQuadraticCurve(int seg){
			curvePoints.clear();

			float step = 1.0f/ (float)seg;

			for (int i = 0; i < controlPoints.size() - 2; i += 2){
				glm::vec3 P0 = controlPoints[i];
				glm::vec3 P1 = controlPoints[i+1];
				glm::vec3 P2 = controlPoints[i+2];

				for (float t = 0.0f; t <= 1.0f; t += step){
					float u = 1.0f - t;
					glm::vec3 p = u * u * P0 + 2.0f * u * t * P1 + t * t * P2;
					curvePoints.push_back(p);
				}
			}
		}

		glm::vec3 getCP(int i){ return controlPoints[i];}

		int getCPSize() {return controlPoints.size();}

		void setControl(vector<glm::vec3> cp){controlPoints = cp;}

		int getPoint() {return curvePoints.size();}

		glm::vec3 getPointsCurve(int i) {return curvePoints[i];}

		void generateCircleControlPointsSet(int nPoints, float radius){

			for (int i = 0; i < nPoints; i++){
				float angle = (2.0f * M_PI * i) / nPoints;
				float x = radius * cos(angle);
				float y = radius * sin(angle);
				controlPoints.push_back(glm::vec3(x ,y, 0.0f));
			}
		}
};
	
		
	
