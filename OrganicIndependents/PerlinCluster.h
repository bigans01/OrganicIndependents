#pragma once

#ifndef PERLINCLUSTER_H
#define PERLINCLUSTER_H

#include "PerlinClusterMeta.h"
#include "NoiseGridTile.h"
#include "Enclave2DKeyMapHasher.h"

/* PerlinCluster: a PerlinCluster contains metadata about a scanned perlin mass, resulting from generated PerlinClusterMeta objects 
found in a call to NoiseGridScanner::start.

The PerlinCluster should have the ability to determine what is known as the perlin mass hash that uniquely identifies the mass.
This value should be able to be generated immediately within this class after it's second constructor below. This unique hash
value can then be applied to or checked against multiple sector files, to update them or determine if the PerlinCluster 
already exists in memory (assuming its mapped correctly).

*/

class PerlinCluster
{
	public:
		PerlinCluster() {};
		PerlinCluster(PerlinClusterMeta in_metaInfo,
			          std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> in_perlinClusterTiles) :
			metaInfo(in_metaInfo),
			perlinClusterTiles(in_perlinClusterTiles)
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

		bool isClusterValid();

		std::string produceHash();

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> perlinClusterTiles;
		PerlinClusterMeta metaInfo;
};

#endif
