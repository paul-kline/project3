// Tetrahedron.h

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"
#include "ModelViewWithLighting.h"

class Tetrahedron : public ModelViewWithLighting
{
public:
	Tetrahedron(const cryph::AffPoint& p0, const cryph::AffPoint& p1,
		const cryph::AffPoint& p2, const cryph::AffPoint& p3,
		float R=1.0, float G=0.0, float B=0.0);
	virtual ~Tetrahedron();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();
private:
	GLuint vao[1];
	GLuint vbo[1];

	float color[3]; // the RGB color of this tetrahedron
	cryph::AffVector normal[4]; // the four normals to the four faces
	double minMax[6]; // for MC bounding box

	bool displayTetrahedronEdges, displayTetrahedronFill;

	void defineTetrahedron(const cryph::AffPoint verts[]);
	void renderTetrahedron(const float* rgb);
	void updateXYZBounds(const cryph::AffPoint& p);
};

#endif
