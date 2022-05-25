#pragma once

#ifndef POLYUTILS_H
#define POLYUTILS_H

#include "TriangleLine.h"
#include "PrimaryLineT1.h"
#include "PrimaryLineT2.h"
#include "EnclaveKeyDef.h"
#include "ECBPoly.h"
#include "ECBPolySkeleton.h"
#include "OrganicLineBase.h"

class IndependentUtils;
class PolyUtils
{
public:
	static void determineLineInterceptSlopes(OrganicLineBase* in_LinePtr, ECBPolyPoint in_thirdPoint);
	static void determineLineInterceptSlopesDebug(TriangleLine* in_LinePtr, ECBPolyPoint in_thirdPoint, int in_lineID);
	static bool determineLineInterceptSlopesDebugForEnclaveTriangle(TriangleLine* in_LinePtr, ECBPolyPoint in_thirdPoint, ECBPolyPoint in_point0Original, ECBPolyPoint in_point1Original, ECBPolyPoint in_point2Original);
	static TriangleLine convertT1toSimpleTriangleLine(PrimaryLineT1 in_T1Line);
	static TriangleLine convertT2toSimpleTriangleLine(PrimaryLineT2 in_T1Line);
	static ECBPolyLine convertToECBPolyLine(TriangleLine in_line, ECBPolyPoint in_thirdPoint);
	static int convertBlockCoordsToSingle(int in_x, int in_y, int in_z);
	static int convertBlockCoordsToSingle(EnclaveKeyDef::EnclaveKey);
	static EnclaveKeyDef::EnclaveKey convertSingleToBlockKey(int in_value);
	static ECBPoly produceECBPolyFromECBPolySkeleton(ECBPolySkeleton in_skeleton);
	static ECBPoly produceTranslatedECBPoly(ECBPoly in_originalPoly, EnclaveKeyDef::EnclaveKey in_originalBlueprintKey);
};

#endif