#pragma once

#ifndef PERLINCLUSTERMETA_H
#define PERLINCLUSTERMETA_H

#include "NGSCScanTargetGrouping.h"
#include "NGSClusterEntry.h"

/*
* 
PerlinClusterOriginSearch: this class is used in an attempt to search for the origin key inside the PerlinClusterMeta class. A call
to wasOriginFound() shows true if successful.

PerlinClusterMeta: this class tracks all of the individual sectors, and groupings in those sectors, that a PerlinCluster touches.
The key of the metaMap tracks all sectors touched, and the corresponding value of each key (the NGSClusterEntry) tracks each grouping
per sector.

The individual origin/start sector that is part of the call to NoiseGridScanner::start can be found by cycling through each member
of the metaMap, and calling NGSClusterEntry::getGroupingStatus, until a value of NGSCGroupingStatus::GROUPING_USED. When this value 
is found, that corresponding key in metaMap represents the sector file to update/write to.
*/

class PerlinClusterOriginSearch
{
	public:
		PerlinClusterOriginSearch() {}
		PerlinClusterOriginSearch(EnclaveKeyDef::Enclave2DKey in_foundOriginKey) 
		{
			originKey = in_foundOriginKey;
			originFound = true;
		}

		bool wasOriginFound()
		{
			return originFound;
		}

		EnclaveKeyDef::Enclave2DKey getFoundOriginKey()
		{
			return originKey;
		}

	private:
		EnclaveKeyDef::Enclave2DKey originKey;
		bool originFound = false;
};

class PerlinClusterMeta
{
	public:
		PerlinClusterMeta() {};
		void insertGrouping(EnclaveKeyDef::Enclave2DKey in_sectorKey, NGSClusterEntry in_clusterEntry);
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NGSClusterEntry, EnclaveKeyDef::KeyHasher> fetchMetaMap();

		// Below: scan for and print the origin sector key where everything started for the PerlinCluster's scan.
		void findAndPrintScanOriginKey()
		{
			for (auto& currentClusterEntry : metaMap)
			{
				if (currentClusterEntry.second.getGroupingStatus() == NGSCGroupingStatus::GROUPING_USED)
				{
					EnclaveKeyDef::Enclave2DKey currentFoundKey = currentClusterEntry.first;
					std::cout << "!! Found scan origin key for this PerlinClusterMeta: ";
					currentFoundKey.printKey();
					std::cout << std::endl;
					break;
				}
			}
		}

		PerlinClusterOriginSearch searchForOriginKey()
		{
			PerlinClusterOriginSearch searchAttempt;

			for (auto& currentClusterEntry : metaMap)
			{
				if (currentClusterEntry.second.getGroupingStatus() == NGSCGroupingStatus::GROUPING_USED)
				{
					PerlinClusterOriginSearch foundOrigin(currentClusterEntry.first);
					searchAttempt = foundOrigin;
					break;
				}
			}

			return searchAttempt;
		}

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NGSClusterEntry, EnclaveKeyDef::KeyHasher> metaMap;
};

#endif
