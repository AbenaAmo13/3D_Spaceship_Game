#ifndef SPACESHIP_H
#define SPACESHIP_H
/************************************************************
*  Class: SpaceShip
*
*  Purpose: The spaceship class to deal with everything concerning the 
* spaceship. 
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
#include "Sphere.h"


class SpaceShip {
private:
	float Light_Ambient_And_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float spaceShipSpeed = 0;
	CThreeDModel spaceShip;  //A threeDModel spaceShip object is needed for each model loaded
	CThreeDModel automaticSpaceShip;
	Sphere userControlledsphere;
	Sphere automaticSphere;
	 COBJLoader objLoader;	//this object is used to load the 3d models.
	CShader* myShader;  ///shader object 
	glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 automated_pos = glm::vec3(5.0f, 20.0f, 0.0f);
	float speed_automated = 0;
	glm::vec3 pos = glm::vec3(0.0f, 15.0f, 0.0f); //vector for the position of the object.
	// Matrixes for vieiwng  
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 objectRotation = glm::mat4(1.0f);
	//Camera vectors and boolean
	bool camera = false;
	glm::vec3 cameraFinalLook;
	glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
	glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);


public: 
	glm::vec3 getSpaceShipPos();
	glm::mat4 getSpaceShipObjectRotation();
	void  InitialiseSpaceShips(CShader* myShader, CShader* myBasicShader);
	void SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix);
	void spaceSpeed(unsigned char key);
	void spaceRotationMovement(float xinc, float yinc, float zinc);
	bool collision_detection();
};


class ComputerControlledSpaceShip :public SpaceShip
{
private:
	glm::vec3 randomlyGeneratedPoint;
	glm::vec3 old_pos;
public:
	//void calcRandomPoint();
	//void spaceShipAutomaticMovement();
};
#endif //#ifndef SPACESHIP_H


