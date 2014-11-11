// ModelViewWithLighting.h

#ifndef MODELVIEWWITHLIGHTING_H
#define MODELVIEWWITHLIGHTING_H

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"

class ModelViewWithLighting : public ModelView
{
public:
	ModelViewWithLighting();
	virtual ~ModelViewWithLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);

	static void setViewingRadius(float viewingRadius_);
	static float viewingRadius;
protected:

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_kd;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;

	static void fetchGLSLVariableLocations();
	
	
	

private:
	static std::string vShaderSource, fShaderSource;
};

#endif
