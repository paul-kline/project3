// Tetrahedron.c++

#include <iostream>
#include <math.h>

#include "Tetrahedron.h"
#include "ShaderIF.h"

Tetrahedron::Tetrahedron(
		const cryph::AffPoint& p0, const cryph::AffPoint& p1,
		const cryph::AffPoint& p2, const cryph::AffPoint& p3,
		float R, float G, float B) :
	displayTetrahedronEdges(false), displayTetrahedronFill(true)
{
	color[0] = R; color[1] = G; color[2] = B;
	// put vertices in array to simplify generation of geometry:
	cryph::AffPoint verts[] = { p0, p1, p2, p3 };
	defineTetrahedron(verts);
	// set min/max coordinates for MC bounding box:
	minMax[0] = minMax[1] = p0.x;
	minMax[2] = minMax[3] = p0.y;
	minMax[4] = minMax[5] = p0.z;
	updateXYZBounds(p1);
	updateXYZBounds(p2);
	updateXYZBounds(p3);
}

Tetrahedron::~Tetrahedron()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Tetrahedron::defineTetrahedron(const cryph::AffPoint verts[])
{
	typedef float vec3[3];

	// We need SIX vertices for GL_TRIANGLE_STRIP; the last two
	// are copies of the first two:
	vec3 vtx[6];
	for (int i=0 ; i<6 ; i++)
		// "i%4" because last two are copies of first two
		verts[i%4].aCoords(vtx, i);

	// There will be FOUR faces, so compute those FOUR normal vectors:
	for (int i=0 ; i<4 ; i++)
	{
		int viP1 = (i + 1) % 4;
		int viP2 = (i + 2) % 4;
		cryph::AffVector v01 = verts[viP1] - verts[i];
		cryph::AffVector v02 = verts[viP2] - verts[i];
		normal[i] = v01.cross(v02);
	}

	// The vertex coordinates are sent in a VBO, but we will apply
	// the normals during rendering since they vary face by face
	// instead of vertex by vertex.
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	// Because we are applying normals in render:
	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

void Tetrahedron::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = minMax[i];
}

void Tetrahedron::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 't')
		displayTetrahedronFill = !displayTetrahedronFill;
	else if (key == 'T')
		displayTetrahedronEdges = !displayTetrahedronEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Tetrahedron::renderTetrahedron(const float* rgb)
{
	glBindVertexArray(vao[0]);
	if (rgb != NULL)
		glUniform3fv(ppuLoc_kd, 1, rgb);
	else // Just use specified color
		glUniform3fv(ppuLoc_kd, 1, color);

	// draw the four faces:
	for (int fi=0 ; fi<4 ; fi++)
	{
		glVertexAttrib3f(pvaLoc_mcNormal, normal[fi].dx, normal[fi].dy, normal[fi].dz);
		glDrawArrays(GL_TRIANGLE_STRIP, fi, 3);
	}
}

void Tetrahedron::render()
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

	if (displayTetrahedronFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderTetrahedron(NULL);
	}
	if (displayTetrahedronEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderTetrahedron(black);
	}

	glUseProgram(pgm);
}

void Tetrahedron::updateXYZBounds(const cryph::AffPoint& p)
{
	if (p.x < minMax[0])
		minMax[0] = p.x;
	else if (p.x > minMax[1])
		minMax[1] = p.x;

	if (p.y < minMax[2])
		minMax[2] = p.y;
	else if (p.y > minMax[3])
		minMax[3] = p.y;

	if (p.z < minMax[4])
		 minMax[4] = p.z;
	else if (p.z > minMax[5])
		minMax[5] = p.z;
}
