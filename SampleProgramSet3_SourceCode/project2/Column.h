// Column.h

#ifndef COLUMN_SUBCLASS_H
#define COLUMN_SUBCLASS_H

#include "ModelViewWithLighting.h"

class Column : public ModelViewWithLighting
{
public:
	Column();
	Column(cryph::AffPoint bottom_,float bradius_, cryph::AffPoint top_, float tradius_, float color_[3], bool capped);
	virtual ~Column();

	cryph::AffPoint bottom;
	cryph::AffPoint top;
	float myBounds[6];
	float color[3];
	float bradius;
	float tradius;
	static int instances;
	
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	bool Ihandle;
	bool capped;
	unsigned int topCap[1000];
	unsigned int bottomCap[1000];
	bool displayCylEdges = false;
	bool displayCylFill=true;
	
	GLuint vao[1];
	GLuint vbo[2];
  
	void myhandleKeys();
	cryph::AffVector defineStarter();
	void defineColumn();
	void renderColumn(float * color);
	void setBounds();
	void initializeCappingIndices();
};

#endif
