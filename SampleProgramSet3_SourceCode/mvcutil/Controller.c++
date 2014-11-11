// Controller.c++: An Abstract base Class for a Controller (in Model-View-Controller sense)

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Controller.h"
#include "ModelView.h"

Controller* Controller::curController = NULL;
int Controller::newWindowWidth = 512;
int Controller::newWindowHeight = 512;

Controller::Controller() : scaleFraction(1.1), glClearFlags(GL_COLOR_BUFFER_BIT)
{
	curController = this;

	// indicate we do not yet have any models by setting min to +1 and max to -1:
	overallMCBoundingBox[0] = overallMCBoundingBox[2] = overallMCBoundingBox[4] = 1.0;
	overallMCBoundingBox[1] = overallMCBoundingBox[3] = overallMCBoundingBox[5] = -1.0;
}

Controller::~Controller()
{
	if (this == curController)
		curController = NULL;
}

void Controller::addModel(ModelView* m)
{
	if (m == NULL)
		return;
	models.push_back(m);
	visible.push_back(true);
	updateMCBoundingBox(m);
}

bool Controller::checkForErrors(std::ostream& os, const std::string& context)
	// CLASS METHOD
{
	bool hadError = false;
	GLenum e = glGetError();
	while (e != GL_NO_ERROR)
	{
		os << "CheckForErrors (context: " <<  context
		   << "): " << (char*)gluErrorString(e) << std::endl;
		e = glGetError();
		hadError = true;
	}
	return hadError;
}

ModelView* Controller::getModel(int which) const
{
	if ((which >= 0) && (which < models.size()))
		return models[which];
	return NULL;
}

void Controller::getOverallMCBoundingBox(double* xyzLimits) const
{
    for (int i=0 ; i<6 ; i++)
        xyzLimits[i] = overallMCBoundingBox[i];
}

double Controller::getViewportAspectRatio() const
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	return static_cast<double>(vp[3]) / static_cast<double>(vp[2]);
}

void Controller::handleAsciiChar(unsigned char theChar, int x, int y)
{
	const unsigned char ESC = 27;
	if (theChar == ESC)
		endProgram();

	double ldsX, ldsY; // only coord system known to both Controller and ModelView
	screenXYToLDS(x, y, ldsX, ldsY);

	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
	{
		// we always pass each individual key stroke
		(*it)->handleCommand(theChar, ldsX, ldsY);
	}

	redraw();
}

void Controller::handleFunctionKey(int whichFunctionKey, int x, int y, int mods)
{
}

void Controller::handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods)
{
}

void Controller::handleMouseMotion(int x, int y)
{
}

void Controller::handleReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	redraw();
}

void Controller::handleScroll(bool up)
{
}

void Controller::handleSpecialKey(SpecialKey key, int x, int y, int mods)
{
}

void Controller::removeAllModels(bool do_delete)
{
	std::vector<ModelView*>::iterator it = models.begin();
	while (it < models.end())
	{
		if (do_delete)
			delete *it;
		models.erase(it);
		it = models.begin();
	}
	visible.clear();
}

ModelView* Controller::removeModel(int which)
{
	if ((which >= 0) && (which < models.size()))
	{
		ModelView* m = models[which];
		models.erase(models.begin()+which);
		visible.erase(visible.begin()+which);
		return m;
	}
	return NULL;
}

void Controller::removeModel(ModelView* m)
{
	int which = 0;
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
	{
		if (*it == m)
		{
			models.erase(it);
			visible.erase(visible.begin() + which);
			break;
		}
		which++;
	}
}

void Controller::reportVersions(std::ostream& os) const
{
	const char* glVer = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* glslVer = reinterpret_cast<const char*>
			(glGetString(GL_SHADING_LANGUAGE_VERSION));
	// glGetString can return NULL if no rendering context has been created
	os << "VERSIONS: GL: ";
	if (glVer == NULL)
		os << "NULL (has RC been created?)\n";
	else
		os << glVer << '\n';
	os << "        GLSL: ";
	if (glslVer == NULL)
		os << "NULL (has RC been created?)\n";
	else
		os << glslVer << '\n';
	reportWindowInterfaceVersion(os);
}

void Controller::screenXYToLDS(int xIn, int yIn, double& ldsX, double& ldsY)
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	double x = xIn - vp[0];
	ldsX = 2.0 * x / static_cast<double>(vp[2]) - 1.0;
	// The window managers report pixel coordinates assuming y=0 is at the top
	// of the window. The main OpenGL API assumes y=0 is at the bottom, hence:
	double y = (vp[3] - yIn) - vp[1];
	ldsY = 2.0 * y / static_cast<double>(vp[3]) - 1.0;
}

void Controller::setClearFlags(int rcFlags)
{
	glClearFlags = GL_COLOR_BUFFER_BIT;
	if ((rcFlags & MVC_USE_DEPTH_BIT) != 0)
		glClearFlags |= GL_DEPTH_BUFFER_BIT;
	if ((rcFlags & MVC_USE_STENCIL_BIT) != 0)
		glClearFlags |= GL_STENCIL_BUFFER_BIT;
}

std::string Controller::titleString(const std::string& str)
{
	int lastSlash = str.length() - 1;
	while (lastSlash > 0)
	{
		if (str[lastSlash] == '/')
			return str.substr(lastSlash+1);
		lastSlash--;
	}
	// No slashes - just return original string
	return str;
}

void Controller::toggleVisibility(int which)
{
	if ((which >= 0) && (which < models.size()))
	{
		visible[which] = !visible[which];
		redraw();
	}
}

void Controller::updateMCBoundingBox(ModelView* m)
{
	if (m == NULL)
		return;
	if (overallMCBoundingBox[0] <= overallMCBoundingBox[1])
	{
        // bounding box already initialized; just update it:
        double xyz[6];
		m->getMCBoundingBox(xyz);
		for (int i=0 ; i<5 ; i+=2)
		{
			if (xyz[i] < overallMCBoundingBox[i])
				overallMCBoundingBox[i] = xyz[i];
			if (xyz[i+1] > overallMCBoundingBox[i+1])
				overallMCBoundingBox[i+1] = xyz[i+1];
		}
	}
	else // use this model to initialize the bounding box
		m->getMCBoundingBox(overallMCBoundingBox);
}
