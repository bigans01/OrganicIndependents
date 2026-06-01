#pragma once

#ifndef PERLINCLUSTER_H
#define PERLINCLUSTER_H

#include "PerlinClusterMeta.h"

/* PerlinCluster: a PerlinCluster contains metadata about a scanned perlin mass, resulting from generated PerlinClusterMeta objects 
found in a call to NoiseGridScanner::start.

The PerlinCluster should have the ability to determine what is known as the perlin mass hash that uniquely identifies the mass.
This value should be able to be generated immediately within this class after it's second constructor below. This unique hash
value can then be applied to or checked against multiple sector files, to update them or determine if the PerlinCluster 
already exists in memory (assuming its mapped correctly).

*/

class NoiseGrid;
class PerlinCluster
{
	public:
		PerlinCluster() {};
		PerlinCluster(NoiseGrid* in_noiseGridPtr, PerlinClusterMeta in_metaInfo) :
			noiseGridPtr(in_noiseGridPtr),
			metaInfo(in_metaInfo)
		{};

		void printPerlinClusterMeta()
		{
			auto fetchedMeta = metaInfo.fetchMetaMap();
			for (auto& currentMetaEntry : fetchedMeta)
			{
				EnclaveKeyDef::Enclave2DKey currentSectorKey = currentMetaEntry.first;
				currentSectorKey.printKey();
				std::cout << " --> ";
				currentMetaEntry.second.printEntry();
			}
		}

		PerlinClusterOriginSearch runOriginSearch()
		{
			return metaInfo.searchForOriginKey();
		}

		std::string produceHash()
		{
			std::string returnHash = "";

			// The process goes as this:
			// 1. get each key from a call to metaInfo.fetchedMetaMap().
			// 
			// 2. go to the sector of each key, and use the NGSClusterEntry value of the current key being looked at,
			//  to determine which grouping ids to look at.
			// 
			// 3. copy the data of each grouping (would come from tileGroupings member of NoiseGridSector) into a single
			//  unordered_map.
			// 
			// 4. call Enclave2DKeyMapHasher on this new map. Verify that the number of keys used in hash calculation equals to the total
			//    sum of all keys from all tile groupings. If they don't match, there's a potential hash collision.


			return returnHash;
		}

	private:
		NoiseGrid* noiseGridPtr = nullptr;
		PerlinClusterMeta metaInfo;
};

#endif
