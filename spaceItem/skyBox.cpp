#include "skyBox.h"

using namespace std;

void SkyBox::DisplaySkyBox(CShader* myShader, glm::mat4 viewingMatrix)
{

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);


}

void SkyBox::InitialiseSkyBox(CShader* myShader, CShader* myBasicShader, std::string filename)

{
	if (objLoader.LoadModel(filename)) {
		
		skyBoxModel.ConstructModelFromOBJLoader(objLoader);
		skyBoxModel.InitVBO(myShader);
	
	}
	else {
		cout << " model failed to load " << endl;
	}
	
}
