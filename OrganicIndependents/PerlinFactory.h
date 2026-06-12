#pragma once

#ifndef PERLINFACTORY_H
#define PERLINFACTORY_H

#include "Grid.h"
#include "PerlinClusterMeta.h"
#include "PerlinCluster.h"
#include "Enclave2DKeyMapHasher.h"

/* Description: PerlinClusterGenResult is used to store metadata about where the results of a PerlinCluster are located within an
* instance of PerlinFactory, as well as whether or not the cluster specified with the given hash (perlinClsuterHashValue) existed
  already in memory or had to be produced from the ground up. It contains a pointer to the generated PerlinCluster as well, so that it may be used by objects outside of the class.
*/

class PerlinClusterGenResult
{
	public:
		PerlinClusterGenResult() {};
		PerlinClusterGenResult(std::string in_resultingClusterHashValue,
			                   bool in_didPerlinClusterExist,
			                   PerlinCluster* in_genResultClusterRef):
			perlinClusterHashValue(in_resultingClusterHashValue),
			didPerlinClusterExist(in_didPerlinClusterExist),
			genResultClusterRef(in_genResultClusterRef)
		{}

		std::string getResultHash() { return perlinClusterHashValue; }
		bool didClusterExist() { return didPerlinClusterExist; }
		PerlinCluster* getClusterPtr() { return genResultClusterRef; }

	private:
		std::string perlinClusterHashValue = "";	// the SHA-256 string that uniquely represents the PerlinCluster, which comes from a call to PerlinCluster::produceHash()

		bool didPerlinClusterExist = false;		// false indicates that the referenced PerlinCluster was not loaded into memory.
												// true indicates that it was already in memory

		PerlinCluster* genResultClusterRef = nullptr;	// should contain a pointer to an entry in the perlinClusterHashMap of the PerlinFactory object that this function was called from.

};

/* Description: the PerlinFactory class serves as a way to store all NoiseGrids and PerlinClusters that may be used, or that
* reside in memory.
* 
* Use setupNewGrid to setup a new grid, populateSectorInGrid to attempte build a sector from the ground up,
* and insertGridProcessOrder to determine the order of grids to process.

*/

class PerlinFactory
{
	public:
		PerlinFactory() {};

		void setupNewGrid(std::string in_gridName, short in_tileDim, short in_gSectorSize, float in_thresholdValue, int in_seedValue);

		std::vector<PerlinClusterGenResult> populateSectorInGrid(std::string in_gridName, int in_coordA, int in_coordB);

		void insertGridProcessOrder(std::string in_gridName, int in_order);

	private:
		// below: a stringed map of noise grids.
		std::unordered_map<std::string, NoiseGrid> noiseGridMap;

		// below: gridProcessOrderMap, to determine the order of grids to process. Grids at 0 would be first, then 1, then 2, etc;
		// modify or insert all grids wishing to be used 
		std::unordered_map<std::string, int> gridProcessOrderMap;

		std::unordered_map<std::string, PerlinCluster> perlinClusterHashMap;

		bool doesPerlinClusterExist(std::string in_clusterHash);

		void insertPerlinCluster(std::string in_clusterHash, PerlinCluster in_clusterToInsert);

};

#endif
