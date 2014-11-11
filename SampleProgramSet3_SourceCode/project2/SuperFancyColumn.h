// SuperFancyColumn.h

#ifndef SUPER_FANCYCOLUMN_SUBCLASS_H
#define SUPER_FANCYCOLUMN_SUBCLASS_H

#include "ModelViewWithLighting.h"
#include "HalfColumn.h"
#include "Block.h"
#include "Column.h"
#include "FancyColumn.h"
class SuperFancyColumn : public ModelViewWithLighting
{
public:
	SuperFancyColumn(float height_, float width_, cryph::AffPoint bottomLeft_,cryph::AffVector upVector_, cryph::AffVector toRightFrontVector_,float color_[3], int numFancies_);
	virtual ~SuperFancyColumn();


	float height;
	float width;
	float color[3];
	float myBounds[6];
	Block topBlock;
	Column topColumn;
	FancyColumn fancyColumn;
	Column lowerColumn;
	Block baseBlock;
	void render();
	cryph::AffPoint bottomLeft;
	cryph::AffVector upVector;
	cryph::AffVector toRightFrontVector;
	void getMCBoundingBox(double* xyzLimitsF) const;
	

private:
  
  
	int numFancies;

	//const int NUM_AROUND_CIRCLE = 100;
	typedef float vec3[3];
	void setBounds();
	void defineSuperFancyColumn();
	bool displayCylEdges;
	bool displayCylFill;
	float black[3];


};

#endif
