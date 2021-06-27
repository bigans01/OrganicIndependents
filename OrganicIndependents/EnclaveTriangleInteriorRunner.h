#pragma once

#ifndef ENCLAVETRIANGLEINTERIORRUNNER_H
#define ENCLAVETRIANGLEINTERIORRUNNER_H

#include "ECBPolyPoint.h"
#include "PrimaryLineT1.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include "OrganicTriangleTertiary.h"
//#include "OrganicUtils.h"
#include "IndependentUtils.h"
#include "PolyUtils.h"
#include <unordered_set>

class EnclaveTriangleInteriorRunner
{
public:
	EnclaveTriangleInteriorRunner(ECBPolyPoint in_beginPoint,
		ECBPolyPoint in_endPoint,
		PrimaryLineT1 in_primaryLine,
		BlockBorderLineList* in_blockBorderLineListRef,
		BorderDataMap* in_borderDataMapRef,
		std::map<int, OrganicWrappedBBFan>* in_wrappedBBFanMapRef,
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* in_exteriorTracedBlocksRef,
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* in_allTracedBlocksRef,
		short in_materialID,
		short in_isPolyPerfectlyClamped,
		ECBPolyPoint emptyNormal,
		EnclaveKeyDef::EnclaveKey in_blockKey,
		int in_segmentCount,
		bool in_debugModeFlag
	);

	short isRunSingleIteration = 0;			// flag for when both points exist in same block
	short lineID = 0;						// the line this runner is running on
	bool isRunComplete = false;
	int debugFlag = 0;
	int specialCheckFlag = 0;				// this indicates when to do a special check
	PrimaryLineT1 lineMeta;
	ECBPolyPoint lineSlope;
	BlockBorderLineList* blockBorderLineListRef;
	BorderDataMap* borderDataMapRef;
	ECBPolyPoint currentIterationBeginPoint;	// the begin point of the current iteration
	ECBPolyPoint currentIterationEndPoint;		// the end point of the current iteration
	ECBPolyPoint normalizedPrimaryLineSlope;	// the normalized direction of the line slope
	bool debugModeFlag = false;

	short materialID;
	short isPolyPerfectlyClamped;
	ECBPolyPoint emptyNormal;
	int segmentCount = 0;						// determines special condition
	int traceCount = 0;

	// current block key the runner is in
	EnclaveKeyDef::EnclaveKey currentBlockKey;

	std::map<int, OrganicWrappedBBFan>* wrappedBBFanMapRef = NULL;
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* exteriorTracedBlocksMapRef = NULL;
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* allTracedBlocksMapRef = NULL;

	// point orientations
	ECBPPOrientationResults currentBeginOrientation;
	ECBPPOrientationResults currentEndOrientation;

	PolyLineEndpointMeta currentEndpointMeta;	// will store the currently stored endpoint metadata from the latest iteration
	ECBPolyPointLocation currentIterationMeta;	// meta data for the current iteration
	ECBPolyPoint slopeDirection;			// normalized slope, for positive/negative x/y/z
	ECBPolyPoint intendedFaces;
	ECBPolyPoint x_int_slope;
	ECBPolyPoint y_int_slope;
	ECBPolyPoint z_int_slope;

	ECBPolyPoint initialBeginPoint, initialEndPoint;	// for debugging
	EnclaveKeyDef::EnclaveKey initialBlockKey;
	EnclaveKeyDef::EnclaveKey cornerTerminatingKey;
	bool wasCornerDetermined = false;

private:
	void attemptIteration();
	void applyMovementToEnclaveKey(ECBPolyPoint in_moveVals);
	bool checkIfBBFanWasTraced(EnclaveKeyDef::EnclaveKey in_enclavePolyKey);
	void checkBlockKeyValidity();
	void performCornerCheck(ECBPolyPoint in_moveVals);

	void fillCircuitMetaData(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	int findIndexOfSegmentWithNonFreeEndpoint(BlockCircuit* in_circuitRef);
	ECBPolyPoint getTraceResultForEndpointSegment(BlockCircuit* in_circuitRef, int in_segmentIndex, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	void populateOrganicWrappedBBFanWithCircuitData(BlockCircuit* in_circuitRef, int in_secondaryID);
};

#endif
