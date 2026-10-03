#include "stdafx.h"
#include "PerlinClusterOutputBase.h"

PerlinClusterOutputBase::PerlinClusterOutputBase()
{

}

void PerlinClusterOutputBase::initializeBase(
	ECBMap* in_pcoBaseECBMapRef,
	int in_parentClusterSectorWidth,
	int in_parentClusterTileWidth,
	std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* in_parentPerlinClusterTilesRef,
	std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_parentTileToSectorMappingRef,
	std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTileSamplingField, EnclaveKeyDef::KeyHasher>* in_parentSectorSamplingFieldsRef)
{
	pcoBaseECBMapRef = in_pcoBaseECBMapRef;

	parentClusterSectorWidth = in_parentClusterSectorWidth;
	parentClusterTileWidth = in_parentClusterTileWidth;

	parentPerlinClusterTilesRef = in_parentPerlinClusterTilesRef;
	parentTileToSectorMappingRef = in_parentTileToSectorMappingRef;
	parentSectorSamplingFieldsRef = in_parentSectorSamplingFieldsRef;

	// Step 1: build the shifting keys.
	buildRelativeShiftingKey();
	calculateBlueprintShiftbackKey();

	// Step 2: build the local to absolute tile and sector lookups.
	buildLocalToAbsoluteTileLookup();
	buildLocalTiletoLocalSectorLookup();
	buildLocalToAbsoluteSectorLookup();


	// DEBUG

	std::cout << "!! Parent cluster sector width: " << parentClusterSectorWidth << std::endl;
	std::cout << "!! Parent cluster tile width: " << parentClusterTileWidth << std::endl;

	printRelativeShiftingKey();
	printBlueprintShiftbackKey();
	printOriginalSamplingFields();
	printLocalToAbsoluteSectorLookup();
	printLocalToAbsoluteTileLookup();
	printLocalTileToLocalSectorLookup();

	// Test coord finding.
	//checkIfPointCanExistInCluster(416, 768);	// would be 4 tiles
	//checkIfPointCanExistInCluster(417, 768);	// 2 tiles
	//runSamplingAttempt(416, 768); // 4 tile attempts, 2 found
	auto attempt1Results = runSamplingAttempt(417, 768);	// 2 tiles.
	std::cout << "!!!!! Size of attempt1Results: " << attempt1Results.size() << std::endl;

	auto attempt2Results = runSamplingAttempt(0, 0);	// should do nothing...
	std::cout << "!!!!! Size of attempt2Results: " << attempt2Results.size() << std::endl;
	//runSamplingAttempt(417, 769);	// 1 tile.


	// Below: test, just to make sure the ref is working; remove when needed.
	//std::cout << "!!!! Printing all keys in referenced map: " << std::endl;
	//pcoBaseECBMapRef->printAllKeys();

}

void PerlinClusterOutputBase::printRelativeShiftingKey()
{
	std::cout << "###### Printing out value of relativeShiftingKey: " << std::endl;
	relativeShiftingKey.printKey();
	std::cout << std::endl;
}

void PerlinClusterOutputBase::printBlueprintShiftbackKey()
{
	std::cout << "###### Printing out value of blueprintShiftbackKey: " << std::endl;
	blueprintShiftbackKey.printKey();
	std::cout << std::endl;
}

void PerlinClusterOutputBase::printOriginalSamplingFields()
{
	std::cout << "###### Printing out original sampling field values: " << std::endl;
	for (auto& currentSamplingField : *parentSectorSamplingFieldsRef)
	{
		EnclaveKeyDef::Enclave2DKey current2dKey = currentSamplingField.first;
		current2dKey.printKey();
		std::cout << std::endl;
	}
}

void PerlinClusterOutputBase::printLocalTileToLocalSectorLookup()
{
	std::cout << "###### Printing out localTiletoLocalSectorLookup: " << std::endl;
	for (auto& currentLookupEntry : localTiletoLocalSectorLookup)
	{
		EnclaveKeyDef::Enclave2DKey currentLocalKey = currentLookupEntry.first;
		EnclaveKeyDef::Enclave2DKey currentAbsoluteKey = currentLookupEntry.second;

		std::cout << "Local tile key: ";
		currentLocalKey.printKey();

		std::cout << " --> local sector key: ";
		currentAbsoluteKey.printKey();

		std::cout << std::endl;
	}
}

void PerlinClusterOutputBase::printLocalToAbsoluteSectorLookup()
{
	std::cout << "###### Printing out localToAbsoluteSectorLookup: " << std::endl;
	for (auto& currentLookupEntry : localToAbsoluteSectorLookup)
	{
		EnclaveKeyDef::Enclave2DKey currentLocalKey = currentLookupEntry.first;
		EnclaveKeyDef::Enclave2DKey currentAbsoluteKey = currentLookupEntry.second;

		std::cout << "Local sector key: ";
		currentLocalKey.printKey();

		std::cout << " --> Absolute sector key: ";
		currentAbsoluteKey.printKey();

		std::cout << std::endl;

	}

}

void PerlinClusterOutputBase::printLocalToAbsoluteTileLookup()
{
	std::cout << "###### Printing out localToAbsoluteTileLookup: " << std::endl;
	for (auto& currentLookupEntry : localToAbsoluteTileLookup)
	{
		EnclaveKeyDef::Enclave2DKey currentLocalKey = currentLookupEntry.first;
		EnclaveKeyDef::Enclave2DKey currentAbsoluteKey = currentLookupEntry.second;

		std::cout << "Local tile key: ";
		currentLocalKey.printKey();

		std::cout << " --> Absolute tile key: ";
		currentAbsoluteKey.printKey();

		std::cout << std::endl;
	}
}

std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> PerlinClusterOutputBase::checkIfPointCanExistInCluster(float in_coordX, float in_coordZ)
{
	// Create a map for keys that are valid and found; we will return this.
	std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> foundKeySet;

	// Calculate for X.
	std::set<int> xCandidateSet;
	int pointToTileX = NoiseGridUtils::findTileCoordinate(in_coordX, parentClusterTileWidth);
	float remainderX = std::fmod(in_coordX, parentClusterTileWidth);

	// for most cases: there is a remainder, so we're not on the border.
	if (remainderX != 0.0f)
	{
		xCandidateSet.insert(pointToTileX);
	}

	// all other cases: we are on the border. So put one entry in the assumed sector X coord,
	// and another at assumed sector X coord - 1. the assumed coord will be a float of 0,
	// and the negative coord will be 1.0f.
	else
	{
		xCandidateSet.insert(pointToTileX);
		xCandidateSet.insert(pointToTileX - parentClusterTileWidth);
	}




	// Calculate for Z.
	std::set<int> zCandidateSet;
	int pointToTileZ = NoiseGridUtils::findTileCoordinate(in_coordZ, parentClusterTileWidth);
	float remainderZ = std::fmod(in_coordZ, parentClusterTileWidth);

	// for most cases: there is a remainder, so we're not on the border.
	if (remainderZ != 0.0f)
	{
		zCandidateSet.insert(pointToTileZ);
	}

	// all other cases: we are on the border. So put one entry in the assumed sector X coord,
	// and another at assumed sector X coord - 1. the assumed coord will be a float of 0,
	// and the negative coord will be 1.0f.
	else
	{
		zCandidateSet.insert(pointToTileZ);
		zCandidateSet.insert(pointToTileZ - parentClusterTileWidth);
	}


	// Now, go through all and check if the tiles exist.
	std::cout << "!! checkIfPointCanExistInCluster checking tiles that belong to this point: " << in_coordX << ", " << in_coordZ << std::endl;

	for (auto& currentX : xCandidateSet)
	{
		for (auto& currentZ : zCandidateSet)
		{
			std::cout << "!! checkIfPointCanExistInCluster, searching for tile -> ";
			EnclaveKeyDef::Enclave2DKey currentKey(currentX, currentZ);
			currentKey.printKey();
			std::cout << " --> ";
			
			auto tileFinder = localToAbsoluteTileLookup.find(currentKey);
			if (tileFinder != localToAbsoluteTileLookup.end())
			{
				std::cout << " found. " << std::endl;
				foundKeySet.insert(currentKey);
			}
			else
			{
				std::cout << " not found. " << std::endl;
			}
		}
	}

	return foundKeySet;
}

float PerlinClusterOutputBase::findLocalBicubicInterpolationRange(float in_rangeValue, float in_sectorWidth)
{
	float bicubicRangeValue;
	if (in_rangeValue < 0)
	{
		// Remember, when calculating reverse values, the difference between the length of the sector and the range value is what we want.
		// (i.e, 1024 sector length, for -128, would be 896, ... 896 / 1024 would be a value of .85~)
		bicubicRangeValue = (in_sectorWidth + in_rangeValue) / in_sectorWidth;
	}
	else if (in_rangeValue >= 0)
	{
		bicubicRangeValue = in_rangeValue / in_sectorWidth;
	}
	return bicubicRangeValue;
}

std::vector<TileToSectorLink> PerlinClusterOutputBase::runSamplingAttempt(float in_coordX, float in_coordZ)
{
	std::vector<TileToSectorLink> producedLinks;

	std::cout << "--------starting: runSamplingAttempt. " << std::endl;

	// Get the list of valid local tile coords; if the point borders one tile, it should
	// return two sets of tile coords, etc.
	auto foundKeys = checkIfPointCanExistInCluster(in_coordX, in_coordZ);
	std::cout << "--------size of foundKeys: " << foundKeys.size() << std::endl;

	std::vector<TileToSectorLink> tileLinks;

	// Part 1: --------------------------------------------------------------------------
	// ... we will now iterate per each of the foundKeys.
	for (auto& currentKey : foundKeys)
	{
		// The key should be the local tile value; get this and search for the local sector, by doing the following:
		//
		// 1. Use currentKey in the localTiletoLocalSectorLookup, to get the local sector key value. Make sure the key exists.
		// 2. Assuming #1 exists, use the local sector to lookup the absolute sector in localToAbsoluteSectorLookup. Make sure the key exists.
		auto localSectorFinder = localTiletoLocalSectorLookup.find(currentKey);

		std::cout << "!!!! Searching for sector of key: ";
		EnclaveKeyDef::Enclave2DKey outerCopy = currentKey;
		outerCopy.printKey();
		std::cout << std::endl;

		// Step #1 - Proceed only if there is a local key -> local sector mapping, where local key is currentKey.
		if (localSectorFinder != localTiletoLocalSectorLookup.end())
		{
			std::cout << "!! Found local tile: ";
			EnclaveKeyDef::Enclave2DKey keyCopy = currentKey;
			keyCopy.printKey();

			EnclaveKeyDef::Enclave2DKey valueCopy = localSectorFinder->second;
			std::cout << " | mapped to local sector: ";
			valueCopy.printKey();
			
			std::cout << std::endl;

			// Step #2 - get the absolute sector, by taking the value of the local sector key discovered in the previous check, and
			// using that is the key to use in the map-find operation on localToAbsoluteSectorLookup. The resulting value should be 
			// the absolute sector coordinate, which we can then use on the parentSectorSamplingFieldsRef to fetch a reference to
			// the appropriate sampling field to run sampling on.
			auto absoluteSectorFinder = localToAbsoluteSectorLookup.find(valueCopy);
			if (absoluteSectorFinder != localToAbsoluteSectorLookup.end())
			{
				std::cout << "!! Found local sector: ";
				valueCopy.printKey();

				EnclaveKeyDef::Enclave2DKey absoluteSectorCopy = absoluteSectorFinder->second;
				std::cout << " | mapped to absolute sector: ";
				absoluteSectorCopy.printKey();

				std::cout << std::endl;

				// Create the new tile link.
				TileToSectorLink newLink(outerCopy, valueCopy, absoluteSectorCopy, &(*parentSectorSamplingFieldsRef)[absoluteSectorCopy]);
				newLink.printLink();
				tileLinks.push_back(newLink);
			}
		}
	}

	// Part 2: --------------------------------------------------------------------------
	// Calculate for X.
	std::map<int, float> xCandidateMap;
	int tileToSectorX = NoiseGridUtils::findTileCoordinate(in_coordX, parentClusterSectorWidth);
	float remainderX = std::fmod(in_coordX, parentClusterSectorWidth);
	remainderX = IndependentUtils::roundToHundredth(remainderX);

	// for most cases: there is a remainder, so we're not on the border.
	if (remainderX != 0.0f)
	{
		float sectorU = findLocalBicubicInterpolationRange(remainderX, parentClusterSectorWidth);
		xCandidateMap[tileToSectorX] = sectorU;
	}

	// all other cases: we are on the border. So put one entry in the assumed sector X coord,
	// and another at assumed sector X coord - 1. the assumed coord will be a float of 0,
	// and the negative coord will be 1.0f.
	else
	{
		xCandidateMap[tileToSectorX] = 0.0f;
		xCandidateMap[tileToSectorX - parentClusterSectorWidth] = 1.0f;
	}

	// Do the same thing for Z.
	std::map<int, float> zCandidateMap;
	int tileToSectorZ = NoiseGridUtils::findTileCoordinate(in_coordZ, parentClusterSectorWidth);
	float remainderZ = std::fmod(in_coordZ, parentClusterSectorWidth);
	remainderZ = IndependentUtils::roundToHundredth(remainderZ);

	if (remainderZ != 0.0f)
	{
		float sectorV = findLocalBicubicInterpolationRange(remainderZ, parentClusterSectorWidth);
		zCandidateMap[tileToSectorZ] = sectorV;
	}
	else
	{
		zCandidateMap[tileToSectorZ] = 0.0f;
		zCandidateMap[tileToSectorZ - parentClusterSectorWidth] = 1.0f;
	}

	// Create an unordered map, keyed by EnclaveKeyDef::Enclave2DKey, with a vec2 that represents the ranges..
	std::unordered_map<EnclaveKeyDef::Enclave2DKey, glm::vec2, EnclaveKeyDef::KeyHasher> candidateKeyRanges;

	for (auto& currentX : xCandidateMap)
	{
		for (auto& currentZ : zCandidateMap)
		{
			EnclaveKeyDef::Enclave2DKey currentKey(currentX.first, currentZ.first);
			glm::vec2 currentUV(currentX.second, currentZ.second);

			
			std::cout << "! fetchBicubicallyInterpolatedCoordinateV2: found candidate key ";
			currentKey.printKey();
			std::cout << " with the following u/v pair -> " << currentUV.x << ", " << currentUV.y << std::endl;
			

			candidateKeyRanges[currentKey] = currentUV;
		}
	}

	// Part 3: Iterate over each TileToSectorLink, using the value of localSectorKey in each one as a lookup value for the glm::vec2 sitting in
	// canddiateKeyRanges.
	for (auto& currentLink : tileLinks)
	{
		// Just set the uvSamplingCoord directly; the localSectorKey should be in candidateKeyRanges, and if it's not,
		// you should reconsider drinking beer while programming. 
		currentLink.runSampling(candidateKeyRanges[currentLink.getLocalSectorKey()]);

		std::cout << "########## Final link stats: " << std::endl;
		currentLink.printLink();

		producedLinks.push_back(currentLink);
	}

	std::cout << "--------ending: runSamplingAttempt. " << std::endl;

	return producedLinks;
}

void PerlinClusterOutputBase::buildRelativeShiftingKey()
{
	// Go through all members parentSectorSamplingFieldsRef, to find the min X / min Z values.
	std::set<int> x_values;
	std::set<int> z_values;

	for (auto& currentSamplingField : *parentSectorSamplingFieldsRef)
	{
		x_values.insert(currentSamplingField.first.a);
		z_values.insert(currentSamplingField.first.b);
	}

	// Set the key to be the minimum of X/Z (the begin() of both sets), multiplied by -1.
	relativeShiftingKey = EnclaveKeyDef::Enclave2DKey((*x_values.begin()) * -1, (*z_values.begin()) * -1);
}

void PerlinClusterOutputBase::calculateBlueprintShiftbackKey()
{
	EnclaveKeyDef::Enclave2DKey shiftbackKey = relativeShiftingKey;
	shiftbackKey.a *= -1;
	shiftbackKey.b *= -1;

	std::cout << "!! shiftback value calculation: current values after inversion" << std::endl;
	std::cout << "X -> " << shiftbackKey.a;
	std::cout << "Z -> " << shiftbackKey.b;
	std::cout << std::endl;

	shiftbackKey.a = (shiftbackKey.a / 32);
	shiftbackKey.b = (shiftbackKey.b / 32);

	blueprintShiftbackKey = shiftbackKey;
}

void PerlinClusterOutputBase::buildLocalToAbsoluteTileLookup()
{
	// For each found key in the parentPerlinClusterTilesRef, apply the
	// value of the relativeShiftingKey to it and store it.
	for (auto& currentTile : *parentPerlinClusterTilesRef)
	{
		EnclaveKeyDef::Enclave2DKey originalKeyValue = currentTile.first;
		EnclaveKeyDef::Enclave2DKey localKey(originalKeyValue);
		localKey = localKey + relativeShiftingKey;

		localToAbsoluteTileLookup[localKey] = originalKeyValue;
	}
}

void PerlinClusterOutputBase::buildLocalTiletoLocalSectorLookup()
{
	// For each tile -> sector link, grab both 2d Keys 
	// and apply the relativeShiftingKey. Then, put both of the
	// new tile / sector values, respectively, into localToAbsoluteTileLookup.
	for (auto& currentTileToSectorLink : *parentTileToSectorMappingRef)
	{
		EnclaveKeyDef::Enclave2DKey originalTileKey = currentTileToSectorLink.first;
		EnclaveKeyDef::Enclave2DKey originalSectorKey = currentTileToSectorLink.second;

		EnclaveKeyDef::Enclave2DKey localTileKey(originalTileKey);
		EnclaveKeyDef::Enclave2DKey localSectorKey(originalSectorKey);

		localTileKey = localTileKey + relativeShiftingKey;
		localSectorKey = localSectorKey + relativeShiftingKey;

		localTiletoLocalSectorLookup[localTileKey] = localSectorKey;

	}
}

void PerlinClusterOutputBase::buildLocalToAbsoluteSectorLookup()
{
	// For each found key in the parentSectorSamplingFieldsRef, apply the
	// value of the relativeShiftingKey to it and store it.
	for (auto& currentSamplingField : *parentSectorSamplingFieldsRef)
	{
		EnclaveKeyDef::Enclave2DKey originalKeyValue = currentSamplingField.first;
		EnclaveKeyDef::Enclave2DKey localKey(originalKeyValue);
		localKey = localKey + relativeShiftingKey;

		localToAbsoluteSectorLookup[localKey] = originalKeyValue;
	}
}