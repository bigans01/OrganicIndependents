#pragma once

#ifndef BLOCKCIRCUIT_H
#define BLOCKCIRCUIT_H

#include "SCircuitSegment.h"
#include "PrimarySegmentMeta.h"
#include "ECBPPOrientations.h"
#include "ECBPPOrientationResults.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include "MicroPolyPointContainer.h"
#include "TertiaryTriangleContainer.h"
#include "PolyLogger.h"

class IndependentUtils;
class BlockCircuit
{
public:
	BlockCircuit() {};
	BlockCircuit(BlockBorderLineList* in_blockBorderLineListRef, 
				BorderDataMap* in_borderDataMapRef, 
				PerfectClampEnum in_perfectClampValue, 
				PolyDebugLevel in_debugLevel);

	SCircuitSegment segmentArray[4];	// stores up to for segments
	short currentSegmentIndex = 0;
	short currentSegmentCount = 0;
	int currentMaxIndex = 0;

	bool isCircuitValid = true;			// 99.9999% of the time, this should be true; if there's no way to figure out the circuit (i.e, produces invalid triangles), set to false.

	void insertNewSegment(PrimarySegmentMeta in_primarySegment);
	bool fillSegmentArrayMetaDataWithPointsV2(int in_faceMatchThreshold, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2);
	bool fillSegmentArrayMetaDataWithPointsV1(int in_faceMatchThreshold, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, std::string in_optionalString);	// debugging only
	bool fillSegmentArrayMetaDataFromCircuit(int in_faceMatchThreshold, EnclaveKeyDef::EnclaveKey in_enclaveKey, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2);
	void printSegmentMetaData();
	TertiaryTriangleContainer produceAndReturnTertiaryTriangles(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
	MicroPolyPointContainer finalCircuitPoints;

	bool checkForDuplicatePoints(MicroPolyPointContainer* in_containerRef);	// temporarily added on 8/7/2022; may be removed after coming up with a solution.

private:
	PerfectClampEnum perfectClampValue = PerfectClampEnum::NONE;
	PolyLogger logger;
	PolyDebugLevel debugLevel = PolyDebugLevel::NONE;

	void calculateSegmentLinks(int in_faceMatchThreshold);
	void determineFinalTypeForFirstSegment();
	void determineFinalTypeForLastSegment();
	bool gatherCircuitPointsWithString(std::string in_optionalString, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2);
	void gatherCircuitPointsWithStringV2(std::string in_optionalString, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2);	// should be used for debugging only
	BorderDataMap* borderDataMapRef;
	BlockBorderLineList* blockBorderLineListRef;


	// for debugging
	EnclaveKeyDef::EnclaveKey circuitEnclaveKey;
	EnclaveKeyDef::EnclaveKey circuitBlockKey;
};

#endif
