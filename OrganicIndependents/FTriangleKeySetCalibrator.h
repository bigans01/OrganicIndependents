#pragma once

#ifndef FTRIANGLEKEYSETCALIBRATOR_H
#define FTRIANGLEKEYSETCALIBRATOR_H

#include "EnclaveKeyPair.h"
#include "FKeyCalibrationMode.h"
#include "FTriangleType.h"

/*

Description:

This class is meant to calibrate EnclaveKeyDef::EnclaveKeys, based on two factors A.) the type of FracturingMachine calling the operation, and
B.) the FKeyCalibrationMode, which is of either FTRIANGLE_LINE or FTRIANGLE_SCANNER

*/

class FTriangleKeySetCalibrator
{
	public:
		FTriangleKeySetCalibrator(FTriangleType in_fTriangleOriginType);

		EnclaveKeyDef::EnclaveKey startKey;
		EnclaveKeyDef::EnclaveKey endKey;
		DoublePoint pointA;
		DoublePoint pointB;
		DoublePoint pointC;

		void initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, DoublePoint in_pointA, DoublePoint in_pointB, DoublePoint in_pointC);
		void initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);
		void calibrate(FKeyCalibrationMode in_calibrationMode);
		EnclaveKeyPair getBeginAndEndKeys();

	private:

		float fixedDimInterval = 0.0f;	// must be determined by constructor, based on the value of the input paramater that references the origin FTriangleType. 
										// WORLD = 32.0f, BLUEPRINT = 4.0f, ORE = 1.0f.

		void isCalibratable(int* in_blueprintLocationPtr,
			float in_normalizedSlopeValue,
			float in_pointAxyz,
			float in_pointBxyz,
			float in_pointCxyz,
			float in_keyAfloat,
			float in_keyBfloat,
			FKeyCalibrationMode in_calibrationMode);
};

#endif