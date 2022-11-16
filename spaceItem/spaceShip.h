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

#include <iostream>
#include "..\3DStruct\threeDModel.h";
#include "..\Obj\OBJLoader.h"
#include "..\shaders\Shader.h"
#include "Sphere.h"
#include <string>


class SpaceShip {
protected:
	float Light_Ambient_And_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float spaceShipSpeed = 0;
	CShader* myBasicShader;

	CThreeDModel spaceShip;//A threeDModel spaceShip object is needed for each model loaded
	Sphere objectSphere;
	COBJLoader objLoader;	//this object is used to load the 3d models.
	CShader* myShader;  ///shader object 
	glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 pos; //vector for the position of the object.
	// Matrixes for vieiwng  
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 objectRotation = glm::mat4(1.0f);

	//Camera vectors and boolean
	bool camera = false;
	glm::vec3 cameraFinalLook;
	glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
	glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);

public: 
	//SpaceShip();

	void setSpaceShipPos(glm::vec3 position);
	bool collisionDetection(Sphere sphere1,  Sphere sphere2);
	glm::vec3 getSpaceShipPos();
	Sphere getObjectsSphere();
	glm::mat4 getSpaceShipObjectRotation();
	void InitialiseSpaceShip(CShader* myShader, glm::vec3 initalPos, CShader* myBasicShader);
	void SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix, glm::vec3 pos);
	void spaceSpeed(unsigned char key);
	void spaceRotationMovement(float xinc, float yinc, float zinc);
};
#endif //#ifndef SPACESHIP_H


class ComputerControlledSpaceShip:public SpaceShip
{
private:
	
	float speed_automated = 0;
	glm::vec3 randomlyGeneratedPoint;
	glm::vec3 old_pos;
	public: 
		void calcRandomPoint();
		void spaceShipAutomaticMovement();
};

class UserControleldSpaceShip:public SpaceShip {
	private:
		CThreeDModel spaceShip;
public:
	UserControleldSpaceShip() {
		if (objLoader.LoadModel("TestModels/spaceShip.obj"))//returns true if the model is loaded
		{
			//std::cout << " model loaded " << endl;

			//copy data from the OBJLoader object to the threedmodel class
			spaceShip.ConstructModelFromOBJLoader(objLoader);

			//if you want to translate the object to the origin of the screen,
			//first calculate the centre of the object, then move all the vertices
			//back so that the centre is on the origin.
			//model.CalcCentrePoint();
			//model.CentreOnZero();


			spaceShip.InitVBO(myShader);
		}

	}

};