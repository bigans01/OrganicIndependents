#pragma once

#ifndef SCIRCUITSEGMENT_H
#define SCIRCUITSEGMENT_H

#include "PrimarySegmentMeta.h"
#include "CircuitSegmentType.h"
#include "ECBPolyPoint.h"
#include "MicroPolyPointContainer.h"
#include "ECBPPOrientationResults.h"
#include "EnclaveKeyDef.h"
#include "BorderMDFaceList.h"
#include "PolyLogger.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include <string>

class IndependentUtils;
class SCircuitSegment
{
public:
	PrimarySegmentMeta segment;
	int indexOfNextSegment = 0;
	CircuitSegmentType type = CircuitSegmentType::NOVAL;
	ECBPolyPoint targetPoint;
	short perfectClampValue = 0;		// set by the SCircuit
	short debugFlag = 0;

	BlockBorderLineList* blockBorderLineListRef;
	BorderDataMap* borderDataMapRef;
	void setAsStandaloneSegment();
	void determineInitialType();
	void setDebugMode(PolyDebugLevel in_debugLevel);
	MicroPolyPointContainer getCircuitPointsV1(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey, std::string in_optionalString);
	MicroPolyPointContainer getCircuitPointsDebug(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
	MicroPolyPointContainer getCircuitPoints(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, std::string in_optionalString);
	int faceMatchThreshold = 1;		// for face list matching

private:

	// stand alone function
	MicroPolyPointContainer generatePointsForStandalone();

	// closed functions
	MicroPolyPointContainer generatePointsForClosedFirst();
	MicroPolyPointContainer generatePointsForClosed();
	MicroPolyPointContainer generatePointsForClosedLast();

	// open_single functions
	MicroPolyPointContainer generatePointsForSingleFirst();
	MicroPolyPointContainer generatePointsForSingle();
	MicroPolyPointContainer generatePointsForSingleLast();

	// open_multi functions
	MicroPolyPointContainer generatePointsForMultiFirst();
	MicroPolyPointContainer generatePointsForMulti();
	MicroPolyPointContainer generatePointsForMultiLast();

	// fetch the resulting segments
	MicroPolyPointContainer getNewSegments(PrimarySegmentMeta in_segmentMeta);
	PolyLogger segmentLogger;
	PolyDebugLevel segmentLogMode = PolyDebugLevel::NONE;

	// for debugging
	EnclaveKeyDef::EnclaveKey enclaveKey;
	EnclaveKeyDef::EnclaveKey blockKey;
	ECBPolyPoint triangle_point0;
	ECBPolyPoint triangle_point1;
	ECBPolyPoint triangle_point2;
	std::string callingString = "";				// a string that may be optionally specified to indicate something that called this function.


};

#endif
