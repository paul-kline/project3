// Your implementation of ModelView::getMatrices

//#include "ModelView.h"
#include "ModelViewWithLighting.h"
void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
	// TODO:
	// 1. Create the mc_ec matrix from the eye, center, and up. You can use the
	//    following utility from Matrix4x4:
	//
    
// 	cryph::Matrix4x4 eye = ModelView::eye;
// 	cryph::Matrix4x4 center;
// 	cryph::Matrix4x4 up;
//   
      
	float d = eye[2] - center[2];
	cryph::Matrix4x4 t = cryph::Matrix4x4::IdentityMatrix;
	t*=d;
	//const cryph::Matrix4x4 rot = (-1)*t * dynamic_view * (t);
  	mc_ec=  cryph::Matrix4x4::lookAt(eye, center,up) * dynamic_view;
  
  
	
	
	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	
	//std::cout << "ymin: " << ymin << " ymax: " << ymax << "\n\n";
	// BEGIN: ASPECT RATIO CONSIDERATIONS
	// If we wish to preserve aspect ratios, make "region of interest"
	// wider or taller to match the Controller's viewport aspect ratio.
	
	//aspect ratio code cited to be from previous scaletrans.
	double vAR = Controller::getCurrentController()->getViewportAspectRatio();

	double wHeight = ymax - ymin;
	double wWidth = xmax - xmin;
	double wAR = wHeight / wWidth; // wAR == MC Window Aspect Ratio
	if (wAR > vAR)
	{
		// make MC window wider
		wWidth = wHeight / vAR;
		double xmid = 0.5 * (xmin + xmax);
		xmin = xmid - 0.5*wWidth;
		xmax = xmid + 0.5*wWidth;
	}
	else
	{
		// make MC window taller
		wHeight = wWidth * vAR;
		double ymid = 0.5 * (ymin + ymax);
		ymin = ymid - 0.5*wHeight;
		ymax = ymid + 0.5*wHeight;
	}
	
	//  float ecYmax,ecYmin,ecXmax,ecXmin=0;
	 // float Ycenter,Xcenter,Zcenter=0;
	  
	  float ecXmin;
	  ecXmin= -ModelViewWithLighting::viewingRadius;
	  float ecYmin;
	  ecYmin= -ModelViewWithLighting::viewingRadius;
	  float ecXmax;
	  ecXmax= ModelViewWithLighting::viewingRadius;
	  float ecYmax;
	  ecYmax= ModelViewWithLighting::viewingRadius;
	  
	  ecXmin/=dynamic_zoomScale;
	  ecXmax/= dynamic_zoomScale;
	  ecYmax/=dynamic_zoomScale;
	  ecYmin/=dynamic_zoomScale;
	  //ecZmax/=dynamic_zoomScale;
	  //ecZmin/=dynamic_zoomScale;
	  
	switch (ModelView::projType){
	  
	  case PERSPECTIVE:
	    ec_lds = cryph::Matrix4x4::perspective( zpp,  ecXmin, ecXmax,
		      ecYmin,  ecYmax,  ecZmin,  ecZmax);
	     // ec_lds = cryph::Matrix4x4::perspective( zpp,  -(xmax - xmin)/2, (xmax-xmin)/2,
		//      -(ymax-ymin)/2,  (ymax-ymin)/2,  ecZmin,  ecZmax);
	      break;
	  case OBLIQUE:
	    ec_lds = cryph::Matrix4x4::orthogonal( ecXmin, ecXmax,
		ecYmin,  ecYmax, ecZmin, ecZmax);
	    break;
	  case ORTHOGONAL:
	    cryph::AffVector dir(-1,0,-1);
	    ec_lds = cryph::Matrix4x4::oblique(-4*ModelViewWithLighting::viewingRadius, ecXmin, ecXmax,
		ecYmin,  ecYmax, ecZmin, ecZmax, dir);
	    break;
	      
	}
	
	ec_lds = ec_lds;//dynamic_view

	//ec_lds = cryph::Matrix4x4::scale(dynamic_zoomScale,dynamic_zoomScale,dynamic_zoomScale) * ec_lds;
	      
	//ec_lds = cryph::Matrix4x4::orthogonal(xmin,xmax,ymin,ymax,ecZmin, ecZmax);
	//    cryph::Matrix4x4 cryph::Matrix4x4::lookAt(
	//          const cryph::AffPoint& eye, const cryph::AffPoint& center,
	//          const cryph::AffVector& up);
	//
	//    NOTE: eye, center, and up are specified in MODEL COORDINATES (MC)
	//
	//    So, for example:
	//       mc_ec = cryph::Matrix4x4::lookAt(eye, center, up);
	//
	// 2. Using the WIDTHS of the established mcRegionOfInterest (ADJUSTED in the x OR
	//    y direction to match the viewport aspect ratio), create the matrix using the
	//    method for the desired type of projection.
	//
	//    Any of the three Matrix4x4 methods shown below (declared in Matrix4x4.h)
	//    can be used to create ec_lds. On a given call to this "getMatrices" routine,
	//    you will use EXACTLY ONE of them, depending on what type of projection you
	//    currently want.

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!   All coordinate data in the parameter lists below are specified      !!!!!!
	// !!!!!   in EYE COORDINATES (EC)! Be VERY sure you understand what that      !!!!!!
	// !!!!!   means!  (This is why I emphasized "WIDTHS" above.)                  !!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	/* The three choices:

	cryph::Matrix4x4 cryph::Matrix4x4::orthogonal(double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::Matrix4x4::perspective(double zpp, double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::Matrix4x4::oblique(double zpp, double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax, const cryph::AffVector& projDir);

	*/

	// For example:
	//ec_lds = cryph::Matrix4x4::perspective(zpp, ecXmin, ecXmax, ecYmin, ecYmax, ecZmin, ecZmax);
	// ec_lds = cryph::Matrix4x4::perspective(zpp, -10, 10, -10, 10, ecZmin, ecZmax);
	//
	// RECALL: Use the class variables ecZmin, ecZmax, and zpp in these calls.

	// THEN IN THE CALLER OF THIS METHOD:
	//
	// float mat[16];
	// glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	// glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));
	//
	// (The extractColMajor method copies the elements of the matrix into the given
	// array which is assumed to be of length 16. It then returns the array pointer
	// so it can be used as indicated in the two calls. Since the array is immediately
	// copied by glUniformMatrix to the GPU, "mat" can be reused as indicated.)
}
