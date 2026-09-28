#pragma once

#ifndef PERLINCLUSTEROUTPUTBASE_H
#define PERLINCLUSTEROUTPUTBASE_H

#include "NoiseGridTileSamplingField.h"
#include "NoiseGridTile.h"


/* Description: TileToSectorLink stores metadata on the relationship between a local tile's data and it's corresponding absolute mirrors.
* The non-defauult constructor should be used when creating a new link, and the copy constructor should be used when doing oeprations
* such as copying to a map. The TileToSectorLink's non default constructor will store the value of the original sampling field 
* that the tile was referencing, prior to when it was translated. 
* 
* After the constructor has been called, the runSampling function can be called to perform a sampling operation on the corresponding field,
* and then store it. 
* 
* The runSamplingAttempt function of PerlinClusterOutputBase should return only TileToSectorLink objects that refer to valid points that can exist;
* an empty vector that is returned by this function means that no TileToSectorLink could be found to perform sampling on, which indicates the point 
* doesn't exist.

*/
class TileToSectorLink
{
	public:
		TileToSectorLink() {};
		TileToSectorLink(EnclaveKeyDef::Enclave2DKey in_localTileKey,
			EnclaveKeyDef::Enclave2DKey in_localSectorKey,
			EnclaveKeyDef::Enclave2DKey in_absoluteSectorKey,
			NoiseGridTileSamplingField* in_absoluteSectorRef) :
			localTileKey(in_localTileKey),
			localSectorKey(in_localSectorKey),
			absoluteSectorKey(in_absoluteSectorKey),
			absoluteSectorRef(in_absoluteSectorRef)
		{
		}

		EnclaveKeyDef::Enclave2DKey getLocalSectorKey()
		{
			return localSectorKey;
		}

		void printLink()
		{
			std::cout << "!!! ---> printing TileToSectorLink data..." << std::endl;
			std::cout << "> localTileKey: "; localTileKey.printKey(); std::cout << std::endl;
			std::cout << "> localSectorKey: "; localSectorKey.printKey(); std::cout << std::endl;
			std::cout << "> absoluteSectorKey: "; absoluteSectorKey.printKey(); std::cout << std::endl;
			std::cout << "> u/v coords: " << uvSamplingCoord.x << ", " << uvSamplingCoord.y << std::endl;
			std::cout << "> sampled value: " << sampledValue << std::endl;
		};

		// Below: assuming constructor is set up with valid points, this will 
		void runSampling(glm::vec2 in_uvSamplingCoord)
		{
			uvSamplingCoord = in_uvSamplingCoord;
			sampledValue = absoluteSectorRef->calculateBicubicInterpolation(in_uvSamplingCoord.x, in_uvSamplingCoord.y);
		}

		float getSampledValue() { return sampledValue; }

	private:

		EnclaveKeyDef::Enclave2DKey localTileKey;		// the value of the local tile key, BEFORE the calculated value of the relativeShiftingKey has been applied from the PerlinClusterOutputBase class.
		EnclaveKeyDef::Enclave2DKey localSectorKey;		// the value of the sector key that a local tile belongs to, BEFORE the calculated value of the relativeShiftingKey has been applied from the PerlinClusterOutputBase class.
		EnclaveKeyDef::Enclave2DKey absoluteSectorKey;  // the value of the absolute sector key that a local tile belongs to, which is determined AFTER the relativeShiftingKey has been calculated..
		NoiseGridTileSamplingField* absoluteSectorRef = nullptr;	// contains a pointer to the original sampling field that the tile was using, prior to any translation.
		glm::vec2 uvSamplingCoord;	//	the uv sampling coord, with valid ranges for x/z as being between 0 and 1.0f, that is used when runSampling is called.
		float sampledValue = 0.0f;	//  stores sampling value after runSampling is called.
};



/* Description: The base class where all the magic happens with PerlinCluster output operations. Derivatives of this class should be 
* instantiated and setup through the function PerlinCluster::generate().
*/

class PerlinClusterOutputBase
{
	public:
		PerlinClusterOutputBase();

		void initializeBase(int in_parentClusterSectorWidth, 
						    int in_parentClusterTileWidth,
							std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* in_parentPerlinClusterTilesRef,
							std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_parentTileToSectorMappingRef,
							std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTileSamplingField, EnclaveKeyDef::KeyHasher>* in_parentSectorSamplingFieldsRef);

	private:

		// Below: the relativeShiftingKey key is used to shift the tiles and produced blueprint values,
		// Depending on which item that is being shifted, this key is used in certain formulas of multiplication. The value of
		// parentGridSectorLength must come from the PerlinCluster instance that instantiates a derivative of this base class.
		// 
		// It is assumed that the corresponding parentClusterSectorWidth value is a multiple of 32 (the standard blueprint dimension), and is not less than 32.
		//
		// For tiles -> the value to shift by is equal to the X/Z dimension x parentGridSectorLength (the length of all tiles side-to-side for a sector in the NoiseGrid that this object is produced from)
		// For blueprints -> the valeu to shift by is equal to the X/Z dimension x (parentGridSectorLength / 32) (a standard-length 256 sector covers 8 blueprints; a 512-length covers 16)
		//
		// The value of this key should be equal to the inverse value of the dimension(s) closest to 0. 
		// For example, imagine a noise grid sector length of 1024. Pretend that the PerlinCluster touches the following sectors of the grid:
		//
		// 2048, 1024
		// 2048, 2048
		// 3072, 1024
		// 3072, 2048
		//
		// In this case, the closest to X value is 2048, and the closest to Z value is 1024. The relativeShiftingKey value would be
		// -2048, -1024.
		// 
		// This key is applied in the following way(s)
		//
		// For tiles: because root tile cooredinates are in absolute world space, apply the relativeShiftingKey to the tile's coordinate
		//            to get it's new local key value.
		//
		// For new points traced after shifting: determine the local sector that the local coordinate should appear in, then apply the inverse 
		//                                       of the relativeShiftingKey (ie, 2048, 1024) to get the actual true sampling field it should be looking at.
		//										 The localToAbsoluteSectorLookup member should store the local to absolute lookup values, where the "key" to lookup
		//										 is equal to the local sector value to use, and the "value" of the lookup is the absolute location of the sector coordinate.
		EnclaveKeyDef::Enclave2DKey relativeShiftingKey;

		EnclaveKeyDef::Enclave2DKey blueprintShiftbackKey;	// this key determines how to adjust the blueprints built in local space, back to their true blueprint coordinates.
															// It's value is based on dividing the a/b values of the inverse value of the relativeShiftingKey, by 32. Assumes that there can be no smaller noise grid sector size than the width of a blueprint (32).
															// 
															// For example, if the relativeShiftingKey is -1024, -512, the blueprint shiftback key is 1024 / 32 and 512 / 32, which equates to 32, 16.

		int parentClusterSectorWidth = 0;	// the unit length of a noise grid sector from the parent PerlinCluster's.
		int parentClusterTileWidth = 0;		// the unit length of an individual tile in the parent PerlinCluster's sectors.

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher>* parentPerlinClusterTilesRef = nullptr;
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* parentTileToSectorMappingRef = nullptr;
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTileSamplingField, EnclaveKeyDef::KeyHasher>* parentSectorSamplingFieldsRef = nullptr;

		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> localTiletoLocalSectorLookup; // key = local tile coord value after adjustment by relativeShiftingKey, value = local sector coord value after adjusted by relativeShiftingKey
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> localToAbsoluteTileLookup;	// key = local tile coord value after adjusted by relativeShiftingKey, value = original coord value prior to adjustment
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> localToAbsoluteSectorLookup; // key = local sector coord value after adjusted by relativeShiftingKey, value = original coord value prior to adjustment

		void buildRelativeShiftingKey();
		void calculateBlueprintShiftbackKey();	// call only after buildRelativeShiftingKey has been called.

		void buildLocalTiletoLocalSectorLookup();
		void buildLocalToAbsoluteTileLookup();
		void buildLocalToAbsoluteSectorLookup();

		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> checkIfPointCanExistInCluster(float in_coordX, float in_coordZ);

		std::vector<TileToSectorLink> runSamplingAttempt(float in_coordX, float in_coordZ);		// attempts to find all possible sampling combinations, by first checking
																								// if the point to sample has tiles it can exist in; then, attempt to produce
																								// a TileToSectorLink for each of the tiles that can possibly be sampled; each of the resulting 
																								// TileToSectorLink in the returning vector will need to have their corresponding runSamplin functions called,
																								// prior to the function returning.
		float findLocalBicubicInterpolationRange(float in_rangeValue, float in_sectorWidth);

		// Debug functions
		void printRelativeShiftingKey();
		void printBlueprintShiftbackKey();
		void printOriginalSamplingFields();
		void printLocalTileToLocalSectorLookup();
		void printLocalToAbsoluteSectorLookup();
		void printLocalToAbsoluteTileLookup();

};

#endif
