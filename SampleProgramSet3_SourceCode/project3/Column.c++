// Column.c++

#include "Column.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"

int NUM_AROUND_CIRCLE = 100;

int Column::instances;

typedef float vec3[3];



 
float black[] = { 0.0, 0.0, 0.0 };
cryph::AffVector direction;
Column::Column()
{
 // Ihandle = (Column::instances ==1)? true : false;

}

Column::Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_){
  Column::instances++;
  
  Ihandle = (Column::instances ==1)? true : false; //take care of the handling.
  
 capped = capped_;
 bottom = bottom_;
 top=top_;
 bradius =bradius_ ;
 tradius= tradius_;
 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 direction = (top-bottom);
 direction.normalize();
 
 defineColumn();
 initializeCappingIndices();
 setBounds();
  
 displayCylEdges = false;
 displayCylFill=true;
  
};

Column::~Column()
{

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Column::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void Column::defineColumn()
{
  

	typedef float vec3[3];
	int nPoints = 2 * (NUM_AROUND_CIRCLE + 1);
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	double theta = 0.0;
	double dTheta = 2.0*M_PI/NUM_AROUND_CIRCLE;

	
	
	cryph::AffVector currVToPoint = defineStarter(); //starter is unit vector.
	//std::cout << "direction vector: " << direction.dx << ", " << direction.dy << ", " << direction.dz << "\n";
	std::cout << "starter vector: " << currVToPoint.dx << ", " << currVToPoint.dy << ", " << currVToPoint.dz << "\n";
	cryph::AffVector mvDir = direction.cross(currVToPoint);
	cryph::AffPoint currpoint_b = bottom + bradius*currVToPoint;
	cryph::AffPoint currpoint_t = top + tradius*currVToPoint;
	

	cryph::AffVector paralleltrash; //I don't need this
	cryph::AffVector perp_b;
	
	//(currpoint_b-bottom).decompose((currpoint_t-currpoint_b),paralleltrash, perp_b); //sets the normal of the point and puts in perp_b. note this should be the normal at both ends of the columnthingy.
	
		perp_b = (currpoint_b-bottom).cross(currpoint_t-currpoint_b).cross(currpoint_t-currpoint_b);
	
	
	//std::cout <<"HEre is the normal: "<< perp_b.dx << ", " << perp_b.dy << ", " << perp_b.dz << "\n\n";
	//std::cout <<"Here is the INITIAL lengthof the b point: "<< (currpoint_b-bottom).length()<<"\n";
	//std::cout <<"Here is the INITIAL lengthof the b point: "<< (currpoint_t-top).length()<<"\n\n";
	for (int i=0 ; i<=NUM_AROUND_CIRCLE ; i++)
	{
		int j = 2 * i;

		normals[j][0] = perp_b.dx; normals[j][1] = perp_b.dy; normals[j][2] = perp_b.dz;
		coords[j][0] = currpoint_b.x; coords[j][1] = currpoint_b.y; coords[j][2] = currpoint_b.z;
		
		//otherside
		normals[j+1][0] = perp_b.dx; normals[j+1][1] = perp_b.dy; normals[j+1][2] = perp_b.dz;
		coords[j+1][0] = currpoint_t.x; coords[j+1][1] = currpoint_t.y; coords[j+1][2] =currpoint_t.z;
		
		
		//now it is  time for some magic to happen and move over dTheta and do all this again.
		//first of all, theta needs incremented.
		if(i ==NUM_AROUND_CIRCLE)
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
		perp_b = helperV.cross(currpoint_t-currpoint_b);
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

cryph::AffVector Column::defineStarter(){
  cryph::AffVector v(direction.dx-1,direction.dy,direction.dz-1);
  
  cryph::AffVector result = v.cross(direction);
  
  result.normalize();
 // std::cout << "result vector: " << result.dx << ", " << result.dy << ", " << result.dz << "\n";
 // std::cout << "here is the result of the dot with the direction: " << result.dot(direction);
  return result;
  
    
    
    
  
  
}
 float delta_eye=0.03;
 bool quad34=false;
void Column::handleCommand(unsigned char key, double ldsX, double ldsY)
{
  
//   //std::cout << "Key pressed: "<< key <<"\n";
//   if(!Ihandle)
//     return;
//   
//  
// 
//      GLFWController* glfwC =
//  				dynamic_cast<GLFWController*>(Controller::getCurrentController());
//  			if (glfwC != NULL)
//  			{
// 			//glfwEnable( GLFW_STICKY_KEYS );
//  				//std::cout << "Are you holding down the 'o' key?: " << glfwGetKey(glfwC->theWindow , '+');
//  			}
//      //info about keys I'm interested in:
//       bool a_down = (glfwGetKey(glfwC->theWindow , 'A') == 1)? true : false;
//       bool s_down = (glfwGetKey(glfwC->theWindow , 'S') == 1)? true : false;
//       bool d_down = (glfwGetKey(glfwC->theWindow , 'D') == 1)? true : false;
//       bool w_down = (glfwGetKey(glfwC->theWindow , 'W') == 1)? true : false;
//       bool u_down = (glfwGetKey(glfwC->theWindow , 'U') == 1)? true : false;
//       
//   if(key == '+' || key == '-'){
// 	  cryph::AffVector v = ((key =='+')? (center-eye) : (eye-center));
// 	  v.normalize();
// 	  float zoomSize = 10;
// 	  eye= eye + (v*zoomSize);
// 	  //return;
//   }
//   if(u_down){
//    // delete &up;
//     up = cryph::AffVector(0,1,0);
//    // return;
//   }
//     
//   //get outa here!
//   float mvsize = 10;
// 	//lateral movement
// 	if (a_down || d_down){
// 	  cryph::AffVector v = eye -center;
// 	  //begin different experiement.
// 	  cryph::AffVector dir = up.cross(v);
// 	  dir.normalize();
// 	  float length = v.length();
// 	  
// 	  
// 	  dir = dir*mvsize * ((a_down)? -1 : 1);
// 	  cryph::AffVector newVec = v +dir;
// 	  newVec.normalize();
// 	  newVec*=length;
// 	  
// 	  eye =center +  newVec;
// 	  
// 	/*  
// 	  
// 	//  std::cout << "this is the v vector" << v.dx << ", " << v.dy << ", " << v.dz  << "\n";
// 	  //std::cout << "EYE BEFORE" << eye.x << ", " << eye.y << ", " << eye.z  << "\n";
// 	    float r = sqrt(pow(v.dx, 2) + pow(v.dz, 2));
// 	    float theta = atan(v.dx/v.dz);
// 	    float thetanew= (theta + delta_eye*((key =='d')? (-1) : 1)); //tertiary for if we should move in the other direction or not.
// 	    if(thetanew >= M_PI/2){
// 	      quad34 = !quad34;
// 		//delta = -1 * delta;
// 	      
// 	    }
// 
// //	    std::cout <<"new theta :" << thetanew << "\n";
// //   	   
// 	    
// 	    v.dx = sin(thetanew)*r;
// 	    v.dz = cos(thetanew) * r;
// 	    if(quad34){
// 		v= -v;
// 	      
// 	    }
// 	    eye = center + v;*/
// 	//     std::cout << "EYE AFTER" << eye.x << ", " << eye.y << ", " << eye.z ;
// 	  //  std::cout << "in!!!!!";
// 	    GLFWController* glfwC =
//  				dynamic_cast<GLFWController*>(Controller::getCurrentController());
//  			if (glfwC != NULL)
//  			{
//  			//	glfwC->setRunWaitsForAnEvent(false);
//  			}
// 	    
// 	
// 	} 
// 	
// 	if (w_down || s_down){
// 	  cryph::AffVector v = eye -center;
// 	  float length = v.length();
// 	  cryph::AffVector dir = up * ((w_down)? 1 : -1);
// 	  dir.normalize();
// 	  dir *=mvsize;
// 	  cryph::AffVector newVec = v + dir;
// 	  newVec.normalize();
// 	  newVec*=length;
// 	  eye = center + newVec;
// 	  
// 	  //fix up. He's a bit of a fixer upper.
// 	  cryph::AffVector temp1 = up.cross(newVec);
// 	  //temp1 is pointing to the "right" from the eye-ish point of view. right hand rule, recalling that newVec is pointing TOWARD the eye pos. 
// 	  up = newVec.cross(temp1);
// 	  up.normalize();
// 	  
// 	  
// 	  
// 	/*  
// 	//  std::cout << "this is the v vector" << v.dx << ", " << v.dy << ", " << v.dz  << "\n";
// 	  //std::cout << "EYE BEFORE" << eye.x << ", " << eye.y << ", " << eye.z  << "\n";
// 	  
// 	//  double useThisDelta = delta_eye * ((key == 'w')? 1 : -1);
// 	  double r = v.length();
// 	  cryph::AffVector mvThisWay;
// 	  up.normalizeToCopy(mvThisWay);
// 	  mvThisWay.normalize();
// 	  v.normalize();
// 	  mvThisWay = mvThisWay * ((key=='w')? 1 : -1);
// 	  cryph::AffVector tempV;
// 	  (center-eye).normalizeToCopy(tempV);
// 	  
// 	  
// 	  double b= sin(delta_eye)*r; //b to align with what I have on my sketchy paper.
// 	  
// 	   up =up + (sin(delta_eye)*b) * tempV * ((key=='s')? -1: 1);
// 	   up.normalize();
// 	  eye = center + (cos(delta_eye)*r)*v + (sin(delta_eye)*r)*mvThisWay;*/
// 	  
// 	 
// 	//     std::cout << "EYE AFTER" << eye.x << ", " << eye.y << ", " << eye.z ;
// 	  //  std::cout << "in!!!!!";
// 	    GLFWController* glfwC =
//  				dynamic_cast<GLFWController*>(Controller::getCurrentController());
//  			if (glfwC != NULL)
//  			{
//  				//glfwC->setRunWaitsForAnEvent(false);
//  			}
// 	    
// 	
// 	}


}

void Column::myhandleKeys(){
  //std::cout << "Key pressed: "<< key <<"\n";
  if(!Ihandle)
    return;
  
 

     GLFWController* glfwC =
 				dynamic_cast<GLFWController*>(Controller::getCurrentController());
 			if (glfwC != NULL)
 			{
			//glfwEnable( GLFW_STICKY_KEYS );
 				//std::cout << "Are you holding down the 'o' key?: " << glfwGetKey(glfwC->theWindow , '+');
 			}
     //info about keys I'm interested in:
      bool a_down = (glfwGetKey(glfwC->theWindow , 'A') == 1)? true : false;
      bool s_down = (glfwGetKey(glfwC->theWindow , 'S') == 1)? true : false;
      bool d_down = (glfwGetKey(glfwC->theWindow , 'D') == 1)? true : false;
      bool w_down = (glfwGetKey(glfwC->theWindow , 'W') == 1)? true : false;
      bool u_down = (glfwGetKey(glfwC->theWindow , 'U') == 1)? true : false;
      bool o_down = (glfwGetKey(glfwC->theWindow , 'O') == 1)? true : false;
      bool i_down = (glfwGetKey(glfwC->theWindow , 'I') == 1)? true : false;
      
  if(i_down || o_down){
	  cryph::AffVector v = ((i_down)? (center-eye) : (eye-center));
	  v.normalize();
	  float zoomSize = 10;
	  eye= eye + (v*zoomSize);
	  //return;
  }
  if(u_down){
   // delete &up;
    up = cryph::AffVector(0,1,0);
   // return;
  }
    
  //get outa here!
  float mvsize = 10;
	//lateral movement
	if (a_down || d_down){
	  cryph::AffVector v = eye -center;
	  //begin different experiement.
	  cryph::AffVector dir = up.cross(v);
	  dir.normalize();
	  float length = v.length();
	  
	  
	  dir = dir*mvsize * ((a_down)? -1 : 1);
	  cryph::AffVector newVec = v +dir;
	  newVec.normalize();
	  newVec*=length;
	  
	  eye =center +  newVec;
	  
	/*  
	  
	//  std::cout << "this is the v vector" << v.dx << ", " << v.dy << ", " << v.dz  << "\n";
	  //std::cout << "EYE BEFORE" << eye.x << ", " << eye.y << ", " << eye.z  << "\n";
	    float r = sqrt(pow(v.dx, 2) + pow(v.dz, 2));
	    float theta = atan(v.dx/v.dz);
	    float thetanew= (theta + delta_eye*((key =='d')? (-1) : 1)); //tertiary for if we should move in the other direction or not.
	    if(thetanew >= M_PI/2){
	      quad34 = !quad34;
		//delta = -1 * delta;
	      
	    }

//	    std::cout <<"new theta :" << thetanew << "\n";
//   	   
	    
	    v.dx = sin(thetanew)*r;
	    v.dz = cos(thetanew) * r;
	    if(quad34){
		v= -v;
	      
	    }
	    eye = center + v;*/
	//     std::cout << "EYE AFTER" << eye.x << ", " << eye.y << ", " << eye.z ;
	  //  std::cout << "in!!!!!";
	    GLFWController* glfwC =
 				dynamic_cast<GLFWController*>(Controller::getCurrentController());
 			if (glfwC != NULL)
 			{
 			//	glfwC->setRunWaitsForAnEvent(false);
 			}
	    
	
	} 
	
	if (w_down || s_down){
	  cryph::AffVector v = eye -center;
	  float length = v.length();
	  cryph::AffVector dir = up * ((w_down)? 1 : -1);
	  dir.normalize();
	  dir *=mvsize;
	  cryph::AffVector newVec = v + dir;
	  newVec.normalize();
	  newVec*=length;
	  eye = center + newVec;
	  
	  //fix up. He's a bit of a fixer upper.
	  cryph::AffVector temp1 = up.cross(newVec);
	  //temp1 is pointing to the "right" from the eye-ish point of view. right hand rule, recalling that newVec is pointing TOWARD the eye pos. 
	  up = newVec.cross(temp1);
	  up.normalize();
	  
	  
	  
	/*  
	//  std::cout << "this is the v vector" << v.dx << ", " << v.dy << ", " << v.dz  << "\n";
	  //std::cout << "EYE BEFORE" << eye.x << ", " << eye.y << ", " << eye.z  << "\n";
	  
	//  double useThisDelta = delta_eye * ((key == 'w')? 1 : -1);
	  double r = v.length();
	  cryph::AffVector mvThisWay;
	  up.normalizeToCopy(mvThisWay);
	  mvThisWay.normalize();
	  v.normalize();
	  mvThisWay = mvThisWay * ((key=='w')? 1 : -1);
	  cryph::AffVector tempV;
	  (center-eye).normalizeToCopy(tempV);
	  
	  
	  double b= sin(delta_eye)*r; //b to align with what I have on my sketchy paper.
	  
	   up =up + (sin(delta_eye)*b) * tempV * ((key=='s')? -1: 1);
	   up.normalize();
	  eye = center + (cos(delta_eye)*r)*v + (sin(delta_eye)*r)*mvThisWay;*/
	  
	 
	//     std::cout << "EYE AFTER" << eye.x << ", " << eye.y << ", " << eye.z ;
	  //  std::cout << "in!!!!!";
	    GLFWController* glfwC =
 				dynamic_cast<GLFWController*>(Controller::getCurrentController());
 			if (glfwC != NULL)
 			{
 				//glfwC->setRunWaitsForAnEvent(false);
 			}
	    
	
	} 
  
  
  
}
void Column::setBounds()
{
  //cryph::AffVector direction =(top - bottom).normalize(); I'll come back to proper bounding when in clear thought
  //x
  myBounds[0] = fmin(bottom.x, top.x) - fmax(bradius, tradius);
  myBounds[1] = fmax(bottom.x, top.x) + fmax(bradius, tradius);
  //y
  myBounds[2] = fmin(bottom.y, top.y) - fmax(bradius, tradius);
  myBounds[3] = fmax(bottom.y, top.y) + fmax(bradius, tradius);
  //z
  myBounds[4] = fmin(bottom.z, top.z) - fmax(bradius, tradius);
  myBounds[5] = fmax(bottom.z, top.z) + fmax(bradius, tradius);
  
  
}


void Column::initializeCappingIndices()
{
  for (int i=0 ; i<=NUM_AROUND_CIRCLE ; i++)
	{
		unsigned int j = 2 * i;
		bottomCap[i]=j;
		topCap[i]=j+1;//((unsigned int)(j+1));
		
	}

}

void Column::renderColumn(vec3 color){
	//typedef float vec3[3];
	//vec3 colColor = {1, 0.0, 0.0};
	glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(NUM_AROUND_CIRCLE+1)); 
  
}



void Column::render()
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
		renderColumn(color);
	}
	if (displayCylEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderColumn(black);
	}
	if(capped){
	    glDisableVertexAttribArray(pvaLoc_mcNormal);
	    glVertexAttrib3f(pvaLoc_mcNormal, -direction.dx,-direction.dy,-direction.dz);
	    glDrawElements(GL_TRIANGLE_FAN,(NUM_AROUND_CIRCLE+1),GL_UNSIGNED_INT,  bottomCap);
	    glVertexAttrib3f(pvaLoc_mcNormal, direction.dx,direction.dy,direction.dz);
	    glDrawElements(GL_TRIANGLE_FAN,(NUM_AROUND_CIRCLE+1),GL_UNSIGNED_INT,  topCap);
	    glEnableVertexAttribArray(pvaLoc_mcNormal);
	  
	}
	
	
	myhandleKeys();
	glUseProgram(pgm);
}


