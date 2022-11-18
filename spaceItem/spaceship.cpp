#include <iostream>
#include "spaceShip.h"
using namespace std;




glm::vec3 SpaceShip::getSpaceShipPos()
{

	return pos;
}

glm::mat4 SpaceShip::getSpaceShipObjectRotation()
{
	return objectRotation;
}

void  SpaceShip::InitialiseSpaceShips(CShader* myShader, CShader* myBasicShader)
{
	if (objLoader.LoadModel("TestModels/spaceShip.obj")) {
		std::cout << " SpaceShip model loaded " << endl;
		//copy data from the OBJLoader object to the threedmodel class
		spaceShip.ConstructModelFromOBJLoader(objLoader);

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		spaceShip.CalcCentrePoint();
		spaceShip.CentreOnZero();
		spaceShip.InitVBO(myShader);

		automaticSpaceShip.ConstructModelFromOBJLoader(objLoader);
		automaticSpaceShip.InitVBO(myShader);
	}else
	{
		cout << " model failed to load " << endl;
	}
}




void SpaceShip::SpaceShipDisplay(CShader* myShader, glm::mat4 viewingMatrix)
{
	

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_specular"), 1, Light_Specular);

	
	pos.x += objectRotation[1][0] * spaceShipSpeed;
	pos.y += objectRotation[1][1] * spaceShipSpeed;
	pos.z += objectRotation[1][2] * spaceShipSpeed;
	

	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	ModelViewMatrix = viewingMatrix * modelmatrix * objectRotation;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	spaceShip.DrawElementsUsingVBO(myShader);
	spaceShip.DrawBoundingBox(myShader);



	modelmatrix = glm::translate(glm::mat4(1.0f), automated_pos);
	//objectRotation = automateDirection();
	ModelViewMatrix = viewingMatrix * modelmatrix;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	automaticSpaceShip.DrawElementsUsingVBO(myShader);
	automaticSpaceShip.DrawBoundingBox(myShader);


}






void SpaceShip::spaceRotationMovement(float xinc, float yinc, float zinc)
{
	objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1, 0, 0));
	objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0, 1, 0));
	objectRotation = glm::rotate(objectRotation, zinc, glm::vec3(0, 0, 1));

}

bool SpaceShip::collision_detection()
{
	return false;
}

void SpaceShip:: spaceSpeed(unsigned char key) {
	if (key == 65) {
		//press A to increase the speed:
		spaceShipSpeed += 0.002f;

	}
	else if (key == 64) {

		//press D to decrease the speed:
		spaceShipSpeed -= 0.02f;

	}
}
