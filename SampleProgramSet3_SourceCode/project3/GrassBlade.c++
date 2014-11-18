// GrassBlade.c++

#include "GrassBlade.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"


int GrassBlade::instances;

typedef float vec3[3];



 
float blackcolor[] = { 0.0, 0.0, 0.0 };
//cryph::AffVector direction;
GrassBlade::GrassBlade()
{
 // Ihandle = (GrassBlade::instances ==1)? true : false;

}

GrassBlade::GrassBlade(int num_points_,
		       cryph::AffPoint bottom_, float baseWidth_, cryph::AffVector baseDirection_,
		       cryph::AffVector upV_, cryph::AffVector outV_, float upDistance_, float outDistance_, 
		       float colorT_[3],float colorB_[3], 
		       int attenuationCurveCode_,float totalTorque_, int torqueApplicationFunctionCode_ ){
 num_points=num_points_;
 points = new vec3[num_points*2]; //points go along each side, and 1 for the tip. note if you (I) want clipped grass, I can simply put the tip point even with the other two.
 normals = new vec3[num_points*2];
 
 
 bottom = bottom_;
 baseWidth=baseWidth_;
 baseDirection = baseDirection_;
 baseDirection.normalize();
 
 
 
 upDistance = upDistance_;
 outDistance= outDistance_;
 
 upV  = upV_;
 
 upV.normalize();
// upV *= upDistance;
 outV = outV_;
 outV.normalize();
// outV*= outDistance;
 
 
 colorT[0] = colorT_[0];
 colorT[1] = colorT_[1];
 colorT[2] = colorT_[2];
 
 colorB[0] = colorB_[0];
 colorB[1] = colorB_[1];
 colorB[2] = colorB_[2];

 
 

 
 
 
 totalTorque=totalTorque_;
 setf_Radial(torqueApplicationFunctionCode_);
 setf_Attenuation(attenuationCurveCode_);
 
 
 defineGrassBlade();

 setBounds();

  
};

GrassBlade::~GrassBlade()
{
  delete[] points;

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void GrassBlade::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void GrassBlade::defineGrassBlade()
{
  

  
  //the width radius is defined by the attenuation function each time
  //the ratation is defined by the rotation attenuation function each time. okay.
  
	//typedef float vec3[3];
	for(int i=0; i<num_points;i++){
	 int j = 2*i; //yeah.
	 float progress = ((float) i)/(num_points-1);
	// std::cout << "progess: " << progress << "\n";
	 float radialOffset = (this->*this->GrassBlade::f_Radial)(progress) * totalTorque; //map the radialOffset
	 float radialWidth = (baseWidth/2) - (this->*this->GrassBlade::f_Attenuation)(progress)*(baseWidth/2);
	// std::cout << "radial width: " << radialWidth;
	 //now set the left point and the right points
	 cryph::AffPoint outP = bottom + upDistance*upV + (progress*outDistance) * outV;
	 cryph::AffPoint upP =  bottom + (progress*upDistance)* upV;
	 cryph::AffVector betweenthemDirection = outP - upP;
	 float mlength = betweenthemDirection.length();
	 betweenthemDirection.normalize();
	 
	 
	 cryph::AffPoint center = upP + (progress*mlength)*betweenthemDirection; 
	 
	 cryph::AffVector moveDir = upV.cross(baseDirection);
	 moveDir.normalize();
	 
	 cryph::AffVector moveV = moveDir * (tan(radialOffset)*radialWidth);//(baseWidth/2));
	 cryph::AffVector leftpointOffset = baseDirection*radialWidth + moveV;//  baseDirection*(baseWidth/2) + moveV;
	 
	 cryph::AffPoint leftPoint = center + leftpointOffset;
	 cryph::AffPoint rightPoint = center - leftpointOffset; //move the other way.
	 
	// points[j] = new vec3;
	// points[j+1] = new vec3;
	 points[j][0] = leftPoint.x; points[j][1] = leftPoint.y; points[j][2]=leftPoint.z;
	 points[j+1][0]= rightPoint.x; points[j+1][1]=rightPoint.y; points[j+1][2]=rightPoint.z;
	 cryph::AffVector norm =  betweenthemDirection.cross(leftpointOffset);
	 norm.normalize();
	 normals[j][0] = norm.dx; normals[j][1] = norm.dy; normals[j][2] = norm.dz;
	 normals[j+1][0] = norm.dx; normals[j+1][1] = norm.dy; normals[j+1][2] = norm.dz;
	  
	  
	}


	

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, num_points*2*sizeof(vec3), points, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, num_points*2*sizeof(vec3), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

  
  
}


float GrassBlade::fxConstant(float x)
{
  return 1;

}
float GrassBlade::fxLinear(float x)
{
  return x;

}
float GrassBlade::fxSquared(float x)
{
  return x*x;

}
float GrassBlade::fxCubed(float x)
{
  return x*x*x;

}




void GrassBlade::setBounds()
{
  //cryph::AffVector direction =(top - bottom).normalize(); I'll come back to proper bounding when in clear thought
  //x
  myBounds[0] = fmin(bottom.x - baseWidth, bottom.x - outV.dx);
  myBounds[1] = fmax(bottom.x + baseWidth, bottom.x + outV.dx);
  myBounds[2] = bottom.y;
  myBounds[3] = bottom.y + upDistance;
  myBounds[4] = fmin(bottom.z - baseWidth, bottom.z - outV.dz);
  myBounds[5] = fmax(bottom.z - baseWidth, bottom.z - outV.dz);
  
  
}
void GrassBlade::setf_Attenuation(int attenuationCurveCode_)
{
  
  switch(attenuationCurveCode_){
    case 0  :
       f_Attenuation= &GrassBlade::fxConstant;
       break;
    case 1  :
       f_Attenuation= &GrassBlade::fxLinear;
       break;
    case 2  :
       f_Attenuation= &GrassBlade::fxSquared;
       break;
    case 3  :
       f_Attenuation= &GrassBlade::fxCubed;
       break;  
    default :
       f_Attenuation= &GrassBlade::fxSquared;
    }
}
void GrassBlade::setf_Radial(int torqueApplicationFunctionCode_)
{
  
  switch(torqueApplicationFunctionCode_){
    case 0  :
       f_Radial= &GrassBlade::fxConstant;
       break;
    case 1  :
       f_Radial= &GrassBlade::fxLinear;
       break;
    case 2  :
       f_Radial= &GrassBlade::fxSquared;
       break;
    case 3  :
       f_Radial= &GrassBlade::fxCubed;
       break;  
    default :
       f_Radial= &GrassBlade::fxSquared;
    }
}




void GrassBlade::renderGrassBlade(vec3 color){
	//typedef float vec3[3];
	//vec3 colColor = {1, 0.0, 0.0};
	//glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(pvaLoc_mcNormal);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*num_points); 
  
}



void GrassBlade::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	ModelViewWithLighting::letThereBeLight(colorT,colorT,colorT,20);

	if (true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderGrassBlade(colorT);
	}
	if (false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderGrassBlade(blackcolor);
	}
	
	
	
	glUseProgram(pgm);
}


