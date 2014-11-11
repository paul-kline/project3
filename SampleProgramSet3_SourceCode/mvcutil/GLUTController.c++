// GLUTController.c++: A concrete Controller subclass based on GLUT

#include "GLUTController.h"
#include "ModelView.h"

bool GLUTController::glutInitialized = false;

static int zero = 0;

GLUTController::GLUTController(const std::string& windowTitle, int rcFlags,
	int* argcP, char**argv)
{
	if (!glutInitialized)
	{
		if (argcP == NULL)
			argcP = &zero;
		glutInit(argcP, argv);
		glutInitialized = true;
	}
	// First create the window and its Rendering Context (RC)
	createWindow(windowTitle, rcFlags);
}

GLUTController::~GLUTController()
{
}

void GLUTController::createWindow(const std::string& windowTitle, int rcFlags)
{
	// The following calls enforce use of only non-deprecated functionality.
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(newWindowWidth, newWindowHeight);

	if (rcFlags != 0)
	{
		int glutFlags = GLUT_RGB | GLUT_DOUBLE;
		if ((rcFlags & MVC_USE_DEPTH_BIT) != 0)
			glutFlags |= GLUT_DEPTH;
		glutInitDisplayMode(glutFlags);
		setClearFlags(rcFlags);
	}

	windowID = glutCreateWindow(titleString(windowTitle).c_str());

	if ((rcFlags & MVC_USE_DEPTH_BIT) == 0)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);
}

void GLUTController::displayCB() // CLASS METHOD
{
	if (curController != NULL)
		dynamic_cast<GLUTController*>(curController)->handleDisplay();
}

void GLUTController::establishInitialCallbacksForRC()
{
	glutDisplayFunc(displayCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseFuncCB);
	glutMotionFunc(mouseMotionCB);
	glutPassiveMotionFunc(mousePassiveMotionCB);
	glutSpecialFunc(specialKeyCB);
}

void GLUTController::handleDisplay()
{
	// clear the frame buffer
	glClear(glClearFlags);

	// draw the collection of models
	int which = 0;
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->render();

	glutSwapBuffers();

	checkForErrors(std::cout, "GLUTController::handleDisplay");
}

void GLUTController::keyboardCB(unsigned char key, int x, int y)
{
	GLUTController* c = dynamic_cast<GLUTController*>(curController);
	if (c != NULL)
		c->handleAsciiChar(key, x, y);
}

void GLUTController::mouseFuncCB(int button, int state, int x, int y)
{
	GLUTController* c = dynamic_cast<GLUTController*>(curController);
	const int SCROLL_WHEEL_UP = 3;
	const int SCROLL_WHEEL_DOWN = 4;
	if (button == SCROLL_WHEEL_UP)
	{
		if (state == GLUT_UP)
			c->handleScroll(true);
	}
	else if (button == SCROLL_WHEEL_DOWN)
	{
		if (state == GLUT_UP)
			c->handleScroll(false);
	}
	else
	{
		Controller::MouseButton mButton;
		if (button == GLUT_LEFT_BUTTON)
			mButton = Controller::LEFT_BUTTON;
		else if (button == GLUT_RIGHT_BUTTON)
			mButton = Controller::RIGHT_BUTTON;
		else
			mButton = Controller::MIDDLE_BUTTON;
		if (Controller::curController != NULL)
			c->handleMouseButton(
				mButton, (state == GLUT_DOWN), x, y, getModifiers());
	}
}

void GLUTController::mouseMotionCB(int x, int y)
{
	if (Controller::curController != NULL)
		dynamic_cast<GLUTController*>(curController)->handleMouseMotion(x, y);
}

void GLUTController::mousePassiveMotionCB(int x, int y)
{
	if (Controller::curController != NULL)
		dynamic_cast<GLUTController*>(curController)->handleMouseMotion(x, y);
}

void GLUTController::reportWindowInterfaceVersion(std::ostream& os) const
{
	int glutVer = glutGet(GLUT_VERSION);
	int glutVerMajor = glutVer/10000;
	int glutVerMinor = (glutVer % 10000) / 100;
	int glutVerPatch = glutVer % 100;
	os << "        GLUT: " << glutVerMajor << '.';
	if (glutVerMinor < 10)
		os << '0';
	os << glutVerMinor << '.';
	if (glutVerPatch < 10)
		os << '0';
	os << glutVerPatch << " (" << glutVer << ")\n";
}

void GLUTController::reshapeCB(int width, int height)
{
	GLUTController* c = dynamic_cast<GLUTController*>(curController);
	c->handleReshape(width, height);
}

void GLUTController::run()
{
	establishInitialCallbacksForRC();
	glutMainLoop();
}

void GLUTController::setWindowTitle(const std::string& title)
{
	glutSetWindowTitle(title.c_str());
}

void GLUTController::specialKeyCB(int key, int x, int y)
{
	if (Controller::curController != NULL)
	{
		GLUTController* theC = dynamic_cast<GLUTController*>(curController);
		bool haveSpecialKey = true;
		int fcnKey = -1;
		Controller::SpecialKey spKey;
		if (key == GLUT_KEY_RIGHT)
			spKey = Controller::RIGHT_ARROW;
		else if (key == GLUT_KEY_LEFT)
			spKey = Controller::LEFT_ARROW;
		else if (key == GLUT_KEY_DOWN)
			spKey = Controller::DOWN_ARROW;
		else if (key == GLUT_KEY_UP)
			spKey = Controller::UP_ARROW;
		else if (key == GLUT_KEY_HOME)
			spKey = Controller::HOME;
		else
			haveSpecialKey = false;
		if (haveSpecialKey)
			theC->handleSpecialKey(spKey, x, y, getModifiers());
		else if ((key >= GLUT_KEY_F1) && (key <= GLUT_KEY_F12))
		{
			fcnKey = key - GLUT_KEY_F1 + 1;
			theC->handleFunctionKey(fcnKey, x, y, getModifiers());
		}
	}
}
