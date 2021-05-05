#pragma once

#ifndef INDEPENDENTUTILS_H
#define INDEPENDENTUTILS_H

#include "ECBPPOrientationResults.h"
#include "ECBPolyPoint.h"
#include "BlockBorderLineList.h"
#include "EnclaveBorderLineList.h"
#include "EnclaveKeyDef.h"
#include "ECBBorderLineList.h"

class IndependentUtils
{
	public:
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList in_blockBorders);	// default version of GetPointOrientation
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList* in_blockBorders);	// version of GetPointOrientation which takes a pointer instead of a copy of in_blockBorders
		static ECBPPOrientationResults GetEnclavePointOrientation(ECBPolyPoint in_pointToCheck, EnclaveBorderLineList* in_enclaveBorderLineList);
		static ECBPPOrientationResults GetBlueprintPointOrientation(ECBPolyPoint in_pointToCheck, ECBBorderLineList* in_borderLineList);
		static ECBBorderLineList determineBorderLines(EnclaveKeyDef::EnclaveKey in_Key);
		static ECBPolyPoint roundPolyPointToHundredths(ECBPolyPoint in_pointToCheck);
		static float roundToHundredth(float in_float);
};

#endif
