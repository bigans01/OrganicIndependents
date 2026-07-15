#pragma once

#ifndef NOISEGRIDTILESAMPLINGFIELD_H
#define NOISEGRIDTILESAMPLINGFIELD_H

#include "NoiseGridTile.h"
#include "Perlin.h"

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
	// Below: required default constructor for copying into maps, such as in PerlinCluster's sectorSamplingField member.
	NoiseGridTileSamplingField() {};

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

#endif