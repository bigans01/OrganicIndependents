#pragma once

#ifndef GRID_H
#define GRID_H

#include "Perlin.h"
#include "PerlinClusterMeta.h"
#include "NGSCScanTargetGrouping.h"
#include "NGSClusterEntry.h"
#include <iomanip>
#include "NoiseGridTile.h"

/* Description: contains classes related to grids. A GridLocation is a coordinate system that will correlate with and be generated
by an existing Grid object. */

// Overview of relationship between the classes in this file:
//
// -a NoiseGrid is a regular 2d grid, designed to hold multiple NoiseGridSectors. 
//  The NoiseGrid is defined by two parameters: the length that represents the disntance in the X/Z directions 
//	(via the parameter, in_gSectorSize), and the number of tiles in the X and Z direction (the in_tileDim parameter).
//
// -a NoiseGridSector is a collection of NoiseGridTiles. When the generateTilesForSector() function is called,
//  the NoiseGridTile objects reside in the tileMap. Later, createSectorTileSets is called to generate the neighbors per tile,
//  followed by a call to createSectorGroupings, which will create 
//  NoiseGridSectorGrouping instances for the sector.
//
// -a NoiseGridSectorGrouping is simply a collection of NoiseGridTile objects, where each
//  tile in the NoiseGridSectorGrouping has at least one other neighboring NoiseGridTile. A tile that has exactly 0 neighbors
//  will be its own grouping. 
//  
// -a NoiseGridTile is essentially an object that can contain a sampled value, and has facilities to store
//  neighboring NoiseGridTile objects that are near it; there are two categories of neighbors: local (those that are within the same
//  sector as the tile) and remote (neighbors that would belong to a separate sector).
//
// A rudimentary relationship diagram of the classes is given below:
//
// NoiseGrid
// |--> Contains one or more NoiseGridSector(s)
//      |--> Contain NoiseGridTile objects, which can be grouped into...
//          |--> NoiseGridSectorGrouping, which contains a set of NoiseGridTile objects.
// 
// NoiseGridZoomMode: in development, currently unused.
//
//
// Ultimately, this combination of classes allows for tile groupings to exist in a given NoiseGridSector that belongs to a
// NoiseGrid. An example order of function calls to do this, all done on the NoiseGrid class, is given below:
//
// Where testGrid is an object of NoiseGrid:
//
// 1. testGrid.generateSector(-1030, -24);
// 2. testGrid.generateSectorTiles(-1030, -24);
// 3. (optional) testGrid.generateSamplesForSector(-1030, -24, 37) (the 37 is just a randomly selected seed value)
// 4. testGrid.runClippingOnExistingSector(-1030, -24, 0.0f);
// 5. testGrid.generateSectorTilesets(-1030, -24);
//
// 6. ...(optional) call function to manually delete tiles here, in a given sector., if desired
// testGrid.removeTileFromSector(-1030, -24, -2048, - 1024);
//
// 7. testGrid.generateSectorGroupings(-1030, -24);
//
// The NoiseGridScanner object combines steps 1 through 7 above, for the initial sector it's working on; other sectors
// get operated on should be done through the NoiseGridScanAttempt object (also used within NoiseGridScanner). The process for 
// starting a NoiseGridScanner is done by calling the populateSector function that is found in the NoiseGrid class.
//
// Another functionality is the findSubSector function. When utilized on an already-generate sector in a NoiseGrid, it will produce a smaller
// range of tiles found within said sector, assuming that the sector has been generate and its tile groupings are done.
//

// NoiseGridZoomMode: currently unused.
enum class NoiseGridZoomMode
{
	NOZOOMVAL,
	SMALLER_OR_EQUAL_TO_BLUEPRINT,    // indicates that the cells of the grid are either smaller or equivalent to the 32x32 size of a blueprint
	LARGER_THAN_BLUEPRINT,            // indicates that the cells of the grid are larger than the 32x32 size of a blueprint (i.e, the Grid is at the 
									  // Sector size, so 1024x1024)
};

// NoiseGridSectorPopulationState: originally intended to be used by NoiseGridSector, but not really utilized at this time.
enum class NoiseGridSectorPopulationState
{
	UNPOPULATED,
    POPULATED_AS_EPICENTER,
	POPULATED_AS_COLLATERAL
};

// NoiseGridLocation: after being constructed, this object can be used to compare against other 
// objects of the same type to determine similarities and differences.
class NoiseGridLocation
{
	public:
		NoiseGridLocation() {};
		NoiseGridLocation(NoiseGridZoomMode in_gZoomMode,
			EnclaveKeyDef::Enclave2DKey in_gSectorCoord,
			EnclaveKeyDef::Enclave2DKey in_absoluteGridCoord,
			EnclaveKeyDef::Enclave2DKey in_sectorRelativeGridCoord):

		gZoomMode(in_gZoomMode),
		gSectorCoord(in_gSectorCoord),
		absoluteGridCoord(in_absoluteGridCoord),
		sectorRelativeGridCoord(in_sectorRelativeGridCoord) {}

		// utility functions for comparison; compare to another instance of GridLocation to see 
		// if one of the variables, such as EnclaveKeys match; can be used by classes such as ProceduralMachine to see
		// if the values are the same, and act based on whether or not they are. For instance, generation of Mountains 
		// probably will rely on whether or not the sector coord is the same.
		bool isGSectorCoordSame(NoiseGridLocation* in_otherLocation) { return (gSectorCoord == in_otherLocation->gSectorCoord); }
		bool isAbsoluteGridCoordSame(NoiseGridLocation* in_otherLocation) { return (absoluteGridCoord == in_otherLocation->absoluteGridCoord); }
		bool isSectorRelativeGridCoordSame(NoiseGridLocation* in_otherLocation) { return (sectorRelativeGridCoord == in_otherLocation->sectorRelativeGridCoord); }

	private:
		NoiseGridZoomMode gZoomMode = NoiseGridZoomMode::NOZOOMVAL;
		EnclaveKeyDef::Enclave2DKey gSectorCoord;					// the absolute sector coord, i.e, sector 0,0 covers 0 -> 1023, 0 -> 1023
		EnclaveKeyDef::Enclave2DKey absoluteGridCoord;			// the absolute grid coord. So an absolute coordinate of 1535, 1535 would lie within in the Grid sector (GSector) of 1,1
		
		EnclaveKeyDef::Enclave2DKey sectorRelativeGridCoord;		// the coordinate of a tile within the sector it lies in; the coordinate is dependent on the properties of the original grid.
																// This value is used for determining how tiles that lie within the same sector get divided into 
																// one or more NoiseGridSectorGrouping, for that sector.
};



// UnitVectorQuad: currently unused; reserved for later use.
class UnitVectorQuad
{
	public:
		UnitVectorQuad(glm::vec2 in_vec0, glm::vec2 in_vec1, glm::vec2 in_vec2, glm::vec2 in_vec3)
		{
			uvArray[0] = in_vec0;
			uvArray[1] = in_vec1;
			uvArray[2] = in_vec2;
			uvArray[3] = in_vec3;
		}
		UnitVectorQuad() {};

		glm::vec2 uvArray[4];
};

// EnclaveKey2DKeyQuad: contains four EnclaveKeyDef::Enclave2DKey values; meant to be used
// by NoiseGridTileSamplingField when generating the 16 points that make up the field (i.e, 4 rows of 4, so
// 4 values of these. 
class EnclaveKey2DKeyQuad
{
	public:
		EnclaveKey2DKeyQuad() {}
		EnclaveKey2DKeyQuad(EnclaveKeyDef::Enclave2DKey in_key0, 
							EnclaveKeyDef::Enclave2DKey in_key1, 
							EnclaveKeyDef::Enclave2DKey in_key2,
							EnclaveKeyDef::Enclave2DKey in_key3)
		{
			keys[0] = in_key0;
			keys[1] = in_key1;
			keys[2] = in_key2;
			keys[3] = in_key3;
		}

		void printValues()
		{
			std::cout << "!! Printing values for EnclaveKey2DKeyQuad: " << std::endl;
			for (int x = 0; x < 4; x++)
			{
				std::cout << x << ": " << keys[x].a << ", " << keys[x].b << std::endl;
			}
		}

		EnclaveKeyDef::Enclave2DKey keys[4];
};



// NoiseGridTileSamplingField: when cosntructed, it contains a series of 16 points that are utilized for bicubic interpolation. The
// 16 points are each generated from a call to a PerlinNoise object; this object takes in the seed value used for the grid and the 2d key of the point to produce a 
// pseudo-random value. This value becomes the value of the sampled point.
// 
// When generated, the function calculateBicubicInterpolation is called, which takes a value of 0 to 1 to sample betweeen the four points closest to the center
// of the 16 point (4x4) grid.
class NoiseGridTileSamplingField
{
	public:
		// Below: the constructor that should always be used when generating a new instance of NoiseGridTileSamplingField;
		// after this is called, call calculateBicubicInterpolation to fetch bicubically interpolated values.
		NoiseGridTileSamplingField(EnclaveKeyDef::Enclave2DKey in_samplingTileRootCoord, short in_tileDim, int in_seedValue) :
			seedValue(in_seedValue)
		{
			tileToSample = NoiseGridTile(in_samplingTileRootCoord, in_tileDim);

			tileRootCoord = in_samplingTileRootCoord;
			tileDim = in_tileDim;
			seedValue = in_seedValue;

			generateField();
		}

		// Fetch a bicubic interpolated value; the values of in_x and in_z must fall between the range of 0 and 1.
		float calculateBicubicInterpolation(float in_x, float in_z)
		{
			BicubicInterpolationSet interpSet(quadPointSets[0], quadPointSets[1], quadPointSets[2], quadPointSets[3]);
			return interpSet.fetchBicubicValue(in_x, in_z);
		}

	private:
		NoiseGridTile tileToSample;

		// Below: this function generates the point field that is based around the root tile that we will be sampling, so that we may
		// sample the root tile.
		void generateField()
		{
			// Step 1: get the coords of the neighboring corner tile's' root coords.
			NoiseGridTileCorners neighboringTileRoots = tileToSample.fetchNeighboringCornerTileRootCoords();

			EnclaveKeyDef::Enclave2DKey posXnegZRootCoordKey = neighboringTileRoots.posXnegZcorner;
			EnclaveKeyDef::Enclave2DKey posXposZRootCoordKey = neighboringTileRoots.posXposZcorner;
			EnclaveKeyDef::Enclave2DKey negXposZRootCoordKey = neighboringTileRoots.negXposZcorner;
			EnclaveKeyDef::Enclave2DKey negXnegZRootCoordKey = neighboringTileRoots.negXnegZcorner;

			// Step 2: Create a new tile for each corner.
			NoiseGridTile posXnegZNeighbor(posXnegZRootCoordKey, tileDim);
			NoiseGridTile posXposZNeighbor(posXposZRootCoordKey, tileDim);
			NoiseGridTile negXposZNeighbor(negXposZRootCoordKey, tileDim);
			NoiseGridTile negXnegZNeighbor(negXnegZRootCoordKey, tileDim);

			// Step 3: Fetch the four corners from the four newly generated neighboring corner tiles.
			NoiseGridTileCorners posXnegZNeighborCorners = posXnegZNeighbor.fetchTileCorners();
			NoiseGridTileCorners posXposZNeighborCorners = posXposZNeighbor.fetchTileCorners();
			NoiseGridTileCorners negXposZNeighborCorners = negXposZNeighbor.fetchTileCorners();
			NoiseGridTileCorners negXnegZNeighborCorners = negXnegZNeighbor.fetchTileCorners();

			std::cout << "TEST: printing out neighboring corners of tile at root coord of: ";
			tileRootCoord.printKey();

			posXnegZNeighbor.printTileRootCoordAndCorners();
			posXposZNeighbor.printTileRootCoordAndCorners();
			negXposZNeighbor.printTileRootCoordAndCorners();
			negXnegZNeighbor.printTileRootCoordAndCorners();

			// Step 4: genereat the quad values, starting at X = 0, Z = 0.
			EnclaveKey2DKeyQuad row0(negXnegZNeighborCorners.negXnegZcorner, negXnegZNeighborCorners.posXnegZcorner, posXnegZNeighborCorners.negXnegZcorner, posXnegZNeighborCorners.posXnegZcorner);
			EnclaveKey2DKeyQuad row1(negXnegZNeighborCorners.negXposZcorner, negXnegZNeighborCorners.posXposZcorner, posXnegZNeighborCorners.negXposZcorner, posXnegZNeighborCorners.posXposZcorner);
			EnclaveKey2DKeyQuad row2(negXposZNeighborCorners.negXnegZcorner, negXposZNeighborCorners.posXnegZcorner, posXposZNeighborCorners.negXnegZcorner, posXposZNeighborCorners.posXnegZcorner);
			EnclaveKey2DKeyQuad row3(negXposZNeighborCorners.negXposZcorner, negXposZNeighborCorners.posXposZcorner, posXposZNeighborCorners.negXposZcorner, posXposZNeighborCorners.posXposZcorner);

			// Debug only: print the EnclaveKeyDef::Enclave2DKey values at each of the 16 points (4 per row)
			std::cout << "~~~~~~ row print, prior ~~~~~~" << std::endl;
			row0.printValues();
			row1.printValues();
			row2.printValues();
			row3.printValues();
			std::cout << "~~~~~~ row print, after ~~~~~~" << std::endl;

			// Create a PerlinNose object to determine the 16 bicubic interpolation input points
			PerlinNoise noiseObj;

			QuadInterpolationPointSet quadSet0(noiseObj.getSeededVecSimiliarity(seedValue, row0.keys[0]), noiseObj.getSeededVecSimiliarity(seedValue, row0.keys[1]), noiseObj.getSeededVecSimiliarity(seedValue, row0.keys[2]), noiseObj.getSeededVecSimiliarity(seedValue, row0.keys[3]));
			QuadInterpolationPointSet quadSet1(noiseObj.getSeededVecSimiliarity(seedValue, row1.keys[0]), noiseObj.getSeededVecSimiliarity(seedValue, row1.keys[1]), noiseObj.getSeededVecSimiliarity(seedValue, row1.keys[2]), noiseObj.getSeededVecSimiliarity(seedValue, row1.keys[3]));
			QuadInterpolationPointSet quadSet2(noiseObj.getSeededVecSimiliarity(seedValue, row2.keys[0]), noiseObj.getSeededVecSimiliarity(seedValue, row2.keys[1]), noiseObj.getSeededVecSimiliarity(seedValue, row2.keys[2]), noiseObj.getSeededVecSimiliarity(seedValue, row2.keys[3]));
			QuadInterpolationPointSet quadSet3(noiseObj.getSeededVecSimiliarity(seedValue, row3.keys[0]), noiseObj.getSeededVecSimiliarity(seedValue, row3.keys[1]), noiseObj.getSeededVecSimiliarity(seedValue, row3.keys[2]), noiseObj.getSeededVecSimiliarity(seedValue, row3.keys[3]));

			quadPointSets[0] = quadSet0;
			quadPointSets[1] = quadSet1;
			quadPointSets[2] = quadSet2;
			quadPointSets[3] = quadSet3;

			// Debug only: print the origin values for each quadPointSet.
			std::cout << "###### Printing origin values (generateFieldV2) #####" << std::endl;

			quadPointSets[3].printOriginValues();
			quadPointSets[2].printOriginValues();
			quadPointSets[1].printOriginValues();
			quadPointSets[0].printOriginValues();

		}
				
		EnclaveKeyDef::Enclave2DKey tileRootCoord;
		short tileDim = 0;
		int seedValue = 0;

		QuadInterpolationPointSet quadPointSets[4];
};

// NoiseGridSectorGroupingLink: meant to be used by NoiseGridSectorGrouping, it is intended to store
// the values of neighboring tiles that could *possibly* border a NoiseGridSectorGrouping; it does not
// mean that these neighbors exist, but it instead provides a list of neighbors to scan for.
class NoiseGridSectorGroupingLink
{
	public:
		NoiseGridSectorGroupingLink() {};
		void insertNeighboringSectorTileKey(EnclaveKeyDef::Enclave2DKey in_tileKey)
		{
			neighboringSectorTiles.insert(in_tileKey);
			numberOfInsertCalls++;
		}

		void printNeighboringSectorTiles()
		{
			std::cout << "!! Printing neighboringSectorTiles..." << std::endl;
			for (auto& currentNeighbor : neighboringSectorTiles)
			{
				EnclaveKeyDef::Enclave2DKey currentKey = currentNeighbor;
				currentKey.printKey();
				std::cout << std::endl;
			}
		}

		int getNumberOfInsertCalls()
		{
			return numberOfInsertCalls;
		}

		int getNumberOfTiles()
		{
			return neighboringSectorTiles.size();
		}

		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> fetchLinkTiles()
		{
			return neighboringSectorTiles;
		}

	private:
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> neighboringSectorTiles;
		int numberOfInsertCalls = 0;

		bool wasLinkAttempted = false;	// bool flag to indicate whether or not the neighboringSectorTiles
										// of an instance of this class have been used when checking for them in the neighboring sector.
};

// NoiseGridSectorGrouping: designed to contain a set of tiles after the NoiseGridSector::buildTileGroupings function 
// has been called. A single NoiseGridSector may have multiple NoiseGridSectorGrouping instances in it, but these
// NoiseGridSectorGrouping instances are not shared between different NoiseGridSector objects.
class NoiseGridSectorGrouping
{
	public:
		NoiseGridSectorGrouping() {};

		void scanForTilesWithRemainingLocalNeighbors()
		{
			std::cout << "Scanning tiles of this grouping (total size of tiles is " << tiles.size() << std::endl;

			for (auto& currentTile : tiles)
			{
				auto currentTileNeighbors = currentTile.second.getLocalNeighborSetCopy();
				if (currentTileNeighbors.size() > 0)
				{
					EnclaveKeyDef::Enclave2DKey keyCopy = currentTile.first;
					std::cout << "! Tile at "; 
					keyCopy.printKey(); 
					std::cout << " has " << currentTileNeighbors.size() << "neighbors. " << std::endl;
				}
			}
		}

		void insertTile(EnclaveKeyDef::Enclave2DKey in_tileKey, NoiseGridTile in_tile)
		{
			tiles[in_tileKey] = in_tile;
		}

		NoiseGridTile fetchTile(EnclaveKeyDef::Enclave2DKey in_tileKey)
		{
			return tiles[in_tileKey];
		}

		int getNumberOfTilesInGrouping()
		{
			return tiles.size();
		}

		void printTileMembers()
		{
			int tileCount = tiles.size();
			std::cout << "Printing tiles in this tile set (total tiles: " << tileCount << "): " << std::endl;
			for (auto& currentTile : tiles)
			{
				EnclaveKeyDef::Enclave2DKey currentTileKey = currentTile.first;
				std::cout << "Key: "; 
				currentTileKey.printKey();
				std::cout << " | number of remaining locals: "; 
				std::cout << currentTile.second.getNumberOfRemainingLocalNeighbors();

				std::cout << std::endl;
			}

			std::cout << "Printing neighboring sector keys: " << std::endl;
			for (auto& currentNeighboringSector : neighboringSectors)
			{
				EnclaveKeyDef::Enclave2DKey currentNeighboringSectorKey = currentNeighboringSector;
				currentNeighboringSectorKey.printKey();
				std::cout << std::endl;
			}
		}

		void findNeighboringSectors(int in_originalGroupingKey)
		{
			// For each tile, get it's remote neighbor map. Go through each entry in the map, and acquire the first key, which is the sector key.
			// Put this key value into the neighboringSectors set.
			for (auto& currentTile : tiles)
			{
				auto currentMap = currentTile.second.getRemoteNeighborMap();
				for (auto& currentMapEntry : currentMap)
				{
					//neighboringSectors.insert(currentMa)
					neighboringSectors.insert(currentMapEntry.first);

					insertNeighborLink(currentMapEntry.first, currentMapEntry.second, in_originalGroupingKey);
				}
			}
		}

		bool doesNeighboringSectorLinkExist(EnclaveKeyDef::Enclave2DKey in_tileKeyToFind)
		{
			bool wasFound = false;
			auto keyFinder = neighboringSectorLinks.find(in_tileKeyToFind);
			if (keyFinder != neighboringSectorLinks.end())
			{
				wasFound = true;
			}
			return wasFound;
		}

		bool doesKeyExist(EnclaveKeyDef::Enclave2DKey in_tileKeyToFind)
		{
			bool exists = false;
			auto existingKeyFinder = tiles.find(in_tileKeyToFind);

			// return true if found.
			if (existingKeyFinder != tiles.end())
			{
				exists = true;
			}
			return exists;
		}

		void printNeighboringLinks()
		{
			std::cout << "!!!!! Printing neighboring links..." << std::endl;
			for (auto& currentLink : neighboringSectorLinks)
			{
				EnclaveKeyDef::Enclave2DKey currentNeighboringSector = currentLink.first;
				std::cout << "Neighboring sector: ";
				currentNeighboringSector.printKey();
				std::cout << std::endl;
				currentLink.second.printNeighboringSectorTiles();
			}
		}

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridSectorGroupingLink, EnclaveKeyDef::KeyHasher> fetchNeighboringSectorLinks()
		{
			return neighboringSectorLinks;
		}

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> fetchTileMapCopy()
		{
			return tiles;
		}

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> tiles;
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> neighboringSectors;
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridSectorGroupingLink, EnclaveKeyDef::KeyHasher> neighboringSectorLinks;

		// Inserts a tile key that may or may not exist in a neighboring sector, but needs to be checked for regardless, to see
		// if there are any groupings in that neighboring sector that link to this grouping.
		void insertNeighborLink(EnclaveKeyDef::Enclave2DKey in_sectorKey, EnclaveKeyDef::Enclave2DKey in_tileKey, int in_originalGroupingId)
		{
			neighboringSectorLinks[in_sectorKey].insertNeighboringSectorTileKey(in_tileKey);	
		}
};

// NoiseGridSectorGroupBuilderV2: this is the new iteration of the class that is designed to produce tile keys that are used 
// when generating NoiseGridSectorGrouping objects. It takes in a referenced tile map as part of it's constructor.
//
// After this, the start() function may then be called. This function selects the first available tile in the referenced map,
// and then keeps iterating; in each iteration, the current tile to insert is then put into the tileClusterMap. When there are no more
// neighboring tiles to iterate over, the function is done.
//
// In its current form, it is used by the while loop found in the function buildTileGroupingsV2. Each NoiseGridSectorGroupBuilderV2 
// object should only call start() once, as it doesn't contain functionality to clear its tileClusterMap.
class NoiseGridSectorGroupBuilderV2
{
	public:
		NoiseGridSectorGroupBuilderV2() {};
		NoiseGridSectorGroupBuilderV2(std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* in_tileMapPtr)
		{
			tileMapPtr = in_tileMapPtr;
		}

		void start()
		{
			auto refedTileMapBegin = (*tileMapPtr).begin();

			// only continue if the tile map is actually populated with something.
			if (refedTileMapBegin != (*tileMapPtr).end())
			{

				bool stopFlag = false;

				// Step 1: Get the first tile in the map; return a LoopHandler that will
				// contain the next tile to find, if it's handlingEnumValue is LoopHandlerEnum::CONTINUE.
				LoopHandler returnedHandlingValue = insertTileIntoCluster(refedTileMapBegin->second);
				while (stopFlag == false)
				{

					// Continue if there was a new tile to find.
					if (returnedHandlingValue.handlingEnumValue == LoopHandlerEnum::CONTINUE)
					{
						auto nextTileFinder = (*tileMapPtr).find(returnedHandlingValue.nextKey);
						returnedHandlingValue = insertTileIntoCluster(nextTileFinder->second);
					}

					// Otherwise, stop.
					else
					{
						stopFlag = true;
					}

					refedTileMapBegin++;
				}
			}
			std::cout << "Final size of tile cluster is: " << tileClusterMap.size() << std::endl;
		}

		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> fetchClusterTileKeys()
		{
			std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> generatedClusterTileKeys;
			for (auto& currentTileClusterMapEntry : tileClusterMap)
			{
				generatedClusterTileKeys.insert(currentTileClusterMapEntry.first);
			}
			return generatedClusterTileKeys;
		}

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* tileMapPtr = nullptr;

		enum class TileStatusEnum
		{
			SCANNED,
			REFERENCED,
			NOVAL
		};


		struct TileStatus
		{
			TileStatus() {};
			TileStatus(EnclaveKeyDef::Enclave2DKey in_tileCoord, TileStatusEnum in_tileStatus)
			{
				tStatusCoord = in_tileCoord;
			}

			EnclaveKeyDef::Enclave2DKey tStatusCoord;
			TileStatusEnum tileState = TileStatusEnum::NOVAL;
		};

		enum class LoopHandlerEnum
		{
			CONTINUE,
			STOP,
			NOHENUM
		};

		struct LoopHandler
		{
			LoopHandler() {}
			LoopHandler(EnclaveKeyDef::Enclave2DKey in_nextKey, LoopHandlerEnum in_handlingEnumValue)
			{
				nextKey = in_nextKey;
				handlingEnumValue = in_handlingEnumValue;
			}
			EnclaveKeyDef::Enclave2DKey nextKey;
			LoopHandlerEnum handlingEnumValue = LoopHandlerEnum::NOHENUM;
		};

		void insertTileAsScanned(EnclaveKeyDef::Enclave2DKey in_tileCoord)
		{
			tileClusterMap[in_tileCoord] = TileStatusEnum::SCANNED;
		}

		void insertTileAsReferenced(EnclaveKeyDef::Enclave2DKey in_tileCoord)
		{
			tileClusterMap[in_tileCoord] = TileStatusEnum::REFERENCED;
		}

		int fetchTileClusterSize()
		{
			return tileClusterMap.size();
		}


		LoopHandler insertTileIntoCluster(NoiseGridTile in_tileToInsert)
		{
			LoopHandler returnHandler;

			bool shouldStop = false;
			EnclaveKeyDef::Enclave2DKey scannedValue = in_tileToInsert.getTile2DKey();
			auto neighboringValues = in_tileToInsert.getLocalNeighborSetCopy();

			// Insert the scanned value directly into tileClusterMap.
			insertTileAsScanned(scannedValue);

			// For each neighbor, insert as referenced, if it is found in the tileMapPtr. Only
			// perform the insert into the clusetr map, if it didnt exist already.
			for (auto& currentNeighbor : neighboringValues)
			{
				auto refedTileMapPtrFinder = tileMapPtr->find(currentNeighbor);
				if (refedTileMapPtrFinder != tileMapPtr->end())
				{
					// Only insert a referenced if the key didnt exist already
					auto clusterMapFinder = tileClusterMap.find(currentNeighbor);
					if (clusterMapFinder == tileClusterMap.end())
					{
						insertTileAsReferenced(currentNeighbor);
					}
				}
			}

			// Check the number of entries in tileClusterMap that have a TileStatusEnum::REFERENCED; if there are 0, we should be done.
			int totalReferenced = 0;
			for (auto& currentTileClusterMapEntry : tileClusterMap)
			{
				if (currentTileClusterMapEntry.second == TileStatusEnum::REFERENCED)
				{
					totalReferenced++;

					returnHandler = LoopHandler(currentTileClusterMapEntry.first, LoopHandlerEnum::CONTINUE);

					break;
				}
			}

			if (totalReferenced == 0)
			{
				// below: key shouldnt matter, as we're just going to flat out stop.
				returnHandler = LoopHandler(EnclaveKeyDef::Enclave2DKey(0, 0), LoopHandlerEnum::STOP);
				shouldStop = true;
			}

			return returnHandler;
		}

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, TileStatusEnum, EnclaveKeyDef::KeyHasher> tileClusterMap;

};

// NoiseGridLinkSearchResult: used to store results by the NoiseGridSector function, runLinkScan, to determine if a grouping
// in a particular sector links with a grouping in a neighboring sector. The second constructor should be used if there is in fact
// a neighboring link that was found.
class NoiseGridLinkSearchResult
{
	public:

		// Required default constructor for copy
		NoiseGridLinkSearchResult() {};

		// Use the below constructor (#2) when a grouping in another sector links.
		NoiseGridLinkSearchResult(EnclaveKeyDef::Enclave2DKey in_foundSectorKey, int in_foundLinkGroupingID)
		{
			foundLinkSector = in_foundSectorKey;
			foundLinkGroupingID = in_foundLinkGroupingID;
			wasLinkFound = true;
		}

		bool wasLinkFound = false;
		EnclaveKeyDef::Enclave2DKey foundLinkSector;
		int foundLinkGroupingID = -1;
};

// NoiseGridSectorGroupingFetcher: used to store metadata about a grouping, and a pointer to said grouping, in 
// a convenient class; primarily used when needing to retrieve info about a particular grouping in a NoiseGridSector,
// such as in the NoiseGridScanner::start function.
class NoiseGridSectorGroupingFetcher
{
	public:
		NoiseGridSectorGroupingFetcher() {};
		NoiseGridSectorGroupingFetcher(int in_groupingId, NoiseGridSectorGrouping* in_groupingPtr):
			groupingId(in_groupingId),
			groupingPtr(in_groupingPtr)
		{}

		int groupingId = -1;
		NoiseGridSectorGrouping* groupingPtr = nullptr;
};

// NoiseGridSector: contains one or more NoiseGridTiles, prior to the call to createSectorGroupings;
// after a call to that function is made, the tiles of the NoiseGridSector will be sorted out into various
// NoiseGridSectorGroupings.
//
// The runClipping function can be used to cull unwanted tiles, primarily after some function (such as Perlin noise)
// has been applied to the tiles; additionally, tiles can manually be removed altogether by a call to removeTile.
// In both of these cases, the removing of the tile before the groupings occur will cause that tile to be discarded altogether.
// Calling removeTile or runClipping after the groupings have occurred will have no effect.

class NoiseGridSector
{
	public:
		NoiseGridSector() {};
		NoiseGridSector(EnclaveKeyDef::Enclave2DKey in_sectorCoordinate,
			            short in_sectorDim,
			            short in_sectorTileDim,
			            short in_sectorTilesPerDim):
		   sectorCoordinate(in_sectorCoordinate),
		   sectorDim(in_sectorDim),
	       sectorTileDim(in_sectorTileDim),
		   sectorTilesPerDim(in_sectorTilesPerDim)
		{};

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* getTileMapRef()
		{
			return &tileMap;
		}

		bool doesSectorHaveGroupings()
		{
			return !tileGroupings.empty();
		}

		EnclaveKeyDef::Enclave2DKey fetchSectorRootCoord()
		{
			return sectorCoordinate;
		}
		
		NoiseGridSectorGroupingFetcher fetchFirstGrouping()
		{ 
			return NoiseGridSectorGroupingFetcher(tileGroupings.begin()->first, &tileGroupings.begin()->second);
		}

		std::vector<NoiseGridSectorGroupingFetcher> fetchGroupingVector()
		{
			std::vector<NoiseGridSectorGroupingFetcher> returnVector;
			for (auto& currentGrouping : tileGroupings)
			{
				NoiseGridSectorGroupingFetcher currentFetcher(currentGrouping.first, &currentGrouping.second);
				returnVector.push_back(currentFetcher);
			}

			return returnVector;
		}

		NoiseGridSectorGrouping* fetchSpecificGroupingById(int in_groupingId)
		{
			return &tileGroupings[in_groupingId];
		}

		void printNeighboringLinksPerGrouping()
		{
			std::cout << "!!!! Printing grouping data: " << std::endl;
			for (auto& currentGrouping : tileGroupings)
			{
				std::cout << "---- Grouping ID: " << currentGrouping.first << std::endl;
				currentGrouping.second.printNeighboringLinks();
			}
		}

		int getNumberOfTilesInGrouping(int in_groupingId)
		{
			int totalFound = 0;
			auto groupFinder = tileGroupings.find(in_groupingId);
			if (groupFinder != tileGroupings.end())
			{
				totalFound = tileGroupings[in_groupingId].getNumberOfTilesInGrouping();
			}
			return totalFound;
		}

		void printGroupingTileCounts()
		{
			std::cout << "Printing tile grouping counts: " << std::endl;
			for (auto& currentGrouping : tileGroupings)
			{
				std::cout << "Group " << currentGrouping.first << ": " << currentGrouping.second.getNumberOfTilesInGrouping() << std::endl;
			}
		}

		// Below: populate the initial the tileMap with actual tiles; these must be populated prior
		// to any clipping or later grouping calls.
		void generateTilesForSector()
		{
			for (int x = 0; x < sectorTilesPerDim; x++)
			{
				int currentXValue = sectorCoordinate.a + (x * sectorTileDim);
				for (int z = 0; z < sectorTilesPerDim; z++)
				{
					int currentZValue = sectorCoordinate.b + (z * sectorTileDim);
					EnclaveKeyDef::Enclave2DKey sectorTileKey(currentXValue, currentZValue);
					NoiseGridTile newGridTile(sectorTileKey, sectorTileDim);
					tileMap[sectorTileKey] = newGridTile;
				}
			}
		};

		void generateSamples(int in_seedValue)
		{
			std::cout << "Generating samples for coordinate: "; sectorCoordinate.printKey(); std::cout << std::endl;

			NoiseGridTileSamplingField sectorSamplingField(sectorCoordinate, sectorDim, in_seedValue);

			// test: print out relative keys
			for (auto& currentTile : tileMap)
			{
				EnclaveKeyDef::Enclave2DKey currentKey = currentTile.first;
				//currentKey.printKey(); std::cout << " --> " << std::endl;

				EnclaveKeyDef::Enclave2DKey offsetCopy = currentKey;
				offsetCopy = (offsetCopy - sectorCoordinate);
				offsetCopy.a /= sectorTileDim;
				offsetCopy.b /= sectorTileDim;

				//offsetCopy.printKey();

				float bicubicSamplingPerTile = 1.0f / sectorTilesPerDim;
				float bicubicTileCenterOffset = float(bicubicSamplingPerTile / 2);
				

				glm::vec2 bicubicSamplingVec2((offsetCopy.a * bicubicSamplingPerTile) + bicubicTileCenterOffset, (offsetCopy.b * bicubicSamplingPerTile) + bicubicTileCenterOffset);
				//std::cout << "bicubicSamplingVec2: " << bicubicSamplingVec2.x << ", " << bicubicSamplingVec2.y;

				float currentTileSamplingResult = sectorSamplingField.calculateBicubicInterpolation(bicubicSamplingVec2.x, bicubicSamplingVec2.y);
				//std::cout << "| Current tile sampling result is: " << currentTileSamplingResult << std::endl;

				//std::cout << std::endl;

				currentTile.second.setSampledValue(currentTileSamplingResult);


			}
		}

		

		// Below: this function is just a public-facing wrapper function to
		// call runNoiseClipping, which will purge tiles that don't meet a certain threshold.
		void runClipping(float in_threshold)
		{
			runNoiseClipping(in_threshold);
			//createNoiseGridSectorGroupings();
			//buildTileGroupings();
			//printTileGroupings();
		}

		// Below: wrapper function for createNoiseGridSectorGroupings()
		void createSectorTileSets()
		{
			createNoiseGridSectorGroupings();
			//buildTileGroupings();
			//printTileGroupings();
		}

		// Below: public facing wrapper function for buildTileGroupings()
		void createSectorGroupings()
		{
			buildTileGroupingsV2();
			//buildTileGroupings();
			
			//printTileGroupings();
		}


		void printTilesForSector()
		{
			std::cout << "Printing tiles for sector..." << std::endl;
			for (auto& currentTile : tileMap)
			{
				EnclaveKeyDef::Enclave2DKey currentKey = currentTile.first;
				currentKey.printKey();
				std::cout << std::endl;
			}
		}

		void removeTile(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey tileToFind(in_coordinateX, in_coordinateZ);
			auto tileSearchResult = tileMap.find(tileToFind);
			if (tileSearchResult != tileMap.end())
			{
				// Get the neighbor list; we must go through all local neighbors of this tile, 
				// and delete the neighbor value for the tile we are deleting, if it is contained with
				// that neighbor.
				auto currentTileNeighbors = tileSearchResult->second.getLocalNeighborSetCopy();
				for (auto& currentNeighboringTileKey : currentTileNeighbors)
				{
					auto currentNeighborSearchResult = tileMap.find(currentNeighboringTileKey);

					// Below: if the tile to erase actually exists in the tileMap, we must remove the tileToFind key 
					// from the neighbor list of that tile.
					if (currentNeighborSearchResult != tileMap.end())
					{
						tileMap[currentNeighborSearchResult->first].removeDiscoveredNeighborKey(tileToFind);
					}
				}

				// Lastly, remove the tile itself.
				tileMap.erase(tileToFind);
			}
		}

		void printNeighborsPerTile()
		{
			for (auto& currentTile : tileMap)
			{
				currentTile.second.printNeighborCategoryMetadata(sectorCoordinate);
			}
		}

		bool doesTileExistInGroupings(EnclaveKeyDef::Enclave2DKey in_keyToFind)
		{
			bool exists = false;
			for (auto& currentGrouping : tileGroupings)
			{
				auto wasKeyFoundInCurrentGrouping = currentGrouping.second.doesKeyExist(in_keyToFind);
				if (wasKeyFoundInCurrentGrouping)
				{
					exists = true;
					break;
				}
			}
			return exists;
		}

		NoiseGridTile fetchTileGromGroupings(EnclaveKeyDef::Enclave2DKey in_keyToFind)
		{
			NoiseGridTile foundTile;
			for (auto& currentGrouping : tileGroupings)
			{
				auto wasKeyFoundInCurrentGrouping = currentGrouping.second.doesKeyExist(in_keyToFind);
				if (wasKeyFoundInCurrentGrouping)
				{
					return currentGrouping.second.fetchTile(in_keyToFind);
				}
			}

			return foundTile;
		}

		void printMappingCoutArt()
		{
			// create horizontal header set < --- > 
			std::set<int> horizontalHeader;
			for (int x = 0; x < sectorTilesPerDim; x++)
			{
				int currentXValue = sectorCoordinate.a + (x * sectorTileDim);
				horizontalHeader.insert(currentXValue);
			}

			/*
			std::cout << "Horizontal header order: " << std::endl;
			for (auto& currentHorizontalValue : horizontalHeader)
			{
				std::cout << currentHorizontalValue << std::endl;
			}
			*/

			// create vertical header set ^
			std::set<int> verticalHeader;
			for (int z = 0; z < sectorTilesPerDim; z++)
			{
				int currentZValue = sectorCoordinate.b + (z * sectorTileDim);
				verticalHeader.insert(currentZValue);
			}

			/*
			std::cout << "Vertical header order (make sure to start from reverse): " << std::endl;
			for (auto revVertIter = verticalHeader.rbegin(); revVertIter != verticalHeader.rend(); revVertIter++)
			{
				std::cout << *revVertIter << std::endl;
			}
			*/
			
			
			
			
			
			
			//std::cout << "Printing out horizontal header (setw)..." << std::endl;
			//std::cout << "Size of horizontal header: " << horizontalHeader.size() << std::endl;
			std::cout << "Printing out hit table for sector at: ";
			sectorCoordinate.printKey();
			std::cout << std::endl;

			std::cout << std::setw(10) << "+";
			for (auto& currentHorizontalValue : horizontalHeader)
			{
				std::cout << std::setw(10) << std::to_string(currentHorizontalValue) + "|";
			}
			std::cout << std::endl;

			
			int totalTilesFlagged = 0;
			
			for (auto revVertIter = verticalHeader.rbegin(); revVertIter != verticalHeader.rend(); revVertIter++)
			{
				//std::cout << std::setw(10) << *revVertIter << std::endl;
				std::cout << std::setw(10) << *revVertIter;

				for (auto& currentHorizontalValue : horizontalHeader)
				{
					int currentX = currentHorizontalValue;
					int currentZ = *revVertIter;
					EnclaveKeyDef::Enclave2DKey keyToFind(currentX, currentZ);
					bool wasKeyFound = false;
					wasKeyFound = doesTileExistInGroupings(keyToFind);

					/*
					for (auto& currentGrouping : tileGroupings)
					{
						auto wasKeyFoundInCurrentGrouping = currentGrouping.second.doesKeyExist(keyToFind);
						if (wasKeyFoundInCurrentGrouping)
						{
							wasKeyFound = true;
							break;
						}
					}
					*/

					std::string searchResult = ".";
					if (wasKeyFound)
					{
						searchResult = "X";
						totalTilesFlagged++;
					}

					std::cout << std::setw(10) << searchResult;
				}
				std::cout << std::endl;
			}

			std::cout << "Total tiles flagged: " << totalTilesFlagged << std::endl;

			// For each tile that should be long in the sector, check to see if it is found in any of the groupings.
			// If it is not found in any of the groupings, assume that it does not exist.
			for (int x = 0; x < sectorTilesPerDim; x++)
			{
				int currentXValue = sectorCoordinate.a + (x * sectorTileDim);
				for (int z = 0; z < sectorTilesPerDim; z++)
				{
					int currentZValue = sectorCoordinate.b + (z * sectorTileDim);
					EnclaveKeyDef::Enclave2DKey sectorTileKey(currentXValue, currentZValue);
					bool wasKeyFound = false;
					for (auto& currentGrouping : tileGroupings)
					{
						auto wasKeyFoundInCurrentGrouping = currentGrouping.second.doesKeyExist(sectorTileKey);
						if (wasKeyFoundInCurrentGrouping)
						{
							wasKeyFound = true;
							break;
						}
					}

					// output if the key was found.
					/*
					std::cout << "Search result of key: ";
					sectorTileKey.printKey();
					std::cout << " -> ";

					std::string foundOrNot = "";
					if (wasKeyFound)
					{
						foundOrNot = "Found";
					}
					else
					{
						foundOrNot = "Not found";
					}
					std::cout << foundOrNot << std::endl;
					*/
				}
			}
		}

		NoiseGridSectorPopulationState fetchSectorState()
		{
			return currentSectorState;
		}

		// When given an unordered_set of Enclave2DKeys, this function will attempt to search for any groupings in this sector that contain
		// those keys. If this turns out to be true, a break is issued and a NoiseGridLinkSearchResult is created with the "valid" constructor
		// (i.e, constructor #2).
		NoiseGridLinkSearchResult runLinkScan(std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> in_linkKeys)
		{
			NoiseGridLinkSearchResult scanResult;

			// Search groupings to see if any of the keys in the link are found.
			for (auto& currentGrouping : tileGroupings)
			{
				// Check if any of the link keys that were input, are found in the curernt grouping.
				for (auto& currentLinkkey : in_linkKeys)
				{
					// A valid linking key was found; populate a valid NoiseGridLinkSearchResult and return it.
					if (currentGrouping.second.doesKeyExist(currentLinkkey))
					{
						std::cout << "!! Key found in grouping " << currentGrouping.first << std::endl;

						NoiseGridLinkSearchResult foundLink(sectorCoordinate, currentGrouping.first);
						scanResult = foundLink;
						break;
					}
				}
			}

			return scanResult;
		}

	private:
		NoiseGridSectorPopulationState currentSectorState = NoiseGridSectorPopulationState::UNPOPULATED;
		EnclaveKeyDef::Enclave2DKey sectorCoordinate;
		short sectorDim = 0;
		short sectorTileDim = 0;
		short sectorTilesPerDim = 0;
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> tileMap;
		std::map<int, NoiseGridSectorGrouping> tileGroupings;

		// Below: Step 1: if any tile doesn't meet the given threshold parameter, it will be removed.
		void runNoiseClipping(float in_threshold)
		{
			std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> clippedTiles;
			for (auto& currentTile : tileMap)
			{
				if (currentTile.second.getSampledValue() < in_threshold)
				{
					clippedTiles.insert(currentTile.first);
				}
			}

			// Go through the tileMap, and erase what didn't meet the threshold.
			for (auto& currentTileToDelete : clippedTiles)
			{
				tileMap.erase(currentTileToDelete);
			}
		}


		// Below: Step 2: create NoiseGridSectorGroupings.
		void createNoiseGridSectorGroupings()
		{



			// Step 2.: generate the neighbor keys, for everything that is in the tileMap;
			for (auto& currentTileToGenerateNeighborsFor : tileMap)
			{
				//currentTileToGenerateNeighborsFor.second.generateNeighbors(sectorCoordinate, sectorDim);
				currentTileToGenerateNeighborsFor.second.generateNeighborCategories(sectorCoordinate, sectorDim, &tileMap);
			}

		}

		void printRemainingTileMapNeighbors()
		{
			std::cout << "!! -> Printing out remaining neighbor counts per tile..." << std::endl;
			for (auto& currentTile : tileMap)
			{
				EnclaveKeyDef::Enclave2DKey currentTileKey = currentTile.first;
				currentTileKey.printKey();
				int currentRemainingNeighbors = currentTile.second.getNumberOfRemainingLocalNeighbors();
				std::cout << " -> " << currentRemainingNeighbors << std::endl;
			}
		}

		void buildTileGroupingsV2()
		{
			int currentTileGrouping = 0;
			while (!tileMap.empty())
			{
				NoiseGridSectorGroupBuilderV2 newBuilder(&tileMap);
				newBuilder.start();
				auto currentPassKeys = newBuilder.fetchClusterTileKeys();

				NoiseGridSectorGrouping currentGrouping;
				for (auto& currentKey : currentPassKeys)
				{
					currentGrouping.insertTile(currentKey, tileMap[currentKey]);
					tileMap.erase(currentKey);
				}

				tileGroupings[currentTileGrouping] = currentGrouping;
				currentTileGrouping++;
			}

			// After all tile groupings are built, find their neighboring sectors (if any exist for a grouping)
			for (auto& currentGeneratedGrouping : tileGroupings)
			{
				currentGeneratedGrouping.second.findNeighboringSectors(currentGeneratedGrouping.first);
			}
		}

		// Below: Step 3: generate tileGroupings
		void buildTileGroupings()
		{
			int currentTileGrouping = 0;

			bool useKeyChain = false;						// set to true, when needing to look at a specific key in the next while loop iteration.
			EnclaveKeyDef::Enclave2DKey nextKeyInChain;		// when useKeyChain is set to true, this would represent the next key to look at 
															// in the next iteration of the while loop.

			bool switchToNextGroupAtIterationEnd = false;	// Set this to true, when the number of remaining neighbors is 0 for both hte currentTileKey and 
															// currentTileNeighborKey, in the else/if codition found in the while loop below.

			// loop until all of the tiles have been cleared out of the tileMap
			while (!tileMap.empty())
			//for (int x = 0; x < 4; x++)
			{
				// Below: debug only -- print out remainders per pass.
				//std::cout << "*********************************!! STARTING LOOP FOR GRID*****************************************" << std::endl;
				//printRemainingTileMapNeighbors();
				// The very first iteration of this while loop should always enter here, but other iterations may go somewhere else.

				if (useKeyChain == false)
				{
					//std::cout << "Entered branch 1." << std::endl;

					// Iterate through the entire map, until we find one of the situations below, then break after the first encountered scenario.
					for (auto& currentTile : tileMap)
					{
						// DEBUG ONLY
						/*
						if (currentTileKeyCopy.equals(-2944, -1792))
						{
							std::cout << "!!!!! DEBUGGING FOR TILE: ";
							currentTileKeyCopy.printKey();
							std::cout << std::endl;

							std::cout << "Remaining local neighbors: " << currentTile.second.getNumberOfRemainingLocalNeighbors() << std::endl;

							currentTile.second.printNeighborCategoryMetadata(EnclaveKeyDef::Enclave2DKey(0, 0));

							int debugKeyHalt = 3;
							std::cin >> debugKeyHalt;
						}
						*/


						// if the current tile is stand alone -- i.e, it has no neighbors, create a new group and iterate to the next group.
						if (currentTile.second.getNumberOfRemainingLocalNeighbors() == 0)
						{
							std::cout << "##### Entered branch 1.1" << std::endl;
							int branch11halt = 3;
							std::cin >> branch11halt;

							tileGroupings[currentTileGrouping].insertTile(currentTile.first, currentTile.second);
							tileMap.erase(currentTile.first);
							currentTileGrouping++;
							break;
							//std::cout << "Exited branch 1.1" << std::endl;
						}

						// otherwise, there are neighbors to group up 
						else if (currentTile.second.getNumberOfRemainingLocalNeighbors() != 0)
						{
							//std::cout << "Entered branch 1.2" << std::endl;

							// This operation requires two keys: the 2d Key of the tile we are looking at,
							// and the key of the next neighbor in that list.
							EnclaveKeyDef::Enclave2DKey currentTileKey = currentTile.first;
							EnclaveKeyDef::Enclave2DKey currentTileNeighborKey = currentTile.second.getNextNeighborKey();

							// Decrement both neighbors by one, passing each other's neighbor keys as arguments.
							tileMap[currentTileKey].removeDiscoveredNeighborKey(currentTileNeighborKey);
							tileMap[currentTileNeighborKey].removeDiscoveredNeighborKey(currentTileKey);

							// Next, determine which tile(s) will have to be moved into the current tileGrouping.
							std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> tilesToMove;
							bool currentTileKeyHas0Neighbors = false;
							int currentTileRemainingNeighbors = tileMap[currentTileKey].getNumberOfRemainingLocalNeighbors();
							if (currentTileRemainingNeighbors == 0)
							{
								currentTileKeyHas0Neighbors = true;
								tilesToMove.insert(currentTileKey);
							}

							// If there are neighbors left in the current tile being analyzed, analyze it again
							else
							{
								useKeyChain = true;
								nextKeyInChain = currentTileKey;
							}

							bool currentTileNeighborKeyHas0Neighbors = false;
							int neighboringTileRemainingNeighbors = tileMap[currentTileNeighborKey].getNumberOfRemainingLocalNeighbors();
							if (neighboringTileRemainingNeighbors == 0)
							{
								currentTileNeighborKeyHas0Neighbors = true;
								tilesToMove.insert(currentTileNeighborKey);
							}

							// neighbors override the original tile logic here, when determining the next key in the chain to use
							else
							{
								useKeyChain = true;
								nextKeyInChain = currentTileNeighborKey;
							}

							// Take the values of tilesToMove, and insert the corresponding tiles into the current grouping; 
							// once moved into the current tile grouping, they must be erased.
							for (auto& currentTileToMove : tilesToMove)
							{
								tileGroupings[currentTileGrouping].insertTile(currentTileToMove, tileMap[currentTileToMove]);
								tileMap.erase(currentTileToMove);
							}

							// If the number of remaining neighbors in the currentTileKey and currentTileNeighborKey is 0, there are no more 
							// links in the chain that can possibly be done, so we can iterate to the next group. We must iterate 
							// currentTileGrouping by 1, and reset the value of useKeyChain
							if (currentTileRemainingNeighbors == 0 && neighboringTileRemainingNeighbors == 0)
							{
								useKeyChain = false;
								currentTileGrouping++;
							}

							//std::cout << "Exiting branch 1.2" << std::endl;
							break;
						}
					}
				}

				else if (useKeyChain == true)
				{
					//std::cout << "Entered branch 2." << std::endl;
					// if we are continuing in a chain, use value of the nextEnclaveKeyForChain.
					EnclaveKeyDef::Enclave2DKey currentTileKey = nextKeyInChain;
					EnclaveKeyDef::Enclave2DKey currentTileNeighborKey = tileMap[nextKeyInChain].getNextNeighborKey();

					bool debugFlag = false;
					/*
					if (
						
						currentTileKey.equals(-2944, -1792)
						||
						currentTileKey.equals(-3072, -1792)
						||
						currentTileKey.equals(-3072, -1920)
						
						)
					{
						std::cout << "!!!!! DEBUGGING FOR TILE: ";
						currentTileKey.printKey();
						std::cout << std::endl;

						std::cout << "Remaining local neighbors: " << tileMap[nextKeyInChain].getNumberOfRemainingLocalNeighbors() << std::endl;
						std::cout << "First key to scan in sequence: "; currentTileNeighborKey.printKey(); std::cout << std::endl;

						tileMap[nextKeyInChain].printNeighborCategoryMetadata(EnclaveKeyDef::Enclave2DKey(0, 0));

						debugFlag = true;

						int debugKeyHalt = 3;
						std::cin >> debugKeyHalt;
					}
					*/

					// Decrement both neighbors by one, passing each other's neighbor keys as arguments.
					tileMap[currentTileKey].removeDiscoveredNeighborKey(currentTileNeighborKey);
					tileMap[currentTileNeighborKey].removeDiscoveredNeighborKey(currentTileKey);

					// Next, determine which tile(s) will have to be moved into the current tileGrouping.
					std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> tilesToMove;
					bool currentTileKeyHas0Neighbors = false;
					int currentTileRemainingNeighbors = tileMap[currentTileKey].getNumberOfRemainingLocalNeighbors();
					if (currentTileRemainingNeighbors == 0)
					{
						currentTileKeyHas0Neighbors = true;
						tilesToMove.insert(currentTileKey);
					}

					// If there are neighbors left in the current tile being analyzed, analyze it again
					else
					{
						useKeyChain = true;
						nextKeyInChain = currentTileKey;
					}

					bool currentTileNeighborKeyHas0Neighbors = false;
					int neighboringTileRemainingNeighbors = tileMap[currentTileNeighborKey].getNumberOfRemainingLocalNeighbors();

					if (debugFlag)
					{
						std::cout << "Remaining locals for neighbor key "; currentTileNeighborKey.printKey();
						std::cout << ": " << neighboringTileRemainingNeighbors << std::endl;
						std::cout << "Those neighbors are: " << std::endl;
						tileMap[currentTileNeighborKey].printNeighborCategoryMetadata(EnclaveKeyDef::Enclave2DKey(0,0));

						int localNeighborDebug1 = 3;
						std::cin >> localNeighborDebug1;
					}


					if (neighboringTileRemainingNeighbors == 0)
					{
						currentTileNeighborKeyHas0Neighbors = true;
						tilesToMove.insert(currentTileNeighborKey);
					}

					// neighbors override the original tile logic here, when determining the next key in the chain to use
					else
					{
						useKeyChain = true;
						nextKeyInChain = currentTileNeighborKey;
					}

					if (debugFlag)
					{
						std::cout << "++++Final remaining local count for currently operated-on tile: " << currentTileRemainingNeighbors << std::endl;
						tileMap[currentTileKey].printNeighborCategoryMetadata(EnclaveKeyDef::Enclave2DKey(0, 0));
						std::cout << "++++Next key in chain is: "; nextKeyInChain.printKey();

						int localNeighborDebug1 = 3;
						std::cin >> localNeighborDebug1;
					}


					// Take the values of tilesToMove, and insert the corresponding tiles into the current grouping; 
					// once moved into the current tile grouping, they must be erased.
					for (auto& currentTileToMove : tilesToMove)
					{
						tileGroupings[currentTileGrouping].insertTile(currentTileToMove, tileMap[currentTileToMove]);
						tileMap.erase(currentTileToMove);
					}

					// If the number of remaining neighbors in the currentTileKey and currentTileNeighborKey is 0, there are no more 
					// links in the chain that can possibly be done, so we can iterate to the next group. We must iterate 
					// currentTileGrouping by 1, and reset the value of useKeyChain
					if (currentTileRemainingNeighbors == 0 && neighboringTileRemainingNeighbors == 0)
					{
						std::cout << "!!! Tile members prior to switch: " << std::endl;
						tileGroupings[currentTileGrouping].printTileMembers();

						std::cout << "!!! Original tile member pool prior to switch: " << std::endl;
						for (auto& currentTile : tileMap)
						{
							std::cout << "Current pooled tile: ";
							EnclaveKeyDef::Enclave2DKey tileKeyCopy = currentTile.first;
							tileKeyCopy.printKey();
							std::cout << ", remaining locals: " << currentTile.second.getNumberOfRemainingLocalNeighbors() << std::endl;

							//std::cout << "Number of remaining locals in currently pooled tile: "currentTile.second.getNumberOfRemainingLocalNeighbors();
						}


						std::cout << "!!!! Switching to next tile grouping; last key was: ";
						currentTileKey.printKey();
						std::cout << std::endl;

						int switchCheck = 3;
						std::cin >> switchCheck;

						useKeyChain = false;
						currentTileGrouping++;
					}
					//break;
				}
			}

			// After all tile groupings are built, find their neighboring sectors (if any exist for a grouping)
			for (auto& currentGeneratedGrouping : tileGroupings)
			{
				currentGeneratedGrouping.second.findNeighboringSectors(currentGeneratedGrouping.first);
			}

		}

		void findTileGroupingNeighboringSectors()
		{
			for (auto& currentTile : tileGroupings)
			{

			}
		}

		void printTileGroupings()
		{
			for (auto& currentGrouping : tileGroupings)
			{
				std::cout << "Printing underlying data for tileGrouping at index " << currentGrouping.first << ": " << std::endl;
				currentGrouping.second.printTileMembers();
			}
			
		}

};

// NGSClusterRunState: Used to determine whether or not scanning should continue in a NoiseGridScanCluster.
enum class NGSClusterRunState
{
	SHOULD_CONTINUE,
	STOP_LIMIT_REACHED,
	STOP_WORK_DONE,
	STATE_NO_VAL
};

// NoiseGridScanCluster: similiar in it's behavior to NoiseGridSectorGroupBuilderV2, 
// but designed to be used by NoiseGridScanAttempt.
class NoiseGridScanCluster
{
	public:


		NGSCScanTargetGrouping fetchNextTargetGroupingFromReferenced()
		{
			NGSCScanTargetGrouping returnGrouping;
			
			for (auto& currentSectorClusterEntry : sectorClusterMap)
			{
				if (currentSectorClusterEntry.second.getState() == NGSCSectorSuperStatus::NGSC_SS_NOT_ALL_USED)
				{
					NGSCScanTargetGrouping newGrouping(currentSectorClusterEntry.first, currentSectorClusterEntry.second.getNextReferencedClusterGroupingId());
					returnGrouping = newGrouping;
					break;
				}
			}

			return returnGrouping;
		}

		void insertSectorAsScanned(EnclaveKeyDef::Enclave2DKey in_scannedSectorKey, int in_groupingId, int in_numberOfTilesInGroup)
		{
			//NGSClusterEntry scannedCluster(NGSCGroupingStatus::GROUPING_USED, in_groupingId, in_numberOfTilesInGroup);
			//sectorClusterMap[in_scannedSectorKey] = scannedCluster;


			sectorClusterMap[in_scannedSectorKey].insertClusterEntry(NGSCGroupingStatus::GROUPING_USED, in_groupingId, in_numberOfTilesInGroup);

			// Add the number of tiles in the group we're adding, to the currentTileCount.
			currentTileCount += in_numberOfTilesInGroup;
			
		}

		void resetCurrentTileCount()
		{
			currentTileCount = 0;
		}

		int fetchCurrentTileCount()
		{
			return currentTileCount;
		}

		void attemptToInsertSectorAsReferenced(EnclaveKeyDef::Enclave2DKey in_scannedSectorKey, int in_groupingId, int in_numberOfTilesInGroup)
		{

			//NGSClusterEntry scannedCluster(NGSCGroupingStatus::GROUPING_REFERENCED, in_groupingId, in_numberOfTilesInGroup);

			//auto clusterEntryFinder = sectorClusterMap.find(in_scannedSectorKey);

			// A sector may only be entered as reference it didn't exist in the map already.
			//if (clusterEntryFinder == sectorClusterMap.end())
			//{
				//sectorClusterMap[in_scannedSectorKey] = scannedCluster;
			//}

			sectorClusterMap[in_scannedSectorKey].insertClusterEntry(NGSCGroupingStatus::GROUPING_REFERENCED, in_groupingId, in_numberOfTilesInGroup);
		}

		void setSizeLimit(int in_sizeLimit)
		{
			sizeLimit = in_sizeLimit;
		}

		void printClusterData()
		{
			std::cout << "!! Printing out data of this NoiseGridScanCluster: " << std::endl;
			for (auto& currentCluster : sectorClusterMap)
			{
				std::cout << "Sector Key: ";
				EnclaveKeyDef::Enclave2DKey keyCopy = currentCluster.first;
				keyCopy.printKey();
				currentCluster.second.printEntry();
			}
		}

		PerlinClusterMeta fetchProducedGroupings()
		{
			PerlinClusterMeta returnGroupings;
			for (auto& currentCluster : sectorClusterMap)
			{
				//NGSCScanTargetGrouping currentGrouping(currentCluster.first, currentCluster.second.targetGroupingId);
				returnGroupings.insertGrouping(currentCluster.first, currentCluster.second);
			}


			return returnGroupings;
		}

		bool isTileCountWithinLimit()
		{
			bool isWithinLimit = true;
			if (!(currentTileCount <= sizeLimit))
			{
				isWithinLimit = false;
			}

			return isWithinLimit;
		}

		NGSClusterRunState fetchRunState()
		{
			NGSClusterRunState runState = NGSClusterRunState::STATE_NO_VAL;

			// If it's beyond the limit, set as STOP_LIMIT_REACHED
			if (!isTileCountWithinLimit())
			{
				runState = NGSClusterRunState::STOP_LIMIT_REACHED;
			}

			// If we're all done, set as STOP_WORK_DONE
			else if (getNumberOfReferencedClusterEntries() == 0)
			{
				runState = NGSClusterRunState::STOP_WORK_DONE;
			}

			// Otherwise, there is more work to do.
			else
			{
				runState = NGSClusterRunState::SHOULD_CONTINUE;
			}

			return runState;
		}

		int getNumberOfReferencedClusterEntries()
		{
			int totalEntriesAsReferenced = 0;
			for (auto& currentEntry : sectorClusterMap)
			{
				if (currentEntry.second.getState() == NGSCSectorSuperStatus::NGSC_SS_NOT_ALL_USED)
				{
					totalEntriesAsReferenced++;
				}
			}

			return totalEntriesAsReferenced;
		}

		void printRunAttemptData()
		{
			auto retrievedState = fetchRunState();
			std::string stateString = "";
			switch (retrievedState)
			{
				case NGSClusterRunState::STOP_LIMIT_REACHED:
				{
					stateString = "STOP_LIMIT_REACHED";
					break;
				}

				case NGSClusterRunState::STOP_WORK_DONE:
				{
					stateString = "STOP_WORK_DONE";
					break;
				}
			}

			std::cout << "++ Final run state: " << stateString << std::endl;

			// Calculate the total number of tiles overall from sectorClusterMap
			int totalTilesOverall = 0;
			for (auto& currentClusterEntry : sectorClusterMap)
			{
				totalTilesOverall += currentClusterEntry.second.getTotalTiles();
			}

			std::cout << "++ Total tiles in cluster: " << totalTilesOverall << std::endl;
			std::cout << "++ Tile limit: " << sizeLimit << std::endl;

		}

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NGSClusterEntry, EnclaveKeyDef::KeyHasher>  sectorClusterMap;
		int currentTileCount = 0;
		int sizeLimit = 700;
};


class NoiseGrid;	// forward declaration required for NoiseGridScanAttempt

// NoiseGridScanAttempt: meant to be used by NoiseGridScanner, when attempting to determine the next sector to "jump" to
// from the current NoiseGridSectorGrouping that is being operated on. 
class NoiseGridScanAttempt
{
	public:
		NoiseGridScanAttempt() {};
		NoiseGridScanAttempt(NoiseGrid* in_scanAttemptGridPtr,
							 NoiseGridSector* in_currentScanSectorPtr,
							 NoiseGridSectorGroupingFetcher in_currentScanSectorGroupingPtr):
			       scanAttemptGridPtr(in_scanAttemptGridPtr),
			       currentScanSectorPtr(in_currentScanSectorPtr),
			       currentScanSectorFetchedGrouping(in_currentScanSectorGroupingPtr)
		{}

		void runScan();

		void setupNextScanIteration();

		NGSClusterRunState getFinalRunState();

		PerlinClusterMeta fetchGeneratedMeta();

		int getFinalTileCount();

		void resetAttemptClusterTileCount();

	private:
		NoiseGrid* scanAttemptGridPtr = nullptr;
		NoiseGridSector* currentScanSectorPtr = nullptr;
		NoiseGridSector* currentNeighboringScanSectorPtr = nullptr;
		NoiseGridSectorGroupingFetcher currentScanSectorFetchedGrouping;

		NoiseGridScanCluster attemptCluster;

		PerlinClusterMeta generatedClusterMeta;	// produced at the end of runScan(), if the attempt resulted in a valid finalRunState
												// (aka, a value of NGSClusterRunState::STOP_WORK_DONE).

		NGSClusterRunState finalRunState = NGSClusterRunState::STATE_NO_VAL;	// the final state that comes from the attemptCluster,
																				// after it makes its attempt.

		int finalTileCount = 0;	 // stores the total number of tiles detected in the attempt
};


// NoiseGridScanner: called by NoiseGrid::populateSector, this populates the initial sector (the one that is being populated),
// and then uses the avaiable NoiseGridSectorGrouping objects found in the sector to perform jumping to other sectors and "glueing"
class NoiseGridScanner
{
	public:
		NoiseGridScanner() {};
		
		void setGrid(NoiseGrid* in_gridPtr)
		{
			gridPtr = in_gridPtr;
		}

		void start(EnclaveKeyDef::Enclave2DKey in_startSectorKey);
		std::vector<PerlinClusterMeta> fetchClusterMetaVector();
	private:
		NoiseGrid* gridPtr = nullptr;
		std::vector<PerlinClusterMeta> generatedClusterMetaVector;	// one instance of PerlinClusterMeta per generated cluster;
																	// a scan can generate more thean one PerlinClusterMeta.
};

// NoiseGridSubSector: can be used on an existing sector that has been generated and has its tile groupings available; will return a subset of 
// tiles within the sector. The sub sector root coord must be given, along with the number of sub sector tiles per dim and the dim per tile
// in the sector being looked at; the in_subSecotrTilesPerDim arg must be a multiple of 2 and also less than tiles per dim of the sector being looke at.
// The value of in_subSectorTileDim needs to be equal to the value of noiseGridTileDim from the NoiseGridSector being looked at. 
class NoiseGridSubSector
{
	public:
		NoiseGridSubSector() {};
		NoiseGridSubSector(EnclaveKeyDef::Enclave2DKey in_subSectorRootCoord, int in_subSectorTilesPerDim, int in_subSectorTileDim) :
			subSectorRootCoord(in_subSectorRootCoord),
			subSectorTilesPerDim(in_subSectorTilesPerDim),
			subSuctorTileDim(in_subSectorTileDim)
		{}

		void insertTile(EnclaveKeyDef::Enclave2DKey in_subSectorTileCoord, NoiseGridTile in_tileToInsert)
		{
			subSectorTiles[in_subSectorTileCoord] = in_tileToInsert;
		}

		void printSubSectorCoutArt()
		{
			// create horizontal header set < --- > 
			std::set<int> horizontalHeader;
			for (int x = 0; x < subSectorTilesPerDim; x++)
			{
				int currentXValue = subSectorRootCoord.a + (x * subSuctorTileDim);
				horizontalHeader.insert(currentXValue);
			}

			/*
			std::cout << "Horizontal header order: " << std::endl;
			for (auto& currentHorizontalValue : horizontalHeader)
			{
				std::cout << currentHorizontalValue << std::endl;
			}
			*/

			// create vertical header set ^
			std::set<int> verticalHeader;
			for (int z = 0; z < subSectorTilesPerDim; z++)
			{
				int currentZValue = subSectorRootCoord.b + (z * subSuctorTileDim);
				verticalHeader.insert(currentZValue);
			}


			//std::cout << "Size of horizontal header: " << horizontalHeader.size() << std::endl;
			std::cout << "Printing out hit table for subsector at: ";
			subSectorRootCoord.printKey();
			std::cout << std::endl;

			std::cout << std::setw(10) << "+";
			for (auto& currentHorizontalValue : horizontalHeader)
			{
				std::cout << std::setw(10) << std::to_string(currentHorizontalValue) + "|";
			}
			std::cout << std::endl;


			int totalTilesFlagged = 0;

			for (auto revVertIter = verticalHeader.rbegin(); revVertIter != verticalHeader.rend(); revVertIter++)
			{
				//std::cout << std::setw(10) << *revVertIter << std::endl;
				std::cout << std::setw(10) << *revVertIter;

				for (auto& currentHorizontalValue : horizontalHeader)
				{
					int currentX = currentHorizontalValue;
					int currentZ = *revVertIter;
					EnclaveKeyDef::Enclave2DKey keyToFind(currentX, currentZ);
					bool wasKeyFound = false;
					//wasKeyFound = doesTileExistInGroupings(keyToFind);

					
					auto wasKeyFoundInCurrentGrouping = subSectorTiles.find(keyToFind);
					if (wasKeyFoundInCurrentGrouping != subSectorTiles.end())
					{
						wasKeyFound = true;
					}
					

					std::string searchResult = ".";
					if (wasKeyFound)
					{
						searchResult = "X";
						totalTilesFlagged++;
					}

					std::cout << std::setw(10) << searchResult;
				}
				std::cout << std::endl;
			}

			std::cout << "Total subsector tiles flagged: " << totalTilesFlagged << std::endl;

		}

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> subSectorTiles;
		EnclaveKeyDef::Enclave2DKey subSectorRootCoord;
		int subSectorTilesPerDim = 0;
		int subSuctorTileDim = 0;
};



// NoiseGrid: a predefined grid of regular sectors (each of which contains tiles) that exist on a 2d grid, 
// of which the contents can be queried; can take multiple forms, and be set up in various ways. 
// Takes in 3 arguments: 1.) the dimension, measured in integer form, of each tile 2.) the size of the sectors in the grid
// and 3.) the threshold for culling tiles within sectors. The following steps should be taken, 
// when generating a new sector and its tiles, and wishing to apply some function that applies a single value to the tiles, prior to clipping:
//
// 1. Call generateSector.
// 2. Call generateSectorTiles.
// 3. TBD: Call the function that applies values to each tile in the sector
// 4. Call runClippingOnExistingSector, to apply the clipping threshold.
// 5. Generate the sector's tile sets (i.e, the local and remote neighbors)
// 6. Optional: remove tiles from the sector.
// 7: Generate the groupings.
class NoiseGrid
{
	public:
		friend class NoiseGridScanner;

		NoiseGrid() {};
		NoiseGrid(short in_tileDim,
			      short in_gSectorSize,
			      float in_thresholdValue,
				  int in_seedValue)
		{
			noiseGridTileDim = in_tileDim;
			gridSectorDim = in_gSectorSize;
			tilesPerDim = in_gSectorSize / in_tileDim;
			thresholdValue = in_thresholdValue;
			seedValue = in_seedValue;
		}

		void findSubSector(double in_coordinateX, double in_coordinateZ, int in_subSectorSize)
		{
			auto xKeys = findSubSectorCoord(in_coordinateX, in_subSectorSize);
			auto zKeys = findSubSectorCoord(in_coordinateZ, in_subSectorSize);

			int trueSectorXCoord = xKeys.a * gridSectorDim;
			int trueSectorZCoord = zKeys.a * gridSectorDim;

			int trueSubSectorXCoord = xKeys.b * in_subSectorSize;
			int trueSubSectorZCoord = zKeys.b * in_subSectorSize;

			int tilesPerSubSector = in_subSectorSize / noiseGridTileDim;

			std::cout << "~~~~~~~ Sub sector stats for coord: " << in_coordinateX << ", " << in_coordinateZ << std::endl;
			std::cout << "X coord: sector:" << trueSectorXCoord << " | subsector: " << trueSubSectorXCoord << std::endl;
			std::cout << "Z coord: sector:" << trueSectorZCoord << " | subsector: " << trueSubSectorZCoord << std::endl;
			std::cout << "Tiles per subsector: " << tilesPerSubSector << std::endl;

			EnclaveKeyDef::Enclave2DKey targetSector(trueSectorXCoord, trueSectorZCoord);
			EnclaveKeyDef::Enclave2DKey targetSubSectorRootCoord(trueSubSectorXCoord, trueSubSectorZCoord);

			NoiseGridSubSector targetSubSector(targetSubSectorRootCoord + targetSector, tilesPerSubSector, noiseGridTileDim);

			if (doesSectorExist(targetSector))
			{
				std::cout << "!! Found target sector, to get subsector for. " << std::endl;

				auto sectorPtr = getSectorRef(in_coordinateX, in_coordinateZ);

				int subSectorXStart = trueSectorXCoord + trueSubSectorXCoord;
				int subSectorZStart = trueSectorZCoord + trueSubSectorZCoord;

				std::cout << "!! Sub sector root tile coord will be: " << subSectorXStart << ", " << subSectorZStart << std::endl;


				for (int x = 0; x < tilesPerSubSector; x++)
				{
					for (int z = 0; z < tilesPerSubSector; z++)
					{
						int currentTileToScanX = subSectorXStart + (x * noiseGridTileDim);
						int currentTileToScanZ = subSectorZStart + (z * noiseGridTileDim);
						EnclaveKeyDef::Enclave2DKey currentKeyToScan(currentTileToScanX, currentTileToScanZ);

						std::cout << "Scanning for key: ";
						currentKeyToScan.printKey();
						std::cout << ": ";

						if (sectorPtr->doesTileExistInGroupings(currentKeyToScan))
						{
							std::cout << " Found ";
							auto fetchedTile = sectorPtr->fetchTileGromGroupings(currentKeyToScan);
							targetSubSector.insertTile(currentKeyToScan, fetchedTile);
						}
						else
						{
							std::cout << " Not found ";
						}
						std::cout << std::endl;

					}
				}

				targetSubSector.printSubSectorCoutArt();

			}
			else
			{
				std::cout << "!! Couldn't find sector for subsector!" << std::endl;
			}

		}

		EnclaveKeyDef::Enclave2DKey findSubSectorCoord(double in_coordinate, int in_subSectorSize)
		{
			int superSectorCoord = 0;
			int subSectorCoord = 0;

			float divide = in_coordinate / gridSectorDim;


			if (in_coordinate < 0)
			{
				// STEP 1: perform logic based on if coord is negative or positive.
				
				if ((divide < 0) && in_coordinate >= -gridSectorDim)
				{
					// as long as it stays within the first negative sector, it's -1.
					if (fmod(in_coordinate, gridSectorDim) != 0)
					{
						superSectorCoord = -1;
					}

					// ...otherwise for all cases, do a floor on the value to get the coordinate. (same as casting a float to int)
					else
					{
						superSectorCoord = int(divide);
					}
					
				}

				else if ((divide < 0) && in_coordinate < -gridSectorDim)
				{
					// if going beyond the first negative sector, check if it's on a precise border in the negative direction.
					// If it is at the border, it won't increment.
					// 
					// For example, -2048 against a gridSectorDim of 1024 results in 0, so the coord would be at -2.
					if (fmod(in_coordinate, gridSectorDim) == 0)
					{
						superSectorCoord = int(divide);
					}
					// ...otherwise, get the floor of the value, then subtract 1, to get the correct value.
					//
					// So if using -2049, the value would be at -3.
					else
					{
						superSectorCoord = int(divide) - 1;
					}
				}

				// STEP 2: get remainder
				float negModResult = float(fmod(in_coordinate, gridSectorDim));
				float sectorWidth = gridSectorDim;
				float truePositionInSector = sectorWidth + negModResult;

				subSectorCoord = int((fmod(truePositionInSector, gridSectorDim) / in_subSectorSize));

			}

			else if (in_coordinate >= 0)
			{
				// STEP 1: 
				// If we are in the positive direction, beyond the 0 sector...
				if ((divide > 0) && in_coordinate > gridSectorDim)
				{
					superSectorCoord = int(divide);
				}

				// STEP 2: get remainder
				subSectorCoord = int((fmod(in_coordinate, gridSectorDim) / in_subSectorSize));

			}

			return EnclaveKeyDef::Enclave2DKey(superSectorCoord, subSectorCoord);
		}

		int getSeedValue()
		{
			return seedValue;
		}

		EnclaveKeyDef::Enclave2DKey findGridTileSectorCoordinate(double in_coordinateX, double in_coordinateZ)
		{
			return EnclaveKeyDef::Enclave2DKey(NoiseGridUtils::findTileCoordinate(in_coordinateX, gridSectorDim), NoiseGridUtils::findTileCoordinate(in_coordinateZ, gridSectorDim));
		}

		EnclaveKeyDef::Enclave2DKey findAbsoluteTileCoordinate(double in_coordinateX, double in_coordinateZ)
		{
			return EnclaveKeyDef::Enclave2DKey(NoiseGridUtils::findTileCoordinate(in_coordinateX, noiseGridTileDim), NoiseGridUtils::findTileCoordinate(in_coordinateZ, noiseGridTileDim));
		}

		EnclaveKeyDef::Enclave2DKey findSectorRelativeCoordinatePair(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoordinate = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			EnclaveKeyDef::Enclave2DKey absoluteTileCoordinate = findAbsoluteTileCoordinate(in_coordinateX, in_coordinateZ);
			int relativeX = findSectorRelativeCoordinate(sectorCoordinate.a, absoluteTileCoordinate.a);
			int relativeY = findSectorRelativeCoordinate(sectorCoordinate.b, absoluteTileCoordinate.b);
			return EnclaveKeyDef::Enclave2DKey(relativeX, relativeY);
		}

		NoiseGridSector* getSectorRef(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			return &sectorMap[sectorCoord];
		}

		void generateSector(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);

			std::cout << "!!! generateSector: original values to generate from: " << in_coordinateX << ", " << in_coordinateZ << std::endl;
			std::cout << "!!! generateSector: new sector coord is: "; sectorCoord.printKey(); std::cout << std::endl;

			if (!doesSectorExist(sectorCoord))
			{
				NoiseGridSector newSector(sectorCoord, gridSectorDim, noiseGridTileDim, tilesPerDim);
				sectorMap[sectorCoord] = newSector;
			}
		}

		void generateSectorTiles(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].generateTilesForSector();
			}
		}

		void generateSamplesForSector(double in_coordinateX, double in_coordinateZ, int in_seedValue)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].generateSamples(in_seedValue);
			}
		}

		void runClippingOnExistingSector(double in_coordinateX, double in_coordinateZ, float in_clippingValue)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].runClipping(in_clippingValue);
			}
		}

		void removeTileFromSector(double in_sectorCoordX, double in_sectorCoordZ, double in_tileCoordX, double in_tileCoordZ)
		{
			std::cout << "!!! calling removeTileFromSector..." << std::endl;
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_sectorCoordX, in_sectorCoordZ);
			if (doesSectorExist(sectorCoord))
			{
				std::cout << "!!! removeTileFromSector: attempting to delete tile from sector..." << std::endl;

				EnclaveKeyDef::Enclave2DKey sectorTileCoord(in_tileCoordX, in_tileCoordZ);
				sectorMap[sectorCoord].removeTile(in_tileCoordX, in_tileCoordZ);
			}
		}

		void generateSectorTilesets(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].createSectorTileSets();
			}
		}

		void generateSectorGroupings(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].createSectorGroupings();
			}
		}

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* getSectorTileMapPtr(double in_coordinateX, double in_coordinateZ)
		{
			std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* fetchedPtr = nullptr;
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			std::cout << "!!! Checking sector existence..." << std::endl;
			if (doesSectorExist(sectorCoord))
			{
				std::cout << "!!! Sector found..." << std::endl;
				fetchedPtr = sectorMap[sectorCoord].getTileMapRef();
				std::cout << "!!! Size of refed tileMap: " << fetchedPtr->size() << std::endl;
			}
			return fetchedPtr;
		}

		void printNeighborMetadataForTilesInSector(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].printNeighborsPerTile();
			}
		}

		void printSectorCoutArt(double in_coordinateX, double in_coordinateZ)
		{
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			if (doesSectorExist(sectorCoord))
			{
				sectorMap[sectorCoord].printMappingCoutArt();
			}
		}

		std::vector<PerlinClusterMeta> populateSector(double in_coordinateX, double in_coordinateZ)
		{
			std::vector<PerlinClusterMeta> returnMeta;


			// Step 1: call function that generates all the sector data (i.e, the groupings)
			// << call function here >>
			// 
			// Step 2: for each generated NoiseGridSectorGrouping in the NoiseGridSector that was generated,
			// generate a new NoiseGridSectorScan. A scan will need to be done for each NoiseGridSectorGrouping,
			// where the currently selected NoiseGridSectorGrouping is the "origin" of the scan.
			// 
			// NOTE: the number of tiles per NoiseGridSectorGrouping must go towards a counter "cap." To prevent infinite looping,
			//       the scan should stop if the limit is surpassed.
			//
			// Step 2.1: each neighboringSectors key in each NoiseGridSectorGrouping must be attempted as part of the scan.
			//           If the sector that a neighboringSectors key represents doesnt exist, it needs to be created, along with its
			//           NoiseGridSectorGrouping members.co
			//
			// Step 2.2: when a sector is created, it's NoiseGridSectorGroupings must be checked to see if it connects with the currently
			//           selected NoiseGridSectorGrouping that is the focus of the scan. If a NoiseGridSectorGrouping connects, the contents of it
			//           must be appended to the NoiseGridSectorScan; the tile counter must also be incremented by the number of tiles in the newly appended NoiseGridSectorGrouping.
			//           If there are still remaining keys in the newly appended NoiseGridSectorGrouping, that NoiseGridSectorGrouping will now be the focus.

			// Step 1: check if the sector exists
			EnclaveKeyDef::Enclave2DKey sectorCoord = findGridTileSectorCoordinate(in_coordinateX, in_coordinateZ);
			NoiseGridScanner newScanner;
			newScanner.setGrid(this);
			newScanner.start(sectorCoord);

			// fetch the produced vector of PerlinClusterMeta objects; 1 PerlinClusterMeta per instance of PerlinCluster will be needed.
			auto generatedMeta = newScanner.fetchClusterMetaVector();

			std::cout << "+++++++++++++++++++++++++++ Size of generatedMeta: " << generatedMeta.size() << std::endl;

			// Now, produce the un-populated PerlinClusters.

			// Note for below: if this is a completely fresh sector being generated, the very first currentMeta value below
			// should create a new sector file if it doesn't exist already; subsequent loops through will not need to create a new file, but will 
			// instead update the new file.
			for (auto& currentMeta : generatedMeta)
			{
				
				if (currentMeta.searchForOriginKey().wasOriginFound())
				{
					std::cout << "!! Found origin key!" << std::endl;
				}
				

				currentMeta.findAndPrintScanOriginKey();

				std::cout << "!! Printing cluster contents..." << std::endl;
				
				//PerlinCluster newCluster(this, currentMeta);
				//newCluster.printPerlinClusterMeta();
				//newCluster.produceHash();
				// 
				// Step 1: calculate the PerlinCluster's unique hash value. We can then use this to see if it exists in memory,
				// or write/update a sector file with the appropriate data.
				//
				// Step 2: Run the following two checks, after the cluster hash has been calculated:
				//
				// A.) Find the origin key of the currentMeta, and look for the sector file corresponding to that key. Check if the file exists, 
				//     AND if it exists, does it contain the PerlinCluster's unique hash?
				//
				// B.) Does a materialized PerlinCluster with the corresponding perlin cluster hash already exist in memory?
				//
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
			}

			returnMeta = generatedMeta;
			return returnMeta;
		}

		void generateSectorContents(EnclaveKeyDef::Enclave2DKey in_sectorToGenerateKey, bool in_printOutArt)
		{
			generateSector(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b);
			generateSectorTiles(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b);
			generateSamplesForSector(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b, 35);
			runClippingOnExistingSector(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b, thresholdValue);
			generateSectorTilesets(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b);
			generateSectorGroupings(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b);

			if (in_printOutArt)
			{
				printSectorCoutArt(in_sectorToGenerateKey.a, in_sectorToGenerateKey.b);
			}
		}

		bool doesSectorExist(EnclaveKeyDef::Enclave2DKey in_sectorKey)
		{
			bool exists = false;
			auto searchResult = sectorMap.find(in_sectorKey);
			if (searchResult != sectorMap.end())
			{
				exists = true;
			}
			return exists;
		}

		float getThresholdValue()
		{
			return thresholdValue;
		}

	private:
		short noiseGridTileDim = 0;	// the dim of a tile within a grid sector; must be less than or equal to the value of gridSectorDim, and have a modulus of 0 when used as the divisor when the dividend is gridSectorDim.
		short gridSectorDim = 0;	// the overall dim / length of an individual sector; all sectors in an instance of this class will have this same value.
		int tilesPerDim = 0;
		float thresholdValue = 0.0f;
		int seedValue = 0;

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridSector, EnclaveKeyDef::KeyHasher> sectorMap;

		//std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>

		int findSectorRelativeCoordinate(int in_gridTileSectorCoordinate, int in_absoluteTileCoordinate)
		{
			//std::cout << "-- in_gridTileSectorCoordinate value: " << in_gridTileSectorCoordinate << std::endl;
			//std::cout << "-- in_absoluteTileCoordinate value: " << in_absoluteTileCoordinate << std::endl;

			int relativeCoordinate = 0;

			if (in_gridTileSectorCoordinate == 0)
			{
				relativeCoordinate = in_absoluteTileCoordinate / noiseGridTileDim;
			}

			else if (in_gridTileSectorCoordinate > 0)
			{
				// For below: imagine a value of 1536 for the in_absoluteTileCoordinate, and 1024 for in_gridTileSectorCoordinate;
				// this yields a modulus of 512.
				int relativeModulus = in_absoluteTileCoordinate % in_gridTileSectorCoordinate;

				// Using same example, the below calculation will yield a value of 1.
				relativeCoordinate = relativeModulus / noiseGridTileDim;
			}

			else if (in_gridTileSectorCoordinate < 0)
			{
				// For the below condition, imagine a in_gridTileSectorCoordinate of -1024.
				//
				// if the noiseGridTile we're looking at is at the border of the first negative sector, going in the negative direction,
				// do this logic.
				if (in_gridTileSectorCoordinate == (gridSectorDim * -1))
				{
					relativeCoordinate = (gridSectorDim + in_absoluteTileCoordinate) / noiseGridTileDim;
				}

				// ...otherwise, for the belwo, imagine -2048, and a tile sector at -1280. In that case, we have:
				// (-1280 % (-2048 + 1024))
				// (-1280 % (-1024))
				// = (-256)
				//
				// We then take this modulus, add it to the gridSectorDim, then divide that value by the noiseGridTileDim, 
				// to get our coordinate
				else
				{
					int negModulus = (in_absoluteTileCoordinate % (in_gridTileSectorCoordinate + gridSectorDim));
					relativeCoordinate = (gridSectorDim + negModulus) / noiseGridTileDim;
				}
			}

			return relativeCoordinate;
		}

};



#endif
