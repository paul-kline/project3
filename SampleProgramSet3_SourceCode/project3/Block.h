// Block.h

#ifndef BLOCKS_SUBCLASS_H
#define BLOCKS_SUBCLASS_H

#include "ModelViewWithLighting.h"

class Block : public ModelViewWithLighting
{
public:
	typedef float vec3[3];
	Block();
	Block(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffVector toRightCorner_, vec3 color_); //smarter constructor.
	Block(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffPoint frontRightBottomCorner_, vec3 color_);
	virtual ~Block();

	
	float myBounds[6];
	float color[3];
	float height;
	float width;
	float length;
	cryph::AffVector mainNormal;
	vec3* coords;
	cryph::AffPoint frontLeftBottomCorner;
	cryph::AffPoint frontRightBottomCorner;
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	
	
	
	
	float getWidth();
	float getHeight();
 
	cryph::AffVector getTowardsBackUnitVec();
private:
  
  


	cryph::AffVector widthVector;
	cryph::AffVector lengthVector;
	GLuint vao[1];
	GLuint vbo[1];

	void defineBlock();
	void renderBlock(float * color);
	void setBounds();
	void assignCoords();
	double calculateMax(char axis);
	double calculateMin(char axis);
};

#endif
