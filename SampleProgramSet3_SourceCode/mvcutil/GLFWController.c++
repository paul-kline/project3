// GLFWController.c++: a basic GLFWController (in Model-View-GLFWController sense)

#include <stdlib.h>

#include "GLFWController.h"
#include "ModelView.h"

bool GLFWController::glfwInitialized = false;

GLFWController::GLFWController(const std::string& windowTitle, int rcFlags) :
	returnFromRun(false), runWaitsForAnEvent(true),
	lastPixelPosX(0), lastPixelPosY(0)
{
	if (!glfwInitialized)
	{
		glfwInit();
		glfwInitialized = true;
	}

	// First create the window and its Rendering Context (RC)
	createWindow(windowTitle, rcFlags);
}

GLFWController::~GLFWController()
{
	// IF THIS IS THE LAST CONTROLLER
	{
		glfwTerminate();
		glfwInitialized = false;
	}
}

void GLFWController::charCB(GLFWwindow* window, unsigned int theChar)
{
	if (theChar < 128)
	{
		GLFWController* c = dynamic_cast<GLFWController*>(curController);
		c->handleAsciiChar(
			static_cast<unsigned char>(theChar), c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWController::createWindow(const std::string& windowTitle, int rcFlags)
{
	// The following calls enforce use of only non-deprecated functionality.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// 2014-05-05 on linux:
	// Should specify "CORE", but that seems to produce bad results. Specifying
	// "COMPAT" makes things work. Weirder yet, the "reportWindowInterfaceVersion"
	// comes back saying it is "CORE". (In fact, that report is the same regardless
	// of how this hint is set!)
	// 2014-05-06: But on Mac, seem to need to specify "CORE" as expected. Hence:
#ifdef __APPLE_CC__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

	if ((rcFlags & MVC_USE_DEPTH_BIT) == 0)
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
	else
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
	setClearFlags(rcFlags);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	int minor = 8; // Start AT LEAST one greater than where you really want to start
	theWindow = NULL;
	while ((theWindow == NULL) && (minor > 0))
	{
		minor--;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		theWindow = glfwCreateWindow(newWindowWidth, newWindowHeight, titleString(windowTitle).c_str(), NULL, NULL);
	}
	if (theWindow == NULL)
	{
		std::cerr << "\n**** COULD NOT CREATE A 4.x RENDERING CONTEXT ****\n\n";
		glfwTerminate();
		glfwInitialized = false;
	}

	glfwMakeContextCurrent(theWindow);
	if ((rcFlags & MVC_USE_DEPTH_BIT) == 0)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);
}

void GLFWController::displayCB(GLFWwindow* window) // CLASS METHOD
{
	if (curController != NULL)
		dynamic_cast<GLFWController*>(curController)->handleDisplay();
}

void GLFWController::establishInitialCallbacksForRC()
{
	glfwSetWindowRefreshCallback(theWindow, displayCB);
	glfwSetWindowSizeCallback(theWindow, reshapeCB);
	glfwSetCharCallback(theWindow, charCB);
	glfwSetKeyCallback(theWindow, keyboardCB);
	glfwSetMouseButtonCallback(theWindow, mouseFuncCB);
	glfwSetScrollCallback(theWindow, scrollCB);
	glfwSetCursorPosCallback(theWindow, mouseMotionCB);
}

void GLFWController::handleDisplay()
{
	glfwMakeContextCurrent(theWindow);
	int width, height;
	glfwGetFramebufferSize(theWindow, &width, &height);
	glViewport(0, 0, width, height);

	// clear the frame buffer
	glClear(glClearFlags);

	// draw the collection of models
	int which = 0;
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->render();

	glfwSwapBuffers(theWindow);

	checkForErrors(std::cout, "GLFWController::handleDisplay");
}

void GLFWController::keyboardCB(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (curController != NULL)
	{
		GLFWController* theC = dynamic_cast<GLFWController*>(curController);
		if (key == GLFW_KEY_ESCAPE)
			theC->handleAsciiChar(27, theC->lastPixelPosX, theC->lastPixelPosY);
	}
}

void GLFWController::mouseFuncCB(GLFWwindow* window, int button, int action, int mods)
{
	if (curController != NULL)
	{
		Controller::MouseButton mButton;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			mButton = Controller::LEFT_BUTTON;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mButton = Controller::RIGHT_BUTTON;
		else
			mButton = Controller::MIDDLE_BUTTON;
		bool pressed = (action == GLFW_PRESS);
		GLFWController* c = dynamic_cast<GLFWController*>(curController);
		c->handleMouseButton(
			mButton, pressed, c->lastPixelPosX, c->lastPixelPosY, mods);
	}
}

void GLFWController::mouseMotionCB(GLFWwindow* window, double x, double y)
{
	if (curController != NULL)
	{
		GLFWController* c = dynamic_cast<GLFWController*>(curController);
		c->lastPixelPosX = static_cast<int>(x + 0.5);
		c->lastPixelPosY = static_cast<int>(y + 0.5);
		c->handleMouseMotion(c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWController::reportWindowInterfaceVersion(std::ostream& os) const
{
	os << "        GLFW: " << glfwGetVersionString() << '\n';
}

void GLFWController::reshapeCB(GLFWwindow* window, int width, int height)
{
	dynamic_cast<GLFWController*>(curController)->handleReshape(width, height);
}

void GLFWController::run()
{
	establishInitialCallbacksForRC();
	while (!glfwWindowShouldClose(theWindow) && !returnFromRun)
	{
		handleDisplay();
		if (runWaitsForAnEvent)
			glfwWaitEvents();
		else
			glfwPollEvents();
	}
	glfwDestroyWindow(theWindow);
	theWindow = NULL;
}

void GLFWController::scrollCB(GLFWwindow* window, double xOffset, double yOffset)
{
	dynamic_cast<GLFWController*>(curController)->handleScroll(yOffset > 0.0);
}

void GLFWController::setWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(theWindow, title.c_str());
}
