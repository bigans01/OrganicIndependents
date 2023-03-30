#include "stdafx.h"
#include "FTriangleKeySetCalibrator.h"

FTriangleKeySetCalibrator::FTriangleKeySetCalibrator(FTriangleType in_fTriangleOriginType)
{
	switch (in_fTriangleOriginType)
	{
		case FTriangleType::WORLD: { fixedDimInterval = 32.0f; break;}
		case FTriangleType::BLUEPRINT: { fixedDimInterval = 4.0f; break;}
		case FTriangleType::ORE: { fixedDimInterval = 1.0f; break;}
	}
}

void FTriangleKeySetCalibrator::initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, DoublePoint in_pointA, DoublePoint in_pointB, DoublePoint in_pointC)
{
	startKey = in_startKey;
	endKey = in_endKey;
	pointA = in_pointA;
	pointB = in_pointB;
	pointC = in_pointC;
}

void FTriangleKeySetCalibrator::initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC)
{
	startKey = in_startKey;
	endKey = in_endKey;
	pointA = in_pointA;
	pointB = in_pointB;
	pointC = in_pointC;
}

void FTriangleKeySetCalibrator::calibrate(FKeyCalibrationMode in_calibrationMode)
{
	// find the normalized slopes we will be using.
	ECBPolyPoint normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	ECBPolyPoint normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);

	EnclaveKeyDef::EnclaveKey startKeyCopy = startKey;
	EnclaveKeyDef::EnclaveKey endKeyCopy = endKey;

	//std::cout << "(FTriangleKeySetCalibrator) -> Start key is: " << startKey.x << ", " << startKey.y << ", " << startKey.z << std::endl;
	//std::cout << "(FTriangleKeySetCalibrator) -> End key is: " << endKey.x << ", " << endKey.y << ", " << endKey.z << std::endl;

	isCalibratable(&startKey.x, normalizedSlopeA.x, pointA.x, pointB.x, pointC.x, startKeyCopy.x, endKeyCopy.x, in_calibrationMode);
	isCalibratable(&startKey.y, normalizedSlopeA.y, pointA.y, pointB.y, pointC.y, startKeyCopy.y, endKeyCopy.y, in_calibrationMode);
	isCalibratable(&startKey.z, normalizedSlopeA.z, pointA.z, pointB.z, pointC.z, startKeyCopy.z, endKeyCopy.z, in_calibrationMode);

	isCalibratable(&endKey.x, normalizedSlopeB.x, pointB.x, pointA.x, pointC.x, startKeyCopy.x, endKeyCopy.x, in_calibrationMode);
	isCalibratable(&endKey.y, normalizedSlopeB.y, pointB.y, pointA.y, pointC.y, startKeyCopy.y, endKeyCopy.y, in_calibrationMode);
	isCalibratable(&endKey.z, normalizedSlopeB.z, pointB.z, pointA.z, pointC.z, startKeyCopy.z, endKeyCopy.z, in_calibrationMode);

	//std::cout << "(FTriangleKeySetCalibrator) -> (NEW) Start key is: " << startKey.x << ", " << startKey.y << ", " << startKey.z << std::endl;
	//std::cout << "(FTriangleKeySetCalibrator) -> (NEW) End key is: " << endKey.x << ", " << endKey.y << ", " << endKey.z << std::endl;
}

void FTriangleKeySetCalibrator::isCalibratable(int* in_blueprintLocationPtr,
	float in_normalizedSlopeValue,
	float in_pointAxyz,
	float in_pointBxyz,
	float in_pointCxyz,
	float in_keyAfloat,
	float in_keyBfloat,
	FKeyCalibrationMode in_calibrationMode)
{
	// Below: Always, regardless of calibration mode.
	// Rememeber, fixedDimInterval will be a value of 32.0f, 4.0f, or 1.0f.
	// 
	// CONDITION 1 would only occur when the following is met:
	//	A.) The selected dimension value (X, Y, or Z) of the analyzed point is perfectly aligned at some fixed interval./
	//	B.) The slope of pointB - pointA is -1
	//	C.) this condition is questionable, needs review.

	// CONDITION 1:
	float xyzModResult = float(fmod(in_pointAxyz, fixedDimInterval));
	if
	(
		(xyzModResult == 0)						// must be on a border point
		&&
		in_normalizedSlopeValue == -1.0f		// must be negative direction
		&&
		(in_keyAfloat != in_keyBfloat)			// don't bother calibrating if the points were originally in the same blueprint.
	)	// potential blueprint key calibration, if this is 0
	{
		*in_blueprintLocationPtr -= 1;
	}


	// CONDITION 2 should not apply to FTriangle line tracing, but can apply to ray-casting/scanning logic.
	// The reason is that FTriangleLine instances that are perfectly on a grid line, need to be counted towards both the positive and 
	// negative sides, whenever a class derived from FTriangleTracerBase -- such as FTriangleBlueprintTracer -- calls it's runLineTracing().
	// Normally, when calibrating for situations such as a blueprint's origin keys, we wouldn't do this, but we need to do it 
	// because of the logic required for FTriangle line tracing. 
	//
	// It is met under the following conditions:
	//	A.) The slope value for the selected dimension, between pointA and pointB is 0
	//	B.)	The slope for the selected dimension, between pointA and pointC, is negative.
	//
	// This means that it is impossible for CONDITION 1 and CONDITION 2 to both simulataneously exist.
	//
	// However, we only check for CONDITION 2 ray-casting/scanning logic. Because the FTriangleLine runs perfectly on a grid line, the perfectly aligned dimension(s)
	// (there can be up to 2, but 3 is impossible) need to be adjusted before scanning begins, prior to being used to determine the set of keys to use when FTriangleFracturerBase::getScanningIntervals() is called.


	// Below: Only do this check if we are calibrating for FTRIANGLE_SCANNER
	if (in_calibrationMode == FKeyCalibrationMode::FTRIANGLE_SCANNER)
	{
		if
		(
			(xyzModResult == 0)
			&&
			in_normalizedSlopeValue == 0.0f
			//&&
			//(in_keyAfloat != in_keyBfloat)
		)
		{
			float direction = IndependentUtils::findNormalizedDirection(in_pointAxyz, in_pointCxyz);
			if (direction == -1.0f)
			{
				*in_blueprintLocationPtr -= 1;
			}
		}
	}
}

EnclaveKeyPair FTriangleKeySetCalibrator::getBeginAndEndKeys()
{
	EnclaveKeyPair returnPair;
	returnPair.keyA = startKey;
	returnPair.keyB = endKey;
	return returnPair;
}