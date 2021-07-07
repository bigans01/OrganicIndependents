#pragma once

#ifndef PRIMARYLINET2ISOLATEDTRACER_H
#define PRIMARYLINET2ISOLATEDTRACER_H

#include "ECBPolyPoint.h"
#include "ECBPPOrientations.h"
#include "ECBPPOrientationResults.h"
#include "BorderDataMap.h"
#include "ECBPolyLineSlopesArray.h"
#include "PrimaryLineT2Array.h"
#include "PrimaryLineT2.h"
#include "EnclaveKeyDef.h"
#include "PLTracingResult.h"
#include "ECBBorderLineList.h"

class IndependentUtils;
class PrimaryLineT2IsolatedTracer
{
public:
	PrimaryLineT2IsolatedTracer(int in_lineID,
		EnclaveKeyDef::EnclaveKey in_blueprintKey,
		ECBPPOrientationResults in_beginOrientation,
		ECBPPOrientationResults in_endOrientation,
		ECBPolyPoint in_beginPoint,
		ECBPolyPoint in_endPoint,
		ECBPolyPoint in_thirdPoint,
		ECBBorderLineList* in_borderLineList,
		ECBPolyLineSlopesArray* in_polyLineSlopesArray,
		BorderDataMap* in_borderDataMapRef,
		ECBPolyPoint in_intendedFaces,
		PrimaryLineT2Array* in_primaryLineT2ArrayRef,
		int in_perfectClampFlag,
		int in_x_clamp,
		int in_y_clamp,
		int in_z_clamp
	);
	ECBPPOrientationResults rootBeginOrientation; // the very initial begin point; always the same
	ECBPPOrientationResults currentBeginOrientation; // the orientations of the current begin point
	ECBPPOrientationResults currentEndOrientation; // the orientations of the current end point
	ECBPolyPoint x_int_slope;
	ECBPolyPoint y_int_slope;
	ECBPolyPoint z_int_slope;
	ECBPolyPoint currentBeginPoint;
	ECBPolyPoint currentEndPoint;
	ECBPolyPoint currentThirdPoint;			// for clamping points
	ECBPolyPoint rootBeginPoint;
	ECBPolyPoint currentMoveVals;			// the movevals for the begin point, of the current turn
	ECBPolyPoint intendedFaces;				// the intended faces for x/y/z planar intercepts
	ECBPolyPoint lineSlope;
	ECBBorderLineList* blueprintBorderListRef;
	PrimaryLineT2Array* primaryLineT2ArrayRef;
	BorderMDFaceList rootBeginFaceList;
	BorderMDFaceList rootEndFaceList;
	BorderMDFaceList usedFaceList;		// stores the faces that actually had a line travel on them per turn
	BorderDataMap* borderDataMapRef;		// reference to the border data map
	short faceMatchFlag = 0;
	short perfectClampValue = 0;
	short parentLineID = 0;
	short matchRequiredCount = 1;		// 1 is the default; 2 would be during a perfect clamp on the blueprint border
	EnclaveKeyDef::EnclaveKey moveVals;
	EnclaveKeyDef::EnclaveKey blueprintKey;
	bool isRunValid = true;				// assume true 99.99% of the time

	void determineInitialTrace(int in_perfectClampFlag);			// initial trace function
	void determineTrace();
	void determineFinalTrace();										// for setting up the final trace value
	ECBPolyPoint getInterceptToUseFromLine(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2);
	ECBPolyPoint getInterceptToUseFromCorner(ECBPolyPoint in_moveValues);
	ECBPolyPoint getInterceptToUseFromCorner(ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope);
	ECBPolyPoint getSlopeToUse(ECBPPOrientations in_interceptType);
	ECBPolyPoint getSlopeToUse(ECBPPOrientations in_interceptType, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope);
	PLTracingResult getBlueprintTracingResult(ECBPolyPoint in_interceptToUse);
};

#endif