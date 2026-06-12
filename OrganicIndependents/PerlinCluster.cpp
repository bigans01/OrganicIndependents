#include "stdafx.h"
#include "PerlinCluster.h"

std::string PerlinCluster::produceHash()
{
	Enlave2DKeyMapHasher hasher;
	hasher.processMap(&perlinClusterTiles);
	return hasher.getMapHash();
}

bool PerlinCluster::isClusterValid()
{
	bool clusterValid = false;
	if (!perlinClusterTiles.empty())
	{
		clusterValid = true;
	}
	return clusterValid;
}