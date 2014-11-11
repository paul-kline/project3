// ModelView.h - an Abstract Base Class for a combined Model and View for OpenGL
//
// This definition defines class ModelView as an Abstract Base Class by factoring out
// a minimal set of methods that can be used by a Controller to manage a collection of
// abstract models. The public methods here are those required by the Controller; the
// protected methods implement common operations likely required by all concrete subclasses,
// hence they can be implemented once in this abstract base class. Specifically:
// 1. Methods "getMCBoundingBox" and "render" are now pure virtual.
// 2. Methods "handleCommand" and "handleSpecialCommand" are given default (empty)
//    implementations. Subclasses need only override if they actually want to handle
//    commands from the keyboard or elsewhere.
// 3. PRIMARILY FOR 2D APPLICATIONS:
//    The protected class method "compute2DScaleTrans" is designed to be used in 2D
//    applications. ModelView subclasses can use this method to fetch the scale and
//    translation terms in x and y that implement a basic window-viewport transformation.
//    The implementation uses the current mcRegionOfInterest to compute the scale/translation
//    terms that map points inside that window into the -1 <= [x,y] <= +1 logical viewport
//    range while preserving aspect ratios. Subclasses will generally simply pass these
//    scale and translation terms to their vertex shaders.
// 4. FOR 3D APPLICATIONS:
//    See the protected method "getMatrices".
// 5. The GLSL shader variable lookup routines are also available here via protected methods.

#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <string>

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"
#include "Controller.h"
#include "Matrix4x4.h"
#include "ProjectionType.h"

class ModelView
{
public:
	ModelView();
	virtual ~ModelView();

	virtual void getMCBoundingBox(double* xyzLimits) const = 0;
	virtual void handleCommand(unsigned char key, double ldsX, double ldsY) { }
	virtual void handleFunctionKey(int whichFunctionKey, int x, int y, int mods) { }
	virtual void handleSpecialKey(Controller::SpecialKey key, double ldsX, double ldsY, int mods) { }
	virtual void render() = 0;

	// The portion of the viewing API common to 2D and 3D:
	static void setMCRegionOfInterest(double xyz[]);

	// Additional viewing API strictly for 3D:
	static void setECZminZmax(double zMinIn, double zMaxIn);
	static void setEyeCenterUp(cryph::AffPoint E, cryph::AffPoint C, cryph::AffVector up);
	static void setObliqueProjectionDirection(const cryph::AffVector& dir);
	static void setProjection(ProjectionType pType);
	static void setProjectionPlaneZ(double zppIn);

	// Viewing interfaces for dynamic view manipulation
	static void addToGlobalRotationDegrees(double rx, double ry, double rz);
	static void addToGlobalZoom(double increment);  // dynamic_zoomScale += increment

protected:

	// NOTE: Subclasses will generally use EITHER compute2DScaleTrans OR getMatrices:
	// Method "compute2DScaleTrans" is intended for 2D scenes:
	static void compute2DScaleTrans(float* scaleTransF);
	// Method "getMatrices" is intended for 3D scenes:
	static void getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds);

	static void linearMap(double fromMin, double fromMax,
		double toMin, double toMax, double& scale, double& trans);
	static void matchAspectRatio(double& xmin, double& xmax,
		double& ymin, double& ymax, double vAR);
	// "pp": "per-primitive"; "pv": "per-vertex"
	static GLint ppUniformLocation(GLuint glslProgram, const std::string& name);
	static GLint pvAttribLocation(GLuint glslProgram, const std::string& name);

	// Class variables used to save information passed into the 3D Viewing API:
	static double ecZmin, ecZmax, zpp;
	static cryph::AffPoint eye, center;
	static cryph::AffVector up;
	static ProjectionType projType;
	static cryph::AffVector obliqueProjectionDir;
	static double mcRegionOfInterest[6];

	// Class variables used to save information for dynamic viewing:
	static double dynamic_zoomScale; // dynamic zoom
	static cryph::Matrix4x4 dynamic_view; // dynamic 3D rotation/pan
};

#endif
