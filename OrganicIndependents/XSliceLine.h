#pragma once

#ifndef XSLICELINE_H
#define XSLICELINE_H

#include "YZDimMoveVal.h"
#include "YZDim.h"
#include "FTriangleLineTargets.h"
#include "FTrianglePoint.h"

class XSliceLine
{
	public:
		XSliceLine() {};
		XSliceLine(YZDim in_currentLineYZDim,
					FTrianglePoint in_linePointA,
					FTrianglePoint in_linePointB,
					int in_currentBackwardXValue,
					int	in_currentForwardXValue,
					float in_currentFixedInterval) :
			currentLineYDim(in_currentLineYZDim.y),
			currentLineZDim(in_currentLineYZDim.z),
			linePointA(in_linePointA),
			linePointB(in_linePointB),
			currentBackwardXValue(in_currentBackwardXValue),
			currentForwardXValue(in_currentForwardXValue),
			currentFixedInterval(in_currentFixedInterval)
		{
			determineXLineOutputs();
		}

		YZDimMoveVal fetchXLineMoveVal();
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchAffectedKeys();
		FTriangleLineTargets retrieveProducedTargets();
	private:
		int currentLineYDim = 0;
		int currentLineZDim = 0;
		FTrianglePoint linePointA;
		FTrianglePoint linePointB;
		int currentBackwardXValue = 0;
		int currentForwardXValue = 0;
		float currentFixedInterval = 0.0f;

		YZDimMoveVal calculatedMoveVal;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> affectedKeys;
		FTriangleLineTargets lineTargets;

		void determineXLineOutputs();
};

#endif