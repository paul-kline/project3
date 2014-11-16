// ModelViewWithLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithLighting.h"
#include "ShaderIF.h"
#include "MyController.h"

ShaderIF* ModelViewWithLighting::shaderIF = NULL;
int ModelViewWithLighting::numInstances = 0;
GLuint ModelViewWithLighting::shaderProgram = 0;
GLint ModelViewWithLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithLighting::ppuLoc_kd = -2;
GLint ModelViewWithLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithLighting::ppuLoc_ec_lds = -2;

GLint ModelViewWithLighting::uLoc_p_ecLightPos = -2;
GLint ModelViewWithLighting::uLoc_lightStrength= -2;
GLint ModelViewWithLighting::uLoc_actualNumLights= -2;
GLint ModelViewWithLighting::uLoc_globalAmbient = -2;
GLint ModelViewWithLighting::uProjection = -2;
GLint ModelViewWithLighting::uObliqueDirection = -2;
float ModelViewWithLighting::lightSources[20 * 4]; //maxed at 20. px,py,pz, w. 
float ModelViewWithLighting::lightStrengths[20 * 3]; //also 20.
float ModelViewWithLighting::ambientStrength[3];
GLint ModelViewWithLighting::uLoc_ka = -2;
GLint ModelViewWithLighting::uLoc_ks = -2;

std::string ModelViewWithLighting::vShaderSource = "vsh.c++";
std::string ModelViewWithLighting::fShaderSource = "fsh.c++";

float ModelViewWithLighting::viewingRadius = 1;
double dynamic_zoomScale = 1;

float ModelViewWithLighting::globalRX =0;
float ModelViewWithLighting::globalRY=0;
int ModelViewWithLighting::numberOfLightSources = 0;
ModelViewWithLighting::ModelViewWithLighting()
{
	if (shaderProgram == 0)
	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;
}

ModelViewWithLighting::~ModelViewWithLighting()
{
	if (--numInstances == 0)
	{
		shaderIF->destroy();
		delete shaderIF;
		shaderIF = NULL;
		shaderProgram = 0;
	}
}

void ModelViewWithLighting::fetchGLSLVariableLocations()
{
	if (shaderProgram > 0)
	{
		pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		pvaLoc_mcNormal = pvAttribLocation(shaderProgram, "mcNormal");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");
		
	//	uProjection, uObliqueDirection; 
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
		uLoc_p_ecLightPos = ppUniformLocation(shaderProgram, "p_mcLights");
		uLoc_lightStrength= ppUniformLocation(shaderProgram, "lightStrength");
		uLoc_actualNumLights= ppUniformLocation(shaderProgram,"actualNumLights");
		uLoc_globalAmbient = ppUniformLocation(shaderProgram, "ambientStrength");
		uProjection = ppUniformLocation(shaderProgram, "currentProjection");
		uObliqueDirection = ppUniformLocation(shaderProgram, "obliqueDirection");
		uLoc_ka = ppUniformLocation(shaderProgram, "ka");
		uLoc_ks = ppUniformLocation(shaderProgram, "ks");
		
	}
}

void ModelViewWithLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}
void ModelViewWithLighting::setViewingRadius(float viewingRadius_)
{
  viewingRadius = viewingRadius_;

}

void ModelViewWithLighting::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
  globalRX +=rx;
  globalRY += ry;
	dynamic_view = cryph::Matrix4x4::xRotationDegrees(globalRY) * cryph::Matrix4x4::yRotationDegrees(globalRX);//*dynamic_view;
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. Use dynamic_view in ModelView::getMatrices
}

void ModelViewWithLighting::letThereBeLight()
{ //set up the light shenanigans
  //set the number of light sources 
  //set the lightsource in mc
  //set 
  
  ModelViewWithLighting::ambientStrength[0] = 0.15;
  ModelViewWithLighting::ambientStrength[1] = 0.15;
  ModelViewWithLighting::ambientStrength[2] = 0.15;
  
 // glUniform3fv(uLoc_globalAmbient, 1, ModelViewWithLighting::ambientStrength);
//   
//   
  //float* mainlight= new float[4];
  //mainlight[0] = 100;
  // mainlight[1] = 100;
  //mainlight[2] = 100;
  //mainlight[3] = 0; //directional
  //lightSources[0] = mainlight;
  lightSources[0] = 1000;
  lightSources[1] = 1000;
  lightSources[2] = 1000;
  lightSources[3] = 0; //directional
//   
//   float* mainStrength = new float[3];
//   mainStrength[0] = 0.15;
//   mainStrength[1] = 0.15;
//   mainStrength[2] = 0.15;
//   lightStrengths[0] = mainStrength;
     lightStrengths[0] = 1;//15;
     lightStrengths[1] = 1;//15;
     lightStrengths[2] = 1;//15;
//   
  ModelViewWithLighting::numberOfLightSources++;
//   
//   int numLights = ModelViewWithLighting::numberOfLightSources;
//   glUniform4fv(uLoc_p_ecLightPos, numLights, lightSources);
//   glUniform3fv(uLoc_lightStrength, numLights, lightStrengths);
//   glUniform1i(uLoc_actualNumLights, numLights);
//   
//    int proj = (ModelView::projType == OBLIQUE)? 0 : ( (ModelView::projType == ORTHOGONAL)? 1 : 2);
   //glUniform1i(uProjection, proj);
//     
//   
  
  
  

}

void ModelViewWithLighting::render()
{
//   int numLights = ModelViewWithLighting::numberOfLightSources;
//   glUniform4fv(uLoc_p_ecLightPos, numLights, lightSources);
//   glUniform3fv(uLoc_lightStrength, numLights, lightStrengths);
//   glUniform1i(uLoc_actualNumLights, numLights);
//   int proj = (ModelView::projType == OBLIQUE)? 0 : ( (ModelView::projType == ORTHOGONAL)? 1 : 2);
//   glUniform1i(uProjection, proj);
//   glUniform3fv(uLoc_globalAmbient, 1, ModelViewWithLighting::ambientStrength);
//   glUniform3fv(uLoc_globalAmbient, 1, ModelViewWithLighting::ambientStrength);
}

void ModelViewWithLighting::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyzLimits[1] = xyzLimits[2] = xyzLimits[3] = xyzLimits[4] = xyzLimits[5]=0.0;
}
  
  
  



