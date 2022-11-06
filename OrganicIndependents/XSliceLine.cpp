#include "stdafx.h"
#include "XSliceLine.h"

YZDimMoveVal XSliceLine::fetchLineMoveVal()
{
	return calculatedMoveVal;
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> XSliceLine::fetchAffectedKeys()
{
	return affectedKeys;
}

FTriangleLineTargets XSliceLine::retrieveProducedTargets()
{
	return lineTargets;
}

void XSliceLine::determineLineOutputs()
{
	// Check Y; to have a valid move val for Y, the modulo of point B, when calculated against the fixed interval,
	// must be 0. There are 3 possible scenarios:
	// -the Y of point B is less than point A; so the move val for Y is -1.
	// -the Y of point B is greater than point A; so the move val for Y is +1.
	// -the Y values of both point A and B are equal; so the move val remains 0.
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
		else if (linePointB.y == linePointA.y)
		{
			perfectlyAlignedToZGridLine = true;
		}
	}

	YZDimMoveVal determinedMoveVal(calculatedYDimMove, calculatedZDimMove);
	calculatedMoveVal = determinedMoveVal;

	std::cout << "(XSliceLine::determineCalculatedMoveVal): Y/Z move vals are: Y -> " << calculatedMoveVal.y << " | Z -> " << calculatedMoveVal.z << std::endl;

	// unless the line is perfectly clamped to Y or Z, proceed normally.
	if (!perfectlyAlignedToYGridLine && !perfectlyAlignedToZGridLine)
	{
		EnclaveKeyDef::EnclaveKey backwardXKey(currentBackwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardXKey(currentForwardXValue, currentLineYDim, currentLineZDim);
		
		std::cout << "Backward key is: ";
		backwardXKey.printKey();
		std::cout << std::endl;

		std::cout << "Forward key is: ";
		forwardXKey.printKey();
		std::cout << std::endl;

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardXKey);
		targetKeys.insert(forwardXKey);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::SCANNED),
											   targetKeys);

		lineTargets = determinedTargets;		
	}
}