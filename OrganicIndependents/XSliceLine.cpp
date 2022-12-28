#include "stdafx.h"
#include "XSliceLine.h"

YZDimMoveVal XSliceLine::fetchXLineMoveVal()
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

void XSliceLine::determineXLineOutputs()
{
	// Check Y; to have a valid move val for Y, the modulo of point B.y, when calculated against the fixed interval,
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
		else if (linePointB.z == linePointA.z)
		{
			perfectlyAlignedToZGridLine = true;
		}
	}

	YZDimMoveVal determinedMoveVal(calculatedYDimMove, calculatedZDimMove);
	calculatedMoveVal = determinedMoveVal;

	//std::cout << "(XSliceLine::determineCalculatedMoveVal): Y/Z move vals are: Y -> " << calculatedMoveVal.y << " | Z -> " << calculatedMoveVal.z << std::endl;

	// unless the line is perfectly clamped to Y or Z, proceed normally.
	if (!perfectlyAlignedToYGridLine && !perfectlyAlignedToZGridLine)
	{
		EnclaveKeyDef::EnclaveKey backwardXKey(currentBackwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardXKey(currentForwardXValue, currentLineYDim, currentLineZDim);
		
		// Below: FTDEBUG (uncomment when done)
		/*
		std::cout << "Backward key is: ";
		backwardXKey.printKey();
		std::cout << std::endl;

		std::cout << "Forward key is: ";
		forwardXKey.printKey();
		std::cout << std::endl;
		*/

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardXKey);
		targetKeys.insert(forwardXKey);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::XSLICE_SCANNED),
											   targetKeys);

		lineTargets = determinedTargets;		
	}
	else if (perfectlyAlignedToYGridLine)
	{
	

		EnclaveKeyDef::EnclaveKey backwardXKey(currentBackwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardXKey(currentForwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey backwardXKeyNegative(currentBackwardXValue, currentLineYDim - 1, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardXKeyNegative(currentForwardXValue, currentLineYDim - 1, currentLineZDim);

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardXKey);
		targetKeys.insert(forwardXKey);
		targetKeys.insert(backwardXKeyNegative);
		targetKeys.insert(forwardXKeyNegative);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::XSLICE_SCANNED),
			targetKeys);

		lineTargets = determinedTargets;

		// FTDEBUG, (uncomment when needed for debugging)
		//std::cout << "(XSliceLine::determineCalculatedMoveVal): found line as being aligned to Y. " << std::endl;
		//std::cout << "currentLineYDim value is: " << currentLineYDim << std::endl;
		//int alignedToYWait = 3;
		//std::cin >> alignedToYWait;
	}
	else if (perfectlyAlignedToZGridLine)
	{
		//std::cout << "(XSliceLine::determineCalculatedMoveVal): found line as being aligned to Z. " << std::endl;

		EnclaveKeyDef::EnclaveKey backwardXKey(currentBackwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey forwardXKey(currentForwardXValue, currentLineYDim, currentLineZDim);
		EnclaveKeyDef::EnclaveKey backwardXKeyNegative(currentBackwardXValue, currentLineYDim, currentLineZDim - 1);
		EnclaveKeyDef::EnclaveKey forwardXKeyNegative(currentForwardXValue, currentLineYDim, currentLineZDim - 1);

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> targetKeys;
		targetKeys.insert(backwardXKey);
		targetKeys.insert(forwardXKey);
		targetKeys.insert(backwardXKeyNegative);
		targetKeys.insert(forwardXKeyNegative);

		FTriangleLineTargets determinedTargets(FTriangleLine(linePointA, linePointB, FTriangleLineType::XSLICE_SCANNED),
			targetKeys);

		lineTargets = determinedTargets;


		int alignedToZWait = 3;
		std::cin >> alignedToZWait;
	}
}