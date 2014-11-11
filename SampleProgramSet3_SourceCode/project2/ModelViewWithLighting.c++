// ModelViewWithLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithLighting.h"
#include "ShaderIF.h"

ShaderIF* ModelViewWithLighting::shaderIF = NULL;
int ModelViewWithLighting::numInstances = 0;
GLuint ModelViewWithLighting::shaderProgram = 0;
GLint ModelViewWithLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithLighting::ppuLoc_kd = -2;
GLint ModelViewWithLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithLighting::ppuLoc_ec_lds = -2;

std::string ModelViewWithLighting::vShaderSource = "YOUR_LOCAL.vsh";
std::string ModelViewWithLighting::fShaderSource = "YOUR_LOCAL.fsh";

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
	}
}

void ModelViewWithLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}
