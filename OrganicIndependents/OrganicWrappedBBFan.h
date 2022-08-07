#pragma once

#ifndef ORGANICWRAPPEDBBFAN_H
#define ORGANICWRAPPEDBBFAN_H

#include "EnclaveBlockVertex.h"
#include "PrimarySegmentTracker.h"
#include "BlockCircuit.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include "PolyRunDirection.h"
#include "SegmentResult.h"
#include "FatFan.h"

class IndependentUtils;
class OrganicWrappedBBFan
{
public:
	int keyID;
	PrimarySegmentTracker blockSegmentTracker;
	PrimarySegmentTracker reverseSegmentTracker;
	FatFan poly;
	EnclaveBlockVertex vertices[8];

	void buildBBFan(BlockCircuit* in_blockCircuitRef, short in_materialID, ECBPolyPoint in_emptyNormal);
	void buildBBFanWithBoundaryIndicator(BlockCircuit* in_blockCircuitRef, 
										 short in_materialID, 
										 ECBPolyPoint in_emptyNormal,
										 BoundaryPolyIndicator in_boundaryPolyIndicator);
	int checkIfRunIsValidForTwoSegments(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	bool eraseBadFans();	// clean up any triangles that are actually points or lines, and reform the appropriate involved members
							// (i.e., reform the vertices[] array and the poly.
	SegmentResult checkIfRunIsValidForTwoSegmentsSpecial(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	int checkIfRunIsValidForTwoSegmentsViaCopy(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PrimarySegmentTracker in_trackerCopy, PolyRunDirection in_direction);
	int checkIfRunIsValidForTwoSegmentsReverse(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
};

#endif
