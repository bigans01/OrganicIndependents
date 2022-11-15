#include "stdafx.h"
#include "YSliceLine.h"

XZDimMoveVal YSliceLine::fetchYLineMoveVal()
{
	return calculatedMoveVal;
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> YSliceLine::fetchAffectedKeys()
{
	return affectedKeys;
}

FTriangleLineTargets YSliceLine::retrieveProducedTargets()
{
	return lineTargets;
}

void YSliceLine::determineYLineOutputs()
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

	// Do the same for Z.
	int calculatedZDimMove = 0;
	bool perfectlyAlignedToZGridLine = false;
	float pointBzModulo = fmod(linePointB.z, currentFixedInterval);
	if (pointBzModulo == 0.0f)
	{
		if (linePointB.z > linePointA.z)
		{
			calculatedZDimMove++;
		}
		else if (linePointB.z < linePointA.z)
		{
			calculatedZDimMove--;
		}
		else if (linePointB.z == linePointA.z)
		{
			perfectlyAlignedToZGridLine = true;
		}
	}

	XZDimMoveVal determinedMoveVal(calculatedXDimMove, calculatedZDimMove);
	calculatedMoveVal = determinedMoveVal;

	std::cout << "(YSliceLine::determineCalculatedMoveVal): X/Z move vals are: Y -> " << calculatedMoveVal.x << " | Z -> " << calculatedMoveVal.z << std::endl;

	// unless the line is perfectly clamped to X or Z, proceed normally.
	if (!perfectlyAlignedToXGridLine && !perfectlyAlignedToZGridLine)
	{
		EnclaveKeyDef::EnclaveKey backwardYKey(currentLineXDim, currentBackwardYValue, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardYKey(currentLineXDim, currentForwardYValue, currentLineZDim);

		std::cout << "Backward key is: ";
		backwardYKey.printKey();
		std::cout << std::endl;

		std::cout << "Forward key is: ";
		forwardYKey.printKey();
		std::cout << std::endl;

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardYKey);
		targetKeys.insert(forwardYKey);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::YSLICE_SCANNED),
															targetKeys);

		lineTargets = determinedTargets;
	}
}