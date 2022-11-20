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
		spaceShip.CalcBoundingBox(minx, miny, minz, maxx, maxy, maxz);
		initial_max_c = { maxx, maxy, maxz, 1.0f};
		initial_min_c = { minx, miny, minz,1.0f };
		//Automatic spaceship code. 
		automaticSpaceShip.ConstructModelFromOBJLoader(objLoader);
		automaticSpaceShip.InitVBO(myShader);
		automaticSpaceShip.CalcBoundingBox(minx2, miny2, minz2, maxx2, maxy2, maxz2);
		initial_automax_c = { max2, maxy2, maxz2, 1.0f };
		initial_automin_c = { minx2, miny2, minz2,1.0f };

		
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
	
	//Transforming Bounding Box Collison axes
	min_c = ModelViewMatrix * initial_min_c;
	max_c = ModelViewMatrix * initial_max_c;

	modelmatrix = glm::translate(glm::mat4(1.0f), automated_pos);
	ModelViewMatrix = viewingMatrix * modelmatrix;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	automaticSpaceShip.DrawElementsUsingVBO(myShader);
	automaticSpaceShip.DrawBoundingBox(myShader);
	//Transforming Bounding Box Collison axes for automatic spaceship
	auto_min_c = ModelViewMatrix * initial_automin_c;
	auto_max_c = ModelViewMatrix * initial_automax_c;

	if (collision_detection()) {
		drawExplosions();
	};


}






void SpaceShip::spaceRotationMovement(float xinc, float yinc, float zinc)
{
	objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1, 0, 0));
	objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0, 1, 0));
	objectRotation = glm::rotate(objectRotation, zinc, glm::vec3(0, 0, 1));

}

bool SpaceShip::collision_detection()
{
	
	if ((min_c.x <= auto_max_c.x) && (max_c.x >= auto_min_c.x)&& (min_c.y <= auto_max_c.y) && (max_c.y >= auto_min_c.y)&&(min_c.z <= auto_max_c.z) && (max_c.z >= auto_min_c.z)) {
			cout << "collision detected";
			return true;

		}
		else {
			return false;
		}
	

	
		
}

void SpaceShip::test_collision(unsigned char key)
{
	if (key ==97) {
		
		pos.x =pos.x-1;

	}else if (key == 119) {

		
		pos.y = pos.y+1;

	}
	else if (key == 115) {
		pos.y =pos.y-1;
	}else if (key == 100) {
		pos.x = pos.x+1;
	}
	else if (key == 113) {
		pos.z = pos.z - 1;
	}
	else if (key == 101) {
		pos.z = pos.z + 1;
	}
	
}

void SpaceShip::drawExplosions()
{
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
