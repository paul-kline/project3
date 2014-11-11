// GrassBlade.h

#ifndef GRASS_BLADE_SUBCLASS_H
#define GRASS_BLADE_SUBCLASS_H

#include "ModelViewWithLighting.h"

class GrassBlade : public ModelViewWithLighting
{
public:
	GrassBlade();
	GrassBlade(int num_points_,
		   cryph::AffPoint bottom_,float baseWidth_, cryph::AffVector baseDirection_,
		   cryph::AffVector upV_, cryph::AffVector outV_, float upDistance_, float outDistance_, 
		   float colorT_[3],float colorB_[3], 
		   int attenuationCurveCode_,float totalTorque_ , int torqueApplicationFunctionCode_ );
	virtual ~GrassBlade();

	cryph::AffPoint bottom;
	cryph::AffVector baseDirection;
	--cryph::AffPoint* points; 
	typedef float vec3[3];
	vec3* points;
	float baseWidth;
	cryph::AffVector upV;
	cryph::AffVector outV;
	float upDistance;
	float outDistance;
	float myBounds[6];
	float colorT[3];
	float colorB[3];
	int num_points;

	float totalTorque;
	
	
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	
	GLuint vao[1];
	GLuint vbo[2];
  
	float (*f_Radial)(float);
	float (*f_Attenuation)(float);

	void defineGrassBlade();
	void renderGrassBlade(float * color);
	void setBounds();
	float fxConstant(float x);
	float fxLinear(float x);
	float fxSquared(float x);
	float fxCubed(float x);

	
	void setf_Radial(int torqueApplicationFunctionCode_);
	void setf_Attenuation(int attenuationCurveCode_);
	
};

#endif
