//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"

#include "SceneNodes.h"
#include "APCNode.h"
#include "Camera.h"

Blit3D *blit3D = NULL;

//GLOBAL DATA
GLSLProgram* prog = NULL;
SceneManager *sceneManager = NULL;
MeshManager* meshManager = NULL;
SceneNode* currentNode = NULL;

// Nodes
TranslaterNode* t2 = NULL;
RotatorNode* r1 = NULL;
FloaterNode* f1 = NULL;
APCNode* tank = NULL;
MeshNode* terrain = NULL;

// The camera
Camera* camera;

// Other variables
float elapsedTime = 0;
const float timeSlice = 1.f / 60.f;
bool stripped = false;

/**
* This method is called when starting the program.
*/
void Init()
{
	// Global objects
	sceneManager = new SceneManager();
	meshManager = new MeshManager(blit3D);
	blit3D->SetMode(Blit3DRenderMode::BLIT3D);
	prog = blit3D->sManager->UseShader("lighting.vert", "lighting.frag"); //load/compile/link

	//3d perspective projection
	blit3D->projectionMatrix = glm::perspective(45.0f, (GLfloat)(blit3D->screenWidth) / (GLfloat)(blit3D->screenHeight), 0.1f, 10000.0f);

	prog->setUniform("projectionMatrix", blit3D->projectionMatrix);
	prog->setUniform("viewMatrix", blit3D->viewMatrix);
	prog->setUniform("modelMatrix", glm::mat4(1.f));

	//lighting variables
	glm::vec3 LightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 LightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);

	//send lighting info to the shader
	prog->setUniform("LightPosition", LightPosition);
	prog->setUniform("LightIntensity", LightIntensity);

	//send alpha to the shader
	prog->setUniform("in_Alpha", 1.f);

	//attributes
	prog->bindAttribLocation(0, "in_Position");
	prog->bindAttribLocation(1, "in_Normal");
	prog->bindAttribLocation(2, "in_Texcoord");

	prog->printActiveUniforms();
	prog->printActiveAttribs();

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Tank and camera
	tank = new APCNode(sceneManager->sceneGraph, meshManager);
	camera = new Camera(tank, glm::vec3(10.f, 3.f, 0.f));

	// Add the terrain
	t2 = new TranslaterNode(sceneManager->sceneGraph, glm::vec3(0.f, -1.5f, 0.f));
	r1 = new RotatorNode(t2, glm::vec3(1.f, 0.f, 0.f), -90);
	terrain = new MeshNode(r1, "Data/Terrain.s3d", meshManager);

}
/**
* This method is called when exiting the program.
*/
void DeInit(void)
{
	// Delete the objects in the scene
	if (sceneManager != NULL) delete sceneManager;

	// Thelete all the meshes
	if (meshManager != NULL) delete meshManager;

	// Delete the camera
	if (camera != NULL) delete camera;
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}
/**
* This method is called every x seconds.
*/
void Update(double seconds)
{
	if (seconds > 0.15) elapsedTime += 0.15f; //prevent lag spike
	else elapsedTime += static_cast<float>(seconds);

	while (elapsedTime >= timeSlice)
	{
		elapsedTime -= timeSlice;
		// Update all the elements
		sceneManager->UpdateWorld(timeSlice);
		camera->Update(timeSlice);
	}
	
}

void Draw(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a 

	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//move camera
	blit3D->viewMatrix = camera->viewMatrix;
	prog->setUniform("viewMatrix", blit3D->viewMatrix);

	//draw stuff here
	sceneManager->DrawWorld();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	// Change the camera mode to follow or fixed
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		camera->followMode = !camera->followMode;
	}

	// Movement Controls
	if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS)
	{
		tank->steering = true;
	}
	else if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_RELEASE)
	{
		tank->steering = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		tank->left = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		tank->left = false;
	}
	if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_PRESS)
	{
		tank->moving = true;
	}
	else if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_RELEASE)
	{
		tank->moving = false;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		tank->forward = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		tank->forward = false;
	}

	// Rotate the Torret
	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
	{
		tank->movingTorret = true;
	}
	else if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE)
	{
		tank->movingTorret = false;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		tank->torretAngleLeft = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		tank->torretAngleLeft = false;
	}

	// Mpve the gun
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) && action == GLFW_PRESS)
	{
		tank->movingGun = true;
	}
	else if ((key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE)
	{
		tank->movingGun = false;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		tank->gunAngleUp = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		tank->gunAngleUp = false;
	}
	
}

void DoFileDrop(int count, const char** files)
{
	// Not used here
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::DECORATEDWINDOW, 1024, 1024);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoFileDrop(DoFileDrop);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}