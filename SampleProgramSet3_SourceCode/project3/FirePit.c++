// FirePit.c++

#include "FirePit.h"
#include "ShaderIF.h"
#include <cmath>
#include "GLFWController.h"
#include "HalfColumn.h"
#include "Block.h"



  



FirePit::FirePit(float height_, float width_, cryph::AffPoint bottomLeft_,cryph::AffVector upVector_, cryph::AffVector toRightFrontVector_,float color_[3], int numFancies_){

  height = height_;
  width = width_;
  bottomLeft = bottomLeft_;
  numFancies= numFancies_;
  toRightFrontVector = toRightFrontVector_;
  toRightFrontVector.normalize(); //just in case I was silly.
  
  upVector=upVector_;
  upVector.normalize();
 color[0] = color_[0];
 color[1] = color_[1];
 color[2] = color_[2];
 
 defineFirePit();
  setBounds();
  black[0] =0.0; black[1] =0.0; black[2]=0.0;
 //for moving the eye.
  
  
  
};

FirePit::~FirePit()
{
  delete &baseBlock;
  delete &bufferColumn;// bufferColumn->render();
  delete &lowerColumn;// lowerColumn.render();
  delete &topColumn;// topColumn.render();
  delete &fancyColumn;// fancyColumn.render();
  
}



void FirePit::defineFirePit()
{
  
  float blockHeightPercentage = 0.20;
  float leadingColumnHeightPercentage = 0.08;
  float bowlHeightPercentage = 0.30;
  float fancyColumnHeightPercentage = 1 - (blockHeightPercentage + leadingColumnHeightPercentage + bowlHeightPercentage); // he gets the rest
  float radiusPercentOfWidth = 0.90;
  
  float blockHeight = blockHeightPercentage * height;
  float blockWidth = width;
  
  
  //Block(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffPoint frontRightBottomCorner_, vec3 color_)
  baseBlock = *(new Block(blockHeight, blockWidth,blockWidth, upVector, bottomLeft, bottomLeft + (blockWidth*toRightFrontVector),color));
  
//   cryph::AffPoint secondBottomLeft = (bottomLeft + (upVector* (height - blockHeight)));
//   cryph::AffPoint secondBottomRight = secondBottomLeft + (blockWidth*toRightFrontVector);
//   topBlock = *(new Block(blockHeight,blockWidth,blockWidth, upVector, secondBottomLeft,secondBottomRight,color));
//   
  //now the leader columns
  //Column(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[6], bool capped_)
  cryph::AffVector towardsBack = upVector.cross(toRightFrontVector);
  towardsBack.normalize();
    cryph::AffPoint bottom_dead_centerPoint = (bottomLeft + ((blockWidth/2)*toRightFrontVector)) + ((blockWidth/2)*towardsBack);
    cryph::AffPoint lowercolumnbottom = bottom_dead_centerPoint + (blockHeight*upVector);
    float leadingcolRadius = (radiusPercentOfWidth*blockWidth)/2;
    float smallerRadius = 0.80 * leadingcolRadius; //lets try 80% of the big one.
    float leadingcolHeight = leadingColumnHeightPercentage*height;
    
  lowerColumn= *(new Column(lowercolumnbottom, leadingcolRadius, (lowercolumnbottom + (leadingcolHeight*upVector)), smallerRadius, color, true));
  
  
  float bigRadius = blockWidth;
  float bowlHeight = bowlHeightPercentage*height;
  cryph::AffPoint topColumnBottom = lowercolumnbottom + (leadingcolHeight * upVector) + fancyColumnHeightPercentage*height*upVector ;
  
  //so it can hold something
  bufferColumn = (new Column(topColumnBottom ,leadingcolRadius , topColumnBottom+ (0.05*upVector) ,leadingcolRadius  ,color,true));
  
  topColumn = *(new Column(topColumnBottom ,leadingcolRadius , topColumnBottom+ (bowlHeight*upVector) ,bigRadius  ,color,false));
  
  
  //FancyColumn(cryph::AffPoint bottom_, float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, int numCircs_){
  fancyColumn = *(new FancyColumn( (lowercolumnbottom + leadingcolHeight*upVector) , smallerRadius, topColumnBottom, smallerRadius, color,true,numFancies));
  
  
//  Controller* c = GLFWController::getCurrentController();
//   c->addModel(&fancyColumn);
//   c->addModel(&baseBlock);
//   c->addModel(&lowerColumn);
//   c->addModel(&topColumn);
//   c->addModel(bufferColumn);
  
  
}


void FirePit::setBounds()
{
  double bounds1[6]; baseBlock.getMCBoundingBox(bounds1);
  double bounds2[6]; bufferColumn->getMCBoundingBox(bounds2);
  double bounds3[6]; lowerColumn.getMCBoundingBox(bounds3);
  double bounds4[6]; topColumn.getMCBoundingBox(bounds4);
  double bounds5[6]; fancyColumn.getMCBoundingBox(bounds5);
  
  for(int i=0; i<3; i++){
    int j = 2*i;
    myBounds[j] = std::min(bounds1[j], std::min(bounds2[j], std::min(bounds3[j], std::min(bounds4[j], bounds5[j]))));
    myBounds[j+1] = std::max(bounds1[j+1], (std::max(bounds2[j+1], std::max(bounds3[j+1], std::max(bounds4[j+1], bounds5[j+1])))));
  }

}

void FirePit::getMCBoundingBox(double* xyzLimitsF) const
{
    for(int i=0; i<6; i++){
      xyzLimitsF[i] = myBounds[i];
  }
}
void FirePit::render()
{
  baseBlock.render();
  bufferColumn->render();
  lowerColumn.render();
  topColumn.render();
  fancyColumn.render();
}








