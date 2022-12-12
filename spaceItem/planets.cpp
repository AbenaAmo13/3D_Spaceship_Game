#include <iostream>
#include "planets.h"
using namespace std;

void Planets::InitialisePlanet(CShader* myShader, CShader* myBasicShader, string fileName)
{

	if (objLoader.LoadModel(fileName))//returns true if the model is loaded
	{
		planets.ConstructModelFromOBJLoader(objLoader);
		planets.CalcCentrePoint();
		planets.CentreOnZero();
		//Place to centre geometry before creating VBOs.
		planets.InitVBO(myShader);

		float xCentre = planets.GetCentrePoint()->x;
		float yCentre = planets.GetCentrePoint()->y;
		float zCentre = planets.GetCentrePoint()->z;

		planetSphere.setCentre(xCentre, yCentre, zCentre);
		planetSphere.setRadius(153);
		planetSphere.constructGeometry(myBasicShader, 16);
	}
	else
	{
		cout << " spaceSurface failed to load " << endl;
	}
}


	void Planets::PlanetsDisplay(CShader * myShader, glm::mat4 viewingMatrix, glm::vec3 planetPos){
		//glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLightPos"), 1, planetLightPos);
		//glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_specular"), 1, Light_Specular);

		//glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_diffuse;"), 1, PlanetLight_Ambient_And_Diffuse);
		glm::mat4 modelmatrixEnvironment = glm::translate(glm::mat4(1.0f),planetPos);
		//angleInDegrees = 90.0f;
		//modelmatrixEnvironment = glm::rotate(modelmatrixEnvironment, glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 0.0));
		//modelmatrixEnvironment = glm::scale(modelmatrixEnvironment,scaleFactor);
		//glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelMatrix"), 1, GL_FALSE, &modelmatrixEnvironment[0][0]);
		ModelViewMatrix = viewingMatrix * modelmatrixEnvironment;
		glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
		planets.DrawElementsUsingVBO(myShader);
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		planetSphere.render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	}

	Sphere Planets::getPlanetSphere()
	{
		return planetSphere;
	}

	void Planets::PlanetsSetPos(glm::vec3 pos)
	{

		planetPos = pos;
	}


	glm::vec3  Planets::PlanetsGetPos() {
		return planetPos;
	}

