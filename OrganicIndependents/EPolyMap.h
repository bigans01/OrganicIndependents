#pragma once

#ifndef EPOLYMAP
#define EPOLYMAP

#include "EnclavePoly.h"
#include "PrimaryLineT1.h"
#include "PrimaryLineT1Array.h"
#include "EnclaveBorderLineList.h"
#include "PrimarySegmentMeta.h"
#include "ECBPPOrientationResults.h"
#include "BorderDataMap.h"
#include "BorderMDFaceList.h"
#include "BlockBorderLineList.h"
#include "PolyDebugLevel.h"

class EPolyMap
{
public:
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclavePoly, EnclaveKeyDef::KeyHasher> enclavePolyMap;
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> exteriorTracedEnclaves;
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> allTracedEnclaves;
	PolyDebugLevel debugLevel = PolyDebugLevel::NONE;
	BlockBorderLineList blockBorderList;

	PrimarySegmentMeta buildPrimarySegment(int in_lineID, ECBPolyPointPair in_pair, PrimaryLineT1Array* in_t1ArrayRef, EnclaveBorderLineList* in_enclaveBorderLineListRef, BorderDataMap* in_borderDataMapRef);		// builds by using a pointer to a PrimaryLineT1Array
	PrimarySegmentMeta buildPrimarySegment(int in_lineID, ECBPolyPointPair in_pair, PrimaryLineT1* in_primaryLineRef, EnclaveBorderLineList* in_enclaveBorderLineListRef, BorderDataMap* in_borderDataMapRef);		// builds by using a pointer to a PrimaryLine
	EnclavePoly* getEnclavePolyRef(EnclaveKeyDef::EnclaveKey in_key);
};

#endif
