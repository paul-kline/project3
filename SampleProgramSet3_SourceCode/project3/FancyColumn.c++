// FancyColumn.c++

#include "FancyColumn.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"
#include "HalfColumn.h"




  
FancyColumn::FancyColumn(){
  
  NUM_AROUND_CIRCLE= 100;
}


FancyColumn::FancyColumn(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, int numCircs_){

  NUM_AROUND_CIRCLE=100;
  
 capped = capped_;
 bottom = bottom_;
 top=top_;
 bradius =bradius_ ;
 tradius= tradius_;
 
 numCircs= numCircs_;
 numPoints = numCircs_*3; //center, intersecting points.
 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 direction = (top-bottom);
 direction.normalize();
 
 defineFancyColumn();
// initializeCappingIndices();
// setBounds();
 
  black[0] =0.0; black[1] =0.0; black[2]=0.0;
 //for moving the eye.
  
  
  
};

FancyColumn::~FancyColumn()
{

  for(int i =0; i< numCircs; i++){
   delete &halfColumns[i]; 
  }
  //delete halfColumns;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void FancyColumn::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void FancyColumn::defineFancyColumn()
{
  

	typedef float vec3[3];
	//int nPoints = 2 * (numCircs + 1);
	//vec3* coords = new vec3[nPoints];
	//vec3* normals = new vec3[nPoints];
	//double theta = 0.0;
	double dTheta = 2.0*M_PI/numPoints;

	cryph::AffPoint* bpoints[numPoints]; //points along the bottom
	cryph::AffPoint* tpoints[numPoints]; //points along the top of the fancy column
	
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
	//cryph::AffPoint currpoint_b2;	
	for (int i=0 ; i<numPoints ; i++)
	{
	
		//int j = 2 * i;

		//normals[j][0] = perp_b.dx; normals[j][1] = perp_b.dy; normals[j][2] = perp_b.dz;
		//coords[j][0] = currpoint_b.x; coords[j][1] = currpoint_b.y; coords[j][2] = currpoint_b.z;
		bpoints[i] = &(*(new cryph::AffPoint(currpoint_b.x,currpoint_b.y, currpoint_b.z))); //this is ridiculous. 
		tpoints[i]=&(*(new cryph::AffPoint(currpoint_t.x,currpoint_t.y,currpoint_t.z))); //this is ridiculous. 
		
		
		if(i<2){
		  std::cout << "currpointb[i]: " << currpoint_b.x << currpoint_b.y << currpoint_b.z << "\n";
		  
		}
		//otherside
		//normals[j+1][0] = perp_b.dx; normals[j+1][1] = perp_b.dy; normals[j+1][2] = perp_b.dz;
		//coords[j+1][0] = currpoint_t.x; coords[j+1][1] = currpoint_t.y; coords[j+1][2] =currpoint_t.z;
		
		
		//now it is  time for some magic to happen and move over dTheta and do all this again.
		//first of all, theta needs incremented.
		
		
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
	
	//use that data to make half columns.
	
	//float tinyR = //distance between two points/2
	 Controller* c = GLFWController::getCurrentController();
	
	 float radians = M_PI - 2*dTheta; //total 'spanage' of each little halfy column thingy. This is much more clear if you make a picture. Isosolese triangles with 'r' sides.
	 float bRadius = (*bpoints[0] - *bpoints[1]).length();
	 float tRadius = (*tpoints[0] - *tpoints[1]).length();
	  std::cout << "bradius: " << bRadius << " tradius: " << tRadius << "\n\n\n";
// 	  std::cout << "bpoints[0]: " << (*bpoints[0]).x << (*bpoints[0]).y << (*bpoints[0]).z << "\n";
// 	   std::cout << "bpoints[1]: " << (*bpoints[1]).x << (*bpoints[1]).y << (*bpoints[1]).z << "\n";
// 	  std::cout << "bpoints[2]: " << (*bpoints[2]).x << (*bpoints[2]).y << (*bpoints[2]).z << "\n";
	   int HalfColCounter=0;
	 for(int i=0;i<numPoints-1;i= i+2){
	//    cryph::AffPoint startP = stairFrontLeft + (stairFrontRight - stairFrontLeft);
	//    float myradius = (stairFrontRight-stairFrontLeft).length();
	   cryph::AffPoint* bcenter_P;
	   cryph::AffPoint* bpoint_P;
	   cryph::AffPoint* botherPoint_P;
	   cryph::AffPoint* tcenter_P;
	   cryph::AffPoint* tpoint_P;
	   cryph::AffPoint* totherPoint_P;
	   
	   if(i==numPoints-2){
	    bcenter_P = bpoints[i+1];
	    bpoint_P = bpoints[i];
	    botherPoint_P = bpoints[0];  
	    //bcenter_P = bpoints[0];
	    //botherPoint_P = bpoints[i+1];  
	    
	    
	    tcenter_P = tpoints[i+1];
	    tpoint_P = tpoints[i];
	    totherPoint_P = tpoints[0];
	   }else{
	    bcenter_P = bpoints[i+1];
	    bpoint_P = bpoints[i];
	    botherPoint_P = bpoints[i+2];
	    
	    tcenter_P = tpoints[i+1];
	    tpoint_P = tpoints[i];
	    totherPoint_P = tpoints[i+2];
	   }
	 //  HalfColumn halfColumn( stairFrontLeft,myradius,(stairFrontLeft + (buildingUpVector*20)),200,color2,false,false, 3.14159/1,startP);
	   //HalfColumn(cr            bottom_, bradius_, Point top_,               float tradius_, float color_[3], bool capped_, bool inwardNormal_, float radians_,cryph::AffPoint startAt_){
	   std::cout << "bcenter: " << bcenter_P->x << ", " <<  bcenter_P->y << ", " << bcenter_P->z << "\n";
	   std::cout << "tcenter: " << tcenter_P->x << ", " <<  tcenter_P->y << ", " << tcenter_P->z << "\n\n";
	  
	   
	   //cryph::AffPoint b(0,200,0);
	   //cryph::AffPoint t(0,220,0);
	   //cryph::AffPoint s(5,200,0);
	   //float colorx[3]= {0.8,0.45,0.77};
	   //HalfColumn* halfColumn1 = new HalfColumn(b , 5, t,5,color,false,false, 3.14159/2 ,s);
	  //c->addModel(halfColumn1);
	    // std::cout << "samde it here~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.\n\n\n\n\n\n";
	    HalfColumn* halfColumn = new HalfColumn( *bcenter_P, bRadius, *tcenter_P,tRadius,color,capped,true, radians ,*botherPoint_P);
	    
	    halfColumns[HalfColCounter++] = *halfColumn;
	    double bounds[6];c->addModel(halfColumn);
	    //column1.getMCBoundingBox(bounds);
 	    
	  
	}
	
	
/*	
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
	glEnableVertexAttribArray(pvaLoc_mcNormal);*/

	
	
	
	
	//delete [] coords;
	//delete [] normals;
  
  
}

cryph::AffVector FancyColumn::defineStarter(){
  cryph::AffVector v(direction.dx-1,direction.dy,direction.dz-1);
  
  cryph::AffVector result = v.cross(direction);
  
  result.normalize();
 // std::cout << "result vector: " << result.dx << ", " << result.dy << ", " << result.dz << "\n";
 // std::cout << "here is the result of the dot with the direction: " << result.dot(direction);
  return result;
  
    
    
    
  
  
}

void FancyColumn::setBounds()
{
  
  //double extremeBounds[6];
  halfColumns[0].getMCBoundingBox(myBounds); 
    
  for(int i=1; i<numCircs; i++){
    double currentBounds[6];   
    halfColumns[i].getMCBoundingBox(currentBounds); 
    for(int j = 0; j<3; j++){
	int k= j*2;
	if(currentBounds[k] < myBounds[k]){
	  myBounds[k] = currentBounds[k];
	}
	if(currentBounds[k+1] > myBounds[k+1]){
	  myBounds[k+1] = currentBounds[k+1];
	  
	}
      
    }   
  }

}


void FancyColumn::initializeCappingIndices()
{
  for (int i=0 ; i<=numCircs ; i++)
	{
		unsigned int j = 2 * i;
		bottomCap[i]=j;
		topCap[i]=j+1;//((unsigned int)(j+1));
		
	}

}



void FancyColumn::render()
{
 //std::cout << "numcircs: " << numCircs;
	for(int i=0; i<numCircs; i++){
	  halfColumns[i].render();
	 // std::cout << "I am here: " << i << "\n\n\n";
	}
}


