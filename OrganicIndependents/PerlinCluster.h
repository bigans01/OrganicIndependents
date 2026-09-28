#pragma once

#ifndef PERLINCLUSTER_H
#define PERLINCLUSTER_H

#include "PerlinClusterMeta.h"
#include "NoiseGridTile.h"
#include "Enclave2DKeyMapHasher.h"
#include "Perlin2DSectorMappings.h"
#include "Perlin.h"
#include "NoiseGridTileSamplingField.h"
#include "EnclaveCollectionBlueprint.h"
#include "PerlinClusterGeneratorEnum.h"

#include "PerlinClusterOutputBase.h"
#include "MountainPCO.h"

/*
* 
* PerlinClusterSectorPointSearch: used to find the value of a point within a sector; should only have setValue called on it if 
* actually finds an existing sector. See it's use in fetchBicubicallyInterpolatedCoordinateV2.

*/

class PerlinClusterSectorPointSearch
{
	public:
		PerlinClusterSectorPointSearch() {};
		void setValue(float in_foundValue)
		{
			foundValue = in_foundValue;
			wasPointSearchFound = true;
		}

		bool wasFound()
		{
			return wasPointSearchFound;
		}

		float getValue() { return foundValue; }

	private:
		bool wasPointSearchFound = false;
		float foundValue = 0.0f;
};


/* Description: PerlinClusterGenerationState is meant to describe whether or not
* the blueprint data produced by the PerlinCluster is actually sitting in memory.
* It is used by functions such as OSectorManager::checkProcessingColumn (see OrganicServerLib),
* in order to determine whether or not to generate the data of the PerlinCluster.
*/
enum class PerlinClusterGenerationState
{
	PERLIN_NOVAL,
	PERLIN_BASE,
	PERLIN_MATERIALIZED
};

/*
* 
* Description: PerlinClusterSectorOutput is meant to serve as a container of completed blueprints for a given 
* OSector.

*/
class PerlinClusterSectorOutput
{
	public:
		PerlinClusterSectorOutput() {};
		void setSectorOutputState(PerlinClusterSectorState3D in_stateToSet) { sectorOutputState = in_stateToSet; }
		PerlinClusterSectorState3D getSectorOutputState() { return sectorOutputState; }
		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> sectorOutputBlueprints;
	private:
		PerlinClusterSectorState3D sectorOutputState;
};


/* PerlinCluster: a PerlinCluster contains metadata about a scanned perlin mass, resulting from generated PerlinClusterMeta objects 
found in a call to NoiseGridScanner::start.

The PerlinCluster should have the ability to determine what is known as the perlin mass hash that uniquely identifies the mass.
This value should be able to be generated immediately within this class after it's second constructor below. This unique hash
value can then be applied to or checked against multiple sector files, to update them or determine if the PerlinCluster 
already exists in memory (assuming its mapped correctly).

Additionally, this class should have a pointer to what is known as a "PerlinMachine" derivative. PerlinMachine objects run off
of the data stored in the PerlinCluster, in order to produce PerlinClusterSectorOutput, which can be fetched by the function
getClusterOutputs(), once the PerlinMachine has produced its outputs. The PerlinMachine needs the following, at a minimum:

-references to the following: perlinClusterTiles, tileToSectorMapping, sectorSamplingFields
-dimensional values, such as: parentGridSectorLength, oSectorDimSize
-the seed value used to generate the grid
-optional message data (TBD, not ready for testing yet)

This class also contains facilities that allows it to determine items such as:

1.	the unique NoiseGrid sectors used by all tiles in this clusters; the "unique sectors" pertain to the parent NoiseGrid that the PerlinCluster originated from 
2.	a mapping of individual tiles to the NoiseGrid sectors they belong to (the tileToSectorMapping) member
3.	a separate NoiseGridTileSamplingField for each unique sector used by the PerlinCluster
4.	the ability to generate a Perlin2DSectorMappingContainer, which contains a std::vector<PerlinClusterSectorState>. A PerlinClusterSectorState
	contains the hash representing the cluster, a sector key representing the keyed OSector file to update, and the state to put into that OSector file.
	The key is used to determine where the producable PerlinClusterHashMeta goes; this PerlinClusterHashMeta comes from PerlinClusterSectorState::generaeteClusterHashMeta,
	and the PerlinClusterHashMeta is ultimately what goes in the OSector file.
5.	The ability to calculate a value via bicubic interpolation, when given a coordinate; this is done by using one of the NoiseGridTileSamplingField
	for a given sector, found in the sectorSamplingFields member. The 0 and 1 range used when querying a NoiseGridTileSamplingField can be calculated by
	using NoiseGridUtils::findTileCoordinate on the given cooredinate to find the target NoiseGridTileSamplingField to use, and then using the modulo of each
	operation to calculate localized offset value. Dividing the localized offset value by the length of a sector will produce a value between 0 and 1 for both X and Z,
	that can be used in when querying the target NoiseGridTileSamplingField.


TBD items:
6.	in order to simplify debugging and understanding of the PerlinCluster, and to avoid using high values, a method to translate tiles and their corresponding sectors
	to near-zero should occur; this will allow offsets and float/double values to remain low. (NOTE: as of 7/11/2026, this needs to be development and is still TBD)



*/
class PerlinCluster
{
	public:
		PerlinCluster() {};
		PerlinCluster(PerlinClusterMeta in_metaInfo,
					std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> in_perlinClusterTiles,
					int in_parentGridSectorLength,
					int in_gridSeedValue,
			        int in_oSectorDimSize,
			        int in_parentGridTileLength) :
			metaInfo(in_metaInfo),
			perlinClusterTiles(in_perlinClusterTiles),
			parentGridSectorLength(in_parentGridSectorLength),
			gridSeedValue(in_gridSeedValue),
			oSectorDimSize(in_oSectorDimSize),	// should always be a value of 256...if so, why initialize?
			parentGridTileLength(in_parentGridTileLength),
			currentClusterState(PerlinClusterGenerationState::PERLIN_BASE)
		{};

		void printPerlinClusterMeta();	// print the groupings per sector key.

		PerlinClusterOriginSearch runOriginSearch(); // attempt to find the origin sector key of the PerlinCluster; a call to wasOriginFound
													 // on the return value will return false if it wasn't found.

		bool isClusterValid();	// returns true if there are actual tiles in the cluster.

		std::string produceHash();		// produce the unique SHA256 hash of this cluster, through all tiles involved.

		void printClusterOutputArt();	// print the output art that involves all tiles that the cluster is involved with.

		Perlin2DSectorMappingContainer generateMappingContainer(); // generate and return the Perlin2DSectorMappingContainer of this PerlinCluster;
																   // the contents of the Perlin2DSectorMappingContainer can be used to update the OSector files 
																   // with the appropriate entries in their perlin cluster hash lookup table(s).

		std::vector<PerlinClusterSectorState3D> generateAffected3DSectorKeys();	// IN-DEVELOPMENT (8/15/2026): saved for later
																	
		void generateTileToSectorMappingsAndSamplingFields();	// generates the contents of tileToSectorMapping, which allows for the mapping of tiles to their sectors 
																// that they belong to, then generate the individual NoiseGridTileSamplingField(s) to use.

		void fetchBicubicallyInterpolatedCoordinate(float in_coordX, float in_coordZ);	// attempts to find a bicubically interpolated coordinate;
																						// this will fail if the estimated/calculated sector that the coordinate is in
																						// does not exist, and should only be called after sampling fields have been set up.

		PerlinClusterSectorPointSearch fetchBicubicallyInterpolatedCoordinateV2(float in_coordX, float in_coordZ);	// assuming that all corresponding mappings are setup after the call to
																													// generateTileToSectorMappingsAndSamplingFields, this can be used to return a point value in 
																													// a sampling field, if said field exists.

		PerlinClusterGenerationState fetchClusterState();

		void generate(PerlinClusterGeneratorEnum in_generatePlanEnum);	// IN-DEVELOPMENT (8/15/2026): generate the desired PerlinCluster form (i.e, mouintain, plains, forest, desert, etc).
																		// Currently only called by OSectorManager::checkProcessingColumn function (see OrganicServerLib)

		std::unordered_map<EnclaveKeyDef::EnclaveKey, PerlinClusterSectorOutput, EnclaveKeyDef::KeyHasher>* getClusterOutputs();	// IN-DEVELOPMENT (8/15/2026): fetch the map of PerlinClusterSectorOutput objects produced
																																	// as a result of calling the generate function.

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> perlinClusterTiles;					// stores all tiles invovled with the cluster; populated via non-default constructor.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> tileToSectorMapping;		// maps tiles to their corresponding sectors, before any translation of tiles/sectors occurs.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTileSamplingField, EnclaveKeyDef::KeyHasher> sectorSamplingFields;		// stores all unique sampling fields that could be used by the PerlinCluster.
																																		// The key of the sampling fields is the absolute root point of the starting sector, not the OSector file key
																																		// that the sector relates to. For example, a sector starting at X = 256, Z = 512 has a coord of 256, 512, not 1,2.
																																		//
																																		// Also remember that sampling field sizes may differ from the typical 256x256 area that an OSector file is supposed to cover;
																																		// For instance, a noise grid that has large sectors of 512x512 would cover 4 256x256 areas per field, so that would
																																		// Need to be accounted for.			
		PerlinClusterMeta metaInfo;		// stores the groupings from each sector that this PerlinCluster will use.

		PerlinClusterGenerationState currentClusterState = PerlinClusterGenerationState::PERLIN_NOVAL;	// keeps track of whether or not the blueprint data of the PerlinCluster has been generated.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, PerlinClusterSectorOutput, EnclaveKeyDef::KeyHasher> clusterOutputs;	// a map of 

		int parentGridSectorLength = 0;		// the length of a sector, based on the NoiseGrid that this PerlinClsuter originated from.
		int gridSeedValue = 0;				// the seed value from the NoiseGrid object that this PerlinCluster originated from. Needed when 
											// generating the sampling fields per sector.

		int oSectorDimSize = 0;	// the cubic size of an OSector file, that determines the number of blocks in each x/y/z dimension it could track; i.e, 256 would be 256^3.
								// It is required that this is compared against the parentGridSectorLength, when a PerlinMachine produces its outputs, and must be set on initialization.

		int parentGridTileLength = 0;	// stores the value passed down from PerlinFactory::populateSectorInGrid, which instantiates new instances of this class;
										// this value eventually gets passed down to the pco during it's call to initializeBase.

		std::shared_ptr<PerlinClusterOutputBase> pco;	// set up by the call to generate()
									

		void generateSamplingFieldLookups();	// populate the contents of sectorSamplingFields. Must be called before attempting to find 
												// bicubically interpolated values, and should be called immediately after generateTileToSectorMappingsAndSamplingFields.

		float findBicubicInterpolationRange(float in_rangeValue);  // find the range value between 0.00 and 1.00f, which is required for retreiving values from
																   // NoiseGridTileSamplingField.
};

#endif
