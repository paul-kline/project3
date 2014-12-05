// Simpleshape.h

#ifndef SIMPLESHAPE_SUBCLASS_H
#define SIMPLESHAPE_SUBCLASS_H

#include "ModelViewWithLighting.h"

class Simpleshape : public ModelViewWithLighting
{
public:
	Simpleshape();
	virtual ~Simpleshape();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
};

#endif
