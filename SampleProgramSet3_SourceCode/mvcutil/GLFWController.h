// GLFWController.h - A concrete Controller subclass using the GLFW window interface

#ifndef GLFWCONTROLLER_H
#define GLFWCONTROLLER_H

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include "GLFW/glfw3.h"

#include "Controller.h"

class GLFWController : public Controller
{
public:
	GLFWController(const std::string& windowTitle, int rcFlags = 0);
	virtual ~GLFWController();

	void run();
	void setRunWaitsForAnEvent(bool b) { runWaitsForAnEvent = b;}
	void setWindowTitle(const std::string& title);
	GLFWwindow* theWindow;
protected:
	GLFWController(const GLFWController& c) : Controller(c) {}

	void establishInitialCallbacksForRC();
	virtual void handleDisplay();
	void reportWindowInterfaceVersion(std::ostream& os) const;
	void swapBuffers() const { glfwSwapBuffers(theWindow); }

private:
	//GLFWwindow* theWindow;
	bool returnFromRun, runWaitsForAnEvent;
	int lastPixelPosX, lastPixelPosY;

	void createWindow(const std::string& windowTitle, int rcFlags);

	static bool glfwInitialized;

	static void charCB(GLFWwindow* window, unsigned int theChar);
	static void displayCB(GLFWwindow* window);
	static void keyboardCB(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void mouseFuncCB(GLFWwindow* window, int button, int action, int mods);
	static void mouseMotionCB(GLFWwindow* window, double x, double y);
	static void reshapeCB(GLFWwindow* window, int width, int height);
	static void scrollCB(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
