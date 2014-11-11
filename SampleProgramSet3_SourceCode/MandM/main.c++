// main.c++: main program for simple 3D scene consisting
//           of an "M and M" shape along with a cylinder.

#include <iostream>
#include <fstream>

#include "GLFWController.h"
#include "Block.h"
#include "Cylinder.h"
#include "MandM.h"
#include "Tetrahedron.h"

void addTetrahedra(Controller& c)
{
	cryph::AffPoint p0(-1.1, -0.4, 0.4);
	cryph::AffPoint p1(-0.7, -0.4, 0.6);
	cryph::AffPoint p2(-0.3, -0.4, 0.5);
	cryph::AffPoint p3(-0.6, -0.1, 0.5);
	c.addModel(new Tetrahedron(p0, p1, p2, p3));

	p0.assign(0.971345, -0.500694, 1.11945);
	p1.assign(0.651218, -0.376426, 1.26641);
	p2.assign(1.04476, -0.30766, 1.16418);
	p3.assign(0.73595, -0.240765, 1.29965);
	c.addModel(new Tetrahedron(p0, p1, p2, p3, 0, 0.6, 0.2));

	p0.assign(0.650288, -0.167408, 0.514672);
	p1.assign(0.559247, -0.394237, 0.405895);
	p2.assign(0.548077, -0.133869, 0.479483);
	p3.assign(0.500305, -0.177127, 0.48515);
	c.addModel(new Tetrahedron(p0, p1, p2, p3, 0.2, 0.3, 0.8));

	p0.assign(1.25395, -0.248015, 0.674291);
	p1.assign(1.31247, -0.0525569, 0.6631);
	p2.assign(1.20728, -0.103504, 0.68977);
	p3.assign(1.22578, -0.324471, 0.594728);
	c.addModel(new Tetrahedron(p0, p1, p2, p3, 0.9, 0.6, 0.2));

	p0.assign(0.399133, -0.239639, 0.412327);
	p1.assign(0.214498, -0.113268, 0.450438);
	p2.assign(0.23978, -0.397119, 0.427417);
	p3.assign(0.113959, -0.212317, 0.450724);
	c.addModel(new Tetrahedron(p0, p1, p2, p3, 0.3, 0.7, 0.2));

	p0.assign(-0.286891, -0.140591, 0.471908);
	p1.assign(-0.0966624, -0.258691, 0.352737);
	p2.assign(-0.0833539, -0.0689214, 0.398166);
	p3.assign(-0.178247, -0.0579734, 0.466438);
	c.addModel(new Tetrahedron(p0, p1, p2, p3, 0.3, 0.3, 0.7));
}

bool addTetrahedra(Controller& c, const char* fName)
{
	std::ifstream is(fName);
	if (!is.good())
	{
		std::cerr << "Could not open " << fName << " for reading.\n";
		return false;
	}
	cryph::AffPoint p0, p1, p2, p3;
	float r, g, b;
	while (is >> p0 >> p1 >> p2 >> p3 >> r >> g >> b)
	{
		c.addModel(new Tetrahedron(p0, p1, p2, p3, r, g, b));
	}
	is.close();
	return true;
}

#define USE_COMMON_HEURISTIC 1

void set3DViewingInformation(double xyz[6])
{
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	// COMMON HEURISTIC FOR ESTABLISHING THE MC ==> EC TRANSFORMATION:
	// Create the line of sight through the center of the scene:
	// 1) Make the center of attention be the center of the bounding box.
	// 2) Move the eye away along some direction - here (0,0,1) - so that the
	//    distance between the eye and the center is (2*max scene dimension).
	// 3) Set the "up" direction vector to orient the 3D view
	//
	// IF YOU DO NOT CHOOSE TO USE THE COMMON HEURISTIC:
	// 1) Simple set eye, center, and up however you choose.
	//
	// NOTE: ModelView::getMatrices - used during display callbacks -
	//       implicitly assumes the line of sight passes through what we want
	//       to have in the center of the display window. Hence, even if you
	//       want to define the line of sight in a specific way, you MAY still
	//       want to use the center of the bounding box as the center of
	//       attention. (One common exception: focusing on a part of the scene
	//       far away from the center of the overall scene.)

#if USE_COMMON_HEURISTIC
	// 1:
	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);
	cryph::AffPoint center(xmid, ymid, zmid);

	// 2:
	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;

	double distEyeCenter = 2.0 * maxDelta;
	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);

	// 3:
	cryph::AffVector up = cryph::AffVector::yu;
#else
	cryph::AffPoint eye(10.0, 2.5, 20.0);
	cryph::AffPoint center(0.0, 0.0, 0.0);
	double distEyeCenter = eye.distanceTo(center);
	cryph::AffVector up(0.0, 1.0, 0.0);
#endif

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	// COMMON HEURISTIC FOR ESTABLISHING THE PROJECTION TRANSFORMATION:
	// Place the projection plane roughly at the front of scene and
	// set eye coordinate zmin/zmax clipping planes relative to it.
	// IMPORTANT NOTE: For perspective projections, the following must hold:
	// 1) zpp < 0
	// 2) zmin < zmax < 0
	// For non-perspective projections, it is only necessary that zmin < zmax.
#if USE_COMMON_HEURISTIC
	double zpp = -(distEyeCenter - 0.5*maxDelta);
	double zmin = zpp - maxDelta;
	double zmax = zpp + 0.5*maxDelta;
#else
	double zpp = -0.2;
	double zmin = -(distEyeCenter + 3.0);
	double zmax = -0.01;
#endif
	ModelView::setProjection(ORTHOGONAL);
	ModelView::setECZminZmax(zmin, zmax);
	ModelView::setProjectionPlaneZ(zpp);
}

int main(int argc, char* argv[])
{
	GLFWController c("Simple 3D", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ModelViewWithLighting::setShaderSources("simple3d.vsh", "simple3d.fsh");

	c.addModel(new MandM());
	c.addModel(new Block(0.3, -0.4, 1.2, 0.3, 0.3, 0.4));
	c.addModel(new Cylinder());
	if (argc > 1)
	{
		if (!addTetrahedra(c, argv[1]))
			addTetrahedra(c);
	}
	else
		addTetrahedra(c);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
