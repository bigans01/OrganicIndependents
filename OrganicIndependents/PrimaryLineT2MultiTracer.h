#pragma once

#ifndef PRIMARYLINET2MULTITRACER_H
#define PRIMARYLINET2MULTITRACER_H

#include "ECBBorderLineList.h"
#include "ECBPPOrientations.h"
#include "ECBPPOrientationResults.h"
#include "BorderDataMap.h"
#include "BorderMDFaceList.h"
#include "ECBPolyLineSlopesArray.h"
#include "PrimaryLineT1.h"
#include "PrimaryLineT2Array.h"
#include "PrimaryLineT2.h"
#include "PrimaryLineT1LinkArray.h"
#include "PLTracingResult.h"
#include "SecondaryLineType.h"

class PrimaryLineT2MultiTracer
{
public:
	PrimaryLineT2MultiTracer(PrimaryLineT2Array* in_PLT2ArrayRef, 
							ECBBorderLineList* in_blueprintBorderLines, 
							BorderDataMap* in_borderDataMapRef, 
							EnclaveKeyDef::EnclaveKey in_blueprintKey, 
							PerfectClampEnum in_perfectClampFlag);	// constructor
	PrimaryLineT2Array* PLT2ArrayRef;			// a reference to the T2 array that will be modified
	ECBBorderLineList* blueprintBorderLinesRef;
	PrimaryLineT1LinkArray primaryLinkArray;	// for storing the primary links
	BorderDataMap* borderDataMapRef;			// pointer to the border data map
	void beginRun();	// begins the run; called after primaryLinkArray.setNextLinks() is called
	int faceMatchCount = 1;		// number of faces that match for a run to complete. Default is 1, but should be 2 for a perfect clamp condition.
	PerfectClampEnum currentPerfectClampValue = PerfectClampEnum::NONE;	// perfect clamp value, of the current PT2 we are on (doesn't really matter, it will be the same for all 3 lines; look into this later)
	EnclaveKeyDef::EnclaveKey moveVals;			// movevals for current PT2 line 
	EnclaveKeyDef::EnclaveKey blueprintKey;		// blueprint key for calculating borders for new PT2 lines
	ECBPPOrientationResults nextPrimaryBeginOrientation; // the very initial begin point; always the same
	ECBPPOrientationResults currentPrimaryBeginOrientation; // the orientations of the current begin point
	ECBPPOrientationResults currentPrimaryEndOrientation; // the orientations of the current end point
	ECBPolyPoint x_int_slope;
	ECBPolyPoint y_int_slope;
	ECBPolyPoint z_int_slope;
	ECBPolyPoint currentPointA;
	ECBPolyPoint currentPointB;
	ECBPolyPoint currentPointC;			// for clamping points
	ECBPolyPoint recentIntendedFaces;	// intended faces from the current line.
	int debugFlag = 0;			// for debugging purposes
	int wasBSet = 0;			// indicates if B has been set
	SecondaryLineType currentTracingMethod = SecondaryLineType::SEGMENT;	// the tracing method of the current line; should always be reset back to SEGMENT when iterating to a new primary T2 line
	int checkIfPointsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
	int checkIfFaceListsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
	//int OrganicUtils::checkIfFaceListsMatch(BorderMDFaceList in_faceListA, BorderMDFaceList in_faceListB, int in_numberOfMatches)
	void createPT2LineViaSegmentEnd();
	void createFirstOfMultiplePT2Lines();		// creates the first line in a set of multiple T2 lines
	void createNextOfMultiplePT2Lines();
	void createLastOfMultiplePT2Lines();
	void getEndpoint();
	PLTracingResult getBlueprintTracingResult(ECBPolyPoint in_interceptToUse);

	void calculateFaceMatchCount();
};

#endif