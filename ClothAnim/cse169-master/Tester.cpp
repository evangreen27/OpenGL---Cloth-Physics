////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include "Tester.h"
#include <sstream>
////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER=0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER=new Tester("Spinning Cube",argc,argv);
	glutMainLoop();
	delete TESTER;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize components
	Program = new ShaderProgram("../Model.glsl", ShaderProgram::eRender);
	clothProgram = new ShaderProgram("../cloth.glsl", ShaderProgram::eRender);
	Cube=new SpinningCube;
	Cam=new Camera;
	/*if (argc == 1) {
		Skelet = new Skeleton("../wasp.skel.txt");
		skin = new Skin("../wasp.skin.txt", Skelet);
	}
	else
	{
		Skelet = new Skeleton(argv[1]);
		skin = new Skin(argv[2], Skelet);
		anime = new AnimationClip(argv[3]);
	}*/
	Skelet = new Skeleton("../house.skel.txt");
	cloth = new Cloth(16, 64, 0.1f, 0.9f, 0.1f,glm::vec3(-3,2,0));//make sure damper const is not too big
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete anime;
	delete Program;
	delete Cube;
	delete Cam;
	delete Skelet;
	delete skin;
	delete cloth;
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cube->Update();
	Cam->Update();
	Skelet->Update();
	//anime->Update(Skelet);
	//skin->Update();
	cloth->Update(windDir);
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
	const GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		const char* str = (const char*)gluErrorString(err);
		std::cerr << "OpenGL error : " <<  ", " << str << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));
	Cube->Reset();
	cloth->reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Skelet->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	//skin->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	//cloth->Draw(Cam->GetViewProjectMtx(), clothProgram->GetProgramID());
	if (target != NULL) {
		target->Draw(glm::translate(glm::mat4(1.0f), targetpos), Cam->GetViewProjectMtx(), clothProgram->GetProgramID());
	}
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		/*case '=':
			if (focusJoint < Skelet->joints.size()-1)
			{
				focusJoint++;
			}
			break;
		case '-':
			if (focusJoint > 0)
			{
				focusJoint--;
			}
			break;
		case '1':
			Skelet->UpdateJoint(focusJoint, 0, 0.1);
			break;
		case '2':
			Skelet->UpdateJoint(focusJoint, 0, -0.1);
			break;
		case '3':
			Skelet->UpdateJoint(focusJoint, 1, 0.1);
			break;
		case '4':
			Skelet->UpdateJoint(focusJoint, 1, -0.1);
			break;
		case '5':
			Skelet->UpdateJoint(focusJoint, 2, 0.1);
			break;
		case '6':
			Skelet->UpdateJoint(focusJoint, 2, -0.1);
			break;*/
		/*case '=':
			if (state == 0)
			{
				state++;
				std::cout << "holding left corner" << std::endl;
			}
			else if (state == 1)
			{
				state++;
				std::cout << "holding right corner" << std::endl;
			}
			break;
		case '-':
			if (state == 1)
			{
				state--;
				std::cout << "controlling wind" << std::endl;
			}
			else if (state == 2)
			{
				state--;
				std::cout << "holding left corner" << std::endl;
			}
			break;
		case '1':
			handleKeyboard(glm::vec3(0.1f, 0, 0));
			break;
		case '2':
			handleKeyboard(glm::vec3(-0.1f, 0, 0));
			break;
		case '3':
			handleKeyboard(glm::vec3(0, 0.1f, 0));
			break;
		case '4':
			handleKeyboard(glm::vec3(0, -0.1f, 0));
			break;
		case '5':
			handleKeyboard(glm::vec3(0, 0, 0.1f));
			break;
		case '6':
			handleKeyboard(glm::vec3(0, 0, -0.1f));
			break;*/
		case 'i':
			int index;
			float x, y, z;
			std::string line;
			if (std::getline(std::cin, line)) {
				std::istringstream iss(line);
				iss >> index;
				iss >> x;
				iss >> y;
				iss >> z;
				targetpos = glm::vec3(x, y, z);

				Skelet->begin(index, x, y, z);
				target = new Model();
				Joint * join=Skelet->joints[index];
				target->MakeBox(join->minBox, join->maxBox);
			}
		}
	if (state == 0) {
		std::cout << "wind direction: " << windDir.x << " " << windDir.y << " " << windDir.z << std::endl;
	}
	if (state == 1) {
		glm::vec3 leftpos = cloth->particles[0]->position;
		std::cout << "left corner pos: " << leftpos.x<<" "<<leftpos.y<<" "<<leftpos.z << std::endl;
	}
	if (state == 2) {
		glm::vec3 rightpos = cloth->particles[cloth->width - 1]->position;
		std::cout << "left corner pos: " << rightpos.x << " " << rightpos.y << " " << rightpos.z << std::endl;
	}

}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::handleKeyboard(glm::vec3 dir) {
	if (state == 0) {
		windDir += dir;
	}
	if (state == 1)	{
		cloth->move(true, dir);
	}
	if (state == 2) {
		cloth->move(false, dir);
	}
}