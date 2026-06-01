#include "stdafx.h"
#include "PerlinClusterMeta.h"

void PerlinClusterMeta::insertGrouping(EnclaveKeyDef::Enclave2DKey in_sectorKey, NGSClusterEntry in_clusterEntry)
{
	metaMap[in_sectorKey] = in_clusterEntry;
}

std::unordered_map<EnclaveKeyDef::Enclave2DKey, NGSClusterEntry, EnclaveKeyDef::KeyHasher> PerlinClusterMeta::fetchMetaMap()
{
	return metaMap;
}