#pragma once

#ifndef FTRIANGLEPOINTUVLINK_H
#define FTRIANGLEPOINTUVLINK_H

#include "FTrianglePoint.h"

class FTrianglePointUVLink
{
	public:
		FTrianglePointUVLink() {}
		FTrianglePointUVLink(FTrianglePoint* in_linkedPointRef) :
			linkedPointRef(in_linkedPointRef)
		{
			// we need a copy of the original point, that we will translate, 
			// in order to help us find the UV.
			linkedPointCopy = *linkedPointRef;
		}
							
	private:
		FTrianglePoint* linkedPointRef = nullptr;
		FTrianglePoint linkedPointCopy;

};

#endif
