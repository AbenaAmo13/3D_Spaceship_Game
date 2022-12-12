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
#include "spaceItem/spaceShip.h"
#include "spaceItem/planets.h"

#include "GL\freeglut.h"

#include "Images\FreeImage.h"

#include "shaders\Shader.h"
#include "spaceItem/Camera.h"
#include "spaceItem/skyBox.h"

CShader* myShader;  ///shader object 
CShader* myBasicShader;
CShader* ExplodeGeoShader;
SpaceShip spaceShip;
ComputerControlledSpaceShip automaticSpaceShip;
ComputerControlledSpaceShip damagedAutomaticSpaceShip;
Planets saturn, neptune, venus;
Camera ThirdPersonCamera;
SkyBox galaxy;


//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"

//Classes loading


float amount = 0;
float temp = 0.002f;
float zoomLevel;

	
CThreeDModel spaceEnvironment, boxRight, boxFront, automatedSpaceShip;
//Camera ThirdPersonCamera(spaceShip);

COBJLoader objLoader;	//this object is used to load the 3d models.
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
glm::mat4 viewingMatrix; //matrix for the viewing

glm::mat4 objectRotation;
glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFinalLook;
glm::vec3 distanceOfCamera = glm::vec3(0.0f, 0.0f, 0.0F);
//glm::vec3 init_automatic_pos = glm::vec3(5, 38.0, 0.0f);
glm::vec3 init_automatic_pos = glm::vec3(20.0f, 173.0f, 0.0f);
glm::vec3 saturnPos = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 saturnScaleFactor = glm::vec3(0.5, 0.5, 0.5);
//glm::vec3 venusPos = glm::vec3(0.0, 30.0, 0.0);
//glm::vec3 venusScaleFactor = glm::vec3(5, 5, 5);



//glm::vec3 venusPos = glm::vec3(0.0, -30, 0.0);
glm::vec3 venusPos = glm::vec3(0.0, 0.0, 0.0);
//glm::vec3 venusScaleFactor = glm::vec3(20, 20, 20);
//glm::vec3 venusScaleFactor = glm::vec3(1, 1, 1);

glm::vec3 neptunePos = glm::vec3(10.0, -30, 0.0);
glm::vec3 neptuneScaleFactor = glm::vec3(5, 5, 5);
glm::vec3 marsPos = glm::vec3(0., 50.0, 0.0);
glm::vec3 skyBoxPos = glm::vec3(0, 200.0, 0.0);

//rotation angle
float angleInDegrees=0.0f;


//Material properties
float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
float Material_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
float Material_Shininess = 50;

//Light Properties
float Light_Ambient_And_Diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f };
float Light_Specular[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
float LightPos[4] = { 0.0f, 1.0f, 1.0f, 0.0f };
//Second light properties at the entrance of the planet.
//float PlanetLight_Ambient_And_Diffuse[4] = {0.0f, 0.0f, 1.0f, 1.0f};
//float planetLight_specular[4];
//float planetLightPos[4] = { 0.0f,1.0f,1.0f,0.0f};

//Third Light_Properties



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
bool tab = false;
bool escape = false;
bool overhead = false;
bool sideView = false;
bool explodes = false;

float spin=180;
float speed;
float speed_automated = 0.0f;
float pitchAngle, yawAngle, rollAngle, pitchChange, cameraAngle = 0.0f;


//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();//called in winmain when the program starts.

void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function
glm::mat4 changeCameraView();
glm::mat4 automateDirection();


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

	
	//use of glm::lookAt for viewing instead.
	viewingMatrix = changeCameraView();
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
	
	

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_specular"), 1, Light_Specular);


	//glUniform3f(glGetUniformLocation(myShader->GetProgramObjID(), "spotLight.direction"), spaceShip.getSpaceShipPos() + glm::vec3(spaceShip.getSpaceShipObjectRotation()[1]) * 10.0f);
	
	//glUniform3f(glGetUniformLocation(myShader->GetProgramObjID(), "viewPos"), cameraPos[0], cameraPos[1], cameraPos[2]);
	
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "material_shininess"), Material_Shininess);
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "constantAttenuation"), 0.5f);
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "linearAttenuation"), 0.05f);
	glUniform1f(glGetUniformLocation(myShader->GetProgramObjID(), "quadraticAttenuation"), 0.005f);
	






	automaticSpaceShip.automaticSpaceRotationMovement();
	spaceShip.collision_detection(&automaticSpaceShip);
	automaticSpaceShip.collision_detection(&spaceShip);
	spaceShip.SpaceShipDisplay(myShader, viewingMatrix);
	automaticSpaceShip.SpaceShipDisplay(myShader, viewingMatrix);
	
	glUniform3f(glGetUniformLocation(myShader->GetProgramObjID(), "SpotLightPos"), spaceShip.getSpaceShiplastPos().x, spaceShip.getSpaceShiplastPos().y, spaceShip.getSpaceShiplastPos().z);
	glUniform3f(glGetUniformLocation(myShader->GetProgramObjID(), "SpotLightDir"),objectRotation[1][0], objectRotation[1][1], + objectRotation[1][2]);


	venus.PlanetsDisplay(myShader, viewingMatrix, venusPos);

	//spaceShip.SpaceShipDisplay(myShader, viewingMatrix);
	//cout << glm::vec3(spaceShip.getSpaceShipObjectRotation()[1]).x << yawAngle<< rollAngle<< endl;

	//glUseProgram(myBasicShader->GetProgramObjID());
	//projMatLocation = glGetUniformLocation(myBasicShader->GetProgramObjID(), "ProjectionMatrix");
	//glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(myBasicShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//model.DrawAllBoxesForOctreeNodes(myBasicShader);
	//model.DrawBoundingBox(myBasicShader);
	//model.DrawOctreeLeaves(myBasicShader);

	//switch back to the shader for textures and lighting on the objects.
	//glUseProgram(myShader->GetProgramObjID());  // use the shader

	//ModelViewMatrix = glm::translate(viewingMatrix, glm::vec3(0, 0, 0));

	//glm::mat4 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	//glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	//glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
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
	glClearColor(0.0,0.0,0.75,0.0);						//sets the clear colour to yellow
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

	/*ExplodeGeoShader = new CShader();

	if (!ExplodeGeoShader->CreateShaderProgram("ExplosionView", "glslfiles/explosion.vert", "glslfiles/explosion.geom", "glslfiles/explosion.frag"))
	{
		cout << "failed to load shader" << endl;
	}*/

	glUseProgram(myShader->GetProgramObjID());  // use the shader

	glEnable(GL_TEXTURE_2D);

	//lets initialise our object's rotation transformation 
	//to the identity matrix
	objectRotation = glm::mat4(1.0f);

	galaxy.InitialiseSkyBox(myShader, myBasicShader, "TestModels/galaxy_mini.obj");

	venus.PlanetsSetPos(venusPos);
	venus.InitialisePlanet(myShader, myBasicShader, "TestModels/scaled-venus.obj");

	spaceShip.initialiseSpaceShips(myShader, myBasicShader, "TestModels/spaceShip.obj", venus);

	automaticSpaceShip.setSpaceShipPos(init_automatic_pos);
	automaticSpaceShip.initialiseSpaceShips(myShader, myBasicShader, "TestModels/spaceShip.obj", venus);

	/*if (!myShader->CreateShaderProgram("SpotLightView", "glslfiles/spaceShipLight.vert", "glslfiles/spaceShipLight.frag"))
	{
		cout << "failed to load shader" << endl;
	}
	glUseProgram(myShader->GetProgramObjID());

	
	
	if (!myShader->CreateShaderProgram("PlanetView", "glslfiles/secondLight.vert", "glslfiles/secondLight.frag"))
	{
		cout << "failed to load shader" << endl;
	}*/

	//glUseProgram(myShader->GetProgramObjID());
	

	//neptune.InitialisePlanet(myShader, myBasicShader, "TestModels/neptune.obj");



	
}
glm::mat4 automateDirection() {
	float xinc = (float)rand() / RAND_MAX;
	float yinc = (float)rand() / RAND_MAX;


	speed_automated = rand() / RAND_MAX;

	objectRotation = glm::rotate(objectRotation, xinc, glm::vec3(1, 0, 0));
	objectRotation = glm::rotate(objectRotation, yinc, glm::vec3(0, 1, 0));
	return objectRotation * speed_automated;
}

glm::mat4 changeCameraView() {
	float cameraDistance = 30.0f;
	float angleAroundThePlayer = 0.0f;
	glm::vec3 distanceAwayFromCamera = {2,2,2};
	if (camera) {
		//Cockpit view
		cameraPos = spaceShip.getSpaceShipPos() + glm::vec3(spaceShip.getSpaceShipObjectRotation()[1]) * 2.5f;

		if (spaceShip.exploding_space_ship) {
			viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShiplastPos() + glm::vec3(spaceShip.getSpaceShipObjectRotation()[1]) * 10.0f, glm::vec3(spaceShip.getSpaceShipObjectRotation()[0]));
		}
		else {
			viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShipPos() + glm::vec3(spaceShip.getSpaceShipObjectRotation()[1]) * 10.0f, glm::vec3(spaceShip.getSpaceShipObjectRotation()[0]));


		}
	}
	else if (sideView) {
		float verticalDistance = ThirdPersonCamera.calculateHorizontalDistance();
		float horizontalDistance = ThirdPersonCamera.calculateHorizontalDistance();
		cameraPos = ThirdPersonCamera.CalculateCameraPosition(horizontalDistance, verticalDistance, &spaceShip);
		if (spaceShip.exploding_space_ship) {
			viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShiplastPos(), glm::vec3(0.0f, 1.0f, 0.0));
		}
		else {
			viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShipPos(), glm::vec3(0.0f, 1.0f, 0.0));
		}
		
	}else {

		cameraPos = glm::vec3(0.0f, 200.0f, 50.0f);
		

		if (spaceShip.checkPlanetCollision(spaceShip.getSpaceShipPos(), venus.getPlanetSphere(), venusPos)) {
		viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShiplastPos(), glm::vec3(0.0f, 1.0f, 0.0));
	}
	else {
		viewingMatrix = glm::lookAt(cameraPos, spaceShip.getSpaceShipPos(), glm::vec3(0.0f, 1.0f, 0.0));
		
	}
	}
	return viewingMatrix;

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

	
	float spinXinc = 0.0f, spinYinc = 0.0f, spinZinc = 0.0f, increase= 0.005f;
	if (Left)
	{
		spinYinc = -increase;
		
	}
	if (Right)
	{
		spinYinc =increase;
		pitchAngle = increase;
	}
	if (Up)
	{
		spinXinc = increase;
		rollAngle = increase;
	}
	if (Down)
	{
		spinXinc = -increase;
		rollAngle - increase;
		
	}
	if (Home|| tab)
	{
		spinZinc = increase;
		yawAngle = increase;
	}
	if (End || escape )
	{
		spinZinc = -increase;
		yawAngle = -increase;
	}
	//Make the z axis move by half the speed. 
	spaceShip.spaceRotationMovement(spinXinc, spinYinc, spinZinc/2);
}



void mouse_click(int button, int  state, int x, int y) {
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			cameraAngle += 0.1f;

			ThirdPersonCamera.calculateAngleAroundThePlayer(cameraAngle);

			//std::cout << "left click at: (" << x << ", " << y << ")\n";
			break;
		case GLUT_RIGHT_BUTTON:
			//Camera Pitch Change
			pitchChange = pitchChange + 0.1f;
			ThirdPersonCamera.CalculatePitch(pitchChange);
			//std::cout << "right click at: (" << x << ", " << y << ")\n";
			break;
		case GLUT_MIDDLE_BUTTON:
			//std::cout << "middle click at: (" << x << ", " << y << ")\n";
			break;
		case 3:  //mouse wheel scrolls
			zoomLevel = zoomLevel + 0.1f;
			ThirdPersonCamera.IncreaseZoom(zoomLevel);
			std::cout << "mouse wheel scroll up\n";
			break;
		case 4:
			zoomLevel = zoomLevel + 0.1f;
			ThirdPersonCamera.DecreaseZoom(zoomLevel);
			std::cout << "mouse wheel scroll down\n";
			break;
		default:
			break;
		}
	}
}


void keyFunction(unsigned char key, int x, int y) {
	switch(key) {
	case 49:
		//1 to change the camera view to an overhead satelit
		overhead = !overhead;
		break;
	case 50:
		sideView =!sideView;
		break;
	case 65:
		//press A to increase the speed:
		spaceShip.spaceSpeed(65);
		break;
	case 64: 
		//press D to decrease the speed:
		spaceShip.spaceSpeed(64);
		break;
	case 32:
		//use the spacebar to change view to cockpitview
		camera = !camera;
		break;
	case 97:
		//press v to test collision--> x to the right.
		spaceShip.test_collision(97);
		break;
	case 119:
		//press
		spaceShip.test_collision(119);
		break;
	case 115:
		//press s 
		//spaceShip.test_collision(115);
		spaceShip.landSpaceShip();
		break;

	case 100:
		//press t
		spaceShip.test_collision(100);
		break;
	case 113:
		//press t
		spaceShip.test_collision(113);
		break;
	case 101:
		//press t
		spaceShip.test_collision(101);
		break;
	

	
	}
	
	

}




void idle()
{

	
	
	 	spin += speed;
	if(spin > 360)
		spin = 0;
		processKeys();
		
	/*frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		cout << fps << endl;
		timebase = time;
		frame = 0;
	}*/
	


	

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
	glutMouseFunc(mouse_click);

	int frame = 0, time = 0, timebase = 0, fps=0;
	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}
