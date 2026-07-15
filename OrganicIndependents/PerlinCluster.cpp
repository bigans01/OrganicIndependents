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

void PerlinCluster::printPerlinClusterMeta()
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

PerlinClusterOriginSearch PerlinCluster::runOriginSearch()
{
	return metaInfo.searchForOriginKey();
}

Perlin2DSectorMappingContainer PerlinCluster::generateMappingContainer()
{
	std::string currentHash = produceHash();
	auto keyedStateVector = metaInfo.fetchKeyedStates();

	PerlinClusterOriginSearch searchedKey = metaInfo.searchForOriginKey();
	Perlin2DSectorMappingContainer returnContainer(currentHash, searchedKey.getFoundOriginKey());

	returnContainer.insertKeyedStates(keyedStateVector);
	return returnContainer;
}

void PerlinCluster::generateTileToSectorMappingsAndSamplingFields()
{
	std::cout << "#### Generating tileToSectorMappings..." << std::endl;
	for (auto& currentTile : perlinClusterTiles)
	{
		// Use the coordinates of the current tile, plus the parentGridSectorLength, to call
		// NoiseGridUtils::findTileCoordinate twice. Once for X, once for Z.
		int tileToSectorX = NoiseGridUtils::findTileCoordinate(currentTile.first.a, parentGridSectorLength);
		int tileToSectorZ = NoiseGridUtils::findTileCoordinate(currentTile.first.b, parentGridSectorLength);
		EnclaveKeyDef::Enclave2DKey mappedSectorCoordForTileCoord = EnclaveKeyDef::Enclave2DKey(tileToSectorX, tileToSectorZ);
		tileToSectorMapping[currentTile.first] = mappedSectorCoordForTileCoord;

		std::cout << "Tile coordinate: ";
		EnclaveKeyDef::Enclave2DKey tileCoordCopy = currentTile.first;
		tileCoordCopy.printKey();
		std::cout << "Sector coordinate: ";
		mappedSectorCoordForTileCoord.printKey();
		std::cout << std::endl;
	}

	// Test only, can move below some place else
	generateSamplingFieldLookups();
}

void PerlinCluster::generateSamplingFieldLookups()
{
	// Step 1: cycle through tileToSectorMapping, to get a list of unique sectors. That map must be populated
	// before calling this!

	std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> unique2dKeys;
	for (auto& currentTileToSectorMapping : tileToSectorMapping)
	{
		// Remember, we are inserting the second value, which is the sector key. So we'll only
		// end up with a list of uniwue sector values.
		unique2dKeys.insert(currentTileToSectorMapping.second);
	}



	// Step 2: cycle through set we just generated.
	for (auto& currentSamplingKey : unique2dKeys)
	{
		NoiseGridTileSamplingField newField(currentSamplingKey, parentGridSectorLength, gridSeedValue);
		sectorSamplingFields[currentSamplingKey] = newField;
	}

	// Debug
	for (auto& currentSamplingKey : unique2dKeys)
	{
		std::cout << "!! Generating sampling field at key: ";
		EnclaveKeyDef::Enclave2DKey keyCopy = currentSamplingKey;
		keyCopy.printKey();
		std::cout << std::endl;
	}

	std::cout << "+++ generateSamplingFieldLookups: generated " << sectorSamplingFields.size() << " NoiseGridTileSamplingField(s). " << std::endl;

}


void PerlinCluster::printClusterOutputArt()
{
	std::set<int> xHeader;
	std::set<int> zHeader;

	// For each tile, insert the "X" into horizontal,
	// the "Z" into vertical
	for (auto& currentTile : perlinClusterTiles)
	{
		xHeader.insert(currentTile.first.a);
		zHeader.insert(currentTile.first.b);
	}

	std::cout << "# xHeader length: " << xHeader.size() << std::endl;
	std::cout << "# zHeader length: " << zHeader.size() << std::endl;

	// Construct and print the horizontal header string.
	std::cout << std::setw(10) << "+";
	for (auto& currentHorizontalValue : xHeader)
	{
		std::cout << std::setw(10) << std::to_string(currentHorizontalValue) + "|";
	}
	std::cout << std::endl;


	//
	for (auto revVertIter = zHeader.rbegin(); revVertIter != zHeader.rend(); revVertIter++)
	{
		//std::cout << std::setw(10) << *revVertIter << std::endl;
		std::cout << std::setw(10) << *revVertIter;

		for (auto& currentHorizontalValue : xHeader)
		{
			int currentX = currentHorizontalValue;
			int currentZ = *revVertIter;
			EnclaveKeyDef::Enclave2DKey keyToFind(currentX, currentZ);
			bool wasKeyFound = false;
			//wasKeyFound = doesTileExistInGroupings(keyToFind);


			auto wasKeyFoundInCurrentGrouping = perlinClusterTiles.find(keyToFind);
			if (wasKeyFoundInCurrentGrouping != perlinClusterTiles.end())
			{
				wasKeyFound = true;
			}


			std::string searchResult = ".";
			if (wasKeyFound)
			{
				searchResult = "X";
				//totalTilesFlagged++;
			}

			std::cout << std::setw(10) << searchResult;
		}
		std::cout << std::endl;
	}
}

void PerlinCluster::fetchBicubicallyInterpolatedCoordinate(float in_coordX, float in_coordZ)
{
	// First, find te sector.
	// Use the coordinates of the current tile, plus the parentGridSectorLength, to call
	// NoiseGridUtils::findTileCoordinate twice. Once for X, once for Z.
	int tileToSectorX = NoiseGridUtils::findTileCoordinate(in_coordX, parentGridSectorLength);
	int tileToSectorZ = NoiseGridUtils::findTileCoordinate(in_coordZ, parentGridSectorLength);
	EnclaveKeyDef::Enclave2DKey mappedSectorCoordForTileCoord = EnclaveKeyDef::Enclave2DKey(tileToSectorX, tileToSectorZ);

	std::cout << "fetchBicubicallyInterpolatedCoordinate -> original X and Z: ";
	std::cout << in_coordX << ", " << in_coordZ << std::endl;

	std::cout << "fetchBicubicallyInterpolatedCoordinate -> Sector coordinate: ";
	mappedSectorCoordForTileCoord.printKey();
	std::cout << std::endl;

	// Get the modulo of X and Z.
	float remainderX = std::fmod(in_coordX, parentGridSectorLength);
	float remainderZ = std::fmod(in_coordZ, parentGridSectorLength);
	std::cout << "Remainder X -> " << remainderX;
	std::cout << std::endl;

	std::cout << "Remainder Z -> " << remainderZ;
	std::cout << std::endl;

	// Search for the target sector, in sectorSamplingFields.
	auto samplingFieldSearch = sectorSamplingFields.find(mappedSectorCoordForTileCoord);
	if (samplingFieldSearch != sectorSamplingFields.end())
	{
		std::cout << "!! Found corresponding sampling field for sector!" << std::endl;
		float sectorU = findBicubicInterpolationRange(remainderX);
		float sectorV = findBicubicInterpolationRange(remainderZ);
		std::cout << "U value to use: " << sectorU << std::endl;
		std::cout << "V value to use: " << sectorV << std::endl;
	}
	else
	{
		std::cout << "!! Correpsonding sampling sector not found! " << std::endl;
	}

}

PerlinClusterSectorPointSearch PerlinCluster::fetchBicubicallyInterpolatedCoordinateV2(float in_coordX, float in_coordZ)
{


	// Calculate for X.
	std::map<int, float> xCandidateMap;
	int tileToSectorX = NoiseGridUtils::findTileCoordinate(in_coordX, parentGridSectorLength);
	float remainderX = std::fmod(in_coordX, parentGridSectorLength);
	remainderX = IndependentUtils::roundToHundredth(remainderX);

	// for most cases: there is a remainder, so we're not on the border.
	if (remainderX != 0.0f)
	{
		float sectorU = findBicubicInterpolationRange(remainderX);
		xCandidateMap[tileToSectorX] = sectorU;
	}

	// all other cases: we are on the border. So put one entry in the assumed sector X coord,
	// and another at assumed sector X coord - 1. the assumed coord will be a float of 0,
	// and the negative coord will be 1.0f.
	else
	{
		xCandidateMap[tileToSectorX] = 0.0f;
		xCandidateMap[tileToSectorX - parentGridSectorLength] = 1.0f;
	}

	// Do the same thing for Z.
	std::map<int, float> zCandidateMap;
	int tileToSectorZ = NoiseGridUtils::findTileCoordinate(in_coordZ, parentGridSectorLength);
	float remainderZ = std::fmod(in_coordZ, parentGridSectorLength);
	remainderZ = IndependentUtils::roundToHundredth(remainderZ);

	if (remainderZ != 0.0f)
	{
		float sectorV = findBicubicInterpolationRange(remainderZ);
		zCandidateMap[tileToSectorZ] = sectorV;
	}
	else
	{
		zCandidateMap[tileToSectorZ] = 0.0f;
		zCandidateMap[tileToSectorZ - parentGridSectorLength] = 1.0f;
	}

	// Create an unordered map, keyed by EnclaveKeyDef::Enclave2DKey, with a vec2 that represents the ranges..
	std::unordered_map<EnclaveKeyDef::Enclave2DKey, glm::vec2, EnclaveKeyDef::KeyHasher> candidateKeyRanges;

	for (auto& currentX : xCandidateMap)
	{
		for (auto& currentZ : zCandidateMap)
		{
			EnclaveKeyDef::Enclave2DKey currentKey(currentX.first, currentZ.first);
			glm::vec2 currentUV(currentX.second, currentZ.second);

			/*
			std::cout << "! fetchBicubicallyInterpolatedCoordinateV2: found candidate key ";
			currentKey.printKey();
			std::cout << " with the following u/v pair -> " << currentUV.x << ", " << currentUV.y << std::endl;
			*/

			candidateKeyRanges[currentKey] = currentUV;
		}
	}

	// Now, find the first available sector in the candidateKeyRanges.
	bool sectorFound = false;
	EnclaveKeyDef::Enclave2DKey foundSectorKey;
	glm::vec2 foundSectorUVOffset;
	for (auto& currentCandidate : candidateKeyRanges)
	{
		auto sectorFinder = sectorSamplingFields.find(currentCandidate.first);
		if (sectorFinder != sectorSamplingFields.end())
		{
			foundSectorKey = currentCandidate.first;
			foundSectorUVOffset = currentCandidate.second;
			sectorFound = true;
			break;
			
		}
	}
	
	// Setup the return value
	PerlinClusterSectorPointSearch returnSearch;

	// Do stuff if we found the sector.
	if (sectorFound)
	{
		/*
		std::cout << "!! Found sector to sample UV coord from: ";
		foundSectorKey.printKey();
		std::cout << std::endl;
		std::cout << "UV offset: " << foundSectorUVOffset.x << ", " << foundSectorUVOffset.y << std::endl;
		*/

		float sampledValue = sectorSamplingFields[foundSectorKey].calculateBicubicInterpolation(foundSectorUVOffset.x, foundSectorUVOffset.y);

		//std::cout << "Sampled value: " << sampledValue << std::endl;

		returnSearch.setValue(sampledValue);
	}


	return returnSearch;
}

float PerlinCluster::findBicubicInterpolationRange(float in_rangeValue)
{
	float bicubicRangeValue;
	if (in_rangeValue < 0)
	{
		// Remember, when calculating reverse values, the difference between the length of the sector and the range value is what we want.
		// (i.e, 1024 sector length, for -128, would be 896, ... 896 / 1024 would be a value of .85~)
		bicubicRangeValue = (parentGridSectorLength + in_rangeValue) / parentGridSectorLength;
	}
	else if (in_rangeValue >= 0)
	{
		bicubicRangeValue = in_rangeValue / parentGridSectorLength;
	}
	return bicubicRangeValue;
}