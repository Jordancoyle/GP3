#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY

#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cLight.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cModelMov.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

	float translationX = 0;
	float translationZ = 0;

	bool cameraCheck = true;

	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	LPCSTR gameSounds[1] = {"Audio/1178__nicstage__guitar-string-ominous.wav"};

	theSoundMgr->add("Theme", gameSounds[0]);

	// Create a camera
	cCamera fpCamera;
	fpCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	fpCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	fpCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); 
	fpCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	fpCamera.setTheProjectionMatrix(45.0f, fpCamera.getTheCameraAspectRatio(), 0.1f, 20000.0f);
	fpCamera.update();

	// Create a camera
	cCamera tdCamera;
	tdCamera.setTheCameraPos(glm::vec3(00.0f, 75.0f, 75.0f));
	tdCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	tdCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); 
	tdCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	tdCamera.setTheProjectionMatrix(45.0f, tdCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	tdCamera.update();

	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 50000.0f, 1.0f, 0.0f, 0.0f);

	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	//cTexture testTexture;
	//testTexture.createTexture("Models/earth/earth.jpg");

	cModelLoader barrelMdl;
	barrelMdl.loadModel("Models/barrels_obj/barrels_obj.obj");

	cModelLoader enemyMdl;
	enemyMdl.loadModel("Models/earth/earth.obj");

	cModelLoader guyMdl;
	guyMdl.loadModel("Models/Guy/Chris_RE6.obj");

	cModelMov ModelMov;
	ModelMov.initialise(glm::vec3(0, 0, 0), 0.0f, glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), 5.0f, true);
	ModelMov.setMdlDimensions(guyMdl.getModelDimensions());
	ModelMov.attachInputMgr(theInputMgr);

	theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth, windowHeight);

		theInputMgr->mouseXY(WM_MOUSEMOVE);
		//fpCamera.setTheCameraLookAt(glm::vec3(theInputMgr->getMouseXPos(), theInputMgr->getMouseYPos(), 0.0f));

		if (theInputMgr->isKeyDown(VK_RIGHT))
		{
			translationX -= 2.0f;
		}
		if (theInputMgr->isKeyDown(VK_LEFT))
		{
			translationX += 2.0f;
		}
		if (theInputMgr->isKeyDown(VK_UP))
		{
			translationZ -= 2.0f;
		}
		if (theInputMgr->isKeyDown(VK_DOWN))
		{
			translationZ += 2.0f;
		}

		fpCamera.setTheCameraPos(glm::vec3((0.0f + translationX), 0.0f, (75.0f + translationZ)));
		fpCamera.update();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (cameraCheck)
		{
			glLoadMatrixf((GLfloat*)&fpCamera.getTheViewMatrix());
		}
		else
		{
			glLoadMatrixf((GLfloat*)&tdCamera.getTheViewMatrix());
		}


		barrelMdl.renderMdl((glm::vec3(0.0f, 40.0f, 0.0f)), 0.0f, (glm::vec3(5.0f, 5.0f, 5.0f)));
		enemyMdl.renderMdl((glm::vec3(0.0f, 90.0f, 20.0f)), 0.0f, (glm::vec3(10.0f, 10.0f, 10.0f)));
		guyMdl.renderMdl(ModelMov.getPosition(), ModelMov.getRotation(), ModelMov.getScale());
		ModelMov.update(elapsedTime);

		sunLight.lightOn();

		if (theInputMgr->wasKeyPressed(VK_TAB))
		{
			if (cameraCheck)
			{
				cameraCheck = false;
			}
			else
			{
				cameraCheck = true;
			}
		}

		pgmWNDMgr->swapBuffers();

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

	return 0; //Return success

}