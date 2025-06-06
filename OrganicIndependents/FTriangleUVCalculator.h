#pragma once

#ifndef FTRIANGLEUVCALCULATOR_H
#define FTRIANGLEUVCALCULATOR_H

#include "FTrianglePoint.h"
#include "UniquePointContainer.h"
#include "FTrianglePointShadow.h"
#include "IndependentUtils.h"
#include "FTriangleUtils.h"

/*
* 
* Description: Designed to be utilized by the FTriangle class, this class takes in the 3 localized points of the FTriangle, 
* plus a container that has all of the points in the FTriangle's plane (i.e, runWorldTracing(), runRaycastCollisionResolver() have 
* already been called). This class will use copies of the original 3 points, rotate those points to the XY 2d plane, and calculate the UV values.
* The UV values will then be written into the original FTrianglePoints that we were calculating the UVs for, via FTrianglePointShadow 
* instances.

This calls should be utilized after points are generated, line tracing is complete, and ray casts have been done, in the runFractuing() virtual
function of the FTriangleFracturerBase class.

*/

class FTriangleUVCalculator
{
	public:
		FTriangleUVCalculator() {};
		FTriangleUVCalculator(FTrianglePoint in_barycentricBasisPoint0,		// The 3 points in this function should be from the localized points of an
			FTrianglePoint in_barycentricBasisPoint1,						// FTriangle; the reference to the UniquePointContainer should be from a 
			FTrianglePoint in_barycentricBasisPoint2,						// fracturing machine, such as WorldFracturingMachine.
			UniquePointContainer* in_targetPointsForCalculations)
		{
			barycentricBasisPoints[0] = in_barycentricBasisPoint0;
			barycentricBasisPoints[1] = in_barycentricBasisPoint1;
			barycentricBasisPoints[2] = in_barycentricBasisPoint2;
			targetPointsForCalculations = in_targetPointsForCalculations;	// remember: this should contain ALL points, including copies of
																			// the original points of the triangle.

			anchoringNormal = glm::vec3(0.0f, 0.0f, 1.0f);

			buildShadowPoints();					// Step 1: build the shadow points from the original referenced points.
			calculateShadowTriangleNormal();		// Step 2: find the normal of the shadow triangle, using cross product of u/v.
			calculateShadowTriangleCentroid();		// Step 3: find the centroid; we will need to translate all points by the inverse of this coordinate, 
													// to get the center the triangle at point 0,0,0. The normal will not be affected by this translation.

			applyCentroidTranslationToShadowPoints();	// Step 4: apply inverse of the centroid value to all shadow points, so that the points are oriented around
														// the centroid at 0,0,0, prior to manipulation via quaternions.

			calculateAndApplyQuaternion();
														
		}

	private:
		FTrianglePoint barycentricBasisPoints[3];	// the three triangle points that UV calculations will be based off of;
													// the UV coordinates to utilize must be set in these points, prior to the
													// calculator being run.
		UniquePointContainer* targetPointsForCalculations = nullptr;

		std::vector<FTrianglePointShadow> shadowPointVector;

		ECBPolyPoint shadowTriangleCentroid;	// the centroid to use as a basis for quaternion rotation; calculated by calling 
												// calculateShadowTriangleCentroid.
		glm::vec3 shadowTriangleNormal;

		glm::vec3 anchoringNormal;	// will get set to 0,0,1.0f, so that we can use this to get to X/Y

	    glm::quat xyQuaternion;

		void buildShadowPoints();
		void calculateShadowTriangleCentroid();
		void calculateShadowTriangleNormal();
		void applyCentroidTranslationToShadowPoints();
		void calculateAndApplyQuaternion();

		glm::vec3 calculateBarycentricCoords(FTrianglePoint in_point0,
			FTrianglePoint in_point1,
			FTrianglePoint in_point2,
			FTrianglePoint in_pointToSolveFor);

		// Below: this function gets the value of a UV point, when the following is given
		// as the input:
		//		-3 points, represented in X/Y (Z is negligible or NA), all of which have a UV value to interpoloate across
		//		-the barycentric coords of the point we are solving the UV for.
		glm::vec2 getUV(FTrianglePoint in_point0,
						FTrianglePoint in_point1,
						FTrianglePoint in_point2,
						glm::vec3 in_barycentricCoords);
};

#endif
