#ifndef CUBEMAP_H
#define CUBEMAP_H
/************************************************************
*  Class: Plants
*
*  Purpose: The camera class deals with everything concerning a class. 
*
/**************************************************************/
//--- OpenGL ---
#include "..\GL\glew.h"
#include "..\GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\glm\gtc\type_ptr.hpp"
#include "..\glm\gtc\matrix_inverse.hpp"
#include "..\GL\freeglut.h"



#include "..\3DStruct\threeDModel.h";
#include "..\Obj\OBJLoader.h"
#include "..\shaders\Shader.h"
#include "..\shaders\Shader.h"
#include "..\sphere\Sphere.h"

#include "spaceShip.h"



class Camera {
private:
	//Planet light information
	float distanceFromPlayer = 40.0f;
	float angleAroundThePlayer = 0;
	glm::vec3* cameraPointer = &cameraPosition;
	glm::vec3 cameraPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	float yawAngle,rollAngle; 
	float pitchAngle = 50.0f;
	//glm::vec3 spaceShipPos;
	//glm::vec3 spaceShipRotation;
	//SpaceShip spaceShip;

public:
	float getPitch();
	float getYawAngle();
	float getRollAngle();
	void DecreaseZoom(float zoomLevel);
	void IncreaseZoom(float zoomLevel);
	void CalculatePitch(float pitchChange);
	void calculateAngleAroundThePlayer(float angleChange);
	float calculateHorizontalDistance();
	float calculateVerticalDistance();
	glm::vec3 CalculateCameraPosition(float horizontalDistance, float verticalDistance, SpaceShip *spaceShip);
	void move(SpaceShip spaceShip);

	 
	



	
};
#endif //#ifndef SPACESHIP_H


