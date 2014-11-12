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
    //Controller::handleScroll(up);
  float value = 0.2;
  value *= (up)? 1 : -1 ;
  ModelView::addToGlobalZoom(value);
  std::cout << "Adding to global zoom";
}

void MyController::handleAsciiChar(unsigned char theChar, int x, int y)
{
  //std::cout << "HANDLING ASCII CHAR!!!!!";
    //Controller::handleAsciiChar(theChar, x, y);
  if(theChar == 'O'){
    ModelView::setProjection(ORTHOGONAL);
    
  }else if(theChar == 'P'){
    ModelView::setProjection(PERSPECTIVE);
    
  }else if(theChar == 'Q'){
   // std::cout << "you hit 'q'!!!!!!!!!!!!!!!!!!!!!";
    ModelView::setProjection(OBLIQUE);
    
  }
}
void MyController::handleFunctionKey(int whichFunctionKey, int x, int y, int mods)
{
    Controller::handleFunctionKey(whichFunctionKey, x, y, mods);
}
void MyController::handleSpecialKey(Controller::SpecialKey key, int x, int y, int mods)
{
    Controller::handleSpecialKey(key, x, y, mods);
}
