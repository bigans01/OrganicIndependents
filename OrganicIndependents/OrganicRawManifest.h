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



		std::unique_ptr<GLfloat[]> worldPositionGL;
		std::unique_ptr<GLfloat[]> vertexColorGL;
		std::unique_ptr<GLfloat[]> emptyNormalGL;
		std::unique_ptr<GLfloat[]> textureCoordGL;
		std::unique_ptr<GLfloat[]> atlasTileGL;

		int worldPositionSize = 0;
		int vertexColorSize = 0;
		int emptyNormalSize = 0;
		int textureCoordSize = 0;
		int atlasTileSize = 0;

		// a pointer to the raw enclave
		OrganicRawEnclave* rawEnclaveRef = NULL;

		// references to data
		OrganicVtxColorDict *vtxColorsRef = NULL;
		std::mutex* rawManifestMutex;

		// world location keys
		EnclaveKeyDef::EnclaveKey enclaveKey;
		EnclaveKeyDef::EnclaveKey collectionKey;

		AtlasMap* atlasMapRef = NULL;

		int totalPolys = 0;	// for blueprint polys only

		void initializeRawManifest(OrganicRawEnclave* in_rawEnclaveRef, 
								OrganicVtxColorDict* in_organicVtxColorDictRef, 
								EnclaveKeyDef::EnclaveKey in_enclaveKey, 
								EnclaveKeyDef::EnclaveKey in_collectionKey, 
								std::mutex* in_mutexRef, 
								AtlasMap* in_atlasMapRef);	// set up the raw manifest (required before loading, obviously)

		void initializeRawManifestFromBlueprintPolys(EnclaveFractureResultsMap* in_fractureResultsMapRef,
			std::map<int, ECBPoly>* in_polyMapRef,
			OrganicVtxColorDict* in_organicVtxColorDictRef,
			std::mutex* in_mutexRef,
			EnclaveKeyDef::EnclaveKey in_collectionKey,
			AtlasMap* in_atlasMapRef);

		void loadDataFromOrganicRawEnclave();
		void loadDataFromOrganicRawEnclaveDebug();
		void allocateArrays(int in_numberOfVertexFloats, int in_numberOfColorFloats, int in_numberOfTextureFloats);
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
};

#endif
