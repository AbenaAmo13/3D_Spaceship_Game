#include <iostream>
#include "spaceShip.h"

using namespace std;




void SpaceShip::setSpaceShipPos(glm::vec3 position)
{
	pos = position;

}

glm::vec3 SpaceShip::getSpaceShipPos()
{

	return pos;
}

glm::mat4 SpaceShip::getSpaceShipObjectRotation()
{
	return objectRotation;
}

void SpaceShip::InitialiseSpaceShip(CShader* myShader, glm::vec3 initialPos, CShader* myBasicShader)
{

		if (objLoader.LoadModel("TestModels/spaceShip.obj"))//returns true if the model is loaded
		{
			cout << " model loaded " << endl;

			//copy data from the OBJLoader object to the threedmodel class
			spaceShip.ConstructModelFromOBJLoader(objLoader);

			//if you want to translate the object to the origin of the screen,
			//first calculate the centre of the object, then move all the vertices
			//back so that the centre is on the origin.
			spaceShip.CalcCentrePoint();
			spaceShip.CentreOnZero();


			spaceShip.InitVBO(myShader);
		}

		double minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
		spaceShip.CalcBoundingBox(minx, miny, minz, maxx, maxy, maxz);
		float radius_sqr = (maxy * maxy) + (maxx * maxx);
		float radius = sqrt(radius_sqr) / 2;

		
	


		float zCentre = spaceShip.GetCentrePoint()->z;
		float xCentre = spaceShip.GetCentrePoint()->x;
		float yCentre = spaceShip.GetCentrePoint()->y;
		objectSphere.setCentre(xCentre, yCentre, zCentre);
		objectSphere.setRadius(radius);
		objectSphere.constructGeometry(myBasicShader, 16);

	}






void SpaceShip::SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix, glm::vec3 initialPos)
{

	pos = initialPos;

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_specular"), 1, Light_Specular);


	pos.x += objectRotation[1][0] * spaceShipSpeed;
	pos.y += objectRotation[1][1] * spaceShipSpeed;
	pos.z += objectRotation[1][2] * spaceShipSpeed;

	
	objectSphere.setCentre(pos.x, pos.y, pos.z);


	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	ModelViewMatrix = viewingMatrix * modelmatrix * objectRotation;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	spaceShip.DrawElementsUsingVBO(myShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	objectSphere.render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




}





//This is used to 
void SpaceShip::spaceRotationMovement(float xinc, float yinc, float zinc)
{
	objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1, 0, 0));
	objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0, 1, 0));
	objectRotation = glm::rotate(objectRotation, zinc, glm::vec3(0, 0, 1));

}

//This would be used to get the specific three model's sphere
Sphere SpaceShip::getObjectsSphere()
{
	
	return objectSphere;
}

void SpaceShip::spaceSpeed(unsigned char key) {
	if (key == 65) {
		//press A to increase the speed:
		spaceShipSpeed += 0.002f;

	}
	else if (key == 64) {

		//press D to decrease the speed:
		spaceShipSpeed -= 0.02f;

	}
}

void ComputerControlledSpaceShip::spaceShipAutomaticMovement()
{
	glm::vec3 pointDirection;

	//spaceShipSpeed = 0.02f;

	/*
	* 	pos.x = pos.x+ (randomlyGeneratedPoint.x - pos.x) * 0.1;
	pos.y = pos.y + (randomlyGeneratedPoint.y - pos.y) * 0.1;
	pos.x = pos.z + (randomlyGeneratedPoint.z - pos.z) * 0.1;
	old_pos = pos;
	if (old_pos == pos) {

		calcRandomPoint();
	}

	*/
	pointDirection = glm::normalize(pos - randomlyGeneratedPoint);
	/*
	*/
	if ((pos - randomlyGeneratedPoint).length() < 0)
	{
		//exit - don't do any rotation
		//distance is too small for rotation to be numerically stable
	}
	cout << randomlyGeneratedPoint.y << endl;
	float rotationAngle = acos(glm::dot(pos, pointDirection));
	if (isnan(rotationAngle)) {
		calcRandomPoint();
	}

	/*
	*/
	glm::vec3 rotationAxis = glm::cross(pos, pointDirection);
	rotationAxis = glm::normalize(rotationAxis);
	objectRotation = glm::rotate(objectRotation, rotationAngle, rotationAxis);


	//objectRotation = glm::rotate(objectRotation, rotationAngle, glm::vec3(0, 1, 0));

}


void ComputerControlledSpaceShip::calcRandomPoint()
{
	old_pos = getSpaceShipPos();


	// Providing a seed value
	srand((unsigned)time(NULL));

	randomlyGeneratedPoint.x = 0 + (rand() % 100);
	cout << randomlyGeneratedPoint.x << endl;
	randomlyGeneratedPoint.y = 0 + (rand() % 100);

	randomlyGeneratedPoint.z = 0 + (rand() % 100);;
	//cout << randomlyGeneratedPoint
}

bool collisionDetection(Sphere sphere1, Sphere sphere2) {
	//Calculate the euclidean distance between the centre
	float distance = pow((sphere2.getCentre().x - sphere1.getCentre().x), 2) + pow((sphere2.getCentre().y - sphere1.getCentre().y), 2) + pow((sphere2.getCentre().z - sphere1.getCentre().z), 2);
	float sqrt_distance = sqrt(distance);
	float radius = sphere1.getRadius() + sphere2.getRadius();

	cout << radius << endl;
	cout << sqrt_distance << endl;

	//float centre2 = sphere2.getCentre();
	if (sqrt_distance < radius) {
		cout << "collision" << endl;
		return true;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//sphere2.render()
	}

}






