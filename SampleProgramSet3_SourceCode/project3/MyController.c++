// MyController.c++: a basic MyController (in Model-View-MyController sense)

#include <stdlib.h>

#include "MyController.h"
#include "ModelView.h"



MyController::MyController(const std::string& windowTitle, int rcFlags) :
	GLFWController(windowTitle,rcFlags)
{
	
}


void MyController::handleMouseButton(Controller::MouseButton button, bool pressed, int x, int y, int mods)
{
    Controller::handleMouseButton(button, pressed, x, y, mods);
}
void MyController::handleMouseMotion(int x, int y)
{
    Controller::handleMouseMotion(x, y);
}
void MyController::handleScroll(bool up)
{
    Controller::handleScroll(up);
}
