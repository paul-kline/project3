#version 410 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

// Naming convention for variables holding coordinates:
// mc - model coordinates
// ec - eye coordinates
// lds - logical device space
// "p_" prefix on any of the preceding indicates the coordinates have been
//      embedded in projective space
// (gl_Position would have label p_lds)

// Transformation Matrices
uniform mat4 mc_ec =   // (dynamic rotations) * (ViewOrientation(E,C,up))
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> no dynamic rotations -AND- MC = EC
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
uniform mat4 ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
             
// There are MANY ways to deal with the basic object color.
// For now we will  simply assume:
uniform vec3 kd = // "kd" - diffuse reflectivity; basic object color
	vec3(0.8, 0.0, 0.0); // default: darkish opaque red

// There are also MANY ways to deal with light sources (number, type,
// strength, etc.).  For now we simply assume one directional source.
// Exercise: Generalize in any of several ways we will discuss in class.
const vec3 lightSourceDir = vec3(0.7, 0.2, 1.0);

// Per-vertex attributes
// 1. incoming vertex position in model coordinates
layout (location = 0) in vec3 mcPosition;
// 2. incoming vertex normal vector in model coordinates
in vec3 mcNormal; // incoming normal vector in model coordinates

out vec3 colorToFS;


uniform int currentProjection=2; //alphabetical: 0=OBLIQUE, 1=ORTHOGONAL, 2=PERSPECTIVE
uniform vec3 obliqueDirection = vec3(0,0,1);
uniform vec4 p_mcLights[20];
uniform vec3 lightStrength[20];
uniform int actualNumLights = 1;
uniform vec3 ambientStrength = vec3(0.515, 0.515, 0.515);
uniform vec3 ka = vec3( 0.50, 0.0, 0.0); //these should all be set.
//uniform vec4 kd = vec4( 1.0, 0.5, 0.5, 1.0 );
uniform vec3 ks = vec3( 1.0, 0.5, 0.5); 
uniform float m=1.0;


in vec2 texCoords;
out vec2 texCoordsToFS;
vec3 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	// Simplistic lighting model:
	// Reflected color will be a function of angle between the normal
	// vector and the light source direction. Specifically:
//find v, the vector towards the viewer
  //
    vec3 v = vec3(0,0,1);
    switch (currentProjection) {
	case 0://OBLIQUE
	  v = normalize((mc_ec* vec4(obliqueDirection,0)).xyz);
	  break;
	case 1: //ORTHOGONAL
	  //no change needed, already 0,0,1;
	  break;
	case 2: //PERSPECTIVE
	  v= -normalize(ec_Q);
	  break;
	
	}
  
	
    vec3 theColor = ka * ambientStrength; //kd = ka typically anyways
  
	for(int i = 0; i < actualNumLights; i++){
	 
	  if(dot(ec_nHat, v) <0){
	    ec_nHat*=-1; //flip!
	  }
	  //0 means directional! otherwise, it's a location and direction toward light source must be computed.
	  bool directional = p_mcLights[i].w == 0 ; 
	  vec3 ecLight = (mc_ec * p_mcLights[i]).xyz;
	  vec3 li_hat = normalize((directional)? ecLight : (ecLight - ec_Q));
	  if(dot(ec_nHat, li_hat)<0){
	    continue;
	  }
	  //calculate reflector ray!
	  //R = 2N (N . L) - L
	  vec3 r_i = 2*ec_nHat * (dot(ec_nHat, li_hat)) - li_hat;
	  if(dot (r_i, v) < 0){
	    r_i = vec3(0,0,0);
	  }//we ignore
	  
	  
	  //linear kill off for now
	  float fi_Q = (directional)? 1 : (1.0 / (length(ecLight - ec_Q)));
	  theColor += fi_Q * lightStrength[i]* ( kd * (dot(ec_nHat, li_hat))  + ks*(pow(dot(r_i, v), m)));
	 
	  
	  
	  
	  
	}
  
  
  
	float factor = abs(dot(normalize(lightSourceDir), ec_nHat));
	
	
	//vec3 theColor = factor * kd;
	
	
	//fix the overflow!!
	float max =((theColor.x > theColor.y)? theColor.x : theColor.y);
	max = (theColor.z > max)? theColor.z : max;
	
	if(max>1){
	 theColor.x /= max;
	 theColor.y /= max;
	 theColor.z /= max;
	}
	
	return theColor; // factor * kd; //
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	vec3 ec_nHat = normalize(normalMatrix * mcNormal);

	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);

	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position = ec_lds * p_ecPosition;
	
	
	texCoordsToFS = texCoords;
}
