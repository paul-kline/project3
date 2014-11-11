// MandM.c++

#include <iostream>
#include <math.h>

#include "MandM.h"
#include "ShaderIF.h"

MandM::MandM() : displayMandMEdges(false), displayMandMFill(true)
{
	defineMandM();
}

MandM::~MandM()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void MandM::defineMandM()
{
	typedef float vec3[3];

	vec3 vtx[] = {
		// First "M"
		{-0.6, -0.25, -0.7}, { -0.6, -0.25, 0.25},
		{-0.6,  0.25, -0.7}, { -0.6,  0.25, 0.25},
		{-0.35, 0.0 , -0.7}, { -0.35, 0.0 , 0.25},
		{-0.1 , 0.25, -0.7}, { -0.1 , 0.25, 0.25},
		{-0.1 ,-0.25, -0.7}, { -0.1 ,-0.25, 0.25},
		// Second "M" is a copy with deltaX=0.7
		{ 0.1, -0.25, -0.7}, { 0.1, -0.25, 0.25},
		{ 0.1,  0.25, -0.7}, { 0.1,  0.25, 0.25},
		{ 0.35, 0.0 , -0.7}, { 0.35, 0.0 , 0.25},
		{ 0.6 , 0.25, -0.7}, { 0.6 , 0.25, 0.25},
		{ 0.6 ,-0.25, -0.7}, { 0.6 ,-0.25, 0.25}
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 20*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void MandM::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -0.6;
	xyzLimits[1] = 0.6;
	xyzLimits[2] = -0.25;
	xyzLimits[3] = 0.25;
	xyzLimits[4] = -0.7;
	xyzLimits[5] = 0.25;
}

void MandM::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'm')
		displayMandMFill = !displayMandMFill;
	else if (key == 'M')
		displayMandMEdges = !displayMandMEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void MandM::renderMandM(float* color)
{
	typedef float vec4[4];

	vec4 first_M_Color = { 0.3, 1.0, 1.0 };
	vec4 second_M_Color = { 1.0, 0.25, 0.2 };
	glBindVertexArray(vao[0]);
	// Exercise: show that the normal vectors specified here
	//           are correct.
	if (color == NULL)
		glUniform3fv(ppuLoc_kd, 1, first_M_Color);
	else
		glUniform3fv(ppuLoc_kd, 1, color);
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);

	if (color == NULL)
		glUniform3fv(ppuLoc_kd, 1, second_M_Color);
	else
		glUniform3fv(ppuLoc_kd, 1, color);
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
}

void MandM::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	float black[] = { 0.0, 0.0, 0.0 };

	if (displayMandMFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderMandM(NULL);
	}
	if (displayMandMEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderMandM(black);
	}

	glUseProgram(pgm);
}
