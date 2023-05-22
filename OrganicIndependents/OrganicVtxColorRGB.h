/*------------------------------------------------------------------------------------------

--OrganicVtxColorRGB.h (Last Update: 9/4/2017)

Description: Contains 3 GLfloat values representing the RGB value of a vertex to color.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORRGB_H
#define ORGANICVTXCOLORRGB_H

#include <GL/glew.h>

class OrganicVtxColorRGB
{
	private:
		friend class OrganicSystem;
		friend class EnclaveManifest;
		friend class EnclaveManifestFactoryT1;
		friend class OrganicRawManifest;
		GLfloat red;
		GLfloat green;
		GLfloat blue;
};

#endif
