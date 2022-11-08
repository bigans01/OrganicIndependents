#pragma once

#ifndef FRAYINTERCEPTIONMACHINE_H
#define FRAYINTERCEPTIONMACHINE_H

#include "FRayIntersectionRoundingMode.h"
#include "Vec3Pair.h"
#include "FTrianglePoint.h"

/*

Description:

This class, when instantiated, will attempt to run the given ECBPolyPointPair (in_rayToCast), against the triangle 
that is made up with the points from the in_interceptableTrianglePoints argument. It also takes in a reference to a 
UniquePointContainer that is used to store intersected points, and an enum value of FRayIntersectionRoundingMode to
determine that dimension should be rounded based on the ray we are casting 
(i.e, only X gets rounded if casting an XFRay -- while the Y and Z values remain their original value from the in_rayToCast.pointA)

*/

class FRayIntersectionMachine
{
	public:
		FRayIntersectionMachine(ECBPolyPointPair in_rayToCast,
								ECBPolyPoint in_interceptableTrianglePoints[3],
								UniquePointContainer* in_interceptorUniquePointsRef,
								FRayIntersectionRoundingMode in_interceptionType)
		{
			rayToCast = in_rayToCast;
			for (int x = 0; x < 3; x++)
			{
				interceptableTrianglePoints[x] = IndependentUtils::convertECBPolyPointToVec3(in_interceptableTrianglePoints[x]);
			}
			intersectorUniquePointsRef = in_interceptorUniquePointsRef;
			intersectionRoundingMode = in_interceptionType;

			auto currentIntersectionResult = runIntersectionTest();	// run the intersection test, and only insert the point if the value is INTERSECTS;
																	// there can be other values as shown below in the enum list, but this is the only one we truly care about for now.
																	// The other values in the enum list are there for logic purposes, and may be used in the future if we need to take
																	// the enum class out of this one and make it it's own class.

			// If we did intersect, convert the point and insert (in a perfect world, we probably wouldn't need to convert, but that's not important right now)
			if (currentIntersectionResult.intersectionType == FRayIntersectionType::INTERSECTS)
			{
				intersectorUniquePointsRef->insertFTrianglePoint(FTrianglePoint(currentIntersectionResult.intersectedPoint, FTrianglePointType::INTERIOR));
			}
		}
	private:
		Vec3Pair rayToCast;		// a pair of Vec3 points that constitute a ray; exactly two of the dimensions of point A in this ray
								// must be part of the intersected point in the FRayIntersectionResult value returned by runIntersectionTest().
								// See below for how this is done.
		glm::vec3 interceptableTrianglePoints[3];	// the points that constitute the triangle we will be attempt an intersection on.
		UniquePointContainer* intersectorUniquePointsRef = nullptr;		// a reference to the container that intersected points will be inserted into.
		FRayIntersectionRoundingMode intersectionRoundingMode = FRayIntersectionRoundingMode::NOVAL;	// must be set by constructor.


		enum class FRayIntersectionType
		{
			NOVAL,					// the base value until the intersection test is done.
			PARALLEL,				// the line is parallel to the triangle's plane, but not within it.
			WITHIN_PLANE,			// part of the line is within the triangle, and is also aligned exactly to that triangle's plane.
			INTERSECTS,				// point B of the ray intersects with the triangle.
			DOES_NOT_INTERSECT		// the ray does not touch the triangle in any way.
		};

		// Below: this struct is designed to store metadata about the intersection attempt. Regardless of the value of in_intersectionType,
		// some rounding will be done. The values of FRayIntersectionRoundingMode come from the intersectingRoundingMode member of the parent
		// FRayIntersectionMachine class, which is set when said class is instantiated.
		//
		// The result of a legitimate intersection will be a point that has had only one dimension rounded, and where the other two non-rounded
		// dimensions are equal to pointA of the casting ray.
		struct FRayIntersectionResult
		{
			FRayIntersectionResult() {};
			FRayIntersectionResult(
								glm::vec3 in_rayToCastStartPoint,
								glm::vec3 in_intersectedPoint, 
								FRayIntersectionRoundingMode in_typeToRoundFor,
								FRayIntersectionType in_intersectionType)
			{
				switch (in_typeToRoundFor)
				{
					// used by XFRayCastQuad, for XFRays
					case (FRayIntersectionRoundingMode::ROUND_FOR_X):	
					{
						intersectedPoint.x = IndependentUtils::roundToHundredth(in_intersectedPoint.x);
						intersectedPoint.y = in_rayToCastStartPoint.y;
						intersectedPoint.z = in_rayToCastStartPoint.z;
						break;
					}

					// used by YFRayCastQuad, for YFRays
					case (FRayIntersectionRoundingMode::ROUND_FOR_Y):
					{
						intersectedPoint.x = in_rayToCastStartPoint.x;
						intersectedPoint.y = IndependentUtils::roundToHundredth(in_intersectedPoint.y);
						intersectedPoint.z = in_rayToCastStartPoint.z;
						break;
					}

					// used by ZFRayCastQuad, for ZFRays
					case (FRayIntersectionRoundingMode::ROUND_FOR_Z):
					{
						intersectedPoint.x = in_rayToCastStartPoint.x;
						intersectedPoint.y = in_rayToCastStartPoint.y;
						intersectedPoint.z = IndependentUtils::roundToHundredth(in_intersectedPoint.z);
						break;
					}
				}

				intersectionType = in_intersectionType;
			};

			FRayIntersectionType intersectionType = FRayIntersectionType::NOVAL;	// the intersectionType that was passed in during the constructor;
																					// this value is determined in the function runIntersectionTest()
			glm::vec3 intersectedPoint;	// the resulting intersected point determined by the call to runIntersectionTest()
		};

		FRayIntersectionResult runIntersectionTest();	// runs the ray against the triangle to check for an intersection.
};

#endif