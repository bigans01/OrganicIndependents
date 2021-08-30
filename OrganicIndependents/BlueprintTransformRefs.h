#pragma once

#ifndef BLUEPRINTTRANSFORMREFS_H
#define BLUEPRINTTRANSFORMREFS_H

#include "ECBPoly.h"
#include <map>
#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "OrganicRawEnclave.h"
#include "PolyGroupRange.h"

class BlueprintTransformRefs
{
public:
	BlueprintTransformRefs(std::map<int, ECBPoly>::iterator in_polyMapBeginIter,
		std::map<int, ECBPoly>::iterator in_polyMapEndIter,
		int in_polyMapSize,
		std::unordered_map<EnclaveKeyDef::EnclaveKey, OrganicRawEnclave, EnclaveKeyDef::KeyHasher>* in_organicRawManifestRef,
		std::map<int, PolyGroupRange>* in_groupRangeRef)
		: polyMapBeginIter(in_polyMapBeginIter),
    	  polyMapEndIter(in_polyMapEndIter),
		  polyMapSize(in_polyMapSize),
		  organicRawManifestRef(in_organicRawManifestRef),
		  groupRangeRef(in_groupRangeRef) 
	{};

	//std::map<int, ECBPoly>* polyMapRef;																						// a ref to the ECBPoly map contained in the blueprint we are reading/writing

	std::map<int, ECBPoly>::iterator polyMapBeginIter;
	std::map<int, ECBPoly>::iterator polyMapEndIter;
	int polyMapSize = 0;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, OrganicRawEnclave, EnclaveKeyDef::KeyHasher>* organicRawManifestRef;		// same, but for OrganicRawManifests ""
	std::map<int, PolyGroupRange>* groupRangeRef;																			// same, but for PolyGroupRanges
};

#endif
