#pragma once

#ifndef ORGANICRAWMANIFEST_H
#define ORGANICRAWMANIFEST_H

#include <mutex>
#include "OrganicRawEnclave.h"
#include "OrganicVtxColorDict.h"
#include "TertiaryTriangleProducer.h"
#include "UVCoordProducer.h"
#include "UVCoordProducerECBPoly.h"
#include <GL/glew.h>
#include "ECBPoly.h"
#include "AtlasMap.h"	// moved to OrganicIndependentsLib
#include "TerrainTriangle.h"
#include "UVCoordProducerEnclaveTriangle.h"
#include "GPUCoordinateMode.h"

class EnclaveFractureResultsMap;
class OrganicRawManifest
{
	public:
		OrganicRawManifest();
		OrganicRawManifest& operator=(const OrganicRawManifest& organicRawManifest_b)
		{
			// copy world position data
			worldPositionSize = organicRawManifest_b.worldPositionSize;
			if (worldPositionSize > 0)
			{
				//std::cout << "!!! Copying world position data..." << std::endl;
				worldPositionGL.reset(new float[worldPositionSize]);
				for (int x = 0; x < worldPositionSize; x++)
				{
					worldPositionGL[x] = organicRawManifest_b.worldPositionGL[x];
				}

			}

			// copy vertex color data
			vertexColorSize = organicRawManifest_b.vertexColorSize;
			if (vertexColorSize > 0)
			{
				vertexColorGL.reset(new float[vertexColorSize]);
				for (int x = 0; x < vertexColorSize; x++)
				{
					vertexColorGL[x] = organicRawManifest_b.vertexColorGL[x];
				}
			}

			// copy normal data
			emptyNormalSize = organicRawManifest_b.emptyNormalSize;
			if (emptyNormalSize > 0)
			{
				emptyNormalGL.reset(new float[emptyNormalSize]);
				for (int x = 0; x < emptyNormalSize; x++)
				{
					emptyNormalGL[x] = organicRawManifest_b.emptyNormalGL[x];
				}
			}

			// copy texture coord data
			textureCoordSize = organicRawManifest_b.textureCoordSize;
			if (textureCoordSize > 0)
			{
				textureCoordGL.reset(new float[textureCoordSize]);
				for (int x = 0; x < textureCoordSize; x++)
				{
					textureCoordGL[x] = organicRawManifest_b.textureCoordGL[x];
				}
			}

			// copy atlas tile data
			atlasTileSize = organicRawManifest_b.atlasTileSize;
			if (atlasTileSize > 0)
			{
				atlasTileGL.reset(new float[atlasTileSize]);
				for (int x = 0; x < atlasTileSize; x++)
				{
					atlasTileGL[x] = organicRawManifest_b.atlasTileGL[x];
				}
			}

			return *this;
		}



		// Below: initialize the OrganicRawManifest with data for per-ORE rendering; used primarily for T1 style rendering.
		// See its usage in the OrganicSystem::jobProduceRawManifestGLData function of OrganicCoreLib.
		void initializeRawManifest(OrganicRawEnclave* in_rawEnclaveRef, 
								OrganicVtxColorDict* in_organicVtxColorDictRef, 
								EnclaveKeyDef::EnclaveKey in_enclaveKey, 
								EnclaveKeyDef::EnclaveKey in_collectionKey, 
								std::mutex* in_mutexRef, 
								AtlasMap* in_atlasMapRef,
								GPUCoordinateMode in_vertexCoordMode);	// set up the raw manifest (required before loading, obviously)

		// **NOTE**: this definitely needs to be refactored. 
		//
		// Below: This function initializes the OrganicRawManifest --AND-- combines the rendering data for OREs having a dependency state of INDEPENDENT, 
		// with the regular ECBPoly instances that are found in the primaryPolygonMap of an EnclaveCollectionBlueprint, to produce a single array of renderable data. 
		// This style of rendering (T2, aka "Remote" ) is used by the function OrganicSystem::jobProduceAndLoadRawManifestGLDataFromBlueprint in OrganicCoreLib.
		void initializeRawManifestFromBlueprintPolys(EnclaveFractureResultsMap* in_fractureResultsMapRef,
			std::map<int, ECBPoly>* in_polyMapRef,
			OrganicVtxColorDict* in_organicVtxColorDictRef,
			std::mutex* in_mutexRef,
			EnclaveKeyDef::EnclaveKey in_collectionKey,
			AtlasMap* in_atlasMapRef,
			GPUCoordinateMode in_vertexCoordMode);

		// Below: loads rendering data directly from the exposed blocks in a single ORE, which is the same ORE 
		// specified in the call to initializeRawManifest.
		void loadDataFromOrganicRawEnclave();
		void loadDataFromOrganicRawEnclaveDebug();
		void allocateArrays(int in_numberOfVertexFloats, int in_numberOfColorFloats, int in_numberOfTextureFloats);

		std::unique_ptr<GLfloat[]> worldPositionGL;
		std::unique_ptr<GLfloat[]> vertexColorGL;
		std::unique_ptr<GLfloat[]> emptyNormalGL;
		std::unique_ptr<GLfloat[]> textureCoordGL;
		std::unique_ptr<GLfloat[]> atlasTileGL;


		int totalPolys = 0;	// for blueprint polys only

		// a pointer to the raw enclave
		OrganicRawEnclave* rawEnclaveRef = NULL;

		// references to data
		OrganicVtxColorDict *vtxColorsRef = NULL;

		EnclaveKeyDef::EnclaveKey enclaveKey;

	private:
		int getNumberOfTrianglesFromModifiedOREs(EnclaveFractureResultsMap* in_enclaveFractureResultsMapRef,
			EnclaveKeyDef::EnclaveKey in_blueprintKey);
		void fillArraysWithLODDataFromOREs(EnclaveFractureResultsMap* in_enclaveFractureResultsMapRef,
			int* in_currentPointIndexRef,
			int* in_currentVertexColorIndex,
			int* in_currentNormalIndex,
			int* in_currentTextureIndex,
			int* in_currentAtlasTileIndex,
			EnclaveKeyDef::EnclaveKey in_blueprintKey,
			AtlasMap* in_atlasMapRef,
			OrganicVtxColorMeta* in_vertexColorMetaRef
		);
		std::vector<TerrainTriangle> produceTerrainTrianglesFromOREBlocks(OrganicRawEnclave* in_orePointer,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blueprintKey,
			AtlasMap* atlasMapRef);

		std::vector<TerrainTriangle> produceTerrainTrianglesFromOREEnclaveTriangles(OrganicRawEnclave* in_orePointer,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blueprintKey,
			AtlasMap* atlasMapRef,
			bool in_debugFlag);

		// Below: This variable is used to determine whether the vertices should be aligned in a LOCAL
		// or ABSOLUTE manner. This value must be set by initialize functions, such as initializeRawManifest.
		// It's value after the OrganicRawManifest should be either COORDINATE_MODE_LOCAL or COORDINATE_MODE_ABSOLUTE, and never 
		// COORDINATE_MODE_UNDEFINED. COORDINATE_MODE_UNDEFINED is simply the default value until a valid one is determined.
		GPUCoordinateMode manifestGPUCoordMode = GPUCoordinateMode::COORDINATE_MODE_UNDEFINED;

		std::mutex* rawManifestMutex;

		// world location keys
		EnclaveKeyDef::EnclaveKey collectionKey;

		AtlasMap* atlasMapRef = NULL;

		int worldPositionSize = 0;
		int vertexColorSize = 0;
		int emptyNormalSize = 0;
		int textureCoordSize = 0;
		int atlasTileSize = 0;

};

#endif
