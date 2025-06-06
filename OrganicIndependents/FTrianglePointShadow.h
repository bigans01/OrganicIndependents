#pragma once

#ifndef FTRIANGLEPOINTSHADOW_H
#define FTRIANGLEPOINTSHADOW_H

#include "FTrianglePoint.h"

class FTrianglePointShadow
{
	public:

		// This is the primary constructor that should be used when this class is initialized,
		// since this class needs to use a point as reference to work in any capacity.
		FTrianglePointShadow(FTrianglePoint* in_originPointRef):
			originPointRef(in_originPointRef)
		{
			// Copy the original point's contents
			shadowPoint = *originPointRef;
		}

		FTrianglePointShadow() {};	// this should realistically be for copy constructor, not default constructor.

		FTrianglePoint* fetchShadowPoint()	// get a reference to the shadow point; the shadow point is a copy of the original 
											// point, that we can freely use to rotate around etc, without any interference against the original, since
											// it is a copy.
		{
			return &shadowPoint;
		}

		void loadShadowedUVIntoOrigin()
		{
			originPointRef->fTextureU = shadowPoint.fTextureU;
			originPointRef->fTextureV = shadowPoint.fTextureV;
		}

		void printShadowPoint()
		{
			shadowPoint.printPointData();
			std::cout << std::endl;
		}

	private:
		FTrianglePoint* originPointRef = nullptr;
		FTrianglePoint shadowPoint;
};

#endif
