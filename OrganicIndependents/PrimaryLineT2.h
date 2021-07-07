#pragma once

#ifndef PRIMARYLINET2_H
#define PRIMARYLINET2_H

#include "ECBPolyPointLocation.h"
#include "ECBPolyLineSlopesArray.h"
#include "ECBPolyPoint.h"
#include <iostream>

class IndependentUtils;
class PrimaryLineT2
{
public:
	ECBPolyPoint beginPointRealXYZ;
	ECBPolyPoint endPointRealXYZ;
	ECBPolyPoint thirdPointRealXYZ;		// the "mostly" unused point necessary for clamping operations, etc
	ECBPolyPoint x_int_slope;
	ECBPolyPoint y_int_slope;
	ECBPolyPoint z_int_slope;
	ECBPolyPoint intendedFaces;			// intended faces of this line
	ECBPolyLineSlopes int_slopes;
	ECBPolyPointLocation beginPointMeta;
	ECBPolyPointLocation endPointMeta;
	short IDofParentLine = 0;
	short perfectClampValue = 0;			// the perfect clamp value
	int trueLineID = 0;
	void calibrate(ECBPolyPoint in_pointC);
	void isCalibrateable(float* in_remainderPtr, int* in_blockLocationPtr, int* in_chunkLocationPtr, float in_normalizedSlopeValue, float in_pointAxyz, float in_pointBxyz, float in_pointCxyz);
	void applySlopes(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope);
	void applySlopes(ECBPolyPoint in_originalNormalizedSlope, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);
};

#endif
