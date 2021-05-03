#pragma once

#ifndef INDEPENDENTUTILS_H
#define INDEPENDENTUTILS_H

#include "ECBPPOrientationResults.h"
#include "ECBPolyPoint.h"
#include "BlockBorderLineList.h"

class IndependentUtils
{
	public:
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList in_blockBorders);	// default version of GetPointOrientation
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList* in_blockBorders);	// version of GetPointOrientation which takes a pointer instead of a copy of in_blockBorders
		static ECBPolyPoint roundPolyPointToHundredths(ECBPolyPoint in_pointToCheck);
		static float roundToHundredth(float in_float);
};

#endif
