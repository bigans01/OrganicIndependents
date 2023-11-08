#pragma once

#ifndef YSLICELINE_H
#define YSLICELINE_H

#include "XZDimMoveVal.h"
#include "XZDim.h"
#include "FTriangleLineTargets.h"
#include "FTrianglePoint.h"

class YSliceLine
{
	public:
		YSliceLine() {};
		YSliceLine(XZDim in_currentLineXZDim,
					FTrianglePoint in_linePointA,
					FTrianglePoint in_linePointB,
					int in_currentBackwardYValue,
					int in_currentForwardYValue,
					float in_currentFixedInterval) :
			currentLineXDim(in_currentLineXZDim.x),
			currentLineZDim(in_currentLineXZDim.z),
			linePointA(in_linePointA),
			linePointB(in_linePointB),
			currentBackwardYValue(in_currentBackwardYValue),
			currentForwardYValue(in_currentForwardYValue),
			currentFixedInterval(in_currentFixedInterval)
		{
			determineYLineOutputs();
		}

		XZDimMoveVal fetchYLineMoveVal();
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchAffectedKeys();
		FTriangleLineTargets retrieveProducedTargets();
	private:
		int currentLineXDim = 0;
		int currentLineZDim = 0;
		FTrianglePoint linePointA;
		FTrianglePoint linePointB;
		int currentBackwardYValue = 0;
		int currentForwardYValue = 0;
		float currentFixedInterval = 0.0f;

		XZDimMoveVal calculatedMoveVal;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> affectedKeys;
		FTriangleLineTargets lineTargets;

		void determineYLineOutputs();
};

#endif
