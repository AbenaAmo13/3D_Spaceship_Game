#ifndef PLANETS_H
#define PLANETS_H
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



class Planets {
private:
	//Planet light information
	float PlanetLight_Ambient_And_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float planetLight_specular[4];
	float planetLightPos[4] = { 0.0f,1.0f,1.0f,0.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };

	CThreeDModel planets;  //A threeDModel spaceShip object is needed for each model loaded
	COBJLoader objLoader;	//this object is used to load the 3d models.
	CShader* myShader;  ///shader object 
	glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
	// Matrixes for vieiwng  
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 objectRotation = glm::mat4(1.0f);
	float angleInDegrees = 0.0f; //rotation angle
	//Camera vectors and boolean
	bool camera = false;


public: 
	void InitialisePlanet(CShader* myShader);
	void PlanetsDisplay(CShader* myShader, glm::mat4 viewingMatrix);
	
};
#endif //#ifndef SPACESHIP_H


