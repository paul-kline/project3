// MyController.h - A concrete Controller subclass using the GLFW window interface

#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

//#ifdef __APPLE_CC__
//#define GLFW_INCLUDE_GLCOREARB
//#endif
#include "GLFW/glfw3.h"

#include "GLFWController.h"
class MyController : public GLFWController
{
public:
	MyController(const std::string& windowTitle, int rcFlags = 0);

	void handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods);
	void handleMouseMotion(int x, int y);
	void handleScroll(bool up);
	
	void handleAsciiChar(unsigned char theChar, int x, int y);
	void handleFunctionKey(int whichFunctionKey, int x, int y, int mods);
	void handleSpecialKey(SpecialKey key, int x, int y, int mods);
	
protected:
	

private:
	static int clickedx;
	static int clickedy;
	static bool leftclick_down;
	
	
};

#endif
