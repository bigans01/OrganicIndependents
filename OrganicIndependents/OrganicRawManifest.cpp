#include "stdafx.h"
#include "OrganicRawManifest.h"
#include "EnclaveFractureResultsMap.h"

OrganicRawManifest::OrganicRawManifest()
{

}

void OrganicRawManifest::loadDataFromOrganicRawEnclave()
{
	if (rawEnclaveRef->getTotalTriangles() != 0)	// do this only if there are triangles that exist.
	{
		int totalTriangles = rawEnclaveRef->getTotalTriangles();
		allocateArrays(totalTriangles * 9, totalTriangles * 9, totalTriangles * 6);

		// set up index variables, for tracking the current index to use in each smart pointer array
		int currentPointIndex = 0;
		int currentVertexColorIndex = 0;
		int currentNormalIndex = 0;
		int currentTextureIndex = 0;
		int currentAtlasTileIndex = 0;

		OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
		vertexColorMetaArrayRef = &vtxColorsRef->dictionary["base"];
		OrganicVtxColorMeta *vertexColorMetaRef;
		vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

		/*
		auto blockMapBegin = rawEnclaveRef->blockMap.begin();
		auto blockMapEnd = rawEnclaveRef->blockMap.end();
		for (blockMapBegin; blockMapBegin != blockMapEnd; blockMapBegin++)
		{
			EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(blockMapBegin->first);	// get the block key of whatever block we are looking at.
			EnclaveBlock* blockRef = &rawEnclaveRef->blockMap[blockMapBegin->first];				// get a pointer to the block
			//int numberOfSecondariesToRead = int(blockRef->numBBFanByte);
			int numberOfSecondariesToRead = int(blockRef->getNumberOfBBFans());


			for (int z = 0; z < numberOfSecondariesToRead; z++)
			{
				//int numberOfTertiariesInSecondary = blockRef->triangleArray[z].numberOfTertiaries;
				int numberOfTertiariesInSecondary = blockRef->getNumberOfTertiariesInTriangleAtIndex(z);
				//std::cout << "Number of tertiaries to read at current secondary: " << numberOfTertiariesInSecondary << std::endl;
				//TertiaryTriangleProducer triangleProducer(blockRef, &blockRef->triangleArray[z]);
				TertiaryTriangleProducer triangleProducer(blockRef, blockRef->retrieveSecondaryFromIndex(z));
				UVCoordProducer textureCoordProducer;
				UVTriangleCoords testCoords;
				//textureCoordProducer.setAsActive(blockRef, &blockRef->triangleArray[z], texturesRef->terrainAtlas.get(), 0);
				//textureCoordProducer.setAsActive(blockRef, &blockRef->triangleArray[z], atlasMapRef, 0);
				textureCoordProducer.setAsActive(blockRef, blockRef->retrieveSecondaryFromIndex(z), atlasMapRef, 0);

				// create the empty normal
				ECBPolyPoint fetchedEmptyNormal = blockRef->getEmptyNormalFromTriangle(z);

				for (int a = 0; a < numberOfTertiariesInSecondary; a++)
				{
					EnclaveBlockVertexTri currentVertexTri = triangleProducer.getTrianglePointsAndIterateToNext();
					ECBPolyPointTri currentPolyPointTri = OrganicUtils::convertEnclaveBlockVertexesToFloats(currentVertexTri);
					ECBPolyPointTri preciseCoords = OrganicUtils::combineClampedCoordsWithPrecise(currentPolyPointTri, blockKey, enclaveKey, collectionKey);

					GLfloat vertexAttribData[6];
					testCoords = textureCoordProducer.getUVPointsAndIterateToNext();
					vertexAttribData[0] = testCoords.UVpoint[0].x;
					vertexAttribData[1] = testCoords.UVpoint[0].y;
					vertexAttribData[2] = testCoords.UVpoint[1].x;
					vertexAttribData[3] = testCoords.UVpoint[1].y;
					vertexAttribData[4] = testCoords.UVpoint[2].x;
					vertexAttribData[5] = testCoords.UVpoint[2].y;

					int currentUV = 0;		// for testing; will be 0 through 5.
					for (int b = 0; b < 3; b++)		// cycle through all 3 points; index 0 = point A, 1 = point B, 2 = point C
					{
						worldPositionGL[currentPointIndex++] = preciseCoords.triPoints[b].x;
						worldPositionGL[currentPointIndex++] = preciseCoords.triPoints[b].y;
						worldPositionGL[currentPointIndex++] = preciseCoords.triPoints[b].z;

						emptyNormalGL[currentNormalIndex++] = fetchedEmptyNormal.x;
						emptyNormalGL[currentNormalIndex++] = fetchedEmptyNormal.y;
						emptyNormalGL[currentNormalIndex++] = fetchedEmptyNormal.z;

						textureCoordGL[currentTextureIndex++] = vertexAttribData[currentUV++];
						textureCoordGL[currentTextureIndex++] = vertexAttribData[currentUV++];

						atlasTileGL[currentAtlasTileIndex++] = GLfloat(testCoords.U_tile_coord);
						atlasTileGL[currentAtlasTileIndex++] = GLfloat(testCoords.V_tile_coord);

						vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].red;
						vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].green;
						vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].blue;
					}
				}


			}

			//std::cout << "iterating through block..." << std::endl;
		}
		*/

		auto terrainTriangleVector = produceTerrainTrianglesFromOREBlocks(rawEnclaveRef, enclaveKey, collectionKey, atlasMapRef);

		auto terrainTriangleVectorBegin = terrainTriangleVector.begin();
		auto terrainTriangleVectorEnd = terrainTriangleVector.end();
		for (; terrainTriangleVectorBegin != terrainTriangleVectorEnd; terrainTriangleVectorBegin++)
		{
			for (int b = 0; b < 3; b++) // cycle through each point of the terrain triangle.
			{
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.x;
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.y;
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.z;

				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.x;
				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.y;
				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.z;

				textureCoordGL[currentTextureIndex++] = terrainTriangleVectorBegin->ttPoints[b].uTileCoord;
				textureCoordGL[currentTextureIndex++] = terrainTriangleVectorBegin->ttPoints[b].vTileCoord;

				atlasTileGL[currentAtlasTileIndex++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileUCoord);
				atlasTileGL[currentAtlasTileIndex++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileVCoord);

				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].red;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].green;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].blue;
			}
		}

	}
}

void OrganicRawManifest::loadDataFromOrganicRawEnclaveDebug()
{
	std::cout << "(OrganicRawManifest): DEBUG -> total number of triangles for target ORE is: " << rawEnclaveRef->getTotalTriangles() << std::endl;
	if (rawEnclaveRef->getTotalTriangles() != 0)	// do this only if there are triangles that exist.
	{
		int totalTriangles = rawEnclaveRef->getTotalTriangles();
		allocateArrays(totalTriangles * 9, totalTriangles * 9, totalTriangles * 6);

		// set up index variables, for tracking the current index to use in each smart pointer array
		int currentPointIndex = 0;
		int currentVertexColorIndex = 0;
		int currentNormalIndex = 0;
		int currentTextureIndex = 0;
		int currentAtlasTileIndex = 0;

		OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
		vertexColorMetaArrayRef = &vtxColorsRef->dictionary["base"];
		OrganicVtxColorMeta *vertexColorMetaRef;
		vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

		auto terrainTriangleVector = produceTerrainTrianglesFromOREBlocks(rawEnclaveRef, enclaveKey, collectionKey, atlasMapRef);
		std::cout << "(OrganicRawManifest): DEBUG -> size of triangle vector is: " << terrainTriangleVector.size() << std::endl;

		auto terrainTriangleVectorBegin = terrainTriangleVector.begin();
		auto terrainTriangleVectorEnd = terrainTriangleVector.end();
		for (; terrainTriangleVectorBegin != terrainTriangleVectorEnd; terrainTriangleVectorBegin++)
		{
			for (int b = 0; b < 3; b++) // cycle through each point of the terrain triangle.
			{
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.x;
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.y;
				worldPositionGL[currentPointIndex++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.z;

				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.x;
				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.y;
				emptyNormalGL[currentNormalIndex++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.z;

				textureCoordGL[currentTextureIndex++] = terrainTriangleVectorBegin->ttPoints[b].uTileCoord;
				textureCoordGL[currentTextureIndex++] = terrainTriangleVectorBegin->ttPoints[b].vTileCoord;

				atlasTileGL[currentAtlasTileIndex++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileUCoord);
				atlasTileGL[currentAtlasTileIndex++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileVCoord);

				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].red;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].green;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].blue;
			}
		}

	}
}

void OrganicRawManifest::initializeRawManifest(OrganicRawEnclave* in_rawEnclaveRef, 
											OrganicVtxColorDict* in_organicVtxColorDictRef, 
											EnclaveKeyDef::EnclaveKey in_enclaveKey, 
											EnclaveKeyDef::EnclaveKey in_collectionKey, 
											std::mutex* in_mutexRef, 
											AtlasMap* in_atlasMapRef)
{
	rawEnclaveRef = in_rawEnclaveRef;
	vtxColorsRef = in_organicVtxColorDictRef;
	rawManifestMutex = in_mutexRef;
	enclaveKey = in_enclaveKey;
	collectionKey = in_collectionKey;
	atlasMapRef = in_atlasMapRef;
}

void OrganicRawManifest::initializeRawManifestFromBlueprintPolys(EnclaveFractureResultsMap* in_fractureResultsMapRef,
	std::map<int, ECBPoly>* in_polyMapRef,
	OrganicVtxColorDict* in_organicVtxColorDictRef,
	std::mutex* in_mutexRef,
	EnclaveKeyDef::EnclaveKey in_collectionKey,
	AtlasMap* in_atlasMapRef)
{
	atlasMapRef = in_atlasMapRef;

	// variable initialization:
	// set up index variables, for tracking the current index to use in each smart pointer array

	int currentPointIndex = 0;
	int currentVertexColorIndex = 0;
	int currentNormalIndex = 0;
	int currentTextureIndex = 0;
	int currentAtlasTileIndex = 0;
	int totalTriangles = 0;

	rawManifestMutex = in_mutexRef;
	vtxColorsRef = in_organicVtxColorDictRef;

	// color pointers
	OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
	vertexColorMetaArrayRef = &vtxColorsRef->dictionary["base"];
	OrganicVtxColorMeta *vertexColorMetaRef;
	vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

	// First, check for any OREs in the blueprint that have their dependency state set to INDEPENDENT.
	// Get the number of triangles we will get from each ORE, and increment the totalPolys value appropriately; how the numbers 
	// of triangles are calculated is up to the ORE itself.
	int numberOfTrianglesFromORES = getNumberOfTrianglesFromModifiedOREs(in_fractureResultsMapRef, in_collectionKey);
	totalTriangles += numberOfTrianglesFromORES;
	totalPolys += numberOfTrianglesFromORES;

	// Second, get the number of triangles that would be produced by typical ECB Polys
	totalTriangles += in_polyMapRef->size();	// get the total number of polys, increment the total number of triangles by it
	totalPolys += in_polyMapRef->size();

	// Third, allocate the array once the number of triangles has been acquired.
	if (totalTriangles != 0)
	{
		allocateArrays(totalTriangles * 9, totalTriangles * 9, totalTriangles * 6);	// allocate based on the number of ECBPolys

		// Fourth, once the final size of the array has been determined/built, iterate over the OREs we targeted first...


		fillArraysWithLODDataFromOREs(in_fractureResultsMapRef,
			&currentPointIndex,
			&currentVertexColorIndex,
			&currentNormalIndex,
			&currentTextureIndex,
			&currentAtlasTileIndex,
			in_collectionKey,
			in_atlasMapRef,
			vertexColorMetaRef
		);

	}


	// Fourth, then go through the ECBPolys...
	if (in_polyMapRef->size() != 0)		// only do this if there's 0 polys.
	{
		auto polysBegin = in_polyMapRef->begin();
		auto polysEnd = in_polyMapRef->end();


		//OrganicTextureMetaArray *textureMetaArrayRef;
		//textureMetaArrayRef = &texturesRef->dictionary["base"];

		OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
		vertexColorMetaArrayRef = &vtxColorsRef->dictionary["base"];
		OrganicVtxColorMeta *vertexColorMetaRef;
		vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

		for (polysBegin; polysBegin != polysEnd; polysBegin++)
		{
			//if (polysBegin->second.polyType != ECBPolyType::ROGUE)	// don't run rogue polys.
			//{
			//std::cout << "!! Raw data for poly: " << polysBegin->first << std::endl;
			//std::cout << "point 0: " << polysBegin->second.ecbPolyPoints[0].pointA.x << ", " << polysBegin->second.ecbPolyPoints[0].pointA.y << ", " << polysBegin->second.ecbPolyPoints[0].pointA.z << std::endl;
			//std::cout << "point 1: " << polysBegin->second.ecbPolyPoints[1].pointA.x << ", " << polysBegin->second.ecbPolyPoints[1].pointA.y << ", " << polysBegin->second.ecbPolyPoints[1].pointA.z << std::endl;
			//std::cout << "point 2: " << polysBegin->second.ecbPolyPoints[2].pointA.x << ", " << polysBegin->second.ecbPolyPoints[2].pointA.y << ", " << polysBegin->second.ecbPolyPoints[2].pointA.z << std::endl;

			// For the below: 
			//
			// All ECBPoly instances come from an FTriangle that runs in  WORLD-based space, 
			// and produces it's coordinates in FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE;
			// therefore, all values range between 0 and 32.0f -- so we must a zeroed-out EnclaveKey, 
			// to get the texture coordinates.
			UVCoordProducerECBPoly ecbPolyCoords(polysBegin->second.materialID,
				polysBegin->second.ecbPolyPoints[0],
				polysBegin->second.ecbPolyPoints[1],
				polysBegin->second.ecbPolyPoints[2],
				//texturesRef->terrainAtlas.get(),
				atlasMapRef,
				0, EnclaveKeyDef::EnclaveKey(0,0,0));	// ECBPOLY_FIX (see change to EnclaveKey argument here, it's initialized to 0,0,0., versus using in_collectionKey.)

			GLfloat vertexAttribData[6];
			UVTriangleCoords testCoords = ecbPolyCoords.getCoords();
			vertexAttribData[0] = testCoords.UVpoint[0].x;
			vertexAttribData[1] = testCoords.UVpoint[0].y;
			vertexAttribData[2] = testCoords.UVpoint[1].x;
			vertexAttribData[3] = testCoords.UVpoint[1].y;
			vertexAttribData[4] = testCoords.UVpoint[2].x;
			vertexAttribData[5] = testCoords.UVpoint[2].y;



			int currentUV = 0;		// for testing; will be 0 through 5.
			for (int b = 0; b < 3; b++)		// cycle through all 3 points; index 0 = point A, 1 = point B, 2 = point C
			{
				// UPDATE 10

				// ECBPOLY_FIX

				// As of circa 11/14/2023, ECBPoly instances are constructed from a WORLD-based FTriangle,
				// that is running in FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE for it's translation mode;
				// i.e, it no longer uses the old mode where the FTriangleOutput objects (which are used to make the ECBPoly objects)
				// are translated to absolute space. In other words, the FTriangleOutput objects are now localized.
				//
				// This decision allows the option to send the ECBPoly coordinates as localized,
				// or make them absolute (adding the result of multiplying by the blueprint key dimensions by 32.0f)
				//
				// The first 3 lines below, will send the localized points of the ECBPoly to a shader...
				//worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].x;
				//worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].y;
				//worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].z;

				// ...otherwise, if the shader expects absolute coordinates, we will first multiply by the blueprint key
				// before storing the data for rendering.
				worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].x + in_collectionKey.x * 32.0f;
				worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].y + in_collectionKey.y * 32.0f;
				worldPositionGL[currentPointIndex++] = polysBegin->second.ecbPolyPoints[b].z + in_collectionKey.z * 32.0f;

				emptyNormalGL[currentNormalIndex++] = polysBegin->second.emptyNormal.x;
				emptyNormalGL[currentNormalIndex++] = polysBegin->second.emptyNormal.y;
				emptyNormalGL[currentNormalIndex++] = polysBegin->second.emptyNormal.z;


				textureCoordGL[currentTextureIndex++] = vertexAttribData[currentUV++];
				textureCoordGL[currentTextureIndex++] = vertexAttribData[currentUV++];

				atlasTileGL[currentAtlasTileIndex++] = GLfloat(testCoords.U_tile_coord);
				atlasTileGL[currentAtlasTileIndex++] = GLfloat(testCoords.V_tile_coord);

				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].red;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].green;
				vertexColorGL[currentVertexColorIndex++] = vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].blue;

			}

			//}
		}
	}

}

int OrganicRawManifest::getNumberOfTrianglesFromModifiedOREs(EnclaveFractureResultsMap* in_enclaveFractureResultsMapRef,
	EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	int totalTrianglesThatWillBeProduced = 0;
	auto oresToCheckBegin = in_enclaveFractureResultsMapRef->fractureResultsContainerMap.begin();
	auto oresToCheckEnd = in_enclaveFractureResultsMapRef->fractureResultsContainerMap.end();
	for (; oresToCheckBegin != oresToCheckEnd; oresToCheckBegin++)
	{
		auto currentDependencyState = oresToCheckBegin->second.getDependencyState();
		if
			(
				currentDependencyState == OREDependencyState::INDEPENDENT
				)
		{
			// state check ok, now make sure we can get 
			int currentOREExpectedTriangleCount = oresToCheckBegin->second.getNumberOfTrianglesByLOD();
			if (currentOREExpectedTriangleCount != 0)
			{
				//std::cout << "Blueprint (" << in_blueprintKey.x << ", " << in_blueprintKey.y << ", " << in_blueprintKey.z
				//	       << ") -> ORE with key (" << oresToCheckBegin->first.x << ", " << oresToCheckBegin->first.y << ", " << oresToCheckBegin->first.z << ") has " << currentOREExpectedTriangleCount << " triangles to render to GL. " << std::endl;
				totalTrianglesThatWillBeProduced += currentOREExpectedTriangleCount;
			}
		}
	}
	return totalTrianglesThatWillBeProduced;
}

void OrganicRawManifest::fillArraysWithLODDataFromOREs(EnclaveFractureResultsMap* in_enclaveFractureResultsMapRef,
	int* in_currentPointIndexRef,
	int* in_currentVertexColorIndex,
	int* in_currentNormalIndex,
	int* in_currentTextureIndex,
	int* in_currentAtlasTileIndex,
	EnclaveKeyDef::EnclaveKey in_blueprintKey,
	AtlasMap* in_atlasMapRef,
	OrganicVtxColorMeta* in_vertexColorMetaRef
)
{
	//std::cout << "!!! Acquiring modified ORE data, for blueprint with key: " << in_blueprintKey.x << ", " << in_blueprintKey.y << ", " << in_blueprintKey.z << std::endl;
	auto oresToCheckBegin = in_enclaveFractureResultsMapRef->fractureResultsContainerMap.begin();
	auto oresToCheckEnd = in_enclaveFractureResultsMapRef->fractureResultsContainerMap.end();
	int triangleCount = 0;
	//int appendedORETotalTrianglesCount = 0;
	for (; oresToCheckBegin != oresToCheckEnd; oresToCheckBegin++)
	{
		auto appendedState = oresToCheckBegin->second.getAppendedState();

		/*
		if
		(
			(in_blueprintKey.x == 0)
			&&
			(in_blueprintKey.y == -1)
			&&
			(in_blueprintKey.z == 1)

			&&
			(oresToCheckBegin->first.x == 6)
			&&
			(oresToCheckBegin->first.y == 7)
			&&
			(oresToCheckBegin->first.z == 1)
		)
		{
			std::cout << "------------------------------------------------------------------------------------------" << std::endl;
			std::cout << "!!! SPECIAL DEBUG, in OrganicRawManifest::fillArraysWithLODDataFromORES...." << std::endl;
			std::cout << "------------------------------------------------------------------------------------------" << std::endl;
			//int waitVal = 3;
			//std::cin >> waitVal;

			auto currentAppendedState = oresToCheckBegin->second.getAppendedState();
			std::cout << "Current appended state is: ";
			switch (currentAppendedState)
			{
				case OREAppendedState::NONE:
				{
					std::cout << "NONE" << std::endl;
					break;
				}

				case OREAppendedState::SINGLE_APPEND:
				{
					std::cout << "SINGLE_APPEND" << std::endl;
					break;
				}

				case OREAppendedState::MULTIPLE_APPEND:
				{
					std::cout << "MULTIPLE_APPEND" << std::endl;
					break;
				}
			}

			int skeletonCount = oresToCheckBegin->second.skeletonSGM.triangleSkeletonSupergroups.size();
			std::cout << "Skeleton SGM size: " << skeletonCount << std::endl;

			int etcSGMCount = oresToCheckBegin->second.etcSGM.enclaveTriangleSupergroups.size();
			std::cout << "etcSGM size: " << etcSGMCount << std::endl;

			int organicSecondaryCount = oresToCheckBegin->second.organicTriangleSecondarySGM.secondarySupergroups.size();
			std::cout << "organicSecondary size: " << organicSecondaryCount << std::endl;
		}
		*/

		auto currentOreLODState = oresToCheckBegin->second.getLodState();
		auto currentDependencyState = oresToCheckBegin->second.getDependencyState();
		if
			(
				currentDependencyState == OREDependencyState::INDEPENDENT
				)

		{
			bool debugFlag = false;
			/*
			if
			(

				(in_blueprintKey.x == 0)
				&&
				(in_blueprintKey.y == -1)
				&&
				(in_blueprintKey.z == 1)

				&&
				(oresToCheckBegin->first.x == 4)
				&&
				(oresToCheckBegin->first.y == 7)
				&&
				(oresToCheckBegin->first.z == 1)

			)
			{
				debugFlag = true;
			}
			*/


			std::vector<TerrainTriangle> terrainTriangleVector;
			if (currentOreLODState == ORELodState::LOD_BLOCK)
			{
				terrainTriangleVector = produceTerrainTrianglesFromOREBlocks(&oresToCheckBegin->second,
					oresToCheckBegin->first,
					in_blueprintKey,
					in_atlasMapRef);
			}
			else if (currentOreLODState != ORELodState::FULL)
			{
				terrainTriangleVector = produceTerrainTrianglesFromOREEnclaveTriangles(&oresToCheckBegin->second,
					oresToCheckBegin->first,
					in_blueprintKey,
					in_atlasMapRef,
					debugFlag);
			}

			auto terrainTriangleVectorBegin = terrainTriangleVector.begin();
			auto terrainTriangleVectorEnd = terrainTriangleVector.end();
			for (; terrainTriangleVectorBegin != terrainTriangleVectorEnd; terrainTriangleVectorBegin++)
			{
				for (int b = 0; b < 3; b++) // cycle through each point of the terrain triangle.
				{

					worldPositionGL[(*in_currentPointIndexRef)++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.x;
					worldPositionGL[(*in_currentPointIndexRef)++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.y;
					worldPositionGL[(*in_currentPointIndexRef)++] = terrainTriangleVectorBegin->ttPoints[b].threeDLocation.z;

					emptyNormalGL[(*in_currentNormalIndex)++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.x;
					emptyNormalGL[(*in_currentNormalIndex)++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.y;
					emptyNormalGL[(*in_currentNormalIndex)++] = terrainTriangleVectorBegin->ttPoints[b].ttpEmptyNormal.z;

					textureCoordGL[(*in_currentTextureIndex)++] = terrainTriangleVectorBegin->ttPoints[b].uTileCoord;
					textureCoordGL[(*in_currentTextureIndex)++] = terrainTriangleVectorBegin->ttPoints[b].vTileCoord;

					atlasTileGL[(*in_currentAtlasTileIndex)++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileUCoord);
					atlasTileGL[(*in_currentAtlasTileIndex)++] = GLfloat(terrainTriangleVectorBegin->ttPoints[b].atlasTileVCoord);

					vertexColorGL[(*in_currentVertexColorIndex)++] = in_vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].red;
					vertexColorGL[(*in_currentVertexColorIndex)++] = in_vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].green;
					vertexColorGL[(*in_currentVertexColorIndex)++] = in_vertexColorMetaRef->BlockData.FaceIndex[5].FaceMeta[b].blue;

				}
				triangleCount++;
			}
		}
		/*
		else if
		(
			(currentOreLODState == ORELodState::LOD_BLOCK)
		)
		*/
		{

		}
		//}
	}
	std::cout << "!! (Blueprint key " << in_blueprintKey.x << ", " << in_blueprintKey.y << ", " << in_blueprintKey.z << ") actually ingested " << triangleCount << " triangles. " << std::endl;
}

void OrganicRawManifest::allocateArrays(int in_numberOfVertexFloats, int in_numberOfColorFloats, int in_numberOfTextureFloats)
{
	std::lock_guard<std::mutex> lock(*rawManifestMutex);	// lock for safety.
	worldPositionGL.reset(new GLfloat[in_numberOfVertexFloats]);
	vertexColorGL.reset(new GLfloat[in_numberOfColorFloats]);			// we can use color floats for normals too
	emptyNormalGL.reset(new GLfloat[in_numberOfColorFloats]);			// ""
	textureCoordGL.reset(new GLfloat[in_numberOfTextureFloats]);
	atlasTileGL.reset(new GLfloat[in_numberOfTextureFloats]);
}

std::vector<TerrainTriangle> OrganicRawManifest::produceTerrainTrianglesFromOREBlocks(OrganicRawEnclave* in_orePointer,
	EnclaveKeyDef::EnclaveKey in_oreKey,
	EnclaveKeyDef::EnclaveKey in_blueprintKey,
	AtlasMap* in_atlasMapRef)
{
	std::vector<TerrainTriangle> returnVector;
	auto blockMapBegin = in_orePointer->blockMap.begin();
	auto blockMapEnd = in_orePointer->blockMap.end();
	for (; blockMapBegin != blockMapEnd; blockMapBegin++)
	{
		EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(blockMapBegin->first);	// get the block key of whatever block we are looking at.
		EnclaveBlock* blockRef = &in_orePointer->blockMap[blockMapBegin->first];				// get a pointer to the block
		int numberOfSecondariesToRead = int(blockRef->getNumberOfBBFans());
		for (int z = 0; z < numberOfSecondariesToRead; z++)
		{
			int numberOfTertiariesInSecondary = blockRef->getNumberOfTertiariesInTriangleAtIndex(z);
			TertiaryTriangleProducer triangleProducer(blockRef, blockRef->retrieveSecondaryFromIndex(z));
			UVCoordProducer textureCoordProducer;
			UVTriangleCoords testCoords;
			textureCoordProducer.setAsActive(blockRef, blockRef->retrieveSecondaryFromIndex(z), in_atlasMapRef, 0);

			// create the empty normal
			ECBPolyPoint fetchedEmptyNormal = blockRef->getEmptyNormalFromTriangle(z);

			for (int a = 0; a < numberOfTertiariesInSecondary; a++)
			{
				EnclaveBlockVertexTri currentVertexTri = triangleProducer.getTrianglePointsAndIterateToNext();
				ECBPolyPointTri currentPolyPointTri = IndependentUtils::convertEnclaveBlockVertexesToFloats(currentVertexTri);

				// Note: the call to IndependentUtils::combineClampedCoordsWithPrecise below should only be used if we expect to send
				// ECBPoly data in absoloute world coordinates to the shader (not blueprint-localized)
				ECBPolyPointTri preciseCoords = IndependentUtils::combineClampedCoordsWithPrecise(currentPolyPointTri, blockKey, in_oreKey, in_blueprintKey);
				testCoords = textureCoordProducer.getUVPointsAndIterateToNext();

				TerrainTrianglePoint trianglePointArray[3];
				for (int b = 0; b < 3; b++)		// cycle through all 3 points; index 0 = point A, 1 = point B, 2 = point C
				{
					TerrainTrianglePoint pointToInsert(preciseCoords.triPoints[b],	// 3D coordinate
						fetchedEmptyNormal,
						testCoords.UVpoint[b].x,
						testCoords.UVpoint[b].y,
						testCoords.U_tile_coord,
						testCoords.V_tile_coord
					);
					trianglePointArray[b] = pointToInsert;
				}

				TerrainTriangle currentTriangle(trianglePointArray[0], trianglePointArray[1], trianglePointArray[2]);
				returnVector.push_back(currentTriangle);
			}
		}
	}
	return returnVector;

}

std::vector<TerrainTriangle> OrganicRawManifest::produceTerrainTrianglesFromOREEnclaveTriangles(OrganicRawEnclave* in_orePointer,
	EnclaveKeyDef::EnclaveKey in_oreKey,
	EnclaveKeyDef::EnclaveKey in_blueprintKey,
	AtlasMap* in_atlasMapRef,
	bool in_debugFlag)
{
	std::vector<TerrainTriangle> returnVector;

	auto oreProducedTerrainTriangles = in_orePointer->produceAndReturnTerrainTriangles();
	for (auto& currentProducedTriangle : oreProducedTerrainTriangles)
	{
		ECBPolyPoint fetchedEmptyNormal = currentProducedTriangle.otEmptyNormal;
		ECBPolyPointTri currentPolyPointTri;
		currentPolyPointTri.triPoints[0] = currentProducedTriangle.otPoints[0];
		currentPolyPointTri.triPoints[1] = currentProducedTriangle.otPoints[1];
		currentPolyPointTri.triPoints[2] = currentProducedTriangle.otPoints[2];
		ECBPolyPointTri preciseCoords = IndependentUtils::adjustEnclaveTriangleCoordsToWorldSpace(currentPolyPointTri, in_oreKey, in_blueprintKey);

		int debugFlagValue = 0;
		if (in_debugFlag == true)
		{
			std::cout << "|||||||||||||| Start of TerrainTrianglePoint data: " << std::endl;
			debugFlagValue = 1;
		}

		UVCoordProducerEnclaveTriangle enclaveTriangleCoords(currentProducedTriangle.otMaterial,
			currentProducedTriangle.otPoints[0],
			currentProducedTriangle.otPoints[1],
			currentProducedTriangle.otPoints[2],
			in_atlasMapRef,
			debugFlagValue,
			in_blueprintKey);
		UVTriangleCoords testCoords = enclaveTriangleCoords.getCoords();


		TerrainTrianglePoint trianglePointArray[3];
		for (int b = 0; b < 3; b++)
		{

			TerrainTrianglePoint pointToInsert(preciseCoords.triPoints[b],
				fetchedEmptyNormal,
				testCoords.UVpoint[b].x,
				testCoords.UVpoint[b].y,
				testCoords.U_tile_coord,
				testCoords.V_tile_coord);
			if (in_debugFlag == true)
			{
				std::cout << "Point " << b << ": " << std::endl;
				std::cout << "Point coords: " << preciseCoords.triPoints[b].x << ", " << preciseCoords.triPoints[b].y << ", " << preciseCoords.triPoints[b].z << std::endl;
				std::cout << "UV coords: UVPoint.x->" << testCoords.UVpoint[b].x << " | UVPoint.y-> " << testCoords.UVpoint[b].y << std::endl;
			}


			trianglePointArray[b] = pointToInsert;
		}

		if (in_debugFlag == true)
		{
			std::cout << "|||||||||||||| End of TerrainTrianglePoint data: " << std::endl;
		}

		TerrainTriangle currentTriangle(trianglePointArray[0], trianglePointArray[1], trianglePointArray[2]);
		returnVector.push_back(currentTriangle);

	}



	return returnVector;
}
