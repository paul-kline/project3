// FancyColumn.h

#ifndef FANCYCOLUMN_SUBCLASS_H
#define FANCYCOLUMN_SUBCLASS_H

#include "ModelViewWithLighting.h"
#include "HalfColumn.h"
class FancyColumn : public ModelViewWithLighting
{
public:
        FancyColumn();
	FancyColumn(cryph::AffPoint bottom_,float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped_, int numCircs_);
	virtual ~FancyColumn();

	cryph::AffPoint bottom;
	cryph::AffPoint top;
	double myBounds[6];
	float color[3];
	float bradius;
	float tradius;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	int NUM_AROUND_CIRCLE;
	typedef float vec3[3];
	int topCap[1000];
	int bottomCap[1000];
  
	int numCircs;
	int numPoints;
	HalfColumn halfColumns[100];
	
	
	bool displayCylEdges;
	bool displayCylFill;
	float black[3];
	bool capped;
	cryph::AffVector direction;
	GLuint vao[1];
	GLuint vbo[2];
  
  
	cryph::AffVector defineStarter();
	void defineFancyColumn();
	void renderFancyColumn(float * color);
	void setBounds();
	void initializeCappingIndices();
};

#endif
