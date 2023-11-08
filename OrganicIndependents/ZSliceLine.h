#pragma once

#ifndef ZSLICELINE_H
#define ZSLICELINE_H

#include "XYDimMoveVal.h"
#include "XYDim.h"
#include "FTriangleLineTargets.h"
#include "FTrianglePoint.h"

class ZSliceLine
{
	public:
		ZSliceLine() {};
		ZSliceLine(XYDim in_currentLineXYDim,
					FTrianglePoint in_linePointA,
					FTrianglePoint in_linePointB,
					int in_currentBackwardZValue,
					int in_currentForwardZValue,
					float in_currentFixedInterval) :
			currentLineXDim(in_currentLineXYDim.x),
			currentLineYDim(in_currentLineXYDim.y),
			linePointA(in_linePointA),
			linePointB(in_linePointB),
			currentBackwardZValue(in_currentBackwardZValue),
			currentForwardZValue(in_currentForwardZValue),
			currentFixedInterval(in_currentFixedInterval)
		{
			determineZLineOutputs();
		}
		XYDimMoveVal fetchZLineMoveVal();
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchAffectedKeys();
		FTriangleLineTargets retrieveProducedTargets();
	private:
		int currentLineXDim = 0;
		int currentLineYDim = 0;
		FTrianglePoint linePointA;
		FTrianglePoint linePointB;
		int currentBackwardZValue = 0;
		int currentForwardZValue = 0;
		float currentFixedInterval = 0.0f;

		XYDimMoveVal calculatedMoveVal;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> affectedKeys;
		FTriangleLineTargets lineTargets;

		void determineZLineOutputs();
};

#endif
