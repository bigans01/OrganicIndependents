#pragma once

#ifndef ORGANICWRAPPEDBBFAN_H
#define ORGANICWRAPPEDBBFAN_H

//#include "BBFan.h"
#include "EnclaveBlockVertex.h"
#include "PrimarySegmentTracker.h"
#include "BlockCircuit.h"
//#include "OrganicUtils.h"
#include "IndependentUtils.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include "PolyRunDirection.h"
#include "SegmentResult.h"
#include "FatFan.h"

class OrganicWrappedBBFan
{
public:
	int keyID;
	PrimarySegmentTracker blockSegmentTracker;
	PrimarySegmentTracker reverseSegmentTracker;
	//BBFan poly;
	FatFan poly;
	EnclaveBlockVertex vertices[8];

	void buildBBFan(BlockCircuit* in_blockCircuitRef, short in_materialID, ECBPolyPoint in_emptyNormal);
	int checkIfRunIsValidForTwoSegments(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	SegmentResult checkIfRunIsValidForTwoSegmentsSpecial(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	int checkIfRunIsValidForTwoSegmentsViaCopy(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PrimarySegmentTracker in_trackerCopy, PolyRunDirection in_direction);
	int checkIfRunIsValidForTwoSegmentsReverse(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
};

#endif
