#include <iostream>
#include "planets.h"
using namespace std;

void Planets::InitialisePlanet(CShader* myShader)
{

	if (objLoader.LoadModel("TestModels/spaceEnvironmentTest2.obj"))//returns true if the model is loaded
	{
		planets.ConstructModelFromOBJLoader(objLoader);
		planets.CalcCentrePoint();
		planets.CentreOnZero();
		//Place to centre geometry before creating VBOs.
		planets.InitVBO(myShader);

	}
	else
	{
		cout << " spaceSurface failed to load " << endl;
	}
}


	void Planets::PlanetsDisplay(CShader * myShader, glm::mat4 viewingMatrix){
		glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLightPos"), 1, planetLightPos);
		glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_specular"), 1, Light_Specular);

		glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_diffuse;"), 1, PlanetLight_Ambient_And_Diffuse);
		glm::mat4 modelmatrixEnvironment = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -30, 0.0));
		angleInDegrees = 90.0f;
		modelmatrixEnvironment = glm::rotate(modelmatrixEnvironment, glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 0.0));

		modelmatrixEnvironment = glm::scale(modelmatrixEnvironment, glm::vec3(0.5, 0.5, 0.5));
		ModelViewMatrix = viewingMatrix * modelmatrixEnvironment;
		glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
		planets.DrawElementsUsingVBO(myShader);
	}

