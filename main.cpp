#include <iostream>
using namespace std;


//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "GL\freeglut.h"

#include "Images\FreeImage.h"

#include "shaders\Shader.h"

CShader* myShader;  ///shader object 
CShader* myBasicShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"

float amount = 0;
float temp = 0.002f;
	
CThreeDModel spaceEnvironment, boxRight, boxFront;
CThreeDModel spaceShip; //A threeDModel object is needed for each model loaded
COBJLoader objLoader;	//this object is used to load the 3d models.
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
glm::mat4 viewingMatrix; //matrix for the viewing

glm::mat4 objectRotation;
glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f); //vector for the position of the object.
glm::vec3 initalDirectionVector = glm::vec3(0.0f,1.0f, 0.0f);
glm::vec3 directionVector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFinalLook;
glm::vec3 distanceOfCamera = glm::vec3(0.0f, 0.0f, 0.0F);


//rotation angle
float angleInDegrees=0.0f;
float moveSpaceShip = 0.01f;

//Material properties
float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
float Material_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
float Material_Shininess = 50;

//Light Properties
float Light_Ambient_And_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
float LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
//Second light properties at the entrance of the planet.
float PlanetLight_Ambient_And_Diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float planetLight_specular[4];
float planetLightPos[4] = { 0.0f, 0.0,1.0f,0.0f};


//
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
bool Home = false;
bool End = false;
bool camera = false;

float spin=180;
float speed=0;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function
void updateTransform(float xinc, float yinc, float zinc);

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(myShader->GetProgramObjID());  // use the shader

	//Part for displacement shader.
	amount += temp;
	if(amount > 1.0f || amount < -1.5f)
		temp = -temp;
	//amount = 0;
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "displacement"), amount);

	//Set the projection matrix in the shader
	GLuint projMatLocation = glGetUniformLocation(myShader->GetProgramObjID(), "ProjectionMatrix");  
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	viewingMatrix = glm::mat4(1.0f);
	
	
	//translation and rotation for view
	//viewingMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -50));

	//apply a rotation to the view
	//static float angle = 0.0f;
	//angle += 0.01;
	//viewingMatrix = glm::rotate(viewingMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0));

	//use of glm::lookAt for viewing instead.
	
	if(distanceOfCamera.z <=15){
		cout << "out of camera view";
		cout << pos.z << endl;
	}
	
	cameraFinalLook = cameraLookAt + pos;
	if (camera){
		cameraPos = pos + glm::vec3(objectRotation[1])* 2.5f;
		viewingMatrix = glm::lookAt(cameraPos, pos + glm::vec3(objectRotation[1])*10.0f, glm::vec3(objectRotation[0]));

	}
	else {
		cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
		viewingMatrix = glm::lookAt(cameraPos, pos, glm::vec3(0.0f, 1.0f, 0.0));
	}
	

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);


	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "material_shininess"), Material_Shininess);

	

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLightPos"), 1, planetLightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "planetLight_diffuse;"), 1,PlanetLight_Ambient_And_Diffuse);

	

	glm::mat4 modelmatrixEnvironment = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -25, 0.0));
	angleInDegrees = 90.0f;
	modelmatrixEnvironment = glm::rotate(modelmatrixEnvironment, glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 0.0));

	modelmatrixEnvironment = glm::scale(modelmatrixEnvironment, glm::vec3(0.5, 0.5, 0.5));
	ModelViewMatrix = viewingMatrix * modelmatrixEnvironment;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	spaceEnvironment.DrawElementsUsingVBO(myShader);

	//Coding for the spaceship


	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_specular"), 1, Light_Specular);
	


	
	pos.x += objectRotation[1][0]*speed;
	pos.y += objectRotation[1][1]*speed;
	pos.z += objectRotation[1][2]*speed;
	

	
	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	ModelViewMatrix = viewingMatrix * modelmatrix * objectRotation;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	
	 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	
	spaceShip.DrawElementsUsingVBO(myShader);



 
	

	//Switch to basic shader to draw the lines for the bounding boxes
	glUseProgram(myBasicShader->GetProgramObjID());
	projMatLocation = glGetUniformLocation(myBasicShader->GetProgramObjID(), "ProjectionMatrix");
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//model.DrawAllBoxesForOctreeNodes(myBasicShader);
	//model.DrawBoundingBox(myBasicShader);
	//model.DrawOctreeLeaves(myBasicShader);

	//switch back to the shader for textures and lighting on the objects.
	glUseProgram(myShader->GetProgramObjID());  // use the shader

	ModelViewMatrix = glm::translate(viewingMatrix, glm::vec3(0, 0, 0));

	normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	
	
	//boxRight.DrawElementsUsingVBO(myShader);
	//boxFront.drawElementsUsingVBO(myShader);

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 200.0f);
}
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	myShader = new CShader();
	//if(!myShader->CreateShaderProgram("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
	if(!myShader->CreateShaderProgram("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	myBasicShader = new CShader();
	if(!myBasicShader->CreateShaderProgram("Basic", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	glUseProgram(myShader->GetProgramObjID());  // use the shader

	glEnable(GL_TEXTURE_2D);

	//lets initialise our object's rotation transformation 
	//to the identity matrix
	pos.y = 14;
	objectRotation = glm::mat4(1.0f);

	cout << " loading model " << endl;
	if(objLoader.LoadModel("TestModels/spaceShip.obj"))//returns true if the model is loaded
	{
		cout << " SpaceShip model loaded " << endl;		

		//copy data from the OBJLoader object to the threedmodel class
		spaceShip.ConstructModelFromOBJLoader(objLoader);

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		
		spaceShip.CalcCentrePoint();
		spaceShip.CentreOnZero();

	
		spaceShip.InitVBO(myShader);
	}
	else
	{
		cout << " model failed to load " << endl;
	}



	
	if (!myShader->CreateShaderProgram("PlanetView", "glslfiles/secondLight.vert", "glslfiles/secondLight.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	glUseProgram(myShader->GetProgramObjID());
	
	


	if (objLoader.LoadModel("TestModels/spaceEnvironmentTest2.obj"))//returns true if the model is loaded
	{
		spaceEnvironment.ConstructModelFromOBJLoader(objLoader);


		
		spaceEnvironment.CalcCentrePoint();
		spaceEnvironment.CentreOnZero();
		
		


		//Place to centre geometry before creating VBOs.

		

		spaceEnvironment.InitVBO(myShader);
	}
	else
	{
		cout << " spaceSurface failed to load " << endl;
	}

	/*
	* 
	* if (objLoader.LoadModel("TestModels/spaceSurface.obj"))//returns true if the model is loaded
	{
		spaceEnvironment.ConstructModelFromOBJLoader(objLoader);

		//Place to centre geometry before creating VBOs.

		spaceEnvironment.InitVBO(myShader);
	}
	else
	{
		cout << " spaceSurface failed to load " << endl;
	}
	
	*/

	

	/*
	
	
	if (objLoader.LoadModel("TestModels/boxLeft.obj"))//returns true if the model is loaded
	{
		boxLeft.ConstructModelFromOBJLoader(objLoader);

		//Place to centre geometry before creating VBOs.

		boxLeft.InitVBO(myShader);
	}
	if (objLoader.LoadModel("TestModels/boxRight.obj"))//returns true if the model is loaded
	{
		boxRight.ConstructModelFromOBJLoader(objLoader);
		boxRight.InitVBO(myShader);
	}
	
	*/
	
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = true;
		break;
	case GLUT_KEY_RIGHT:
		Right = true;
		break;
	case GLUT_KEY_UP:
		Up = true;
		break;
	case GLUT_KEY_DOWN:
		Down = true;
		break;
	case GLUT_KEY_HOME:
		Home = true;
		break;
	case GLUT_KEY_END:
		End = true;
		break;
	}
}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = false;
		break;
	case GLUT_KEY_RIGHT:
		Right = false;
		break;
	case GLUT_KEY_UP:
		Up = false;
		break;
	case GLUT_KEY_DOWN:
		Down = false;
		break;
	case GLUT_KEY_HOME:
		Home = false;
		break;
	case GLUT_KEY_END:
		End = false;
		break;		
	}
}

void processKeys()
{
	float spinXinc = 0.0f, spinYinc = 0.0f, spinZinc = 0.0f, increase= 0.1f;
	if (Left)
	{
		spinYinc = -increase;
	}
	if (Right)
	{
		spinYinc =increase;
	}
	if (Up)
	{
		spinXinc = increase;
	}
	if (Down)
	{
		spinXinc = -increase;
	}
	if (Home)
	{
		spinZinc = increase;
	}
	if (End)
	{
		spinZinc = -increase;
	}
	//Make the z axis move by half the speed. 
	updateTransform(spinXinc, spinYinc, spinZinc/2);
}


void keyFunction(unsigned char key, int x, int y) {
	/*
	* if((key==65) ||(key==97)) {
		cout << "A is pressed." << endl;
		//Camera should be in the object
		//It should look at the direction that it is facing
		viewingMatrix = glm::lookAt(glm::vec3(0, 0, -100), glm::vec3(pos.x, pos.y, pos.z), glm::vec3(0.0f, 1.0f, 0.0));
	}if (key==101) {
		pos.z = pos.z + 10.0f;

	}if (key == 119) {
		//key w to move the spaceship up
		pos.x += pos.x + 0.01f;
		pos.y += pos.y + 0.01;
		pos.z += pos.z + 0.01;
		
	}if (key == 32) {
		//Use the spacebar to increase the speed
		speed = speed + 0.1f;

	}if
	*/

	switch(key) {
	case 65:
		//press A to increase the speed:
		speed += 0.002f;
		break;
	case 64: 
		//press D to decrease the speed:
		speed -= 0.02f;
		break;
	case 32:
		//use the spacebar to change view to cockpitview
		camera = !camera;
		break;
	case 118:
		//press v to change the view from cockpit to the other view;
		break;
	case 119:
		//press w to move forward in the direction that it is facing;
		
		
	case 115:
		//press s to go backwards in the direction that it is facing;
		break;
	}
	
	

}


void updateTransform(float xinc, float yinc, float zinc)
{
	objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1,0,0));
	objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0,1,0));
	objectRotation = glm::rotate(objectRotation, zinc, glm::vec3(0,0,1));
	directionVector = initalDirectionVector * objectRotation[2][1];
}

void idle()
{

	
	 	spin += speed;
	if(spin > 360)
		spin = 0;
		processKeys();
	


	

	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Obj loading");

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;


	//initialise the objects for rendering
	init();

	glutReshapeFunc(reshape);
	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutKeyboardFunc(keyFunction);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}
