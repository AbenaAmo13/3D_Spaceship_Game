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
#include "..\sphere\Sphere.h"
#include "planets.h"


class SpaceShip {
protected:
	float spaceShipSpeed = 0;
	bool landed = false;
	float xCentre, yCentre, zCentre, landingTestPointX, landingTestPointY, landingTestPointZ;
	double minx, maxx, miny, maxy, minz, maxz; 	//Information necessary for collision
	CThreeDModel spaceShipModel;  //A threeDModel spaceShip object is needed for each model loaded
	CThreeDModel *exploded_parts[119];
	COBJLoader objLoader;	//this object is used to load the 3d models.
	CShader* myShader;  ///shader object 
	Sphere spaceShipSphere;
	Sphere landingSphere;
	Planets venus;
	glm::vec3 centreForExplosionTest;
	glm::vec3 landingCenter;
	glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 landingTestPoint = glm::vec3(0.0, 0.0, 0.0);
	//glm::vec3 pos = glm::vec3(0.0f, 200.0f, 0.0f);
	glm::vec3 pos = glm::vec3(0.0f, 173.0f, 0.0f); //vector for the position of the object.
	//glm::vec3 pos = glm::vec3(0.0f, 38.5f, 0.0f); //vector for the position of the object.
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 objectRotation = glm::mat4(1.0f);
	glm::vec4 initial_max_c = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//Initial coordinates
	glm::vec4 initial_min_c = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 max_c = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 min_c = glm::vec4(0.0f, 0.0f, 0.0f,1.0f);
	
	

public: 
	glm::vec3 getSpaceShipPos();
	glm::vec3 getSpaceShiplastPos();
	Sphere getSpaceShipSphere();
	glm::vec3 setSpaceShipPos(glm::vec3 spaceShipPos);
	glm::mat4 getSpaceShipObjectRotation();
	void  initialiseSpaceShips(CShader* myShader, CShader* myBasicShader,std::string filename, Planets venus);
	void SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix);
	void spaceSpeed(unsigned char key);
	void spaceRotationMovement(float xinc, float yinc, float zinc);
	bool collision_detection(SpaceShip* automaticSpaceShip);
	void test_collision(unsigned char key);
	void initialiseExplodedParts(CShader* myShader);
	void drawExplosions(CShader* myShader, glm::mat4 viewingMatrix);
	//void drawExplosions3(CShader* myShader, glm::mat4 viewingMatrix);
	//void drawExplosions2(CShader* explodeShader, glm::mat4 viewingMatrix, glm::mat4 ProjectionMatrix);
	void calcExplosionRandomPoints();
	void landSpaceShip();
	glm::vec3 randomExplosionPoint;
	glm::vec3 explosionTestPoint1;
	glm::vec3 last_position_before_explosion;
	glm::vec4 explode(glm::vec4 position, glm::vec3 norml);
	bool checkPlanetCollision(glm::vec3 pos, Sphere planetSphere, glm::vec3 planetPos);
	bool CheckPlanetCollisionTestPoints(Sphere planetSphere, glm::vec3 planet_pos);
	bool exploding_space_ship = false;
	bool space_to_space_collision = false;

	
	
};


class ComputerControlledSpaceShip :public SpaceShip
{
private:
	//CThreeDModel automaticSpaceShip;
	void calcRandomPoint();
	glm::vec3 randomlyGeneratedPoint = glm::vec3(20.0f, 200.0f, 0.0);//initial random point
	glm::vec3 pointDirection;
	glm::vec3 lookAtDirectionVector;
	glm::vec3 rotationAxis;
	glm::vec3 distance_to_random_point;
	glm::vec3 old_pos;
	float rotationAngle;
public:
	
	//void damagedSpaceShip();
	void automaticSpaceRotationMovement();
};

class ExplodedSpaceShip:public SpaceShip {
private:
	Vector3d* m_pvVertNormals_exploded;
public:
	//void RandomizeExplosionParts();
	//void EmitExplosionParts();

};





#endif //#ifndef SPACESHIP_H


