// FirePit.h

#ifndef FIREPIT_SUBCLASS_H
#define FIREPIT_SUBCLASS_H

#include "ModelViewWithLighting.h"
#include "HalfColumn.h"
#include "Block.h"
#include "Column.h"
#include "FancyColumn.h"
class FirePit : public ModelViewWithLighting
{
public:
	FirePit(float height_, float width_, cryph::AffPoint bottomLeft_,cryph::AffVector upVector_, cryph::AffVector toRightFrontVector_,float color_[3], int numFancies_);
	virtual ~FirePit();


	float height;
	float width;
	float color[3];
	double myBounds[6];

	Column topColumn;
	FancyColumn fancyColumn;
	Column lowerColumn;
	Block baseBlock;
	Column* bufferColumn;
	
	
	cryph::AffPoint bottomLeft;
	cryph::AffVector upVector;
	cryph::AffVector toRightFrontVector;
	
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	

private:
  
  
	int numFancies;

	//const int NUM_AROUND_CIRCLE = 100;
	typedef float vec3[3];
	
	void setBounds();
	
	void defineFirePit();
	bool displayCylEdges;
	bool displayCylFill;
	float black[3];


};

#endif
