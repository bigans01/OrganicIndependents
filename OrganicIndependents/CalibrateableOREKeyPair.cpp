#include "stdafx.h"
#include "CalibrateableOREKeyPair.h"

void CalibrateableOREKeyPair::initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, DoublePoint in_pointA, DoublePoint in_pointB, DoublePoint in_pointC)
{
	startKey = in_startKey;
	endKey = in_endKey;
	pointA = in_pointA;
	pointB = in_pointB;
	pointC = in_pointC;
}

void CalibrateableOREKeyPair::initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC)
{
	startKey = in_startKey;
	endKey = in_endKey;
	pointA = in_pointA;
	pointB = in_pointB;
	pointC = in_pointC;
}

void CalibrateableOREKeyPair::calibrate(FKeyCalibrationMode in_calibrationMode)
{
	// find the normalized slopes we will be using.
	ECBPolyPoint normalizedSlopeA = IndependentUtils::findNormalizedSlope(pointA, pointB);
	ECBPolyPoint normalizedSlopeB = IndependentUtils::findNormalizedSlope(pointB, pointA);

	EnclaveKeyDef::EnclaveKey startKeyCopy = startKey;
	EnclaveKeyDef::EnclaveKey endKeyCopy = endKey;

	std::cout << "Start key is: " << startKey.x << ", " << startKey.y << ", " << startKey.z << std::endl;
	std::cout << "End key is: " << endKey.x << ", " << endKey.y << ", " << endKey.z << std::endl;
	isCalibratable(&startKey.x, normalizedSlopeA.x, pointA.x, pointB.x, pointC.x, startKeyCopy.x, endKeyCopy.x, in_calibrationMode);
	isCalibratable(&startKey.y, normalizedSlopeA.y, pointA.y, pointB.y, pointC.y, startKeyCopy.y, endKeyCopy.y, in_calibrationMode);
	isCalibratable(&startKey.z, normalizedSlopeA.z, pointA.z, pointB.z, pointC.z, startKeyCopy.z, endKeyCopy.z, in_calibrationMode);


	//isCalibratable(&endKey.x, normalizedSlopeB.x, pointA.x, pointB.x, pointC.x);
	//isCalibratable(&endKey.y, normalizedSlopeB.y, pointA.y, pointB.y, pointC.y);
	//isCalibratable(&endKey.z, normalizedSlopeB.z, pointA.z, pointB.z, pointC.z);

	isCalibratable(&endKey.x, normalizedSlopeB.x, pointB.x, pointA.x, pointC.x, startKeyCopy.x, endKeyCopy.x, in_calibrationMode);
	isCalibratable(&endKey.y, normalizedSlopeB.y, pointB.y, pointA.y, pointC.y, startKeyCopy.y, endKeyCopy.y, in_calibrationMode);
	isCalibratable(&endKey.z, normalizedSlopeB.z, pointB.z, pointA.z, pointC.z, startKeyCopy.z, endKeyCopy.z, in_calibrationMode);

	std::cout << "(NEW) Start key is: " << startKey.x << ", " << startKey.y << ", " << startKey.z << std::endl;
	std::cout << "(NEW) End key is: " << endKey.x << ", " << endKey.y << ", " << endKey.z << std::endl;
}

void CalibrateableOREKeyPair::isCalibratable(int* in_blueprintLocationPtr, 
											float in_normalizedSlopeValue, 
											float in_pointAxyz, 
											float in_pointBxyz, 
											float in_pointCxyz, 
											float in_keyAfloat, 
											float in_keyBfloat,
											FKeyCalibrationMode in_calibrationMode)
{
	// Below: Always, regardless of calibration mode.
	float xyzModResult = float(fmod(in_pointAxyz, 4));
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

EnclaveKeyPair CalibrateableOREKeyPair::getBeginAndEndKeys()
{
	EnclaveKeyPair returnPair;
	returnPair.keyA = startKey;
	returnPair.keyB = endKey;
	return returnPair;
}