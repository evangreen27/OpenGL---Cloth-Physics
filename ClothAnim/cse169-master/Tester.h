////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Skeleton.h"
#include "Skin.h"
#include "AnimationClip.h"
#include "Cloth.h"
////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
	void handleKeyboard(glm::vec3 dir);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	glm::vec3 windDir = glm::vec3(0, 0, 0.01f);
	int state = 0;//wind
	ShaderProgram *Program;
	ShaderProgram *clothProgram;
	SpinningCube *Cube;
	Camera *Cam;
	Skeleton *Skelet;
	Skin *skin;
	AnimationClip* anime;
	Cloth* cloth;
	int focusJoint = 0;
	Model* target;
	glm::vec3 targetpos;
};

////////////////////////////////////////////////////////////////////////////////
