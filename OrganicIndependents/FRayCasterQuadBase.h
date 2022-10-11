#pragma once

#ifndef FRAYCASTERQUADBASE_H
#define FRAYCASTERQUADBASE_H

#include "ECBPolyPoint.h"
#include "UniquePointContainer.h"
#include "FRayCasterInitData.h"
#include <iostream>
#include "IndependentUtils.h"

class FRayCasterQuadBase
{
	public:
		void initialize(FRayCasterInitData in_initData,
						ECBPolyPoint in_targetTrianglePoints[3],
						UniquePointContainer* in_quadPointContainerRef)
		{
			quadDimOneMin = in_initData.dimOneMin;
			quadDimOneMax = in_initData.dimOneMax;
			quadDimTwoMin = in_initData.dimTwoMin;
			quadDimTwoMax = in_initData.dimTwoMax;
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
		float quadRayCastInterval = 0.0f;
		ECBPolyPoint targetTrianglePoints[3];
		UniquePointContainer* quadPointContainerRef = nullptr;
};

#endif