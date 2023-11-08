#pragma once

#ifndef FRAYCASTERQUADBASE_H
#define FRAYCASTERQUADBASE_H

/*

Description: 

This is the base class for XF/YF/ZF RayCastQuad classes.

*/

#include "FRayCasterInitData.h"
#include "FRayIntersectionRoundingMode.h"
#include "FRayIntersectionMachine.h"

class FRayCasterQuadBase
{
	public:
		void initialize(FRayCasterInitData in_initData,
						FTrianglePoint in_targetTrianglePoints[3],
						UniquePointContainer* in_quadPointContainerRef)
		{
			quadDimOneMin = in_initData.dimOneMin;
			quadDimOneMax = in_initData.dimOneMax;
			quadDimTwoMin = in_initData.dimTwoMin;
			quadDimTwoMax = in_initData.dimTwoMax;
			quadTargetRaycastDimMin = in_initData.rayCastDimMin;
			quadTargetRaycastDimMax = in_initData.rayCastDimMax;
			quadRayCastInterval = in_initData.rayCastInterval;
			for (int x = 0; x < 3; x++)
			{
				targetTrianglePoints[x] = in_targetTrianglePoints[x];
			}
			quadPointContainerRef = in_quadPointContainerRef;
		}

		virtual void buildAndCastRays() = 0;

	protected:
		int quadDimOneMin = 0;
		int quadDimOneMax = 0;
		int quadDimTwoMin = 0;
		int quadDimTwoMax = 0;
		int quadTargetRaycastDimMin = 0;
		int quadTargetRaycastDimMax = 0;
		float quadRayCastInterval = 0.0f;
		FTrianglePoint targetTrianglePoints[3];
		UniquePointContainer* quadPointContainerRef = nullptr;
};

#endif