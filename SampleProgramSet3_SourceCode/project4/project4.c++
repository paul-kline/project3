// project3.c++: Starter for EECS 672 Project 2

#include "MyController.h"
#include "Simpleshape.h"
#include "Column.h"
#include "Block.h"
#include "Stairs.h"
#include "HalfColumn.h"
#include "FancyColumn.h"
#include "SuperFancyColumn.h"
#include "FirePit.h"
#include "GrassBlade.h"
void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

	//cryph::AffPoint eye, center;
	//cryph::AffVector up;

	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);
	cryph::AffPoint center(xmid, ymid, zmid);

	// 2:
	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta){
	  maxDelta = delta;
	}
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta){
	  maxDelta = delta;
	}
	float width = xyz[1] -xyz[0];
	float height = xyz[3] - xyz[2];
	float depth = xyz[5] - xyz[4];
	
	float plaindiagonal = sqrt(height*height + width*width);
	float diagonal = sqrt(plaindiagonal*plaindiagonal + depth*depth);
	
	//Its radius will be half the diagonal length of the MC region of interest. Set ecXmin = ecYmin = -radius; 
	//ecXmax = ecYmax = radius, and make sure ecZmin and ecZmax are set so as to avoid unwanted depth clipping. The value for zpp can usually remain as suggested in 2.B.i.
	float viewingRadius = diagonal/2;
	
	double distEyeCenter = 2.0 * diagonal;
	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);
	
	

	cryph::AffVector up = cryph::AffVector::yu;
	
	
	
	
	
	
	
	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double zpp, zmin, zmax;

	// Set values for zpp, zmin, zmax that make sense in the context of
	// the EC system established above, then:
	zmin = -(distEyeCenter + viewingRadius);
//  	zpp = -(distEyeCenter - 0.5*maxDelta);
//	zmin =zpp;//zmin = zpp - 2*maxDelta;
	zmax = zmin + diagonal;
	zpp = zmax;
	//zmax = zmin + diagonal; //zpp + maxDelta; //changed from half maxDelta
//	zpp = zmin;
	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(zpp);
	ModelView::setECZminZmax(zmin, zmax);
	ModelViewWithLighting::setViewingRadius(viewingRadius);
	//ModelViewWithLighting::letThereBeLight(); //inialize a light source!

}
float frand(float L, float H){
 return  L + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(H-L)));
  
}
bool isUnderModelView(float x, float z, ModelView* mv){
  double xyz[6];
  mv->getMCBoundingBox(xyz);
 // std::cout <<"BOUNDING BOX: " << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ", " << xyz[3] <<"\n\n";
  return (x>xyz[0] && x<xyz[1] && z>xyz[4] && z < xyz[5]); 
}
void makeGrassField(Block* ground, Controller* c, ModelView* avoid1, ModelView* avoid2, ModelView* avoid3, ModelView* avoid4){
  float forestGreen[3] = {0.133333, 0.545098, 0.133333}; //forest green
  float darkGreen[3] = {0.000, 0.392, 0.000}; // dark green
  
  cryph::AffPoint topLeftCorner = ground->frontLeftBottomCorner + ground->height*cryph::AffVector(0,1,0);
  float spacing = ground->width / 100;
  
  int numGrasses = 8500;
  cryph::AffVector toRight = ground->frontRightBottomCorner - ground->frontLeftBottomCorner;
  toRight.normalize();
  cryph::AffVector toBack = ground->getTowardsBackUnitVec();
  for(int i =0; i < numGrasses; i++){
   float r = frand(0,1);
   float r2 = frand(0,1);  
   cryph::AffPoint bottomCenter = topLeftCorner + r*ground->length* (toBack) + r2*ground->width*toRight;
   while(isUnderModelView(bottomCenter.x, bottomCenter.z, avoid1) ||
         isUnderModelView(bottomCenter.x, bottomCenter.z, avoid2) ||
	 isUnderModelView(bottomCenter.x, bottomCenter.z, avoid3) ||
	 isUnderModelView(bottomCenter.x, bottomCenter.z, avoid4)
        ){
    r = frand(0,1);
    r2 = frand(0,1);
    bottomCenter = topLeftCorner + r*ground->length* (toBack) + r2*ground->width*toRight;
       
   }
    // 	:GrassBlade(int num_points_,
// 		       cryph::AffPoint bottom_, float baseWidth_, cryph::AffVector baseDirection_,
// 		       cryph::AffVector upV_, cryph::AffVector outV_, float upDistance_, float outDistance_, 
// 		       float colorT_[3],float colorB_[3], 
// 		       int attenuationCurveCode_,float totalTorque_, int torqueApplicationFunctionCode_ ){
   float nump = 20; 
   float green[3] = {0.133333, 0.545098, 0.133333};
   
   float radianss = frand(0,1.2); //between 0 and 1;
   
   float baseWidth = frand(0.5,3);//float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	
	cryph::AffVector baseDir( frand(-1,1), 0, frand(-1,1) );
	
	cryph::AffVector upv( frand(-0.3,0.3),1, frand(-0.3,0.3) );
	cryph::AffVector outv( frand(-1,1) , 0 , frand(-1,1) );  
	float up = frand(3,9); // from 10
	float out = frand(up/12, up/2);
	float shade = frand(0,1.5);
	float randomShade[3] = {shade*forestGreen[0], shade*forestGreen[1], shade*forestGreen[2]};
	int radialAttenuationFunction = (frand(0,1)<0.5)? 2 : 3;
	int attenuation = (frand(0,1)<0.5)? 2 : 3;
	GrassBlade* grassblade = new GrassBlade(nump, bottomCenter, baseWidth, baseDir, upv, outv, up, out, randomShade, randomShade, attenuation, radianss, radialAttenuationFunction);
	c->addModel(grassblade);
   
    
    
    
  }
}
int main(int argc, char* argv[])
{
  
//   cryph::AffVector test(1,1,1);
//   std::cout << "before: " << test.dx << ", " << test.dy << ", " << test.dz <<"\n";
//   test = -test;
//   std::cout << "after negation: " << test.dx << ", " << test.dy << ", " << test.dz;
	ModelViewWithLighting::setShaderSources("vsh.c++", "fsh.c++");
	MyController c("Solomon's Temple", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);
	ModelViewWithLighting::setShaderSources("vsh.c++", "fsh.c++");
	// create your scene, adding things to the Controller....
	
	//ModelViewWithLighting* MVwLight = new ModelViewWithLighting();
	//MVwLight->render();
	//c.addModel(MVwLight);
	Column::instances = 0;
	cryph::AffPoint bottom1(-1.0,2,9);
	cryph::AffPoint top1(1,1,1);
	float color1[3] = {1,1,0};
	//Column column1(bottom1,1,top1,3,color1,true);
	//c.addModel(&column1);
	
	
//	std::cout << "colum1 bounds: " << bounds[0] << ", " << bounds[1] <<  bounds[2] << ", " << bounds[3] <<  bounds[4] << ", " << bounds[5] << "\n\ndone!";
	
	
	double bounds2[6];
	//stairs.getMCBoundingBox(bounds2);
//	c.addModel(&column2);
	//std::cout << "stair bounds: " << bounds2[0] << ", " << bounds2[1] <<", " <<  bounds2[2] << ", " << bounds2[3] <<", " <<   bounds2[4] << ", " << bounds2[5] << "\n\ndone!";
	
	
	//begin real project stuff;
	// base block ;
	float groundColor[3] = {0.0, 0.392,0.0}; // dark green {1,1,1};//392
	Block ground(1,200, 500, cryph::AffVector(0,1,0), cryph::AffPoint(-100,-1,100) ,cryph::AffPoint(100,-1,100), groundColor, -1, -1);
	c.addModel(&ground);
	
	cryph::AffPoint buildingFrontLeft(-50, 0,-80); // most important point to define!!
	cryph::AffPoint buildingFrontRight(50,0,-80); // this is fixed if stupid, but will be corrected to a ray specified by (right-left) vector. 
	cryph::AffVector buildingUpVector(0,1,0);
	float buildingHeight=30;
	float buildingWidth = 100;
	float buildingLength = 30;
	float buildingColor[3] = {0.961, 0.871, 0.702}; //wheat
	//just to check myself
	if( (buildingFrontLeft-buildingFrontRight).length() != buildingWidth){
	    cryph::AffVector v1 = buildingFrontRight-buildingFrontLeft;
	    v1.normalize();
	    v1*=buildingWidth;
	    buildingFrontRight = buildingFrontLeft + v1;
	}
	Block building(buildingHeight,buildingWidth, buildingLength, buildingUpVector, buildingFrontLeft ,buildingFrontRight, buildingColor,1,1);
	c.addModel(&building);
	
	cryph::AffPoint stairsBackCenter = (buildingFrontLeft + buildingFrontRight) /2;
	float stairWidthPercentOfBuildingFront = 0.70;
	float stairWidth = stairWidthPercentOfBuildingFront * buildingWidth;
	cryph::AffPoint stairsBackLeft = stairsBackCenter - ((buildingFrontRight - buildingFrontLeft)*stairWidthPercentOfBuildingFront)/2;
	cryph::AffPoint stairsBackRight = stairsBackCenter + ((buildingFrontRight - buildingFrontLeft)*stairWidthPercentOfBuildingFront)/2;
	cryph::AffVector towardsBuildingBack = buildingUpVector.cross((buildingFrontRight-buildingFrontLeft));
	towardsBuildingBack.normalize();
	float stairLengthPercentOfBuildingFront = 0.35;
	float stairLength = stairLengthPercentOfBuildingFront * buildingWidth;
	cryph::AffVector getUsToStairfront =  (-towardsBuildingBack)*stairLength;
	cryph::AffPoint stairFrontLeft = stairsBackLeft + getUsToStairfront;
	cryph::AffPoint stairFrontRight = stairsBackRight + getUsToStairfront;
	
	float stairHeightPercentOfBuildingHeight = 1;
	int numStairs = 9;
	float eachStepHeight = (stairHeightPercentOfBuildingHeight * buildingHeight)/numStairs; 
	
	//stairs test
	Stairs stairs(eachStepHeight, stairWidth, stairLength, buildingUpVector, stairFrontLeft ,stairFrontRight, buildingColor, 
		      eachStepHeight, numStairs, true);
	c.addModel(&stairs);
	
	
	
	
	//test a half cylinder

// 	float color2[3] = {0,1,0};
// 	cryph::AffPoint startP = stairFrontLeft + (stairFrontRight - stairFrontLeft);
// 	float myradius = (stairFrontRight-stairFrontLeft).length();
	//HalfColumn halfColumn( stairFrontLeft,myradius,(stairFrontLeft + (buildingUpVector*20)),200,color2,false,false, 3.14159/1,startP);
	double bounds[6];
	//column1.getMCBoundingBox(bounds);
	//c.addModel(&halfColumn);
	
// 	  cryph::AffPoint b(0,200,0);
// 	   cryph::AffPoint t(0,220,0);
// 	   cryph::AffPoint s(5,200,0);
	   
// 	  HalfColumn halfColumn1(b , 5, t,5,color2,false,false, 3.14159 ,s);
// 	  c.addModel(&halfColumn1);
// 	  test a fancy column
// 	  FancyColumn(cryph::AffPoint bottom_,float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, int numCircs_);
// 	  FancyColumn fancyColumn(stairFrontLeft, myradius, (stairFrontLeft + (buildingUpVector*20)), myradius, color2, false, 9);
// 	
	
	
	
	//test super fancy column
	
	//SuperFancyColumn(float height_, float width_, cryph::AffPoint bottomLeft_,cryph::AffVector upVector_, cryph::AffVector toRightFrontVector_,float color_[3]);
	float burlywood[3] ={0.871, 0.722, 0.529};
	float columnHeight= buildingHeight*4;
	float columnWidth = buildingWidth/4;
	
	cryph::AffPoint rightColRightPoint = buildingFrontRight + buildingHeight*buildingUpVector;
	cryph::AffVector v = buildingFrontLeft-buildingFrontRight;
	v.normalize();
	cryph::AffPoint rightColLeftPoint = rightColRightPoint + (v*columnWidth);
	float goldRod[3] = {0.855, 0.647, 0.125};
	float gold[3] = {1.000, 0.843, 0.000};
	SuperFancyColumn leftColumn(columnHeight,columnWidth ,buildingFrontLeft + buildingHeight*buildingUpVector, buildingUpVector, buildingFrontRight-buildingFrontLeft, gold, 10);
	SuperFancyColumn rightColumn(columnHeight,columnWidth , rightColLeftPoint  , buildingUpVector, buildingFrontRight-buildingFrontLeft, gold, 10);
	c.addModel(&leftColumn);
	c.addModel(&rightColumn);
	
	float heightSoFar = buildingHeight+columnHeight;
	
	//building(buildingHeight,buildingWidth, buildingLength, buildingUpVector, buildingFrontLeft ,buildingFrontRight, buildingColor);
	Block mainBuilding(heightSoFar, buildingWidth, buildingLength*8, buildingUpVector,(buildingFrontLeft+ towardsBuildingBack*buildingLength), buildingFrontRight-buildingFrontLeft, buildingColor,1, 0);
	c.addModel(&mainBuilding);
	
	
	//more building!
	cryph::AffPoint insertLeftSideLeftPoint = buildingFrontLeft + (towardsBuildingBack*columnWidth) + buildingUpVector*buildingHeight;
	cryph::AffPoint insertLeftSideRightPoint = insertLeftSideLeftPoint + (-v)*buildingWidth*(1.0/3.0);
	float insertHeight = heightSoFar - buildingHeight;
	float insertWidth = (insertLeftSideRightPoint-insertLeftSideLeftPoint).length();
	float insertLength = buildingLength - columnWidth;
	Block leftBlockInsert(insertHeight,insertWidth, insertLength, buildingUpVector, insertLeftSideLeftPoint,insertLeftSideRightPoint, buildingColor,1,2);
	c.addModel(&leftBlockInsert);
	
	cryph::AffPoint insertRightSideRightPoint = insertLeftSideLeftPoint + buildingWidth*(-v);
	cryph::AffPoint insertRigthSideLeftPoint = insertRightSideRightPoint + v*insertWidth;
	Block rightBlockInsert(insertHeight,insertWidth, insertLength, buildingUpVector, insertRigthSideLeftPoint ,insertRightSideRightPoint, buildingColor,1,3); //1 and 3 means 1=texture stones. 3=rightBlock front
	c.addModel(&rightBlockInsert);
	
	float insertMiddleHeight = (1.0/4.0)*heightSoFar;
	float insertMiddleWidth = buildingWidth - (2*insertWidth);
	cryph::AffPoint middleInsertLeftSide = insertLeftSideRightPoint + (heightSoFar - (insertMiddleHeight+ buildingHeight))*buildingUpVector;
	cryph::AffPoint middleInsertRightSide = insertRigthSideLeftPoint + (heightSoFar - (insertMiddleHeight+ buildingHeight))*buildingUpVector;
	Block insertMiddleBlock(insertMiddleHeight,insertMiddleWidth, insertLength, buildingUpVector, middleInsertLeftSide ,middleInsertRightSide, buildingColor,1,4);
	c.addModel(&insertMiddleBlock);
	
	
	//black doorway
	float doorHeight = heightSoFar - (insertMiddleHeight + buildingHeight);
	float doorWidth = insertMiddleWidth;
	cryph::AffPoint doorLeftPoint = insertLeftSideRightPoint  + towardsBuildingBack*(insertLength*(4.0/5.0)); //move it forward just a little bit.
	cryph::AffPoint doorRightPoint = insertRigthSideLeftPoint + towardsBuildingBack*(insertLength*(4.0/5.0)); // same as the prior
	float black[3]={0,0,0};
	Block doorWay(doorHeight,doorWidth, (1), buildingUpVector, doorLeftPoint ,doorRightPoint, black,2, -1); //2 means door texture, -1 means don't care about building code for stone mapping.
	c.addModel(&doorWay);
	
	//decorative backStairs
	float decorativeStairWidth = mainBuilding.length/7;
	float decorativeStairLength= mainBuilding.width/12;
	
	float numDecStairs = 4;
	float decStairEachHeight = mainBuilding.height/numDecStairs;
	float eachDecStairWidth = decorativeStairLength / numDecStairs; 
	cryph::AffPoint backLeft_backLeftPoint = mainBuilding.frontLeftBottomCorner + mainBuilding.getTowardsBackUnitVec()*mainBuilding.length;
	cryph::AffPoint backLeft_backRightPoint= mainBuilding.frontLeftBottomCorner + mainBuilding.getTowardsBackUnitVec()*(mainBuilding.length-decorativeStairWidth);
	//just kidding, they need to come off the wall some.
	backLeft_backLeftPoint += decorativeStairLength*v;
	backLeft_backRightPoint+= decorativeStairLength*v;
	float white[3]={1,1,1};
	int decStairNum=4;
	Stairs decorativeStairs1(decStairEachHeight,decorativeStairWidth, decorativeStairLength, buildingUpVector, backLeft_backLeftPoint ,backLeft_backRightPoint, white,
	  eachDecStairWidth, decStairNum, true);
	c.addModel(&decorativeStairs1);
	cryph::AffPoint otherSideDecStairRight = backLeft_backLeftPoint+ (-v)*(2*decorativeStairLength + buildingWidth); //reflected of course.
	cryph::AffPoint otherSideDecStairLeft = otherSideDecStairRight + (-towardsBuildingBack)*decorativeStairWidth;
	Stairs decorativeStairs2(decStairEachHeight,decorativeStairWidth, decorativeStairLength, buildingUpVector, otherSideDecStairLeft ,otherSideDecStairRight, white,
	  eachDecStairWidth, decStairNum, true);
	c.addModel(&decorativeStairs2);
	
	cryph::AffVector shiftToFrontV = (-towardsBuildingBack)*(mainBuilding.length - decorativeStairWidth);
	cryph::AffPoint decStairFrontLeft_Left = backLeft_backLeftPoint + shiftToFrontV;
	cryph::AffPoint decStairFrontLeft_Right = backLeft_backRightPoint + shiftToFrontV;
	Stairs decorativeStairs3(decStairEachHeight,decorativeStairWidth, decorativeStairLength, buildingUpVector, decStairFrontLeft_Left ,decStairFrontLeft_Right, white,
	  eachDecStairWidth, decStairNum, true);
	c.addModel(&decorativeStairs3);
	
	cryph::AffPoint decStairFrontRight_Left = otherSideDecStairLeft + shiftToFrontV;
	cryph::AffPoint decStairFrontRight_Right = otherSideDecStairRight+ shiftToFrontV;
	Stairs decorativeStairs4(decStairEachHeight,decorativeStairWidth, decorativeStairLength, buildingUpVector, decStairFrontRight_Left ,decStairFrontRight_Right, white,
	  eachDecStairWidth, decStairNum, true);
	c.addModel(&decorativeStairs4);
	
	
	
	//flat roof
	float roofHeight = mainBuilding.height * (1.0/13.0);
	float roofLength = mainBuilding.length + building.length;
	Block roof(roofHeight, buildingWidth,roofLength, buildingUpVector, building.frontLeftBottomCorner + (heightSoFar*buildingUpVector), building.frontRightBottomCorner + (heightSoFar*buildingUpVector), burlywood,1,5); // 1,5= use stone texture, 5 means roof
	c.addModel(&roof);
 	

	
	
	
	
	//place cones on roof
	int numAlongTopLength = 20;
	float spacing = roofLength* (1.0/(numAlongTopLength + (numAlongTopLength -1))); //same size for the thing and the space between it and width!. 
	float coneHeight = mainBuilding.height/15;
	float bottomRadius = spacing/2;
	float topRadius = bottomRadius/30;
	cryph::AffPoint firstConeCenter = roof.frontLeftBottomCorner + (spacing/2)*(-v) + (spacing/2)*roof.getTowardsBackUnitVec() + roof.height*buildingUpVector;
	
	float opacity = 0.5;
	Column column = *(new Column( firstConeCenter, bottomRadius, firstConeCenter + buildingUpVector*coneHeight, topRadius, gold, true, opacity));
	c.addModel(&column);
// 	cryph::AffPoint previousCenter = firstConeCenter - (mainBuilding.getTowardsBackUnitVec()*2*spacing);
// 	cryph::AffPoint centerP=previousCenter;
	cryph::AffVector mvDir2 = mainBuilding.getTowardsBackUnitVec();
	for(int i=0; i<numAlongTopLength; i++){
	  float newx;
	  float newy;
	  float newz;
	  float newxt;
	  float newyt;
	  float newzt;
	  
	  cryph::AffPoint tempB = firstConeCenter + i*(2*spacing)*mvDir2;
	  newx = tempB.x;
	  newy = tempB.y;
	  newz = tempB.z;
	  
	  cryph::AffPoint tempT = tempB + coneHeight*buildingUpVector;
	  newxt = tempT.x;
	  newyt = tempT.y;
	  newzt = tempT.z;
	 //Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[6], bool capped_){
	  cryph::AffPoint bottomP = (*(new cryph::AffPoint(newx,newy,newz)));
	  cryph::AffPoint topP = (*(new cryph::AffPoint(newxt,newyt,newzt)));
	//  std::cout << "feifjeifjeifj: " << bottomP.x << ", " << bottomP.y << ", " << bottomP.z << "\n";
	   Column* column = (new Column( bottomP, bottomRadius, topP, topRadius, gold, true, opacity));
//  	 Column column = *(new Column( *(new cryph::AffPoint(newx,newy,newz)), bottomRadius, *(new cryph::AffPoint(newxt,newyt,newzt)), topRadius, gold, true));
	 c.addModel(column);
 
	}
	firstConeCenter += (buildingWidth-(spacing))*(-v);
	//now do otherside;
	for(int i=0; i<numAlongTopLength; i++){
	  float newx;
	  float newy;
	  float newz;
	  float newxt;
	  float newyt;
	  float newzt;
	  
	  cryph::AffPoint tempB = firstConeCenter + i*(2*spacing)*mvDir2;
	  newx = tempB.x;
	  newy = tempB.y;
	  newz = tempB.z;
	  
	  cryph::AffPoint tempT = tempB + coneHeight*buildingUpVector;
	  newxt = tempT.x;
	  newyt = tempT.y;
	  newzt = tempT.z;
	 //Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[6], bool capped_){
	  cryph::AffPoint bottomP = (*(new cryph::AffPoint(newx,newy,newz)));
	  cryph::AffPoint topP = (*(new cryph::AffPoint(newxt,newyt,newzt)));
	//  std::cout << "feifjeifjeifj: " << bottomP.x << ", " << bottomP.y << ", " << bottomP.z << "\n";
	   Column* column = (new Column( bottomP, bottomRadius, topP, topRadius, gold, true,opacity));
//  	 Column column = *(new Column( *(new cryph::AffPoint(newx,newy,newz)), bottomRadius, *(new cryph::AffPoint(newxt,newyt,newzt)), topRadius, gold, true));
	 c.addModel(column);
	  
	  
	}	
	//now do along front
	
	float spaceToFill = buildingWidth - 3*spacing; //changed from 2*spacing. 2 from moving one cone over from each side, then half over from each side so the spacing is right
	int newNumToFit = round(numAlongTopLength*spaceToFill/roofLength);
// 	//how many spaces do I have?
// 	float spaces = spaceToFill/spacing;
// 	float newSpacing = spaceToFill/spaces;
// 	int numberToFit = (spaceToFill/newSpacing)
	float newSpacing =  spaceToFill* (1.0/(newNumToFit + (newNumToFit -1)));
	float newbottomRadius = newSpacing/2;
	float newtopRadius = newbottomRadius/30;
	
	//reset the direction
	mvDir2 = v;
	//move the starting point.
	firstConeCenter+= mvDir2*(spacing) + mvDir2*newSpacing/2;
	for(int i=0; i<newNumToFit; i++){
	  float newx;
	  float newy;
	  float newz;
	  float newxt;
	  float newyt;
	  float newzt;
	  
	  cryph::AffPoint tempB = firstConeCenter + i*(2*newSpacing)*mvDir2;
	  newx = tempB.x;
	  newy = tempB.y;
	  newz = tempB.z;
	  
	  cryph::AffPoint tempT = tempB + coneHeight*buildingUpVector;
	  newxt = tempT.x;
	  newyt = tempT.y;
	  newzt = tempT.z;
	 //Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[6], bool capped_){
	  cryph::AffPoint bottomP = (*(new cryph::AffPoint(newx,newy,newz)));
	  cryph::AffPoint topP = (*(new cryph::AffPoint(newxt,newyt,newzt)));
//	  std::cout << "feifjeifjeifj: " << bottomP.x << ", " << bottomP.y << ", " << bottomP.z << "\n";
	   Column* column = (new Column( bottomP, newbottomRadius, topP, newtopRadius, gold, true, opacity));
//  	 Column column = *(new Column( *(new cryph::AffPoint(newx,newy,newz)), bottomRadius, *(new cryph::AffPoint(newxt,newyt,newzt)), topRadius, gold, true));
	 c.addModel(column);
	  
	  
	}
	
	//repeat along back
	//reset the direction
	
	//move the starting point.
	firstConeCenter+= (roofLength-newSpacing)*mainBuilding.getTowardsBackUnitVec();
	for(int i=0; i<newNumToFit; i++){
	  float newx;
	  float newy;
	  float newz;
	  float newxt;
	  float newyt;
	  float newzt;
	  
	  cryph::AffPoint tempB = firstConeCenter + i*(2*newSpacing)*mvDir2;
	  newx = tempB.x;
	  newy = tempB.y;
	  newz = tempB.z;
	  
	  cryph::AffPoint tempT = tempB + coneHeight*buildingUpVector;
	  newxt = tempT.x;
	  newyt = tempT.y;
	  newzt = tempT.z;
	 //Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[6], bool capped_){
	  cryph::AffPoint bottomP = (*(new cryph::AffPoint(newx,newy,newz)));
	  cryph::AffPoint topP = (*(new cryph::AffPoint(newxt,newyt,newzt)));
//	  std::cout << "feifjeifjeifj: " << bottomP.x << ", " << bottomP.y << ", " << bottomP.z << "\n";
	   Column* column = (new Column( bottomP, newbottomRadius, topP, newtopRadius, gold, true, opacity));
//  	 Column column = *(new Column( *(new cryph::AffPoint(newx,newy,newz)), bottomRadius, *(new cryph::AffPoint(newxt,newyt,newzt)), topRadius, gold, true));
	 c.addModel(column);
	  
	  
	}
	
	
	//make a FIREPIT
	cryph::AffPoint stairsCenter = (stairs.frontRightBottomCorner-stairs.frontLeftBottomCorner)/2;
	//move foward some
	cryph::AffPoint firePitCenter = stairsCenter - mainBuilding.getTowardsBackUnitVec()*buildingLength*2;
	float firePitWidth = buildingLength;//LOL
	
	cryph::AffPoint firePitLeft = firePitCenter + v*(firePitWidth/2);
	cryph::AffPoint firePitRight = firePitCenter - v*(firePitWidth/2);
	//float height_, float width_, cryph::AffPoint bottomLeft_,cryph::AffVector upVector_, cryph::AffVector toRightFrontVector_,float color_[3], int numFancies_){
	FirePit firepit(buildingHeight*1.2, firePitWidth, firePitLeft, buildingUpVector, firePitRight-firePitLeft, burlywood, 12);
	  c.addModel(&firepit);
// 	
// 	
// 	//other FIREPIT
	cryph::AffPoint otherFireLeftPoint = firePitRight + v*(buildingWidth);
	cryph::AffPoint otherFireRightPoint = otherFireLeftPoint + (-v)*firePitWidth;
	FirePit firepit2(buildingHeight*1.2, firePitWidth, otherFireLeftPoint, buildingUpVector, firePitRight-firePitLeft, burlywood, 12);
	  c.addModel(&firepit2);
// 	  std::cout << "FIREPIT2: "<< firepit2.fancyColumn.top << "\n\n";
// 	  std::cout << "FIREPIT1: "<< firepit.fancyColumn.top << "\n\n";
// 	


// 
// 	
// 	//GRASS
 	cryph::AffPoint gbot =cryph::AffPoint(45,40,20); //firstConeCenter + (cryph::AffVector(0,20,0)) + cryph::AffVector(0,0,200);
// 	
	int attenuation = 3;
// 	:GrassBlade(int num_points_,
// 		       cryph::AffPoint bottom_, float baseWidth_, cryph::AffVector baseDirection_,
// 		       cryph::AffVector upV_, cryph::AffVector outV_, float upDistance_, float outDistance_, 
// 		       float colorT_[3],float colorB_[3], 
// 		       int attenuationCurveCode_,float totalTorque_, int torqueApplicationFunctionCode_ ){
	float nump = 20; float basewidth = 20; float up = 110; float out = 100; float radianss = 3.14592/2.5;
	float green[3] = {0.133333, 0.545098, 0.133333};
	//GrassBlade* grassblade = new GrassBlade(nump, gbot, basewidth, cryph::AffVector(0,0,1), cryph::AffVector(0,1,0), cryph::AffVector(1,0,0), up, out, green, gold, attenuation, radianss, 1);
	//c.addModel(grassblade);
 	makeGrassField(&ground, &c, &mainBuilding, &(firepit.baseBlock),&(firepit2.baseBlock),&stairs);
	
	
	//decorative stair roof
	cryph::AffPoint frontLeftDecStairRoof = roof.frontLeftBottomCorner + roof.mainNormal*roof.height;
	//now frontLeftDecStairRoof is in the corner, but at least on the top. Now I need to move in the size of the cones.
	float inDistance = fmax(spacing,newSpacing);
	inDistance =inDistance + 0.5*inDistance; //give ourselves some buffer.
	cryph::AffVector toright = roof.frontRightBottomCorner - roof.frontLeftBottomCorner;
	toright.normalize();
	cryph::AffVector d = roof.getTowardsBackUnitVec() + toright;
	d.normalize();
	d = d*inDistance;
	frontLeftDecStairRoof= frontLeftDecStairRoof + d;
	//now frontLeftDecStairRoof should be in the right spot.
	
	float moveOverD;
	moveOverD = roof.getWidth()- inDistance*1.5; // this is wrong I think.
	float stepH,stairW,stairL;
	stairL = roof.length - inDistance*1.5;
	int stairNumber = 13;
	//float white[3] = {1,1,1};
	
	Stairs roofstairs(eachStepHeight/2, moveOverD, stairL, buildingUpVector, frontLeftDecStairRoof ,frontLeftDecStairRoof + toright*moveOverD, goldRod, 
		      eachStepHeight, stairNumber, false);
	c.addModel(&roofstairs);
	
	
	
	
	
	glClearColor(0.0, 0.0, 0.0, 0.50);
	 double xyz[6];
	 c.getOverallMCBoundingBox(xyz);
// 	std::cout << "TOTAL  bounds: " << xyz[0] << ", " << xyz[1] <<", " <<  xyz[2] << ", " << xyz[3] <<", " <<   xyz[4] << ", " << xyz[5] << "\n\ndone!";
 	set3DViewingInformation(xyz);
	
	c.run();

	return 0;
}



  
  
  
  
