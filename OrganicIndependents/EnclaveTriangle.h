#pragma once

#ifndef ENCLAVETRIANGLE_H
#define ENCLAVETRIANGLE_H

#include <unordered_set>
#include "ECBPolyLine.h"
#include "OrganicWrappedBBFan.h"
#include "PolyRunDirection.h"
#include "PrimaryLineT1Array.h"
#include "PrimarySegmentMeta.h"
#include "BorderDataMap.h"
#include "BlockBorderLineList.h"
#include "BlockCircuit.h"
#include "EnclaveLineRunner.h"
#include "PolyUtils.h"
#include "IndependentUtils.h"
#include "SecondarySearchResult.h"
#include "OrganicTriangleTertiary.h"
#include "EnclaveTriangleInteriorRunner.h"
#include "TerminatingSetContainer.h"
#include "SegmentResult.h"
#include "IndependentUtils.h"

class EnclaveTriangle
{
public:

	ECBPolyLine lineArray[3];
	short materialID = 0;			// is the triangle dirt, stone, snow, wood, etc
	short isEnclaveTrianglePolyPerfectlyClamped = 0;					// determines if the entire polygon is perfectly clamped to x, y, or z azis
	//ECBPolyPoint mrp;				// probably not needed, but comment out just in case.
	ECBPolyPoint emptyNormal;
	bool isTriangleValid = true;

	//BlockBorderLineList lineList;
	//BorderDataMap borderDataMap;

	// tracing maps
	OrganicTriangleTertiary secondaryMap;		// stores the actual secondary triangle data that is produced.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> exteriorTracedBlocks;	// a set to register exterior-traced blocks.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> allTracedBlocks;		// " " for all traced blocks (including exteriors)
	TerminatingSetContainer forwardTerminatingContainer;
	TerminatingSetContainer reverseTerminatingContainer;

	void executeRun(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	EnclaveKeyDef::EnclaveKey currentEnclaveKey;

	// functions for ensuring the triangle will run OK
	void executeForwardRunTest(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	void executeReverseRunTest(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);

	// reform the triangle, for adhesion
	void reform(ECBPolyPoint in_polyPoint0, ECBPolyPoint in_polyPoint1, ECBPolyPoint in_polyPoint2);

	void runPolyLinesThroughEnclave(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);	// run the exterior lines.
	void runPolyLinesThroughEnclaveReverse(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);	// run the exterior lines.
	void runPolyLinesForReverseTerminatingContainer(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	void runInteriorRunners(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PolyRunDirection in_polyRunDirection);
	void runInteriorRunnerReverse(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PolyRunDirection in_polyRunDirection);
	void generateExteriorLineSegments(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);									// fill the SegmentTrackers for the outer lines
	void performCentroidBlockCheck(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);

	// the below may be flagged for removal later
	SecondarySearchResult checkIfSecondaryExists(EnclaveKeyDef::EnclaveKey in_key);
	void insertNewSecondary(EnclaveKeyDef::EnclaveKey in_key);

	void fillCircuitMetaData(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void fillCircuitMetaDataSpecialDebug(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void fillCircuitMetaDataV2(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void populateOrganicWrappedBBFanWithCircuitData(BlockCircuit* in_circuitRef, int in_secondaryID);
	int findIndexOfSegmentWithNonFreeEndpoint(BlockCircuit* in_circuitRef, EnclaveKeyDef::EnclaveKey in_key);
	ECBPolyPoint getTraceResultForEndpointSegment(BlockCircuit* in_circuitRef, int in_segmentIndex, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	PrimaryLineT1Array generatePrimaryLineT1Array(PolyRunDirection in_polyRunDirection);
	void printExteriorBlocks();
	void printPoints();

	bool isSegmentAndSlopeParallel(SegmentMeta in_segment, ECBPolyPoint in_slope);
private:
	bool isEndpointValidForFreeSelection(int in_lineID, BlockCircuit* in_circuitRef);

	int attemptLimit = 16; // the number of times an EnclaveLineRunner may attempt its completion run before being halted, and the run has to begin again.

};

#endif
