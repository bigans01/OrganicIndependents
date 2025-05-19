#pragma once

#ifndef FBARYCENTRICCOORDPRODUCER_H
#define FBARYCENTRICCOORDPRODUCER_H

/*
*
* Description: The main purpose of this class is to use barycentric coordinates to find UV values, when only
* the vertexes and their UV points of the triangle are known. Primarily intended to be used in calls to the
* runFracturing() series of virtual functions, derived in the FTriangleFracturerBase class. after external lines
* have been traced, and FRay casters have been run (i.e., after runWorldTracing() and buildAndRunFRayCasters()).
*
* All of the points should be rotated around the center of the triangle, such that they exist in a 2D plane
* (use quaternions for this). Rotating all points once and then calculating the values for all points is probably the best idea,
* before rotating back, in order to save CPU cycles.
*
* The input will take in the 3 points of the triangle, and a reference to the point to calculate the value for.
* This class assumes that the triangle has been rotated and aligned to x/y dimensions; i.e, it sits perfectly in a
* 2d plane.

*/

#include "FTrianglePoint.h"

class FBarycentricCoordProducer
{
public:
	FBarycentricCoordProducer() {};
	FBarycentricCoordProducer(FTrianglePoint in_barycentricProductionPoint0,
		FTrianglePoint in_barycentricProductionPoint1,
		FTrianglePoint in_barycentricProductionPoint2,
		FTrianglePoint* in_pointToSolveFor)
	{
		//barycentricProductionPoints[0] = in_barycentricProductionPoint0;
		//barycentricProductionPoints[1] = in_barycentricProductionPoint1;
		//barycentricProductionPoints[2] = in_barycentricProductionPoint2;
		pointToSolveFor = in_pointToSolveFor;
	}

private:
	//FTrianglePoint barycentricProductionPoints[3];	// the 3 points, aligned to 2d dimensions, and also having appropriate UV values.
	FTrianglePoint* pointToSolveFor = nullptr;		// set by constructor; will be a reference to the pointer to solve for.

	void calculateBarycentricBasedUV();
};


#endif