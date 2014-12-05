// ModelViewWithLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithLighting.h"
#include "ShaderIF.h"
#include "MyController.h"
//#include "GLFWController.h"

ShaderIF* ModelViewWithLighting::shaderIF = NULL;
int ModelViewWithLighting::numInstances = 0;
GLuint ModelViewWithLighting::shaderProgram = 0;
GLint ModelViewWithLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithLighting::pvaLoc_texCoords= -2;

GLint ModelViewWithLighting::ppuLoc_kd = -2;
GLint ModelViewWithLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithLighting::ppuLoc_ec_lds = -2;
GLint ModelViewWithLighting::ppuLoc_textureMap= -2;
GLint ModelViewWithLighting::ppuLoc_whichTexture= -2;

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
GLint ModelViewWithLighting::uLoc_m = -2;

std::string ModelViewWithLighting::vShaderSource = "vsh.c++";
std::string ModelViewWithLighting::fShaderSource = "fsh.c++";

float ModelViewWithLighting::viewingRadius = 1;
double dynamic_zoomScale = 1;

float ModelViewWithLighting::globalRX =0;
float ModelViewWithLighting::globalRY=0;
GLint ModelViewWithLighting::numberOfLightSources = 1;
bool ModelViewWithLighting::lightingModelLocked = false;


   float ModelViewWithLighting::goldka[3] = {0.24725,0.2245,0.0645};
   float ModelViewWithLighting::goldkd[3] = {0.34615,0.3143,0.0903};
   float ModelViewWithLighting::goldks[3] = {0.797357,0.723991,0.208006};
   const float ModelViewWithLighting::goldm = 83.2;


ModelViewWithLighting::ModelViewWithLighting()
{
  //std::cout << "MAKING A ModelViewWithLighting!!!!!!!!!!!!!!!! INSTANCE NUMBER: " << numInstances << "\n\n";
	if (shaderProgram == 0)
	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;
  ModelViewWithLighting::ambientStrength[0] = 0.15;
  ModelViewWithLighting::ambientStrength[1] = 0.15;
  ModelViewWithLighting::ambientStrength[2] = 0.15;

 
  
  ModelViewWithLighting::lightSources[0] = -10;
  ModelViewWithLighting::lightSources[1] = 30;
  ModelViewWithLighting::lightSources[2] = 35;
  ModelViewWithLighting::lightSources[3] = 0;//high noon! // 0= directional, 1 = local
  
  ModelViewWithLighting::lightStrengths[0] = 0.25;//15;
  ModelViewWithLighting::lightStrengths[1] = 0.25;//15;
  ModelViewWithLighting::lightStrengths[2] = 1.25;//15;
  
  
  //firepit1
  ModelViewWithLighting::lightSources[4] = 35;
  ModelViewWithLighting::lightSources[5] = 30;
  ModelViewWithLighting::lightSources[6] = 45;
  ModelViewWithLighting::lightSources[7] = 1; // 0= directional, 1 = local
  
  ModelViewWithLighting::lightStrengths[3] = 10.00;//15;
  ModelViewWithLighting::lightStrengths[4] = 0.5;//15;
  ModelViewWithLighting::lightStrengths[5] = 0.5;//15;
  
  //firepit2
  ModelViewWithLighting::lightSources[8] = -35;
  ModelViewWithLighting::lightSources[9] = 30;
  ModelViewWithLighting::lightSources[10] = 45;
  ModelViewWithLighting::lightSources[11] = 1; // 0= directional, 1 = local
  
  ModelViewWithLighting::lightStrengths[6] = 10.00;//15;
  ModelViewWithLighting::lightStrengths[7] = 0.95;//15;
  ModelViewWithLighting::lightStrengths[8] = 0.95;//15;
  
  
  
  ModelViewWithLighting::numberOfLightSources = 3;
	
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
		pvaLoc_texCoords = pvAttribLocation(shaderProgram, "texCoords");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");
		
		ppuLoc_textureMap = ppUniformLocation(shaderProgram, "textureMap");
		ppuLoc_whichTexture = ppUniformLocation(shaderProgram, "whichTexture");
		
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
		ModelViewWithLighting::uLoc_actualNumLights= ppUniformLocation(shaderProgram,"actualNumLights");
		uLoc_globalAmbient = ppUniformLocation(shaderProgram, "ambientStrength");
		uProjection = ppUniformLocation(shaderProgram, "currentProjection");
		uObliqueDirection = ppUniformLocation(shaderProgram, "obliqueDirection");
		uLoc_ka = ppUniformLocation(shaderProgram, "ka");
		uLoc_ks = ppUniformLocation(shaderProgram, "ks");
		uLoc_m = ppUniformLocation(shaderProgram, "m");
		
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
	dynamic_view =  cryph::Matrix4x4::xRotationDegrees(globalRY) * cryph::Matrix4x4::yRotationDegrees(globalRX) ;//*dynamic_view;
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. Use dynamic_view in ModelView::getMatrices
}

void ModelViewWithLighting::letThereBeLight(float ka[3],float kd[3], float ks[3], float m)
{ 
//   
//   ModelViewWithLighting::numberOfLightSources = 3;
//   
//    int numLights = ModelViewWithLighting::numberOfLightSources;
  
   glUniform4fv(uLoc_p_ecLightPos, ModelViewWithLighting::numberOfLightSources, ModelViewWithLighting::lightSources);
   glUniform3fv(uLoc_lightStrength, ModelViewWithLighting::numberOfLightSources, ModelViewWithLighting::lightStrengths);
   glUniform1i(ModelViewWithLighting::uLoc_actualNumLights, ModelViewWithLighting::numberOfLightSources);
//   
    GLint proj = (ModelView::projType == OBLIQUE)? 0 : ( (ModelView::projType == ORTHOGONAL)? 1 : 2);
   glUniform1i(ModelViewWithLighting::uProjection, proj);
   
   glUniform3fv(ppuLoc_kd, 1, kd);
   glUniform3fv(uLoc_ka, 1, ka);
   glUniform3fv(uLoc_ks, 1, ks);
   glUniform1f(uLoc_m, m);
   
   
//     
//   
  
  
  

}

// void ModelViewWithLighting::getMCBoundingBox(double* xyzLimits) const
// {
//   xyzLimits[0] = xyzLimits[1] = xyzLimits[2] = xyzLimits[3] = xyzLimits[4] = xyzLimits[5]=0.0;
// }
  
  
  



