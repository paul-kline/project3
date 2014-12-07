// Block.c++

#include "Block.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"

#include "ImageReader.h"



typedef float vec3[3];


bool displayCylEdges2 = false;
bool displayCylFill2=true;
float black2[] = { 0.0, 0.0, 0.0 };

Block::Block()
{
 height=1;
 width=1;
 length=1;
 
 cryph::AffVector mainNormal(0,1,0);
 mainNormal.normalize(); //just in case someone hadn't..
 
frontLeftBottomCorner = cryph::AffPoint(-1,0,0);
frontLeftBottomCorner=cryph::AffPoint( 1,0,0);

 color[0] = 0;
 color[1] = 0;
 color[2] = 0;
 
 widthVector= frontRightBottomCorner - frontLeftBottomCorner;
 widthVector.normalize();
 
 lengthVector = mainNormal.cross(widthVector);
 lengthVector.normalize();
 
 
// defineBlock(); 
// setBounds();

}
Block::Block(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffVector toRightCorner_, vec3 color_, int whichTexture_, int buildingPieceCode_){

 height=height_;
 width=width_;
 length=length_;
 
 whichTexture = whichTexture_;
 buildingPieceCode = buildingPieceCode_;
 mainNormal=normal_;
 mainNormal.normalize(); //just in case someone hadn't..
 
 frontLeftBottomCorner=frontLeftBottomCorner_;
 cryph::AffVector toRight;
 toRightCorner_.normalizeToCopy(toRight);
 
 frontRightBottomCorner=frontLeftBottomCorner + width*toRight;

 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 
 widthVector= frontRightBottomCorner - frontLeftBottomCorner;
 widthVector.normalize();
 
 lengthVector = mainNormal.cross(widthVector);
 lengthVector.normalize();
 
 
 defineBlock(); 
 setBounds();
 
 texID = defineTexture();
  
};

Block::Block(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffPoint frontRightBottomCorner_, vec3 color_, int whichTexture_, int buildingPieceCode_){

 height=height_;
 width=width_;
 length=length_;
 
 whichTexture = whichTexture_;
 buildingPieceCode = buildingPieceCode_;
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
 
 
 defineBlock(); 
 setBounds();
 
 texID = defineTexture();
  
};

Block::~Block()
{
//   if(coords != NULL){
//    delete coords;
//   coords = NULL;
//   }
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Block::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = myBounds[0];
	xyzLimits[1] = myBounds[1];
	xyzLimits[2] = myBounds[2]; 
	xyzLimits[3] = myBounds[3]; 
	xyzLimits[4] = myBounds[4];    
	xyzLimits[5] = myBounds[5];
}

void Block::defineBlock()
{
	coords = new vec3[8]; //for each point of course.
	assignCoords();
	
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), coords, GL_DYNAMIC_DRAW);
			
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);
	glDisableVertexAttribArray(pvaLoc_mcNormal);
		
	
	
	
	defineTextureCoordinates();
	
	  
	
  
}

void Block::assignCoords()
{
  coords[0][0]=(float)frontLeftBottomCorner.x; coords[0][1]=(float)frontLeftBottomCorner.y; coords[0][2]=(float)frontLeftBottomCorner.z;
  
  cryph::AffPoint tempP;
  tempP = frontLeftBottomCorner + height*mainNormal;
  //vec3 t1;
  //t1[0]= (float)tempP.x; t1[1]=(float)tempP.y; t1[2]=(float)tempP.z;
  coords[1][0]=(float)tempP.x; coords[1][1]=(float)tempP.y; coords[1][2]=(float)tempP.z; //top left
 // coords[1]=t1;
  
  tempP = tempP + width * widthVector; // top right
  coords[2][0] = (float)tempP.x; coords[2][1]=(float)tempP.y; coords[2][2]=(float)tempP.z;
  
  tempP = tempP + (-mainNormal) * height; // bottom right
  coords[3][0] = (float)tempP.x; coords[3][1]=(float)tempP.y; coords[3][2]=(float)tempP.z;
  
  tempP = tempP + length * lengthVector; // back bottom right
  coords[4][0] = (float)tempP.x; coords[4][1]=(float)tempP.y; coords[4][2]=(float)tempP.z;
  
  tempP = tempP + height * mainNormal; //back top right
  coords[5][0] = (float)tempP.x; coords[5][1]=(float)tempP.y; coords[5][2]=(float)tempP.z;
  
  tempP = tempP + (-widthVector)*width; // back top left
  coords[6][0] = (float)tempP.x; coords[6][1]=(float)tempP.y; coords[6][2]=(float)tempP.z;
  
  tempP = tempP + (-mainNormal)*height; // back bottom left
  coords[7][0] = (float)tempP.x; coords[7][1]=(float)tempP.y; coords[7][2]=(float)tempP.z;
  
  
  //now they are in a niiiiice ordering.
  
  
}

void Block::setBounds()
{
  //cryph::AffVector direction =(top - bottom).normalize(); I'll come back to proper bounding when in clear thought
  //x
  myBounds[0] = calculateMin('x');
  myBounds[1] = calculateMax('x');
  //y
  myBounds[2] = calculateMin('y');
  myBounds[3] = calculateMax('y');
  //z
  myBounds[4] = calculateMin('z');
  myBounds[5] = calculateMax('z');
  
  
}
double Block::calculateMax(char axis){
  int j;
  switch ( axis ) {
  case 'x' : 
    j=0;
    break;
  case 'y' : 
    j=1;
    break;
   case 'z' : 
    j=2;
    break;
  default : 
    j=-1;
  }
  double maxval = coords[0][j];
  int i;
  for(i=1; i<8; i++){
    if(coords[i][j]>maxval){
      maxval = coords[i][j];
    }
  }
  return maxval;  
}

double Block::calculateMin(char axis){
  int j;
  switch ( axis ) {
  case 'x' : 
    j=0;
    break;
  case 'y' : 
    j=1;
    break;
   case 'z' : 
    j=2;
    break;
  default : 
    j=-1;
  }
  double minval = coords[0][j];
  int i;
  for(i=1; i<8; i++){
    if(coords[i][j]<minval){
      minval = coords[i][j];
    }
  }
  return minval;  
}

void Block::renderBlock(vec3 color){
	typedef float vec3[3];
	//vec3 colColor = {1, 0.0, 0.0};
	glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(NUM_AROUND_CIRCLE+1)); 
  
 	 glDisableVertexAttribArray(pvaLoc_mcNormal);
	 cryph::AffVector tempV = -lengthVector;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	// glEnableVertexAttribArray(pvaLoc_mcNormal);
	 tempV = widthVector;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawArrays(GL_TRIANGLE_FAN, 2, 4); 
	 
	 tempV = lengthVector;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	 
	 //rest must be drawn special!!
	 
	 unsigned int temp[4]= {7,6,1,0};
	 tempV = -widthVector;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawElements(GL_TRIANGLE_FAN,4,GL_UNSIGNED_INT,  temp);
	 
	 
	 //top
	 temp[0]=6;
	 temp[1]=1;
	 temp[2]=2;
	 temp[3]=5;
	 
	 //glDisableVertexAttribArray(pvaLoc_texCoords);
	 glUniform1i(ppuLoc_whichTexture, -1); //no texture mapping to happen here. This indicates that no blending will occur in the frag shader.
	 tempV = mainNormal;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawElements(GL_TRIANGLE_FAN,4,GL_UNSIGNED_INT,  temp);
	 
	  //bottom
	 temp[0]=0;
	 temp[1]=3;
	 temp[2]=4;
	 temp[3]=7;
	 
	 tempV = mainNormal;
	 glVertexAttrib3f(pvaLoc_mcNormal, tempV.dx,tempV.dy,tempV.dz);
	 glDrawElements(GL_TRIANGLE_FAN,4,GL_UNSIGNED_INT,  temp);
	/* 
	 glDrawElements(GL_TRIANGLE_FAN,(NUM_AROUND_CIRCLE+1),GL_UNSIGNED_INT,  bottomCap);
	 glVertexAttrib3f(pvaLoc_mcNormal, direction.dx,direction.dy,direction.dz);
	 glDrawElements(GL_TRIANGLE_FAN,(NUM_AROUND_CIRCLE+1),GL_UNSIGNED_INT,  topCap);
	 glE*/
	 glEnableVertexAttribArray(pvaLoc_mcNormal); //be nice and turn it back on if someone else forgets.
	 //glEnableVertexAttribArray(pvaLoc_texCoords);
	 glUniform1i(ppuLoc_whichTexture, whichTexture); //set the which Texture back to what it was.
}




void Block::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	glUniform1f(ppuLoc_currentOpacity, 1.0);
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	//
	if(!lightingModelLocked){
	  ModelViewWithLighting::letThereBeLight(color,color,color,40);//ModelViewWithLighting::letThereBeLight(goldka,goldkd,goldks,goldm); 
	}
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the textureMap variable in fragment shader to use "texture unit 0".
	glUniform1i(ppuLoc_textureMap, 0);
	glUniform1i(ppuLoc_whichTexture, whichTexture);
	
	
         
	if (displayCylFill2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderBlock(color);
	}
	if (displayCylEdges2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderBlock(black2);
	}
	
	
	
	glUseProgram(pgm);
	
}

float Block::getWidth(){
  return width;
}
float Block::getHeight(){
  return height;
}
cryph::AffVector Block::getTowardsBackUnitVec(){
  cryph::AffVector t = up.cross(frontRightBottomCorner-frontLeftBottomCorner);
  t.normalize();
  return cryph::AffVector(t.dx, t.dy,t.dz);//(*(new cryph::AffVector(t.dx, t.dy,t.dz)));
  
}


GLuint Block::defineTexture()
{
  
  //return 0;
  if(whichTexture == -1) return 0; //do nothing if no texture for this object.
  
  ImageReader* texImage;
  const char* imageLoc;// = "./";
  switch ( whichTexture ) {
  case 1 : //building texture 
   
   imageLoc = "./BrickOldRounded0134_2_S.jpg";
   texImage = ImageReader::create(imageLoc);
    break;
  case 2 : //door texture 
   imageLoc = "./DoorsMetalOrnate0035_1_L.jpg";
   texImage = ImageReader::create(imageLoc);
    break;
  default : //otherwise I don't know what to do with it and no texture should be drawn.
    whichTexture = -1;
    return 0;
  
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLuint locID[1];
  glGenTextures(1, locID);
  glBindTexture(GL_TEXTURE_2D, locID[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  GLint level = 0; // mipmap level 0
  int pw = texImage->getWidth();
  int ph = texImage->getHeight();
  GLint iFormat = texImage->getInternalFormat();
  GLenum format = texImage->getFormat();
  GLenum type = texImage->getType();
  const GLint border = 0; // must be zero (only present for backwards compatibility)
  const void* pixelData = texImage->getTexture();
  glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData);

  delete texImage; // all relevant information has been copied to OpenGL

  return locID[0];

}
void Block::defineTextureCoordinates()
{
  
  
    
  typedef float vec2[2];  
  vec2* textcoordsArray = new vec2[8];
	
	
  
  
  float width;
  float height;
  float depth;
  float zero;
  zero=0;
  switch ( buildingPieceCode ) {
    case 0 : //main building
      width  = 1;
      height = 3;
      depth  = 5; 
      break;
    
    case 1 : //front base thing
      width  = 3;
      height = 1;
      depth  = 0.7; 
      break;
    
    case 2 : //left of door
      width  = 1;
      height = 2;
      depth  = 0.1; 
      break;
     
    case 3 : //right of door
      width  = 1;
      height = 2;
      depth  = 0.1; 
      break;
     
    case 4 : //above door
      width  = 1;
      height = 1;
      depth  = 0.1; 
      break;
    
    case 5 : //roof
      width  = 3;
      height = 0.16;
      depth  = 5; 
      break;
  
  default : //otherwise I don't know what to do with it and no texture should be drawn.
    width  = 1;
    height = 1;
    depth  = 0.1; 
  }
  
  
  textcoordsArray[0][0]=zero;   textcoordsArray[0][1]=zero;   //bottom left 
  textcoordsArray[1][0]=zero;   textcoordsArray[1][1]=height; // top left
  textcoordsArray[2][0]=width;  textcoordsArray[2][1]=height; // top right
  textcoordsArray[3][0]=width;  textcoordsArray[3][1]=zero;   // bottom right
  textcoordsArray[4][0]=-depth + width;     textcoordsArray[4][1]=zero;   // back bottom right
  textcoordsArray[5][0]=-depth + width;     textcoordsArray[5][1]=height;      // back top right
  textcoordsArray[6][0]=-depth; textcoordsArray[6][1]=height; // back top left
  textcoordsArray[7][0]=-depth; textcoordsArray[7][1]=zero;   // back bottom left
  
  if(whichTexture == 2){ // then this is a door.
    textcoordsArray[0][0]=0; textcoordsArray[0][1]=0; //bottom left 
    textcoordsArray[1][0]=0; textcoordsArray[1][1]=1; // top left
    textcoordsArray[2][0]=1; textcoordsArray[2][1]=1; // top right
    textcoordsArray[3][0]=1; textcoordsArray[3][1]=0; // bottom right
    textcoordsArray[4][0]=-4; textcoordsArray[4][1]=0; // back bottom right
    textcoordsArray[5][0]=-4; textcoordsArray[5][1]=3; // back top right
    textcoordsArray[6][0]=-5; textcoordsArray[6][1]=3; // back top left
    textcoordsArray[7][0]=-5; textcoordsArray[7][1]=0; // back bottom left

  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec2), textcoordsArray, GL_DYNAMIC_DRAW);
	  
  

  glVertexAttribPointer(pvaLoc_texCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(pvaLoc_texCoords);


}



