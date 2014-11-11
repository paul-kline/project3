// Stairs.c++

#include "Stairs.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"
#include "Block.h"




typedef float vec3[3];


bool displayCylEdges3 = false;
bool displayCylFill3=true;
float black3[] = { 0.0, 0.0, 0.0 };
Block* blocks;


//default constructor;
Stairs::Stairs()
{
 height=1;
 width=1;
 length=1;
 stepWidth=1;
 numStairs=1;
 flatBack=false;
 cryph::AffVector mainNormal(0,1,0);
 mainNormal.normalize(); //just in case someone hadn't..
 
frontLeftBottomCorner= cryph::AffPoint(-1,0,0);
frontLeftBottomCorner= cryph::AffPoint( 1,0,0);
 

 color[0] = 0;
 color[1] = 0;
 color[2] = 0;
 
 widthVector= frontRightBottomCorner - frontLeftBottomCorner;
 widthVector.normalize();
 
 lengthVector = mainNormal.cross(widthVector);
 lengthVector.normalize();
 
 
 defineStairs(); 
 setBounds();

}

Stairs::Stairs(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffPoint frontRightBottomCorner_, vec3 color_,
	       float stepWidth_,int numStairs_, bool flatBack_){ // this line is diff from Block.c++ constructor.

 height=height_;
 width=width_;
 length=length_;
 stepWidth=stepWidth_;
 numStairs=numStairs_;
 flatBack=flatBack_;
 mainNormal=normal_;
 mainNormal.normalize(); //just in case someone hadn't..
 
 frontLeftBottomCorner=frontLeftBottomCorner_;
 frontRightBottomCorner=frontRightBottomCorner_;

 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 
 widthVector= frontRightBottomCorner - frontLeftBottomCorner;
 widthVector.normalize();
 
 lengthVector = mainNormal.cross(widthVector);
 lengthVector.normalize();
 
 blocks = new Block[numStairs];
 
 defineStairs(); 
 setBounds();
  
};

Stairs::~Stairs()
{
  for(int i=0; i< numStairs; i++){
    delete &blocks[i];  
    
  }
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Stairs::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void Stairs::defineStairs()
{
  
 

 // Controller* c = GLFWController::getCurrentController();
  
  // std::cout << "defining stairs. c:" << c << "\n";
 
  cryph::AffPoint leftPoint= frontLeftBottomCorner;
  cryph::AffPoint rightPoint=frontRightBottomCorner;
  float lengthShrinkSize;
  if(flatBack){
    lengthShrinkSize = stepWidth;
  }else{
    lengthShrinkSize = 2*stepWidth;
  }
  
      int i;
      for(i=0; i< numStairs; i++){
	Block block(height,width, length, mainNormal,leftPoint,rightPoint,color);
	blocks[i] = block;
	//height same.
	//wdith shrunk by 2*step size
	width-=(2*stepWidth);
	length-= lengthShrinkSize;//(2*stepWidth);
	leftPoint= leftPoint + (widthVector*stepWidth) + (lengthVector*stepWidth) + (height * mainNormal);
	rightPoint= rightPoint + ((-widthVector)*stepWidth) + (lengthVector*stepWidth) + (height * mainNormal);
	//c->addModel(&blocks[i]);
   //   std::cout << "defining a bock!!!! " << c->getNumModels() << "\n";
      }

      
      //   std::cout << "defining stairs" << c->getNumModels() << "\n";

    
 
    
  /*
	coords = new vec3[8]; //for each point of course.
	assignCoords();
	
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), coords, GL_DYNAMIC_DRAW);
			
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);
  */
}


void Stairs::setBounds()
{
  //cryph::AffVector direction =(top - bottom).normalize(); I'll come back to proper bounding when in clear thought
  //x
  myBounds[0] = calculateMin(0);
  myBounds[1] = calculateMax(1);
  //y
  myBounds[2] = calculateMin(2);
  myBounds[3] = calculateMax(3);
  //z
  myBounds[4] = calculateMin(4);
  myBounds[5] = calculateMax(5);
  
  
}
double Stairs::calculateMax(int index){
  int i;
  double maxval=-9999999;
  
  for(i=0; i<numStairs; i++){
    double bounds[6];
    blocks[i].getMCBoundingBox(bounds);
    if(bounds[index]> maxval){
      maxval = bounds[index];
    }
  }
  return maxval;  
}

double Stairs::calculateMin(int index){
  int i;
  double minval=9999999;
  
  for(i=0; i<numStairs; i++){
    double bounds[6];
    blocks[i].getMCBoundingBox(bounds);
    if(bounds[index] < minval){
      minval = bounds[index];
    }
  }
  return minval;  
}


void Stairs::render()
{
  
  for(int i =0; i< numStairs; i++){
   blocks[i].render(); 
  }
  /*
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	

	if (displayCylFill3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderStairs(color);
	}
	if (displayCylEdges3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderStairs(black3);
	}
	

	glUseProgram(pgm);
	*/
}


