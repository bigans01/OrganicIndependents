#pragma once

#ifndef CALIBRATEABLEOREKEYPAIR_H
#define CALIBRATEABLEOREKEYPAIR_H

#include "EnclaveKeyPair.h"

/*

Description:

Given two point/key links (ie., pointA/startKey and pointB/endKey), and a third point -- all points of which fall in a valid local BLUEPRINT coordinate that has a range from 0.0 to 32.0f --
this class is designed to edit/adjust the copied values so that any points that lie directly on a grid line/corner are calibrated appropriately.

*/

class CalibrateableOREKeyPair
{
	public:
		EnclaveKeyDef::EnclaveKey startKey;
		EnclaveKeyDef::EnclaveKey endKey;
		DoublePoint pointA;
		DoublePoint pointB;
		DoublePoint pointC;

		void initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, DoublePoint in_pointA, DoublePoint in_pointB, DoublePoint in_pointC);
		void initialize(EnclaveKeyDef::EnclaveKey in_startKey, EnclaveKeyDef::EnclaveKey in_endKey, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);
		void calibrate();
		EnclaveKeyPair getBeginAndEndKeys();
	private:
		void isCalibratable(int* in_blueprintLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz, float in_keyAfloat, float in_keyBfloat);
};

#endif