#include <iostream>
#include "spaceShip.h"
#include <cmath>
#include <stdio.h>      /* printf */
#include <math.h>  
using namespace std;





glm::vec3 SpaceShip::getSpaceShipPos()
{

	return pos;
}

glm::vec3 SpaceShip::getSpaceShiplastPos()
{
	return last_position_before_explosion;
}

Sphere SpaceShip::getSpaceShipSphere()
{
	return spaceShipSphere;
}

glm::vec3 SpaceShip::setSpaceShipPos(glm::vec3 spaceShipPos)
{
	pos = spaceShipPos;
	return pos;
}

glm::mat4 SpaceShip::getSpaceShipObjectRotation()
{

	return objectRotation;
}

void SpaceShip::initialiseSpaceShips(CShader* myShader, CShader* myBasicShader, string fileName, Planets venus){
	exploding_space_ship = false;
	if (objLoader.LoadModel(fileName)) {
		spaceShipModel.ConstructModelFromOBJLoader(objLoader);
		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		spaceShipModel.CalcCentrePoint();
		spaceShipModel.CentreOnZero();
		spaceShipModel.CalcBoundingBox(minx, miny, minz, maxx, maxy, maxz);
		initial_max_c = { maxx, maxy, maxz, 1.0f };
		initial_min_c = { minx, miny, minz, 1.0f };
		spaceShipModel.InitVBO(myShader);
		//Function that initialises all the exploded parts model.
		 xCentre = (spaceShipModel.GetCentrePoint()->x)-1.0f;
		 yCentre = (spaceShipModel.GetCentrePoint()->y)+3.5f;
		 zCentre = spaceShipModel.GetCentrePoint()->z;

		 centreForExplosionTest = { xCentre, yCentre, zCentre};
	

		landingTestPointX = spaceShipModel.GetCentrePoint()->x;
		landingTestPointY = (spaceShipModel.GetCentrePoint()->y) - 3.0f;
		landingTestPointZ = spaceShipModel.GetCentrePoint()->z;
		  

		landingCenter = {landingTestPointX , landingTestPointY, landingTestPointZ};

		
		landingSphere.setCentre(landingTestPointX, landingTestPointY, landingTestPointZ);



		 landingSphere.setRadius(1.2);
		 landingSphere.constructGeometry(myBasicShader, 16);
	
	}
	else {
		cout << " model failed to load " << endl;
	}

	
	//Load all the exploded pieces. 
	initialiseExplodedParts(myShader);
}

void SpaceShip::SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix)
{
	pos.x += objectRotation[1][0] * spaceShipSpeed;
	pos.y += objectRotation[1][1] * spaceShipSpeed;
	pos.z += objectRotation[1][2] * spaceShipSpeed;

	/*cout << "pos x" << pos.x << endl;
	cout << "pos y" << pos.y << endl;
	cout << "pos z" << pos.z << endl;*/

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	ModelViewMatrix = viewingMatrix * modelmatrix* objectRotation;

	explosionTestPoint1 = centreForExplosionTest + pos;
	explosionTestPoint1 = glm::vec3(glm::vec4(explosionTestPoint1, 1.0) * objectRotation);

	landingTestPoint = landingCenter + pos;
	landingTestPoint = glm::vec3(glm::vec4(landingTestPoint, 1.0) * objectRotation);


	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	
	
	glm::vec3 venusPos = venus.PlanetsGetPos();
	float distanceFromPos = glm::length(venusPos - pos);
	float distanceFromTestPoint = glm::length(explosionTestPoint1 - venusPos);
	float distanceFromLandingTestPoint = glm::length(landingTestPoint - venusPos);
	float venusRadius = venus.getPlanetSphere().getRadius();
	float venusRadiusScaled = venusRadius * 310;
	if (distanceFromTestPoint < venusRadiusScaled){
		// Set the exploding_space_ship variable to true if a collision has occurred
		exploding_space_ship = true;
	}

	if (distanceFromPos < venusRadiusScaled) {
		exploding_space_ship = true;
	}
	if (!exploding_space_ship) {
		spaceShipModel.DrawElementsUsingVBO(myShader);
	}
	else {
		drawExplosions(myShader, viewingMatrix);
	}

	
	//Landing testPoint must be in the positive direction and be less than the venus radius scaled. 
	if ((distanceFromLandingTestPoint < venusRadiusScaled) && landingTestPoint.y > 0) {
		landed = true;
		spaceShipSpeed = 0;
	}






	
	
	//Transforming Bounding Box Collison axes
	min_c = modelmatrix * initial_min_c;
	max_c = modelmatrix * initial_max_c;
	//spaceShipModel.DrawBoundingBox(myShader);
}






void SpaceShip::spaceRotationMovement(float xinc, float yinc, float zinc)
{
	
	if (!exploding_space_ship) {
		objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1, 0, 0));
		objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0, 1, 0));
		objectRotation = glm::rotate(objectRotation, zinc, glm::vec3(0, 0, 1));
	}
		
	
	

}

bool SpaceShip::collision_detection(SpaceShip* otherShip)
{
		exploding_space_ship = false;
		if ((min_c.x < otherShip->max_c.x) && (max_c.x > otherShip->min_c.x)&& (min_c.y < otherShip->max_c.y) && (max_c.y > otherShip->min_c.y)&&(min_c.z < otherShip->max_c.z) && (max_c.z > otherShip->min_c.z)) {
			exploding_space_ship = true;
		}
		return exploding_space_ship;
}

void SpaceShip::test_collision(unsigned char key)
{
	if (!exploding_space_ship) {
		if (key == 97) {

			pos.x = pos.x - 1;

		}
		else if (key == 119) {

			pos.y = pos.y + 1;

		}
		else if (key == 115) {
			pos.y = pos.y - 1*spaceShipSpeed ;

		}
		else if (key == 100) {
			pos.x = pos.x + 1;
		}
		else if (key == 113) {
			pos.z = pos.z - 1;
		}
		else if (key == 101) {
			pos.z = pos.z + 1;
		}
	}

	
}

void SpaceShip::initialiseExplodedParts(CShader* myShader)
{
	string explosionfile;
	for (int i = 17; i < 22; i++) {
		cout << "in here" << endl;
		//Create CD ThreeDModel
		exploded_parts[i] = new CThreeDModel;
		if (to_string(i).length() == 1) {
			explosionfile = "TestModels/explosion/explosion_00000" + to_string(i) + ".obj";
		}
		else if (to_string(i).length() == 2) {
			explosionfile = "TestModels/explosion/explosion_0000" + to_string(i) + ".obj";
		}
		else if (to_string(i).length() == 3) {
			explosionfile = "TestModels/explosion/explosion_000" + to_string(i) + ".obj";
		}
		if (objLoader.LoadModel(explosionfile)) {
			exploded_parts[i]->ConstructModelFromOBJLoader(objLoader);
			exploded_parts[i]->CentreOnZero();
			exploded_parts[i]->InitVBO(myShader);
		}
		else {
			cout << " model failed to load " << endl;

		}
	}
}

void SpaceShip::drawExplosions(CShader* myShader, glm::mat4 viewingMatrix ) {
	last_position_before_explosion = pos;
	glm::vec3 center_of_explosion;
	glm::vec3 direction;
	glm::vec3 exploded_pos;
	glm::vec3 final_movement;
	int bounding_box_corners_index = 0;
	glm::vec3 bounding_box_corners[4] = { glm::vec3(max_c.x, max_c.y, min_c.z), glm::vec3(min_c.x, min_c.y, max_c.z), glm::vec3(min_c.x, min_c.y, min_c.z),glm::vec3(max_c.x, max_c.y, max_c.z)};
	spaceShipSpeed = 0.0001f;
	float force = 2;
	for (int i = 17; i < 22; i++) {

		//spaceShipModel.CalcBoundingBox();
		//All particles should have it's initial position at the center
		center_of_explosion.x = spaceShipModel.GetCentrePoint()->x;
		center_of_explosion.y = spaceShipModel.GetCentrePoint()->y;
		center_of_explosion.z = spaceShipModel.GetCentrePoint()->z;
		//Set all the items intially at the center. 
		glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), center_of_explosion);

		//Rotate object in the direction
		if (bounding_box_corners_index > 3) {
			bounding_box_corners_index = 0;
		}
		direction = glm::normalize(center_of_explosion- bounding_box_corners[bounding_box_corners_index]);
		
		
		glm::vec3 lookAtDirectionVector = glm::normalize(glm::vec3(objectRotation[1][0], objectRotation[1][1], objectRotation[1][2]));
		glm::vec3 rotationAxis = glm::cross(direction, lookAtDirectionVector);
		//Calculate arccos(dot(pointDirection, L)).This is your angle of rotation.
		float rotationAngle = acos(glm::dot(direction, lookAtDirectionVector));
		objectRotation = glm::rotate(objectRotation, rotationAngle, rotationAxis);
		

		exploded_pos.x = pos.x+ objectRotation[1][0] * spaceShipSpeed ;
		exploded_pos.y = pos.y + objectRotation[1][1] * spaceShipSpeed;
		exploded_pos.z = pos.z + objectRotation[1][2] * spaceShipSpeed;

		modelmatrix = glm::translate(glm::mat4(1.0f), pos);
		ModelViewMatrix = viewingMatrix * modelmatrix;
		glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
		exploded_parts[i]->DrawElementsUsingVBO(myShader);
		bounding_box_corners + 1;
		
	}
	//spaceShipSpeed = 0.0f;
	
}

//void SpaceShip::drawExplosions2(CShader* explosionShader, glm::mat4 viewingMatrix, glm::mat4 ProjectionMatrix)
//{
//	glUseProgram(explosionShader->GetProgramObjID());
//	GLuint projMatLocation2 = glGetUniformLocation(explosionShader->GetProgramObjID(), "ProjectionMatrix");
//
//	glUniformMatrix4fv(projMatLocation2, 1, GL_FALSE, &ProjectionMatrix[0][0]);
//	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
//	ModelViewMatrix = viewingMatrix * modelmatrix * objectRotation;
//	glUniformMatrix4fv(glGetUniformLocation(explosionShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
//	// add time component to geometry shader in the form of a uniform
//	glUniform1f(glGetUniformLocation(explosionShader->GetProgramObjID(),"time"), static_cast<float>(GLUT_ELAPSED_TIME));
//	
//	explodingModel.DrawElementsUsingVBO(explosionShader);
//	//glUseProgram(0);
//}

void SpaceShip::calcExplosionRandomPoints()
{
	// Providing a seed value
	srand((unsigned)time(NULL));
	randomExplosionPoint.x = 0 + (rand() % 10);
	randomExplosionPoint.y = 0 + (rand() % 10);
	randomExplosionPoint.z = 0 + (rand() % 10);;
}

void SpaceShip::landSpaceShip()
{
		//Reducing the speed
	if (!landed) {
		spaceShipSpeed = spaceShipSpeed - 0.005f;
		pos.y = pos.y - 0.05f;
	}
		
}

glm::vec4 SpaceShip::explode(glm::vec4 position, glm::vec3 normal)
{
	
	float time = static_cast<float>(GLUT_ELAPSED_TIME);
		float magnitude = 2.0;
		//glm:: vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
		//return position + glm::vec4(direction, 0.0);
		return glm::vec4();
	
}

bool SpaceShip::checkPlanetCollision(glm::vec3 pos, Sphere planetSphere, glm::vec3 planet_pos)
{
	 
	//exploding_space_ship = false;
		//Calculate the distance between the centre
		//float distance = pow((planetSphere.getCentre().x - pos.x), 2) + pow((planetSphere.getCentre().y - pos.y), 2) + pow((planetSphere.getCentre().z - pos.z), 2);
		float distance = glm::length(planet_pos - pos);
		//float sqrt_distance = sqrt(distance);
		float radius = (planetSphere.getRadius());
		//cout << distance << endl;
		//float centre2 = planetSphere.getCentre();
		if (distance < radius) {
			exploding_space_ship = true;
		}

		return exploding_space_ship;

}



bool SpaceShip::CheckPlanetCollisionTestPoints(Sphere planetSphere, glm::vec3 planet_pos) {
	exploding_space_ship = false;
	float distanceFromTestPoint = glm::length(planet_pos- explosionTestPoint1);
	float radius = planetSphere.getRadius();
	cout << explosionTestPoint1.y << endl;
	if (distanceFromTestPoint < radius) {
		exploding_space_ship = true;
	}
	return exploding_space_ship;

}



void SpaceShip:: spaceSpeed(unsigned char key) {
	if (key == 65 && !exploding_space_ship) {
		//press A to increase the speed:
		spaceShipSpeed += 0.0005f;

	}
	else if (key == 64 && !exploding_space_ship) {

		//press D to decrease the speed:
		spaceShipSpeed -= 0.0005f;

	}

}

void ComputerControlledSpaceShip::calcRandomPoint()
{
	// Providing a seed value
	srand((unsigned)time(NULL));
	randomlyGeneratedPoint.x = 0 + (rand() % 1000);
	randomlyGeneratedPoint.y = 0 + (rand() % 1000);
	randomlyGeneratedPoint.z = 0 + (rand() % 1000);;
}




void ComputerControlledSpaceShip::automaticSpaceRotationMovement()
{
	spaceShipSpeed = 0.0002f;
	distance_to_random_point = glm::vec3(randomlyGeneratedPoint.x - pos.x, randomlyGeneratedPoint.y - pos.y, randomlyGeneratedPoint.z - pos.z);
	//cout << distance_to_random_point.y << endl;

	if (distance_to_random_point.y < 1) {
		calcRandomPoint();
		//cout << randomlyGeneratedPoint.x<< " " << randomlyGeneratedPoint.y << " " << randomlyGeneratedPoint.z << endl;
		lookAtDirectionVector = glm::normalize(glm::vec3(objectRotation[1][0], objectRotation[1][1], objectRotation[1][2]));
		//lookAtDirectionVector = glm::normalize((objectRotation[0], objectRotation[1], objectRotation[2]));	//current direction vector of object
		//Calculate the normalized vector from your object to the target. This is simply normalize(P2 - P1). 
		pointDirection = glm::normalize(randomlyGeneratedPoint - pos); // will be normalized vector from object to target
		//Take the cross product of pointDirection and LookAtDirection
		rotationAxis = glm::cross(pointDirection, lookAtDirectionVector);
		//Calculate arccos(dot(pointDirection, L)).This is your angle of rotation.
		rotationAngle = acos(glm::dot(pointDirection, lookAtDirectionVector));
		if (rotationAngle > 1.0) {
			objectRotation = glm::rotate(objectRotation, rotationAngle, rotationAxis);
		}

	}
	
}
	

