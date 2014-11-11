// HalfColumn.h

#ifndef HALFCOLUMN_SUBCLASS_H
#define HALFCOLUMN_SUBCLASS_H

#include "ModelViewWithLighting.h"

class HalfColumn : public ModelViewWithLighting
{
public:
	HalfColumn(); //default constructor for making arrays of them.
	HalfColumn(cryph::AffPoint bottom_,float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, bool inwardNormal_, float radians_,cryph::AffPoint startAt_);
	virtual ~HalfColumn();

	bool inwardNormal;
	
	cryph::AffPoint bottom;
	cryph::AffPoint top;
	double myBounds[6];
	float color[3];
	float bradius;
	float tradius;
	float radians;
	cryph::AffPoint startAt;
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	bool capped;
	unsigned int topCap[1000];
	unsigned int bottomCap[1000];
	bool displayCylEdges;
	bool displayCylFill;
	cryph::AffVector direction;
	GLuint vao[1];
	GLuint vbo[2];
  
	float black[3] = { 0.0, 0.0, 0.0 };
	cryph::AffVector defineStarter();
	void defineHalfColumn();
	void renderHalfColumn(float * color);
	void setBounds();
	void initializeCappingIndices();
};

#endif
