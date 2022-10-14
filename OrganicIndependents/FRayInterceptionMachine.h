#pragma once

#ifndef FRAYINTERCEPTIONMACHINE_H
#define FRAYINTERCEPTIONMACHINE_H

#include "FRayInterceptionType.h"
#include "ECBPolyPoint.h"
#include "UniquePointContainer.h"
#include "ECBPolyPointPair.h"

class FRayInterceptionMachine
{
	public:
		FRayInterceptionMachine(ECBPolyPointPair in_rayToCast,
								ECBPolyPoint in_interceptableTrianglePoints[3],
								UniquePointContainer* in_interceptorUniquePointsRef,
								FRayInterceptionType in_interceptionType)
		{
			rayToCast = in_rayToCast;
			for (int x = 0; x < 3; x++)
			{
				interceptableTrianglePoints[x] = in_interceptableTrianglePoints[x];
			}
			interceptorUniquePointsRef = in_interceptorUniquePointsRef;
			interceptionType = in_interceptionType;
		}
	private:
		ECBPolyPointPair rayToCast;
		ECBPolyPoint interceptableTrianglePoints[3];
		UniquePointContainer* interceptorUniquePointsRef = nullptr;
		FRayInterceptionType interceptionType = FRayInterceptionType::NOVAL;	// must be set by constructor.
};

#endif