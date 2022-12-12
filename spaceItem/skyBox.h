#ifndef SKYBOX_H
#define SKYBOX_H
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
#include "..\sphere\Sphere.h"
#include "planets.h"


class SkyBox {
	private:
	glm::vec3 skyBoxPos;
	COBJLoader objLoader;	//this object is used to load the 3d models.
	CShader* myShader;  ///shader object 
	public: 
		CThreeDModel skyBoxModel; 
		void setGalaxyPos();
		void getGalaxyPos();
		void DisplaySkyBox(CShader* myShader, glm::mat4 viewingMatrix);
		void InitialiseSkyBox(CShader* myShader, CShader* myBasicShader, std::string filename);


	
	
};






#endif //#ifndef SPACESHIP_H


