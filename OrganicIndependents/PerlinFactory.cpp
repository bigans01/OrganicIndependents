#include "stdafx.h"
#include "PerlinFactory.h"

void PerlinFactory::setupNewGrid(std::string in_gridName, short in_tileDim, short in_gSectorSize, float in_thresholdValue, int in_seedValue)
{
	NoiseGrid newGrid(in_tileDim, in_gSectorSize, in_thresholdValue, in_seedValue);
	noiseGridMap[in_gridName] = newGrid;
}

void PerlinFactory::insertGridProcessOrder(std::string in_gridName, int in_order)
{
	gridProcessOrderMap[in_gridName] = in_order;
}

std::vector<PerlinClusterGenResult> PerlinFactory::populateSectorInGrid(std::string in_gridName, int in_coordA, int in_coordB)
{
	std::vector<PerlinClusterGenResult> generatedResultsVector;



	// Step 1: populate the sector, fetch any producd PerlinClusterMeta. Each individual must 
	// be processed accordingly PerlinClusterMeta (i.e, getting its hash value so we can see if it exists yet or not)
	auto producedPerlinClusterMeta = noiseGridMap[in_gridName].populateSector(in_coordA, in_coordB);

	// cycle through the vector of PerlinClusterMeta.
	for (auto& currentClusterMeta : producedPerlinClusterMeta)
	{
		//std::cout << "Cycling through next currentClusterMeta..." << std::endl;

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> currentTileClusterMap;

		int expectedNumberOfTiles = 0;
		auto fetchedMeta = currentClusterMeta.fetchMetaMap();
		for (auto& currentMetaEntry : fetchedMeta)
		{
			// For each grouping in the currently looked at instance of NGSClusterEntry,
			// get the copy of tiles from that grouping. Each tile, from each grouping, will go into the currentTileClusterMap.
			auto currentClusterCopy = currentMetaEntry.second.fetchClusterGroupingCopy();
			for (auto& currentGroupingId : currentClusterCopy)
			{
				auto currentGrouping = noiseGridMap[in_gridName].getSectorRef(currentMetaEntry.first.a, currentMetaEntry.first.b)->fetchSpecificGroupingById(currentGroupingId.first);
				
				int currentGroupingNumberOfTiles = currentGrouping->getNumberOfTilesInGrouping();

				EnclaveKeyDef::Enclave2DKey currentKeyCopy = currentMetaEntry.first;
				
				//std::cout << "produceHash(), sector key ";
				//currentKeyCopy.printKey();
				//std::cout << ", grouping " << currentGroupingId.first << ", number of tiles: " << currentGroupingNumberOfTiles << std::endl;

				// Tracking: we want the number of overall tiles in the final map to be equal to the sum of all tiles from all 
				// groupings combined.
				expectedNumberOfTiles += currentGroupingNumberOfTiles;

				// Insert all tiles from the current grouping.
				auto currentGroupingTilesCopy = currentGrouping->fetchTileMapCopy();
				for (auto& currentTile : currentGroupingTilesCopy)
				{
					currentTileClusterMap[currentTile.first] = currentTile.second;
				}
			}
		}

		//std::cout << "!! Expected number of tiles: " << expectedNumberOfTiles << std::endl;
		//std::cout << "!! Size of current currentTileClusterMap: " << currentTileClusterMap.size() << std::endl;


		PerlinCluster newCluster(currentClusterMeta, std::move(currentTileClusterMap));

		// TODO: if the GridProcessOrder of the current Grid is not the "first" (i.e, 0),
		// we will need to go through all previous produced PerlinClusters in preceding grids,
		// fetch a list of tiles they cover, and cross-reference those tiles against the tiles of the newCluster.
		// Tiles found in both the current newCluster and a preceding cluster, should be cut from the tile map of the newCluster.
		// Once we have done this pass with all preceding PerlinClusters, we will then compute the final pass.
		// For each PerlinClusterMeta, fetch it's meta map; we will cycle through each element in the map
		// to receive a copy of the current cluster. 
		// 
		// There are steps that must be followed next, to determine what to do:
		// 
		// Step 1: After accounting for previous produced / preceding PerlinClusters that might affect this one, and running
		// the materialization of the PerlinCluster, calculate the PerlinCluster's unique hash value. We can then use this to see 
		// if it exists in memory, or write/update a sector file with the appropriate data.
		//
		// Step 2: Run the following two checks, after the cluster hash has been calculated:
		//
		// A.) Find the origin key of the currentMeta, and look for the sector file corresponding to that key. Check if the file exists, 
		//     AND if it exists, does it contain the PerlinCluster's unique hash?
		//
		// B.) Does a materialized PerlinCluster with the corresponding perlin cluster hash already exist in memory?
		//
		// NOTE: Step "A" will have to be performed from another library, such as WindowsEventAdapter, as that is what handle files.
		// The result of "B", which deals with memory, can be checked here and then used in a return value / vector of some sort.
		// 
		// Now, check the results:
		//
		// If "A" and "B" are false:         materialize the PerlinCluster, put it into memory, and update all of the corresponding sector files, 
		//									 creating files that dont exist already. The origin sector file should receive a value of NGSCGroupingStatus::GROUPING_USED 
		//									 for the corresponding cluster hash in that file; the same hash value in the other files should receive NGSCGroupingStatus::GROUPING_REFERENCED.
		//									
		// 
		// If "A" is true, and "B" is false: materialize the PerlinCluster, putting it into memory only (if it isn't already in memory).
		//                                   If the corresponding PerlinCluster hash value from the sector file shows up with a state of NGSCGroupingStatus::GROUPING_REFERENCED,
		//                                   update that sector file to be NGSCGroupingStatus::GROUPING_USED. If the value from the sector file shows up as
		//                                   NGSCGroupingStatus::GROUPING_USED, do nothing to the file itself (we'd just be loading an 
		//                                   already-generated sector)
		// 
		// If "A" is false, and "B" is true: the PerlinCluster exists in memory already, but the corresponding origin key sector file
		//                                   either doesnt exist, or doesn't have the corresponding hash in the file. In this case,
		//									 create the file if it doesnt exist, putting the hash in, or if the file exists, update it
		//									 with the current hash. Do not materialize into memory.
		// 
		// If "A" AND "B" are true:          the file exists already, it has the corresponding cluster hash in it, and the cluster is already in memory.
		//                                   Check the value of the NGSCGroupingStatus in the file; update it to NGSCGroupingStatus::GROUPING_USED.
		//
		//
		//
		// Step 1: account for preceding / older PerlinClusters, and materialize...
		//
		// <materialize logic goes here etc>



		std::string mappedHash = newCluster.produceHash();

		// Cluster exists (for "B") above: 
		bool clusterExists = false;
		bool isClusterValid = newCluster.isClusterValid();	// remember, we can't look at it after the std::move inserts it,
															// during the call to insertPerlinCluster; so store it here.
		// Only insert if: 
		// 1. The cluster doesn't exist in the perlinClusterHashMap, meaning it was never valid after processing and then entered into the map.
		// 2. The cluster is "valid" (i.e, has tiles left to process; this criteria is determined internally by the PerlinCluster itself)
		//
		// When 1.) is false and 2.) is true, it means we should be loading the PerlinCluster into perlinClusterHashMap.
		// We also need to make sure we store the value of clusterExists in the return value(s).
		if (!doesPerlinClusterExist(mappedHash) && isClusterValid)
		{
			insertPerlinCluster(mappedHash, std::move(newCluster));

			// Once inserted into the map, the PerlinCluster can work it's magic to produce a 3d mesh.
			// Once produced, we need to get a list of BlueprintVolume objects that this will affect.
			//
			// NOTE: there is currently no BlueprintVolume class as of 6/4/2026 -- it's a work in progress.
			// 
			// Each BlueprintVolume will need to be updated, so that it contains the hash of the PerlinCluster we produced,
			// if it does not contain it already
			//
			// << TODO: do logic for 3d mesh generation here, then call some function to fetch the keys. >>
		}

		// Otherwise, return true if it does exist in the map only.
		else if (doesPerlinClusterExist(mappedHash))
		{
			clusterExists = true;
		}

		// TODO: for each PerlinClusterMeta, return some struct/class that contains the results of conditions "A" / "B"...maybe an enum?
		// the calling class can determine what to do then (i.e, what files to modify etc)

		// If the cluster was valid, return it in the generated results, along with its metadata.
		if (isClusterValid)
		{
			PerlinClusterGenResult newResult(mappedHash, clusterExists, &perlinClusterHashMap[mappedHash]);
			generatedResultsVector.push_back(newResult);
		}
	}

	return generatedResultsVector;
}

bool PerlinFactory::doesPerlinClusterExist(std::string in_clusterHash)
{
	bool clusterHashExists = false;
	auto stringSearch = perlinClusterHashMap.find(in_clusterHash);

	// it was found in the map, it exists.
	if (stringSearch != perlinClusterHashMap.end())
	{
		clusterHashExists = true;
	}
	return clusterHashExists;
}

void PerlinFactory::insertPerlinCluster(std::string in_clusterHash, PerlinCluster in_clusterToInsert)
{
	perlinClusterHashMap[in_clusterHash] = in_clusterToInsert;
}