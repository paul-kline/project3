// Stairs.h

#ifndef STAIRS_SUBCLASS_H
#define STAIRS_SUBCLASS_H

#include "ModelViewWithLighting.h"
#include "Block.h"
class Stairs : public ModelViewWithLighting
{
public:
	typedef float vec3[3];
	Stairs(float height_, float width_, float length_, cryph::AffVector normal_,cryph::AffPoint frontLeftBottomCorner_,cryph::AffPoint frontRightBottomCorner_, vec3 color_,
	       float stepWidth_,int numStairs_, bool flatBack_);
	Stairs();
	virtual ~Stairs();

	
	float myBounds[6];
	float color[3];
	float height;
	float width;
	float length;
	float stepWidth;
	int   numStairs;
	bool flatBack;
	cryph::AffVector mainNormal;
	vec3* coords;
	cryph::AffPoint frontLeftBottomCorner;
	cryph::AffPoint frontRightBottomCorner;
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
  
	Block* blocks;


	cryph::AffVector widthVector;
	cryph::AffVector lengthVector;
	GLuint vao[1];
	GLuint vbo[1];

	void defineStairs();
	void setBounds();

	double calculateMax(int index);
	double calculateMin(int index);
};

#endif
