#pragma once

#ifndef ENCLAVELINERUNNER_H
#define ENCLAVELINERUNNER_H

#include <iomanip>
#include "PrimaryLineT1.h"
#include "PrimaryLineT2.h"
#include "IndependentUtils.h"
#include "EnclaveBlock.h"
#include "BlockBorderLineList.h"
#include "PolyLineEndpointMeta.h"
#include "ECBPolyPoint.h"
#include "ECBPolyPointTri.h"
#include "LineRunnerState.h"

#endif

class EnclaveLineRunner
{
public:
	EnclaveLineRunner(PrimaryLineT1 in_link, BlockBorderLineList* in_lineListRef, int in_lineID);
	short isRunSingleIteration = 0;			// flag for when both points exist in same block
	short lineID = 0;						// the line this runner is running on
	short isRunComplete = 0;
	short currentBlock = 0;					// the current block we are on (always starts at 0)
	int debugFlag = 0;
	LineRunnerState runState = LineRunnerState::VALID;		// 99.9999% of the time, this is going to be VALID,  until that .0000000001 is found thats INVALID

	PrimaryLineT1 lineMeta;
	ECBPolyPoint lineSlope;						// the actual slope
	BlockBorderLineList* blockBordersRef;			// assigned in constructor
	ECBPolyPoint currentIterationBeginPoint;	// the begin point of the current iteration
	ECBPolyPoint currentIterationEndPoint;		// the end point of the current iteration
	PolyLineEndpointMeta currentEndpointMeta;	// will store the currently stored endpoint metadata from the latest iteration
	ECBPolyPointLocation currentIterationMeta;	// meta data for the current iteration
	ECBPolyPoint slopeDirection;			// normalized slope, for positive/negative x/y/z
	void checkForCompletion();				// check to see if the run is complete
	void iterateToNextBlock();				// iterates the line through a block; calls moveIntoNextBlock and other functions
	void moveIntoNextBlock();				// moves the line into the next block
	void checkDimensionForMove(float* in_moveDirection, int* in_blockKey, int* in_chunkKey);
	int checkIfBlockHasTriangleCorner(unsigned char* in_charRef);
	PolyLineEndpointMeta getEndpointData();
	PolyLineEndpointMeta getEndpointDataDebug();
	void checkEnclaveKeyValidity(EnclaveKeyDef::EnclaveKey in_enclaveKey);		// Added for F-006 (PolyLineRunner fails to reach its appropriate enclave key and block key)
	//PolyLineEndpointMeta calculatedEndpointData(ECBPolyPoint in_originPoint, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords);	// calculates the return values for getEndPointData (this function is separated from that one, for ease of reading)
};