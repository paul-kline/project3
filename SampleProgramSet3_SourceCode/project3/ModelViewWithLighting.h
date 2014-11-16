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
	~ModelViewWithLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);

	static void setViewingRadius(float viewingRadius_);
	static float viewingRadius;
	
	static void addToGlobalRotationDegrees(double rx, double ry, double rz);
	
	static float globalRX;
	static float globalRY;
	
	static void letThereBeLight();
	static int numberOfLightSources;
	static float lightSources[20 * 4]; //maxed at 20. px,py,pz, w. 
	static float lightStrengths[20 * 3]; //also 20.
	static float ambientStrength[3];
	
	
	
	void getMCBoundingBox(double* xyzLimits) const;
	
	void render();
	
protected:

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_kd;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;

	
	static GLint uLoc_p_ecLightPos, uLoc_lightStrength, uLoc_actualNumLights,uLoc_globalAmbient;
	static GLint uProjection, uObliqueDirection, uLoc_ka,uLoc_ks; 
/*	
uniform int currentProjection=2; //alphabetical: 0=OBLIQUE, 1=ORTHOGONAL, 2=PERSPECTIVE
uniform vec3 obliqueDirection = vec3(0,0,1);
uniform vec4 p_mcLights[20];
uniform vec3 lightStrength[20];
uniform int actualNumLights = 1;
uniform vec3 ambientStrength = vec3(0.15, 0.15, 0.15);
uniform vec3 ka = vec3( 1.0, 0.0, 0.0); //these should all be set.
//uniform vec4 kd = vec4( 1.0, 0.5, 0.5, 1.0 );
uniform vec3 ks = vec3( 1.0, 0.5, 0.5); 
uniform float m;*/
	static void fetchGLSLVariableLocations();
	
	
	

private:
	static std::string vShaderSource, fShaderSource;
};

#endif
