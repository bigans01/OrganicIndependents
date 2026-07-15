#pragma once

#ifndef PERLINCLUSTER_H
#define PERLINCLUSTER_H

#include "PerlinClusterMeta.h"
#include "NoiseGridTile.h"
#include "Enclave2DKeyMapHasher.h"
#include "Perlin2DSectorMappings.h"
#include "NoiseGridTile.h"
#include "Perlin.h"
#include "NoiseGridTileSamplingField.h"

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


/* PerlinCluster: a PerlinCluster contains metadata about a scanned perlin mass, resulting from generated PerlinClusterMeta objects 
found in a call to NoiseGridScanner::start.

The PerlinCluster should have the ability to determine what is known as the perlin mass hash that uniquely identifies the mass.
This value should be able to be generated immediately within this class after it's second constructor below. This unique hash
value can then be applied to or checked against multiple sector files, to update them or determine if the PerlinCluster 
already exists in memory (assuming its mapped correctly).

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
			          int in_gridSeedValue) :
			metaInfo(in_metaInfo),
			perlinClusterTiles(in_perlinClusterTiles),
			parentGridSectorLength(in_parentGridSectorLength),
			gridSeedValue(in_gridSeedValue)
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
																	
		void generateTileToSectorMappingsAndSamplingFields();	// generates the contents of tileToSectorMapping, which allows for the mapping of tiles to their sectors 
																// that they belong to, then generate the individual NoiseGridTileSamplingField(s) to use.

		void fetchBicubicallyInterpolatedCoordinate(float in_coordX, float in_coordZ);	// attempts to find a bicubically interpolated coordinate;
																						// this will fail if the estimated/calculated sector that the coordinate is in
																						// does not exist, and should only be called after sampling fields have been set up.

		PerlinClusterSectorPointSearch fetchBicubicallyInterpolatedCoordinateV2(float in_coordX, float in_coordZ);	// assuming that all corresponding mappings are setup after the call to
																													// generateTileToSectorMappingsAndSamplingFields, this can be used to return a point value in 
																													// a sampling field, if said field exists.

	private:
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTile, EnclaveKeyDef::KeyHasher> perlinClusterTiles;					// stores all tiles invovled with the cluster; populated via non-default constructor.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher> tileToSectorMapping;		// maps tiles to their corresponding sectors, before any translation of tiles/sectors occurs.
		std::unordered_map<EnclaveKeyDef::Enclave2DKey, NoiseGridTileSamplingField, EnclaveKeyDef::KeyHasher> sectorSamplingFields;		// stores all unique sampling fields that could be used by the PerlinCluster.
		PerlinClusterMeta metaInfo;		// stores the groupings from each sector that this PerlinCluster will use.

		int parentGridSectorLength = 0;		// the length of a sector, based on the NoiseGrid that this PerlinClsuter originated from.
		int gridSeedValue = 0;				// the seed value from the NoiseGrid object that this PerlinCluster originated from. Needed when 
											// generating the sampling fields per sector.

		void generateSamplingFieldLookups();	// populate the contents of sectorSamplingFields. Must be called before attempting to find 
												// bicubically interpolated values, and should be called immediately after generateTileToSectorMappingsAndSamplingFields.

		float findBicubicInterpolationRange(float in_rangeValue);  // find the range value between 0.00 and 1.00f, which is required for retreiving values from
																   // NoiseGridTileSamplingField.
};

#endif
