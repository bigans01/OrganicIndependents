#include "stdafx.h"
#include "ZSliceLine.h"

XYDimMoveVal ZSliceLine::fetchZLineMoveVal()
{
	return calculatedMoveVal;
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> ZSliceLine::fetchAffectedKeys()
{
	return affectedKeys;
}

FTriangleLineTargets ZSliceLine::retrieveProducedTargets()
{
	return lineTargets;
}

void ZSliceLine::determineZLineOutputs()
{
	// Check X; to have a valid move val for X, the modulo of point B.x, when calculated against the fixed interval,
	// must be 0. There are 3 possible scenarios:
	// -the X of point B is less than point A; so the move val for X is -1.
	// -the X of point B is greater than point A; so the move val for X is +1.
	// -the X values of both point A and B are equal; so the move val remains 0.
	int calculatedXDimMove = 0;
	bool perfectlyAlignedToXGridLine = false;
	float pointBxModulo = fmod(linePointB.x, currentFixedInterval);
	if (pointBxModulo == 0.0f)
	{
		if (linePointB.x > linePointA.x)
		{
			calculatedXDimMove++;
		}
		else if (linePointB.x < linePointA.x)
		{
			calculatedXDimMove--;
		}
		else if (linePointB.x == linePointA.x)
		{
			perfectlyAlignedToXGridLine = true;
		}
	}

	// Do the same for Y.
	int calculatedYDimMove = 0;
	bool perfectlyAlignedToYGridLine = false;
	float pointByModulo = fmod(linePointB.y, currentFixedInterval);
	if (pointByModulo == 0.0f)
	{
		if (linePointB.y > linePointA.y)
		{
			calculatedYDimMove++;
		}
		else if (linePointB.y < linePointA.y)
		{
			calculatedYDimMove--;
		}
		else if (linePointB.y == linePointA.y)
		{
			perfectlyAlignedToYGridLine = true;
		}
	}

	XYDimMoveVal determinedMoveVal(calculatedXDimMove, calculatedYDimMove);
	calculatedMoveVal = determinedMoveVal;

	std::cout << "(ZSliceLine::determineCalculatedMoveVal): X/Y move vals are: Y -> " << calculatedMoveVal.x << " | Z -> " << calculatedMoveVal.y << std::endl;

	// unless the line is perfectly clamped to X or Y, proceed normally.
	if (!perfectlyAlignedToXGridLine && !perfectlyAlignedToYGridLine)
	{
		EnclaveKeyDef::EnclaveKey backwardZKey(currentLineXDim, currentLineYDim, currentBackwardZValue);
		EnclaveKeyDef::EnclaveKey forwardZKey(currentLineXDim, currentLineYDim, currentForwardZValue);

		std::cout << "Backward key is: ";
		backwardZKey.printKey();
		std::cout << std::endl;

		std::cout << "Forward key is: ";
		forwardZKey.printKey();
		std::cout << std::endl;

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardZKey);
		targetKeys.insert(forwardZKey);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::ZSLICE_SCANNED),
															targetKeys);

		lineTargets = determinedTargets;
	}
	else
	{
		
	}
}