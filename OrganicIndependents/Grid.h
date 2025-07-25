#pragma once

#ifndef GRID_H
#define GRID_H

/* Description: contains classes related to grids. A GridLocation is a coordinate system that will correlate with and be generated
by an existing Grid object. */


// GridLocation: the contents of the values returned in this class are dependent on the implementation of the Grid
// It correlates with.

enum class NoiseGridZoomMode
{
	NOZOOMVAL,
	SMALLER_OR_EQUAL_TO_BLUEPRINT,    // indicates that the cells of the grid are either smaller or equivalent to the 32x32 size of a blueprint
	LARGER_THAN_BLUEPRINT,            // indicates that the cells of the grid are larger than the 32x32 size of a blueprint (i.e, the Grid is at the 
									  // Sector size, so 1024x1024)
};


class NoiseGridLocation
{
	public:
		NoiseGridLocation() {};
		NoiseGridLocation(NoiseGridZoomMode in_gZoomMode,
			         EnclaveKeyDef::EnclaveKey in_gSectorCoord,
			         EnclaveKeyDef::EnclaveKey in_absoluteGridCoord,
			         EnclaveKeyDef::EnclaveKey in_sectorRelativeGridCoord):

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
		EnclaveKeyDef::EnclaveKey gSectorCoord;
		EnclaveKeyDef::EnclaveKey absoluteGridCoord;
		EnclaveKeyDef::EnclaveKey sectorRelativeGridCoord;
};

// NoiseGridTile: a individual tile; has a dimension for X/Z, and a location on the grid; the dim value can be used to calculate
// the four corners of the tile, and then use seeding to uniquely give a vector to each corner.
class NoiseGridTile
{
	public:
		NoiseGridTile() {}
		NoiseGridTile(EnclaveKeyDef::Enclave2DKey in_tileDimCoord, short in_tileDim) :
			tileRootCoord(in_tileDimCoord),
			tileLengthPerCell(in_tileDim)
		{ }

		short getTileLength() { return tileLengthPerCell; }
		EnclaveKeyDef::Enclave2DKey getTile2DKey() { return tileRootCoord; }
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> subdivide(int in_cellsPerDim)
		{
			std::cout << "Generating subtiles..." << std::endl;

			std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> newTiles;
			short newTilesLengthPerCell = tileLengthPerCell / in_cellsPerDim;
			for (int x = 0; x < in_cellsPerDim; x++)
			{
				for (int z = 0; z < in_cellsPerDim; z++)
				{
					EnclaveKeyDef::Enclave2DKey newTileKey(tileRootCoord.a + (x * newTilesLengthPerCell), tileRootCoord.b + (z * newTilesLengthPerCell));
					NoiseGridTile newTile(newTileKey, newTilesLengthPerCell);
					newTile.printTileData();
					newTiles[newTileKey] = newTile;
				}
			}

			return newTiles;
		}

		// Return a set that indicates the keys of neighboring tiles, relative to this tile. This will need to be used
		// to scan for neighboring tiles.
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> fetchNeighboringTiles()
		{
			std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> neighborSet;
			EnclaveKeyDef::Enclave2DKey negXNeighbor(tileRootCoord.a - tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey posXNeighbor(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey negZNeighbor(tileRootCoord.a, tileRootCoord.b - tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey posZNeighbor(tileRootCoord.a, tileRootCoord.b + tileLengthPerCell);

			neighborSet.insert(negXNeighbor);
			neighborSet.insert(posXNeighbor);
			neighborSet.insert(negZNeighbor);
			neighborSet.insert(posZNeighbor);

			/*
			std::cout << "Printing neighbors..." << std::endl;
			std::cout << "negX: "; negXNeighbor.printKey(); std::cout << std::endl;
			std::cout << "posX: "; posXNeighbor.printKey(); std::cout << std::endl;
			std::cout << "negZ: "; negZNeighbor.printKey(); std::cout << std::endl;
			std::cout << "posZ: "; posZNeighbor.printKey(); std::cout << std::endl;
			*/

			return neighborSet;

		}

		// Use the insert function for every discovered neighbor key found in the fetchNeighboringTiles() call.
		void insertDiscoveredNeighborKey(EnclaveKeyDef::Enclave2DKey in_discoveredNeighborKey) { discoveredNeighborMap.insert(in_discoveredNeighborKey); }

		// Whenever a neighbor is actually found, we must remove it.
		void removeDiscoveredNeighborKey(EnclaveKeyDef::Enclave2DKey in_discoveredNeighborKey) { discoveredNeighborMap.erase(in_discoveredNeighborKey); }

		// Get the number of remaining neighbors when a new attempt is made to find a neighbor; if this value is 0, there is notthing left to do with this tile,
		// or it had no neighbors to begin with.
		int getNumberOfRemainingNeighbors() { return discoveredNeighborMap.size(); }

		void printTileData()
		{
			std::cout << "NoiseGridTile data is: ";
			tileRootCoord.printKey();
			std::cout << "| Tile length: " << tileLengthPerCell << std::endl;
		}

		void setSampledValue(float in_value) { sampledValue = in_value;  }
		float getSampledValue() { return sampledValue; }

	private:
		EnclaveKeyDef::Enclave2DKey tileRootCoord;		// the "lower left" corner point of a tile; should be a regular integer on a grid that aligns 
														// with the value of tileLengthPerCell (i.e, x = 512, z = 0, when using a tile length of 512) 

		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> discoveredNeighborMap;	// neighboring keys discovered during scanning should go into here;
																											// when the key is "consumed" it needs to be deleted from this map.
		short tileLengthPerCell = 0;	// the regular length between grid corner tile point (i.e, with 512, points would be at 0, 512, 1024, 1536)
		float sampledValue = 0.0f;		// needs to be set by setSampledValue; retrieved by getSampledValue(); this contains the actual value that was sampled
};



// NoiseGridTileSet: should contain all tiles within a certain range of a part of the NoiseGrid (i.e, range of x from 0 to 9 (10 tiles in x), and y = 5 (from 0 to 4)
class NoiseGridTileSet
{
	public:

	private:
};


// Grid: a predefined grid that can be queried; can take multiple forms, and be set up in various ways.
class NoiseGrid
{
	public:

	private:
};

#endif
