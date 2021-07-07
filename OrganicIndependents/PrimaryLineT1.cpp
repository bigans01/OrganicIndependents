#include "stdafx.h"
#include "PrimaryLineT1.h"
#include "IndependentUtils.h"

void PrimaryLineT1::calibrate(ECBPolyPoint in_pointC)
{
	//std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
	//std::cout << ">>>>> Current Line Analysis, NORMAL pre-calibration: >>>>>" << std::endl;
	ECBPolyPoint pointA = beginPointRealXYZ;
	ECBPolyPoint pointB = endPointRealXYZ;
	//checkForSingleAxisClamping();		// check if the line is clamped on any of the axis (would be up to 2, maximum)
	//std::cout << "---- Normalized Slope, point A to point B " << std::endl;
	ECBPolyPoint normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	//std::cout << normalizedSlopeA.x << ", " << normalizedSlopeA.y << ", " << normalizedSlopeA.z << std::endl;
	//std::cout << "---- Normalized Slope, point B to point A " << std::endl;
	ECBPolyPoint normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);
	//std::cout << normalizedSlopeB.x << ", " << normalizedSlopeB.y << ", " << normalizedSlopeB.z << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point A----" << std::endl;
	//std::cout << "Begin XYZ coord: " << beginPointRealXYZ.x << ", " << beginPointRealXYZ.y << ", " << beginPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << beginPointMeta.preciseCoord.x << ", " << beginPointMeta.preciseCoord.y << ", " << beginPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << beginPointMeta.blockKey.x << ", " << beginPointMeta.blockKey.y << ", " << beginPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << beginPointMeta.enclaveKey.x << ", " << beginPointMeta.enclaveKey.y << ", " << beginPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point B---- " << std::endl;
	//std::cout << "End XYZ coord: " << endPointRealXYZ.x << ", " << endPointRealXYZ.y << ", " << endPointRealXYZ.z << std::endl;
	//if (beginPointRealXYZ.y == endPointRealXYZ.y)
	//{
		//std::cout << "Y IS A MATCH! " << std::endl;
	//}
	//std::cout << "Third XYZ coord: " << in_pointC.x << ", " << in_pointC.y << ", " << in_pointC.z << std::endl;
	//std::cout << "Remainder value: " << endPointMeta.preciseCoord.x << ", " << endPointMeta.preciseCoord.y << ", " << endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << endPointMeta.blockKey.x << ", " << endPointMeta.blockKey.y << ", " << endPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << endPointMeta.enclaveKey.x << ", " << endPointMeta.enclaveKey.y << ", " << endPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;

	// calibrate point A
	//std::cout << "Point A: checking for x-shift " << std::endl;
	int x_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.x, normalizedSlopeA.x);	// check if x is 0 or -1 (-1 indicates a negative move in the axis)
	isCalibrateable(&beginPointMeta.preciseCoord.x, &beginPointMeta.blockKey.x, &beginPointMeta.enclaveKey.x, normalizedSlopeA.x, pointA.x, pointB.x, in_pointC.x);

	//std::cout << "Point A: checking for y-shift " << std::endl;
	int y_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.y, normalizedSlopeA.y);	// ...y
	isCalibrateable(&beginPointMeta.preciseCoord.y, &beginPointMeta.blockKey.y, &beginPointMeta.enclaveKey.y, normalizedSlopeA.y, pointA.y, pointB.y, in_pointC.y);

	//std::cout << "Point A: checking for z-shift " << std::endl;
	int z_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.z, normalizedSlopeA.z);	// ...z
	isCalibrateable(&beginPointMeta.preciseCoord.z, &beginPointMeta.blockKey.z, &beginPointMeta.enclaveKey.z, normalizedSlopeA.z, pointA.z, pointB.z, in_pointC.z);

	// calibrate point B
	//std::cout << "::: Point B: checking for x_shift " << std::endl;
	//std::cout << ": Precise coord: " << endPointMeta.preciseCoord.x << ", " << endPointMeta.preciseCoord.y << ", " << endPointMeta.preciseCoord.z << ", " << std::endl;
	//std::cout << ": Normalized slope: " << normalizedSlopeB.x << ", " << normalizedSlopeB.y << ", " << normalizedSlopeB.z << std::endl;

	int x_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.x, normalizedSlopeB.x);
	isCalibrateable(&endPointMeta.preciseCoord.x, &endPointMeta.blockKey.x, &endPointMeta.enclaveKey.x, normalizedSlopeB.x, pointA.x, pointB.x, in_pointC.x);

	//std::cout << "Point B: checking for y_shift " << std::endl;
	int y_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.y, normalizedSlopeB.y);
	isCalibrateable(&endPointMeta.preciseCoord.y, &endPointMeta.blockKey.y, &endPointMeta.enclaveKey.y, normalizedSlopeB.y, pointA.y, pointB.y, in_pointC.y);

	//std::cout << "Point B: checking for z_shift " << std::endl;
	int z_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.y, normalizedSlopeB.z);
	isCalibrateable(&endPointMeta.preciseCoord.z, &endPointMeta.blockKey.z, &endPointMeta.enclaveKey.z, normalizedSlopeB.z, pointA.z, pointB.z, in_pointC.z);


	//std::cout << ">>>>> Current Line Analysis, Post-calibration: >>>>>" << std::endl;
	pointA = beginPointRealXYZ;
	pointB = endPointRealXYZ;
	//std::cout << "---- Normalized Slope, point A to point B " << std::endl;
	//normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	//std::cout << normalizedSlopeA.x << ", " << normalizedSlopeA.y << ", " << normalizedSlopeA.z << std::endl;
	//std::cout << "---- Normalized Slope, point B to point A " << std::endl;
	//normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);
	//std::cout << normalizedSlopeB.x << ", " << normalizedSlopeB.y << ", " << normalizedSlopeB.z << std::endl;

	//std::cout << std::endl;
	//std::cout << "----Point A----" << std::endl;
	//std::cout << "XYZ coord: " << beginPointRealXYZ.x << ", " << beginPointRealXYZ.y << ", " << beginPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << beginPointMeta.preciseCoord.x << ", " << beginPointMeta.preciseCoord.y << ", " << beginPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << beginPointMeta.blockKey.x << ", " << beginPointMeta.blockKey.y << ", " << beginPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << beginPointMeta.enclaveKey.x << ", " << beginPointMeta.enclaveKey.y << ", " << beginPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point B---- " << std::endl;
	//std::cout << "XYZ coord: " << endPointRealXYZ.x << ", " << endPointRealXYZ.y << ", " << endPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << endPointMeta.preciseCoord.x << ", " << endPointMeta.preciseCoord.y << ", " << endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << endPointMeta.blockKey.x << ", " << endPointMeta.blockKey.y << ", " << endPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << endPointMeta.enclaveKey.x << ", " << endPointMeta.enclaveKey.y << ", " << endPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;


}

void PrimaryLineT1::calibrateBlueprintKeys(ECBPolyPoint in_pointC)
{
	ECBPolyPoint pointA = beginPointRealXYZ;
	ECBPolyPoint pointB = endPointRealXYZ;
	//checkForSingleAxisClamping();		// check if the line is clamped on any of the axis (would be up to 2, maximum)
	//std::cout << "---- Normalized Slope, point A to point B " << std::endl;
	ECBPolyPoint normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	//std::cout << normalizedSlopeA.x << ", " << normalizedSlopeA.y << ", " << normalizedSlopeA.z << std::endl;
	//std::cout << "---- Normalized Slope, point B to point A " << std::endl;
	ECBPolyPoint normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);

	isBlueprintKeyCalibrateable(&beginPointBlueprintKey.x, normalizedSlopeA.x, pointA.x, pointB.x, in_pointC.x);
	isBlueprintKeyCalibrateable(&beginPointBlueprintKey.y, normalizedSlopeA.y, pointA.y, pointB.y, in_pointC.y);
	isBlueprintKeyCalibrateable(&beginPointBlueprintKey.z, normalizedSlopeA.z, pointA.z, pointB.z, in_pointC.z);


	isBlueprintKeyCalibrateable(&endPointBlueprintKey.x, normalizedSlopeB.x, pointA.x, pointB.x, in_pointC.x);
	isBlueprintKeyCalibrateable(&endPointBlueprintKey.y, normalizedSlopeB.y, pointA.y, pointB.y, in_pointC.y);
	isBlueprintKeyCalibrateable(&endPointBlueprintKey.z, normalizedSlopeB.z, pointA.z, pointB.z, in_pointC.z);


}


ECBPolyPoint PrimaryLineT1::calibrateWithInverseChecks(ECBPolyPoint in_pointC)
{
	//std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
	//std::cout << ">>>>> Current Line Analysis, REVERSE pre-calibration: >>>>>" << std::endl;
	ECBPolyPoint pointA = beginPointRealXYZ;
	ECBPolyPoint pointB = endPointRealXYZ;
	ECBPolyPoint invertChecks = checkForSingleAxisClamping();		// check if the line is clamped on any of the axis (would be up to 2, maximum)
	//std::cout << "---- Normalized Slope, point A to point B " << std::endl;
	ECBPolyPoint normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	//std::cout << normalizedSlopeA.x << ", " << normalizedSlopeA.y << ", " << normalizedSlopeA.z << std::endl;
	//std::cout << "---- Normalized Slope, point B to point A " << std::endl;
	ECBPolyPoint normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);
	//std::cout << normalizedSlopeB.x << ", " << normalizedSlopeB.y << ", " << normalizedSlopeB.z << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point A----" << std::endl;
	//std::cout << "Begin XYZ coord: " << beginPointRealXYZ.x << ", " << beginPointRealXYZ.y << ", " << beginPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << beginPointMeta.preciseCoord.x << ", " << beginPointMeta.preciseCoord.y << ", " << beginPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << beginPointMeta.blockKey.x << ", " << beginPointMeta.blockKey.y << ", " << beginPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << beginPointMeta.enclaveKey.x << ", " << beginPointMeta.enclaveKey.y << ", " << beginPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point B---- " << std::endl;
	//std::cout << "End XYZ coord: " << endPointRealXYZ.x << ", " << endPointRealXYZ.y << ", " << endPointRealXYZ.z << std::endl;
	//if (beginPointRealXYZ.y == endPointRealXYZ.y)
	//{
		//std::cout << "Y IS A MATCH! " << std::endl;
	//}
	//std::cout << "Third XYZ coord: " << in_pointC.x << ", " << in_pointC.y << ", " << in_pointC.z << std::endl;
	//std::cout << "Remainder value: " << endPointMeta.preciseCoord.x << ", " << endPointMeta.preciseCoord.y << ", " << endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << endPointMeta.blockKey.x << ", " << endPointMeta.blockKey.y << ", " << endPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << endPointMeta.enclaveKey.x << ", " << endPointMeta.enclaveKey.y << ", " << endPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;

	// calibrate point A
	//std::cout << "Point A: checking for x-shift " << std::endl;
	int x_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.x, normalizedSlopeA.x);	// check if x is 0 or -1 (-1 indicates a negative move in the axis)
	isCalibrateable(&beginPointMeta.preciseCoord.x, &beginPointMeta.blockKey.x, &beginPointMeta.enclaveKey.x, normalizedSlopeA.x, pointA.x, pointB.x, in_pointC.x);

	//std::cout << "Point A: checking for y-shift " << std::endl;
	int y_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.y, normalizedSlopeA.y);	// ...y
	isCalibrateable(&beginPointMeta.preciseCoord.y, &beginPointMeta.blockKey.y, &beginPointMeta.enclaveKey.y, normalizedSlopeA.y, pointA.y, pointB.y, in_pointC.y);

	//std::cout << "Point A: checking for z-shift " << std::endl;
	int z_checkA = IndependentUtils::calibrateEnclaveBlockKeys(beginPointMeta.preciseCoord.z, normalizedSlopeA.z);	// ...z
	isCalibrateable(&beginPointMeta.preciseCoord.z, &beginPointMeta.blockKey.z, &beginPointMeta.enclaveKey.z, normalizedSlopeA.z, pointA.z, pointB.z, in_pointC.z);

	// calibrate point B
	//std::cout << "Point B: checking for x_shift " << std::endl;
	int x_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.x, normalizedSlopeB.x);
	isCalibrateable(&endPointMeta.preciseCoord.x, &endPointMeta.blockKey.x, &endPointMeta.enclaveKey.x, normalizedSlopeB.x, pointA.x, pointB.x, in_pointC.x);

	//std::cout << "Point B: checking for y_shift " << std::endl;
	int y_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.y, normalizedSlopeB.y);
	isCalibrateable(&endPointMeta.preciseCoord.y, &endPointMeta.blockKey.y, &endPointMeta.enclaveKey.y, normalizedSlopeB.y, pointA.y, pointB.y, in_pointC.y);

	//std::cout << "Point B: checking for z_shift " << std::endl;
	int z_checkB = IndependentUtils::calibrateEnclaveBlockKeys(endPointMeta.preciseCoord.y, normalizedSlopeB.z);
	isCalibrateable(&endPointMeta.preciseCoord.z, &endPointMeta.blockKey.z, &endPointMeta.enclaveKey.z, normalizedSlopeB.z, pointA.z, pointB.z, in_pointC.z);


	//std::cout << ">>>>> Current Line Analysis, Post-calibration: >>>>>" << std::endl;
	pointA = beginPointRealXYZ;
	pointB = endPointRealXYZ;
	//std::cout << "---- Normalized Slope, point A to point B " << std::endl;
	//normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	//std::cout << normalizedSlopeA.x << ", " << normalizedSlopeA.y << ", " << normalizedSlopeA.z << std::endl;
	//std::cout << "---- Normalized Slope, point B to point A " << std::endl;
	//normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);
	//std::cout << normalizedSlopeB.x << ", " << normalizedSlopeB.y << ", " << normalizedSlopeB.z << std::endl;

	//std::cout << std::endl;
	//std::cout << "----Point A----" << std::endl;
	//std::cout << "XYZ coord: " << beginPointRealXYZ.x << ", " << beginPointRealXYZ.y << ", " << beginPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << beginPointMeta.preciseCoord.x << ", " << beginPointMeta.preciseCoord.y << ", " << beginPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << beginPointMeta.blockKey.x << ", " << beginPointMeta.blockKey.y << ", " << beginPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << beginPointMeta.enclaveKey.x << ", " << beginPointMeta.enclaveKey.y << ", " << beginPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "----Point B---- " << std::endl;
	//std::cout << "XYZ coord: " << endPointRealXYZ.x << ", " << endPointRealXYZ.y << ", " << endPointRealXYZ.z << std::endl;
	//std::cout << "Remainder value: " << endPointMeta.preciseCoord.x << ", " << endPointMeta.preciseCoord.y << ", " << endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Block value: " << endPointMeta.blockKey.x << ", " << endPointMeta.blockKey.y << ", " << endPointMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk value: " << endPointMeta.enclaveKey.x << ", " << endPointMeta.enclaveKey.y << ", " << endPointMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << std::endl;
	//std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;

	return invertChecks;
}

void PrimaryLineT1::calibrateForBlueprintTracingWithInverseChecks(ECBPolyPoint in_pointC)
{
	checkForSingleAxisClamping();
}

ECBPolyPoint PrimaryLineT1::checkForSingleAxisClamping()
{
	ECBPolyPoint returnPoint;
	// check for x
	if (beginPointRealXYZ.x == endPointRealXYZ.x)
	{
		//std::cout << "!!! notice: line is clamped to x-axis. +++!!!" << std::endl;
		intendedFaces.x *= -1;
		//x_clamp_value = 1;
		returnPoint.x = 1.0f;
		switchIntended.x = 1.0f;
		x_int = IndependentUtils::invertSlope(x_int);
	}
	if (beginPointRealXYZ.y == endPointRealXYZ.y)
	{
		//std::cout << "!!! notice: line is clamped to y-axis. " << std::endl;
		//y_clamp_value = 1;
		intendedFaces.y *= -1;
		returnPoint.y = 1.0f;
		switchIntended.y = 1.0f;
		y_int = IndependentUtils::invertSlope(y_int);
	}
	if (beginPointRealXYZ.z == endPointRealXYZ.z)
	{
		//std::cout << "!!! notice: line is clamped to z-axis. " << std::endl;
		//z_clamp_value = 1;
		intendedFaces.z *= -1;
		returnPoint.z = 1.0f;
		switchIntended.z = 1.0f;
		z_int = IndependentUtils::invertSlope(z_int);
	}
	return returnPoint;
}

void PrimaryLineT1::isCalibrateable(float* in_remainderPtr, int* in_blockLocationPtr, int* in_chunkLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz)
{
	//std::cout << "Calibrateable?" << std::endl;
	//std::cout << ">> Remainder: " << *in_remainderPtr << std::endl;
	//std::cout << ">> Block: " << *in_blockLocationPtr << std::endl;
	//std::cout << ">> Normalized slope: " << in_normalizedSlopeValue << std::endl;

	//if (*in_remainderPtr >= .9999f)
	//{
	//	std::cout << "!!! Special Check: remainder is ALMOST 1.0f." << std::endl;
	//}

	if ((*in_remainderPtr == 0.0f) && in_normalizedSlopeValue == -1.0f)	// condition is: there is no remainder, but the slope is negative
	{

		if (*in_blockLocationPtr == 0)		// block's original location was 0
		{
			//std::cout << "Shift type detected: block value was 0, entering new chunk..." << std::endl;
			//std::cout << "Old value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
			*in_remainderPtr = 1.0f;
			*in_blockLocationPtr = 3;						// because the block is 0, it's new value going negative will be 3.
			*in_chunkLocationPtr -= 1;	// ...and the new chunk coord will be the current value -1.
			//std::cout << "New value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
		}
		else if (*in_blockLocationPtr >= 1)	// block's original location was not 0, but is >= 1
		{
			//std::cout << "Shift type detected: block value was not 0, staying in current chunk..." << std::endl;
			*in_remainderPtr = 1.0f;
			*in_blockLocationPtr -= 1;
		}
	}

	if ((*in_remainderPtr == 1.0f) && in_normalizedSlopeValue == 1.0f)	// condition is: there is no remainder, but the slope is positive
	{
		//std::cout << "#### Forward shift detected!!" << std::endl;
		if (*in_blockLocationPtr == 3)		// block's original location was 3
		{
			//std::cout << "Shift type detected: block value was 0, entering new chunk..." << std::endl;
			//std::cout << "Old value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
			*in_remainderPtr = 0.0f;
			*in_blockLocationPtr = 0;						// because the block is 0, it's new value going negative will be 3.
			*in_chunkLocationPtr += 1;	// ...and the new chunk coord will be the current value -1.
										//std::cout << "New value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
		}
		else if (*in_blockLocationPtr >= 1)	// block's original location was not 0, but is >= 1
		{
			//std::cout << "Shift type detected: block value was not 0, staying in current chunk..." << std::endl;
			*in_remainderPtr = 0.0f;
			*in_blockLocationPtr += 1;
		}
	}

	if (in_normalizedSlopeValue == 0.0f)
	{
		//std::cout << "NON-SLOPE detected..." << std::endl;
		CursorPathTraceContainer xContainer = IndependentUtils::getPreciseCoordinate(in_pointAxyz);
		//std::cout << "remainder is: " << xContainer.ExactBlockCoord << std::endl;
		if (xContainer.ExactBlockCoord == 0.0f)
		{
			float direction = IndependentUtils::findNormalizedDirection(in_pointAxyz, in_pointCxyz);
			if ((*in_remainderPtr == 0.0f) && direction == -1.0f)
			{
				//std::cout << "negative block shift required! " << std::endl;
				if (*in_blockLocationPtr == 0)		// block's original location was 0
				{
					//std::cout << "Shift type detected: block value was 0, entering new chunk..." << std::endl;
					//std::cout << "Old value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
					*in_remainderPtr = 1.0f;
					*in_blockLocationPtr = 3;						// because the block is 0, it's new value going negative will be 3.
					*in_chunkLocationPtr -= 1;	// ...and the new chunk coord will be the current value -1.
												//std::cout << "New value of chunkLocationPtr: " << *in_chunkLocationPtr << std::endl;
				}
				else if (*in_blockLocationPtr >= 1)	// block's original location was not 0, but is >= 1
				{
					//std::cout << "Shift type detected: block value was not 0, staying in current chunk..." << std::endl;
					*in_remainderPtr = 1.0f;
					*in_blockLocationPtr -= 1;
				}
			}
		}
	}
}

void PrimaryLineT1::isBlueprintKeyCalibrateable(int* in_blueprintLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz)
{
	
	float xyzModResult = float(fmod(in_pointAxyz, 32));
	if ((xyzModResult == 0) && in_normalizedSlopeValue == -1.0f)	// potential blueprint key calibration, if this is 0
	{
		*in_blueprintLocationPtr -= 1;
	}

	if ((xyzModResult == 0) && in_normalizedSlopeValue == 0.0f)
	{
		float direction = IndependentUtils::findNormalizedDirection(in_pointAxyz, in_pointCxyz);
		if (direction == -1.0f)
		{
			*in_blueprintLocationPtr -= 1;
		}
	}
	
	/*
	if ((xyzModResult == 0) && in_normalizedSlopeValue == 1.0f)
	{

	}
	*/


}

void PrimaryLineT1::applySlopesFromParentLine(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope)
{
	ECBPolyPoint x_slope_to_apply = in_xSlope;
	ECBPolyPoint y_slope_to_apply = in_ySlope;
	ECBPolyPoint z_slope_to_apply = in_zSlope;
	if (in_originalNormalizedSlope.x != intendedFaces.x)
	{
		//std::cout << ">>> Necessary x inversion detected! " << std::endl;
		x_slope_to_apply = IndependentUtils::invertSlope(x_slope_to_apply);
	}
	if (in_originalNormalizedSlope.y != intendedFaces.y)
	{
		//std::cout << ">>> Necessary y inversion detected! " << std::endl;
		y_slope_to_apply = IndependentUtils::invertSlope(y_slope_to_apply);
	}
	if (in_originalNormalizedSlope.z != intendedFaces.z)
	{
		//std::cout << ">>> Necessary z inversion detected! " << std::endl;
		z_slope_to_apply = IndependentUtils::invertSlope(z_slope_to_apply);
	}
	x_int = x_slope_to_apply;
	y_int = y_slope_to_apply;
	z_int = z_slope_to_apply;
}