// House.h - concrete subclass of ModelView that represents a House

#ifndef HOUSE_H
#define HOUSE_H

class ShaderIF;

#include <string>

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"

class CFont;
class CGLString;

class House : public ModelView
{
public:
	// A house is defined with center of base at (xb,yb). The main part of the
	// house has dimensions width x height. The roof is a triangle with (base,
	// height) = (roofWidth, roofHeight).
	// REQUIREMENT: roofWidth >= width.
	House(double xbIn, double ybIn, double widthIn, double heightIn,
		double roofWidthIn, double roofHeightIn, const std::string& label);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

	// House labels use the current font
	static void setHouseLabelFont(std::string font);
private:
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[1]; // Stores points for roof, house, and door
	// original defining data
	double xb, yb, width, height, roofWidth, roofHeight;
	// house label information
	CGLString* houseLabel;

	// House labels use the current font:
	static CFont* houseLabelFont;

	// all instances use same shader program
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_texCoords;
	static GLint ppuLoc_scaleTrans;
	static GLint ppuLoc_housePart;
	static GLint ppuLoc_fontColor, ppuLoc_fontTextureMap, ppuLoc_renderingFontString;

	void buildHouseLabel(const std::string& label);
	void defineModel();
	static void fetchGLSLVariableLocations();
	void renderHouseLabel() const;
	void validateData();
};

#endif
