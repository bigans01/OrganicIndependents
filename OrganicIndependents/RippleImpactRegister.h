#pragma once

#ifndef RIPPLEIMPACTREGISTER_H
#define RIPPLEIMPACTREGISTER_H

#include "EnclaveKeyDef.h"
#include "EnclaveKeyPair.h"
#include <unordered_map>
#include <unordered_set>
#include "NeighboringBlockLocation.h"
#include <vector>

class RippleImpactRegister
{
	public:
		void insertImpactedBlock(EnclaveKeyDef::EnclaveKey in_blueprintKey,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blockKey)
		{
			impacts[in_blueprintKey].impactedOREs[in_oreKey].insertBlockKey(in_blockKey);
		}
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchImpactedBlueprints()
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnSet;
			/*
			auto impactedBlueprintsBegin = impacts.begin();
			auto impactedBlueprintsEnd = impacts.end();
			for (; impactedBlueprintsBegin != impactedBlueprintsEnd; impactedBlueprintsBegin++)
			{

			}
			*/
			for (auto& c : impacts)
			{
				returnSet.insert(c.first);
			}
			return returnSet;
		}

		std::vector<EnclaveKeyPair> fetchImpactedOREs()
		{
			std::vector<EnclaveKeyPair> returnVector;
			for (auto& currentBlueprint : impacts)
			{
				for (auto& currentORE : currentBlueprint.second.impactedOREs)
				{
					EnclaveKeyPair currentPair(currentBlueprint.first, currentORE.first);
					returnVector.push_back(currentPair);
				}
			}
			return returnVector;
		}

	private:
		struct RippleImpactedORE
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> impactedBlockKeys;
			void insertBlockKey(EnclaveKeyDef::EnclaveKey in_blockKey)
			{
				impactedBlockKeys.insert(in_blockKey);
			};
		};

		struct RippleImpactedBlueprint
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, RippleImpactedORE, EnclaveKeyDef::KeyHasher> impactedOREs;
		};

		std::unordered_map<EnclaveKeyDef::EnclaveKey, RippleImpactedBlueprint, EnclaveKeyDef::KeyHasher> impacts;
};

#endif
