// Block.h

#ifndef BLOCK_H
#define BLOCK_H

#include <GL/gl.h>

#include "ModelViewWithLighting.h"

class Block : public ModelViewWithLighting
{
public:
	Block(float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz);// lengths in 3 directions
	virtual ~Block();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();
private:
	GLuint vao[1];
	GLuint vbo[1];

	float xmin, xmax, ymin, ymax, zmin, zmax;

	static GLuint indexList_xmin[4], indexList_ymin[4], indexList_ymax[4];

	bool displayBlockEdges, displayBlockFill;

	void defineBlock();
	void renderBlock(float* color);
};

#endif
