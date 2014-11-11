// MandM.h

#ifndef MANDM_H
#define MANDM_H

#include <GL/gl.h>

#include "ModelViewWithLighting.h"

class MandM : public ModelViewWithLighting
{
public:
	MandM();
	virtual ~MandM();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();
private:
	GLuint vao[1];
	GLuint vbo[1];

	bool displayMandMEdges, displayMandMFill;

	void defineMandM();
	void renderMandM(float* color);
};

#endif
