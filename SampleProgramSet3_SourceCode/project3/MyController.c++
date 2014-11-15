// MyController.c++: a basic MyController (in Model-View-MyController sense)

#include <stdlib.h>

#include "MyController.h"
#include "ModelView.h"
#include "ModelViewWithLighting.h"
int MyController::clickedx=0;
int MyController::clickedy=0;
bool MyController::leftclick_down = false;
MyController::MyController(const std::string& windowTitle, int rcFlags) :
	GLFWController(windowTitle,rcFlags)
{
	
}


void MyController::handleMouseButton(Controller::MouseButton button, bool pressed, int x, int y, int mods)
{
   // Controller::handleMouseButton(button, pressed, x, y, mods);
  
  
  if(button == LEFT_BUTTON && pressed){
    std::cout << "left button and pressed: " << x << ", " << y << "\n";
    MyController::clickedx=x;
    MyController::clickedy=y;
    MyController::leftclick_down = true;
    
  }
  if(button == LEFT_BUTTON && !pressed){
    MyController::leftclick_down = false;
    
    
//    /* //keep it there!
//     float deltaX = (x - clickedx)/5;
//     float deltaY = (y - clickedy)/5;  
//     */ModelViewWithLighting::dynamic_view =  cryph::Matrix4x4::xRotationDegrees(deltaY) * cryph::Matrix4x4::yRotationDegrees(deltaX)*ModelViewWithLighting::dynamic_view;
    
  }
  
  
}
void MyController::handleMouseMotion(int x, int y)
{
  if(MyController::leftclick_down){
    float deltaX = x - clickedx;
    float deltaY = y - clickedy;     
//    std::cout << deltaX << ", " << deltaY << "\n";
    ModelViewWithLighting::addToGlobalRotationDegrees(deltaX/5,deltaY/5, 0);
    redraw();
     clickedx = x;
     clickedy = y;
    
  }  else{
    /*
    clickedx = x;
    clickedy = y; */
  }
    
    //Controller::handleMouseMotion(x, y);
	
      
  
  
  
  
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
