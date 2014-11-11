// HalfColumn.c++

#include "HalfColumn.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"

int NUM_AORUND_HALFCIRLCE = 100;


typedef float vec3[3];


HalfColumn::HalfColumn(){
    capped = false;
    bottom= cryph::AffPoint(0.0,0.0,0.0);
    top=cryph::AffPoint(0.0,1,0.0);
    bradius=1;
    tradius=1;
    startAt=cryph::AffPoint(1.0,0.0,0.0);
    inwardNormal=false;
    color[0]=0; color[1]=1; color[2]=0;
    direction = top-bottom; 
  
}
HalfColumn::HalfColumn(cryph::AffPoint bottom_,float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, bool inwardNormal_, float radians_,cryph::AffPoint startAt_){

  
  
 capped = capped_;
 bottom = bottom_;
 top=top_;
 bradius =bradius_ ;
 tradius= tradius_;
 radians= radians_;
 startAt= startAt_;
 inwardNormal=inwardNormal;
 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 direction = (top-bottom);
 direction.normalize();

 defineHalfColumn();
 initializeCappingIndices();
 setBounds();
  

 

  displayCylEdges = false;
  displayCylFill=true;
  
  
  
};

HalfColumn::~HalfColumn()
{
  delete[] topCap;
  delete[] bottomCap;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void HalfColumn::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void HalfColumn::defineHalfColumn()
{
  

	typedef float vec3[3];
	int nPoints = 2 * (NUM_AORUND_HALFCIRLCE + 1);
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	double theta = 0.0;
	double dTheta =radians/NUM_AORUND_HALFCIRLCE;

	
	
	cryph::AffVector currVToPoint = defineStarter(); //starter is unit vector.
	//std::cout << "direction vector: " << direction.dx << ", " << direction.dy << ", " << direction.dz << "\n";
	std::cout << "starter vector: " << currVToPoint.dx << ", " << currVToPoint.dy << ", " << currVToPoint.dz << "\n";
	cryph::AffVector mvDir = direction.cross(currVToPoint);
	cryph::AffPoint currpoint_b = bottom + bradius*currVToPoint;
	cryph::AffPoint currpoint_t = top + tradius*currVToPoint;
	

	cryph::AffVector paralleltrash; //I don't need this
	cryph::AffVector perp_b;
	
	//(currpoint_b-bottom).decompose((currpoint_t-currpoint_b),paralleltrash, perp_b); //sets the normal of the point and puts in perp_b. note this should be the normal at both ends of the columnthingy.
	
		perp_b = (currpoint_b-bottom).cross(currpoint_t-currpoint_b).cross(currpoint_t-currpoint_b)* ((inwardNormal)? -1: 1);
	
	
	//std::cout <<"HEre is the normal: "<< perp_b.dx << ", " << perp_b.dy << ", " << perp_b.dz << "\n\n";
	//std::cout <<"Here is the INITIAL lengthof the b point: "<< (currpoint_b-bottom).length()<<"\n";
	//std::cout <<"Here is the INITIAL lengthof the b point: "<< (currpoint_t-top).length()<<"\n\n";
	for (int i=0 ; i<=NUM_AORUND_HALFCIRLCE ; i++)
	{
		int j = 2 * i;

		normals[j][0] = perp_b.dx; normals[j][1] = perp_b.dy; normals[j][2] = perp_b.dz;
		coords[j][0] = currpoint_b.x; coords[j][1] = currpoint_b.y; coords[j][2] = currpoint_b.z;
		
		//otherside
		normals[j+1][0] = perp_b.dx; normals[j+1][1] = perp_b.dy; normals[j+1][2] = perp_b.dz;
		coords[j+1][0] = currpoint_t.x; coords[j+1][1] = currpoint_t.y; coords[j+1][2] =currpoint_t.z;
		
		
		//now it is  time for some magic to happen and move over dTheta and do all this again.
		//first of all, theta needs incremented.
		if(i ==NUM_AORUND_HALFCIRLCE)
		  break;
		//theta += dTheta;
		//now I must calculate the direction of travel from currVToPoint.
		//this mvdir is the vector both perp with the (top-bottom) vector and the vector we used to move out the distance of the radius.
		mvDir = direction.cross(currVToPoint); //recalling that direction = (top-bottom).normalize() therefore mvDir is a unit vector since it is the
	//	std::cout << "mvdir length: " << mvDir.length() << "\n";
		//cross product of 2 unit vectors 90 degrees apart. 
		
		//Now we make the new bottom point!
		currpoint_b = bottom + ((bradius*cos(dTheta))*currVToPoint + ((bradius * sin(dTheta)) *mvDir));
		//top is the same
		currpoint_t = top + ((tradius*cos(dTheta))*currVToPoint + ((tradius * sin(dTheta)) *mvDir));
		//better reflect that in the currVToPoint
		currVToPoint =(currpoint_b-bottom); //same as (currpoint_t-top).normalize()
		currVToPoint.normalize();
	
	//I think thats it.. no wait. normals..
		cryph::AffVector helperV = (currpoint_b-bottom).cross(currpoint_t-currpoint_b);
		perp_b = helperV.cross(currpoint_t-currpoint_b)* ((inwardNormal)? -1: 1);
	//	std::cout << "is perp normal? " << perp_b.dot(currpoint_t-currpoint_b) << "\n\n";
		
	//there, that should do it..
	//	std::cout <<"HEre is the normal: "<< perp_b.dx << ", " << perp_b.dy << ", " << perp_b.dz << "\n\n";
		//std::cout <<"Here is the lengthof the b point: "<< (currpoint_b-bottom).length()<<"\n\n";
		//std::cout <<"Here is the lengthof the t point: "<< (currpoint_t-top).length()<<"\n\n";
		
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), coords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

	delete [] coords;
	delete [] normals;
  
  
}

cryph::AffVector HalfColumn::defineStarter(){
  cryph::AffVector v = (startAt - bottom);
  v.normalize();
  return v;
}


void HalfColumn::setBounds()
{
  //cryph::AffVector direction =(top - bottom).normalize(); I'll come back to proper bounding when in clear thought
  //x
  myBounds[0] = std::min(bottom.x, top.x) - std::max(bradius, tradius);
  myBounds[1] = std::max(bottom.x, top.x) + std::max(bradius, tradius);
  //y
  myBounds[2] = std::min(bottom.y, top.y) - std::max(bradius, tradius);
  myBounds[3] = std::max(bottom.y, top.y) + std::max(bradius, tradius);
  //z
  myBounds[4] = std::min(bottom.z, top.z) - std::max(bradius, tradius);
  myBounds[5] = std::max(bottom.z, top.z) + std::max(bradius, tradius);
  
  
}


void HalfColumn::initializeCappingIndices()
{
  for (int i=0 ; i<=NUM_AORUND_HALFCIRLCE ; i++)
	{
		unsigned int j = 2 * i;
		bottomCap[i]=j;
		topCap[i]=j+1;//((unsigned int)(j+1));
		
	}

}

void HalfColumn::renderHalfColumn(vec3 color){
	//typedef float vec3[3];
	//vec3 colColor = {1, 0.0, 0.0};
	glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(NUM_AORUND_HALFCIRLCE+1)); 
  
}



void HalfColumn::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	

	if (displayCylFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderHalfColumn(color);
	}
	if (displayCylEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderHalfColumn(black);
	}
	if(capped){
	    glDisableVertexAttribArray(pvaLoc_mcNormal);
	    glVertexAttrib3f(pvaLoc_mcNormal, -direction.dx,-direction.dy,-direction.dz);
	    glDrawElements(GL_TRIANGLE_FAN,(NUM_AORUND_HALFCIRLCE+1),GL_UNSIGNED_INT,  bottomCap);
	    glVertexAttrib3f(pvaLoc_mcNormal, direction.dx,direction.dy,direction.dz);
	    glDrawElements(GL_TRIANGLE_FAN,(NUM_AORUND_HALFCIRLCE+1),GL_UNSIGNED_INT,  topCap);
	    glEnableVertexAttribArray(pvaLoc_mcNormal);
	  
	}

	glUseProgram(pgm);
}


