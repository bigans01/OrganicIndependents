#include "stdafx.h"
#include "PrimaryLineT2.h"
#include "IndependentUtils.h"

void PrimaryLineT2::calibrate(ECBPolyPoint in_pointC)
{
	//std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
	//std::cout << ">>>>> Current Line Analysis, pre-calibration: >>>>>" << std::endl;
	ECBPolyPoint pointA = beginPointRealXYZ;
	ECBPolyPoint pointB = endPointRealXYZ;
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


}

void PrimaryLineT2::isCalibrateable(float* in_remainderPtr, int* in_blockLocationPtr, int* in_chunkLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz)
{
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

void PrimaryLineT2::applySlopes(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope)
{
	ECBPolyPoint currentIntendedFaces = IndependentUtils::determineIntendedFaces(beginPointRealXYZ, endPointRealXYZ, thirdPointRealXYZ);
	ECBPolyPoint x_slope_to_apply = in_xSlope;
	ECBPolyPoint y_slope_to_apply = in_ySlope;
	ECBPolyPoint z_slope_to_apply = in_zSlope;
	//std::cout << "Root intended faces: " << in_originalNormalizedSlope.x << ", " << in_originalNormalizedSlope.y << ", " << in_originalNormalizedSlope.z << ", " << std::endl;
	//std::cout << "Current intended faces: " << currentIntendedFaces.x << ", " << currentIntendedFaces.y << ", " << currentIntendedFaces.z << ", " << std::endl;
	if (in_originalNormalizedSlope.x != currentIntendedFaces.x)
	{
		//std::cout << ">>> Necessary x inversion detected! " << std::endl;
		x_slope_to_apply = IndependentUtils::invertSlope(x_slope_to_apply);
	}
	if (in_originalNormalizedSlope.y != currentIntendedFaces.y)
	{
		//std::cout << ">>> Necessary y inversion detected! " << std::endl;
		y_slope_to_apply = IndependentUtils::invertSlope(y_slope_to_apply);
	}
	if (in_originalNormalizedSlope.z != currentIntendedFaces.z)
	{
		//std::cout << ">>> Necessary z inversion detected! " << std::endl;
		z_slope_to_apply = IndependentUtils::invertSlope(z_slope_to_apply);
	}
	x_int_slope = x_slope_to_apply;
	y_int_slope = y_slope_to_apply;
	z_int_slope = z_slope_to_apply;
	int_slopes.x_intercept_slope = x_slope_to_apply;
	int_slopes.y_intercept_slope = y_slope_to_apply;
	int_slopes.z_intercept_slope = z_slope_to_apply;
}

void PrimaryLineT2::applySlopes(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC)
{
	ECBPolyPoint currentIntendedFaces = IndependentUtils::determineIntendedFaces(in_pointA, in_pointB, in_pointC);
	ECBPolyPoint x_slope_to_apply = in_xSlope;
	ECBPolyPoint y_slope_to_apply = in_ySlope;
	ECBPolyPoint z_slope_to_apply = in_zSlope;
	//std::cout << "Root intended faces: " << in_originalNormalizedSlope.x << ", " << in_originalNormalizedSlope.y << ", " << in_originalNormalizedSlope.z << ", " << std::endl;
	//std::cout << "Current intended faces: " << currentIntendedFaces.x << ", " << currentIntendedFaces.y << ", " << currentIntendedFaces.z << ", " << std::endl;
	if (in_originalNormalizedSlope.x != currentIntendedFaces.x)
	{
		//std::cout << ">>> Necessary x inversion detected! " << std::endl;
		x_slope_to_apply = IndependentUtils::invertSlope(x_slope_to_apply);
	}
	if (in_originalNormalizedSlope.y != currentIntendedFaces.y)
	{
		//std::cout << ">>> Necessary y inversion detected! " << std::endl;
		y_slope_to_apply = IndependentUtils::invertSlope(y_slope_to_apply);
	}
	if (in_originalNormalizedSlope.z != currentIntendedFaces.z)
	{
		//std::cout << ">>> Necessary z inversion detected! " << std::endl;
		z_slope_to_apply = IndependentUtils::invertSlope(z_slope_to_apply);
	}
	x_int_slope = x_slope_to_apply;
	y_int_slope = y_slope_to_apply;
	z_int_slope = z_slope_to_apply;
	int_slopes.x_intercept_slope = x_slope_to_apply;
	int_slopes.y_intercept_slope = y_slope_to_apply;
	int_slopes.z_intercept_slope = z_slope_to_apply;

	//std::cout << "-----------Slopes are: " << std::endl;
	//std::cout << "x: " << x_int_slope.x << ", " << x_int_slope.y << ", " << x_int_slope.z << std::endl;
	//std::cout << "y: " << y_int_slope.x << ", " << y_int_slope.y << ", " << y_int_slope.z << std::endl;
	//std::cout << "z: " << z_int_slope.x << ", " << z_int_slope.y << ", " << z_int_slope.z << std::endl;

	//std::cout << "ApplySlopes v2 called...." << std::endl;
}