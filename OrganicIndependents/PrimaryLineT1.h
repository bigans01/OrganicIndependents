#pragma once

#ifndef PRIMARYLINET1_H
#define PRIMARYLINET1_H

#include "ECBPolyPointLocation.h"

class IndependentUtils;
class PrimaryLineT1
{
public:
	int isLineActiveInPoly = 0;

	EnclaveKeyDef::EnclaveKey beginPointBlueprintKey;
	EnclaveKeyDef::EnclaveKey endPointBlueprintKey;

	ECBPolyPoint beginPointRealXYZ;
	ECBPolyPoint endPointRealXYZ;
	ECBPolyPoint thirdPointRealXYZ;		// the "mostly" unused point necessary for clamping operations, etc
	ECBPolyPoint x_int, y_int, z_int;
	ECBPolyPoint intendedFaces;			// intended faces of this line
	ECBPolyPoint switchIntended;
	ECBPolyPointLocation beginPointMeta;
	ECBPolyPointLocation endPointMeta;
	short x_clamp_value = 0;		// are the two points of the line clamped on the same x coordinate?
	short y_clamp_value = 0;		// "" y coord?
	short z_clamp_value = 0;		// "" z coord?
	char IDofLine = 100;			// ID of the line, 0 1 and 2 are reserved, so we can't initialize to those (100 chosen for no apparent reason)
	char IDofLinkedLine = 0;	// ID of the linked line (the line whose endpoint would be equal to this line's begin point, if that were to occur)
	short IDofNextLine = 0;		// ID of the next line in the sequence
	short perfectClampValue = 0;			// the perfect clamp value
	int doesLineHaveTwoPrimaryT1Points = 0;	// indicates if both the end and begin poinst are actually primary T1 points
	void calibrate(ECBPolyPoint in_pointC);

	void calibrateBlueprintKeys(ECBPolyPoint in_pointC);
	void isBlueprintKeyCalibrateable(int* in_blueprintLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz);

	void calibrateForBlueprintTracingWithInverseChecks(ECBPolyPoint in_pointC);
	void isCalibrateable(float* in_remainderPtr, int* in_blockLocationPtr, int* in_chunkLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz);
	ECBPolyPoint checkForSingleAxisClamping();
	ECBPolyPoint calibrateWithInverseChecks(ECBPolyPoint in_pointC);
	void applySlopesFromParentLine(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope);
};

#endif