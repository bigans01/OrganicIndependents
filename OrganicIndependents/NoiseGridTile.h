#pragma once

#ifndef NOISEGRIDTILE_H
#define NOISEGRIDTILE_H

// NoiseGridUtils: simply a class for utility functions that can be used independently; 
// not dependent on other classes.
class NoiseGridUtils
{
public:
	static int findTileCoordinate(double in_coordinateToTranslate, short in_dimension)
	{
		int calculatedCoordinate = 0;

		double coordinate_divide = (in_coordinateToTranslate / in_dimension);
		int coordinate_default = 0;

		// operations when the coordinate is less than 0
		if (in_coordinateToTranslate < 0)
		{
			// if the coordinate_divide is less than 0, but still within the first initial sector around 0,
			// we are in negative space, but also in the very first negative sector.
			if ((coordinate_divide < 0) && in_coordinateToTranslate >= (in_dimension * -1.0f))
			{
				// Below: we aren't at the border that is at -1
				if (fmod(in_coordinateToTranslate, in_dimension) != 0)
				{
					//std::cout << "NoiseGrid: case 1 hit." << std::endl;
					//std::cout << "in_coordinateToTranslate: " << in_coordinateToTranslate << std::endl;
					//std::cout << "coordinate_divide: " << coordinate_divide << std::endl;
					calculatedCoordinate = (in_dimension * -1.0f);
				}

				// Below: we are precisely at the negative border, so it will be -1 * in_dimension
				else
				{
					//std::cout << "NoiseGrid: case 2 hit." << std::endl;
					calculatedCoordinate = int(coordinate_divide) * in_dimension;
				}
			}

			// if the coordinate divide is less than 0, but we are beyond the first negative sector around 0,
			// do this.
			else if ((coordinate_divide < 0) && in_coordinateToTranslate < (in_dimension * -1.0f))
			{
				//std::cout << "NoiseGrid: entered negative branch 2." << std::endl;
				// Below: if we are beyond the first negative sector, and fmod is not 0, it means we're in the middle of some sector.
				// We find the sector we are in by taking the integer form of coordiante_divide and then subtracting the value of in_dimension.
				if (fmod(in_coordinateToTranslate, in_dimension) != 0)
				{
					//std::cout << "NoiseGrid: case 3 hit." << std::endl;
					//std::cout << "in_coordinateToTranslate: " << in_coordinateToTranslate << std::endl;
					//std::cout << "coordinate_divide: " << coordinate_divide << std::endl;
					calculatedCoordinate = (int(coordinate_divide) * in_dimension) - (in_dimension);
					//std::cout << "Value of calculatedCoordinate is now --> " << calculatedCoordinate << std::endl;
				}

				// Otherwise, fmod is 0; so we're at the border. I.e, if the coordinate is -2048 in a Grid that has a sector dim of 1024, we'd be at -2048 still, and not in the grid that starts at
				// -3072.
				else
				{
					//std::cout << "NoiseGrid: case 4 hit." << std::endl;
					calculatedCoordinate = (int(coordinate_divide) * in_dimension);
				}
			}
		}

		// operations when the coordinate is greater than 0
		else if (in_coordinateToTranslate > 0)
		{
			// if the coordinate_divide is greater than 0, but is beyond the initial positive sector around 0,
			// we are in positive space, but not in the very first positive sector. In this case,
			// just take the value of coordinate_divide and multiply it by in_dimension.
			if ((coordinate_divide > 0) && in_coordinateToTranslate > in_dimension)
			{
				calculatedCoordinate = (int(coordinate_divide) * in_dimension);
			}

			// if the divide is greater than 0, and the coordinate to translate is equal to the dimension
			else if ((coordinate_divide > 0) && in_coordinateToTranslate == in_dimension)
			{
				calculatedCoordinate = in_dimension;
			}
		}

		return calculatedCoordinate;
	}
};

// NoiseGridTileCorners: this contains four named variables of the type EnclaveKeyDef::Enclave2DKey,
// that should be initialized during instantiation; it is simply meant to store 4 values that represent
// a tile corner, but by giving each corner a name, so that they may be easily referenced/used later.
class NoiseGridTileCorners
{
	public:
		NoiseGridTileCorners(EnclaveKeyDef::Enclave2DKey in_negXnegZcorner,
			EnclaveKeyDef::Enclave2DKey in_posXnegZcorner,
			EnclaveKeyDef::Enclave2DKey in_posXposZcorner,
			EnclaveKeyDef::Enclave2DKey in_negXposZcorner) :
			negXnegZcorner(in_negXnegZcorner),
			posXnegZcorner(in_posXnegZcorner),
			posXposZcorner(in_posXposZcorner),
			negXposZcorner(in_negXposZcorner)
		{}

		EnclaveKeyDef::Enclave2DKey negXnegZcorner;
		EnclaveKeyDef::Enclave2DKey posXnegZcorner;
		EnclaveKeyDef::Enclave2DKey posXposZcorner;
		EnclaveKeyDef::Enclave2DKey negXposZcorner;
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
		{
		}

		short getTileLength() { return tileLengthPerCell; }
		EnclaveKeyDef::Enclave2DKey getTile2DKey() { return tileRootCoord; }

		// Below: subdivide takes the parameters of the current tile, and uses them to subdivide into smaller parts. The newly produced tiles
		// are then put into the newTiles map which is then returned.
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

		// generate local and remote neighbors
		void generateNeighborCategories(EnclaveKeyDef::Enclave2DKey in_parentSectorKey,
			short in_sectorDim,
			std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* in_tileMapRef)
		{
			// Below: generate the neighboring keys of the current tile; whether or not they are local/remote
			// will not be known yet.
			std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> uncategorizedSet;
			EnclaveKeyDef::Enclave2DKey negXNeighbor(tileRootCoord.a - tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey posXNeighbor(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey negZNeighbor(tileRootCoord.a, tileRootCoord.b - tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey posZNeighbor(tileRootCoord.a, tileRootCoord.b + tileLengthPerCell);

			// Insert each generated, uncategorized tile for iteration
			uncategorizedSet.insert(negXNeighbor);
			uncategorizedSet.insert(posXNeighbor);
			uncategorizedSet.insert(negZNeighbor);
			uncategorizedSet.insert(posZNeighbor);

			// Determine whether each uncategorized tile is either local or remote, and
			// store appropriately.

			for (auto& currentUncategorizedItem : uncategorizedSet)
			{
				// find the sector coordinate.
				int coordA = NoiseGridUtils::findTileCoordinate(currentUncategorizedItem.a, in_sectorDim);
				int coordB = NoiseGridUtils::findTileCoordinate(currentUncategorizedItem.b, in_sectorDim);
				EnclaveKeyDef::Enclave2DKey uncategorizedKey(coordA, coordB);

				//if (uncategorizedKey.equals(-3200, -1920))

				// if the sector coordinate of the tile is local, AND the tile we're looking for actually exists, store it in the localNeighborSet.
				// The neighboring tile will be local, if the current uncategorizedKey is the same as the parent sector key argument.
				if (uncategorizedKey == in_parentSectorKey)
				{
					// Remember: only insert the tile if it exists.
					auto existingTileSearch = (*in_tileMapRef).find(currentUncategorizedItem);
					if (existingTileSearch != (*in_tileMapRef).end())
					{
						localNeighborSet.insert(currentUncategorizedItem);
					}
				}

				// if it's remote, insert the remote sector + the currentUncategorizedItem. Note that we are not checking whether or not 
				// it exists, as a tile doesn't contain information about the neighbors in the sectors it doesn't belong to.
				else
				{
					remoteNeighborMap[uncategorizedKey] = currentUncategorizedItem;
				}
			}

		}

		// Below: return a copy of localNeighborSet; ensure this is called only after it has been generated!
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> getLocalNeighborSetCopy()
		{
			return localNeighborSet;
		}

		// Below: return a copy of the remote neighbor map.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> getRemoteNeighborMap()
		{
			return remoteNeighborMap;
		}

		// Below: print metadata about the neighbors of this current tile.
		void printNeighborCategoryMetadata(EnclaveKeyDef::Enclave2DKey in_parentSectorKey)
		{
			std::cout << "NoiseGridTile: printing out metadata..." << std::endl;
			std::cout << "NoiseGridTile coord is: ";
			tileRootCoord.printKey();
			std::cout << std::endl;

			// Print local neighbor data.
			std::cout << "Printing local neighbors in sector coordinate: ";
			in_parentSectorKey.printKey();
			std::cout << std::endl;

			for (auto& currentLocal : localNeighborSet)
			{
				EnclaveKeyDef::Enclave2DKey currentKey = currentLocal;
				currentKey.printKey();
				std::cout << std::endl;
			}

			// Print remote neighbor data.
			std::cout << "Printing remote neighbors..." << std::endl;
			for (auto& currentRemote : remoteNeighborMap)
			{
				EnclaveKeyDef::Enclave2DKey remoteSectorCoordinate = currentRemote.first;
				EnclaveKeyDef::Enclave2DKey remoteSectorTileCoord = currentRemote.second;

				std::cout << "Remote sector coord: ";
				remoteSectorCoordinate.printKey();
				std::cout << " | Remote sectore tile coord: ";
				remoteSectorTileCoord.printKey();
				std::cout << std::endl;
			}
		}


		// Return the first neighbor key found in set.
		EnclaveKeyDef::Enclave2DKey getNextNeighborKey() { return *localNeighborSet.begin(); }

		// Use the insert function for every discovered neighbor key found in the fetchNeighboringTiles() call.
		void insertDiscoveredNeighborKey(EnclaveKeyDef::Enclave2DKey in_discoveredNeighborKey) { localNeighborSet.insert(in_discoveredNeighborKey); }

		// Whenever a neighbor is actually found, we must remove it.
		void removeDiscoveredNeighborKey(EnclaveKeyDef::Enclave2DKey in_discoveredNeighborKey) { localNeighborSet.erase(in_discoveredNeighborKey); }

		// Get the number of remaining neighbors when a new attempt is made to find a neighbor; if this value is 0, there is notthing left to do with this tile,
		// or it had no neighbors to begin with.
		int getNumberOfRemainingLocalNeighbors() { return localNeighborSet.size(); }

		void printTileData()
		{
			std::cout << "NoiseGridTile data is: ";
			tileRootCoord.printKey();
			std::cout << "| Tile length: " << tileLengthPerCell << std::endl;
		}

		// Below: This function returns the corners of a tile, represented as integers, in a NoiseGridTileCorners object.
		NoiseGridTileCorners fetchTileCorners()
		{
			EnclaveKeyDef::Enclave2DKey negXnegZKey = tileRootCoord;
			EnclaveKeyDef::Enclave2DKey posXnegZKey(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey posXposZKey(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b + tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey negXposZKey(tileRootCoord.a, tileRootCoord.b + tileLengthPerCell);

			return NoiseGridTileCorners(negXnegZKey, posXnegZKey, posXposZKey, negXposZKey);
		}

		void printTileRootCoordAndCorners()
		{
			std::cout << "! Tile root coord: " << tileRootCoord.a << ", " << tileRootCoord.b << std::endl;

			EnclaveKeyDef::Enclave2DKey negXnegZKey = tileRootCoord;
			EnclaveKeyDef::Enclave2DKey posXnegZKey(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b);
			EnclaveKeyDef::Enclave2DKey posXposZKey(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b + tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey negXposZKey(tileRootCoord.a, tileRootCoord.b + tileLengthPerCell);

			std::cout << "! negXnegZKey: " << negXnegZKey.a << ", " << negXnegZKey.b << std::endl;
			std::cout << "! posXnegZKey: " << posXnegZKey.a << ", " << posXnegZKey.b << std::endl;
			std::cout << "! posXposZKey: " << posXposZKey.a << ", " << posXposZKey.b << std::endl;
			std::cout << "! negXposZKey: " << negXposZKey.a << ", " << negXposZKey.b << std::endl;
		}

		// Below: This returns the root tile corners of the neighboring corner tiles (in other words, the tiles that are not directly adjacent to the calling tile)
		NoiseGridTileCorners fetchNeighboringCornerTileRootCoords()
		{
			EnclaveKeyDef::Enclave2DKey negXnegZNeighbor(tileRootCoord.a - tileLengthPerCell, tileRootCoord.b - tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey posXnegZNeighbor(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b - tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey posXposZNeighbor(tileRootCoord.a + tileLengthPerCell, tileRootCoord.b + tileLengthPerCell);
			EnclaveKeyDef::Enclave2DKey negXposZNeighbor(tileRootCoord.a - tileLengthPerCell, tileRootCoord.b + tileLengthPerCell);

			return NoiseGridTileCorners(negXnegZNeighbor, posXnegZNeighbor, posXposZNeighbor, negXposZNeighbor);
		}

		void setSampledValue(float in_value) { sampledValue = in_value; }
		float getSampledValue() { return sampledValue; }

	private:
		EnclaveKeyDef::Enclave2DKey tileRootCoord;		// the "lower left" corner point of a tile; should be a regular integer on a grid that aligns 
		// with the value of tileLengthPerCell (i.e, x = 512, z = 0, when using a tile length of 512) 

		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> localNeighborSet;	// neighboring tiles that lie within the same sector that the tile itself sits in.

		// Below: this map stores neighbors that exist in another sector. The key of this map is the Enclave2DKey value of the neighboring sector, and the corresponding values
		// are the Enclave2DKey of the tile in that neighboring sector.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> remoteNeighborMap;

		short tileLengthPerCell = 0;	// the regular length between grid corner tile point (i.e, with 512, points would be at 0, 512, 1024, 1536)
		float sampledValue = 0.0f;		// needs to be set by setSampledValue; retrieved by getSampledValue(); this contains the actual value that was sampled
};

#endif