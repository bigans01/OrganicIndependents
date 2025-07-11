#include "stdafx.h"
#include "FanManager.h"

void FanManager::processTertiaryData(TertiaryTriangleContainer in_polyMetaData, TriangleMaterial in_materialID)
{
	//std::cout << "starting fill..." << std::endl;
	// determine number of points to iterate over, based on the number of triangles
	int numberOfPolyMetaDataPoints = 2 + in_polyMetaData.numberOfTertiaryTriangles;		// i.e, 1 sub-triangle = 3 points, 2 = 4, etc
	checkForTriangleExpansion();	// check whether or not we will have to expand the arrays before adding a new triangle.
	auto currentTriangleRef = retrieveNextAvailableSecondary();
	for (int x = 0; x < numberOfPolyMetaDataPoints; x++)	// the number of used points in the input parameter, in_polyMetaData
	{
		PointSearchData pointData = checkIfPointExists(in_polyMetaData.triangleVertices[x]);	// does a search
		short indexOflocalVertexArrayPointToUse = 0;	// the index of the point to use, from the block's struct array
		if (pointData.isPointFound == 0)	// if it was never found, add a new point
		{
			checkForVertexExpansion();		// SAFETY: check if the number of POINTS exceeds threshold 
			indexOflocalVertexArrayPointToUse = addNewPoint(in_polyMetaData.triangleVertices[x]);
		}
		else if (pointData.isPointFound == 1)
		{
			indexOflocalVertexArrayPointToUse = pointData.foundPointIndex;	// set the 
		}

		// for each point of the input poly, put the found/new index into the appropriate nibble in the EnclaveBlockTriangle
		currentTriangleRef->fillPointIndex(x, indexOflocalVertexArrayPointToUse);
	}

	currentTriangleRef->numberOfTertiaries = numberOfPolyMetaDataPoints - 2;		// set the number of tertiaries
	currentTriangleRef->materialID = in_materialID;


	// be sure to increment the number of secondary polys
	totalFans++;
	//std::cout << "Total triangles is now: " << totalFans << std::endl;
}

void FanManager::insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan)
{
	//std::cout << "starting fill..." << std::endl;
// determine number of points to iterate over, based on the number of triangles
	int numberOfPolyMetaDataPoints = 2 + in_wrappedFan.poly.numberOfTertiaries;		// i.e, 1 sub-triangle = 3 points, 2 = 4, etc
	checkForTriangleExpansion();	// check whether or not we will have to expand the arrays before adding a new triangle.
	auto currentTriangleRef = retrieveNextAvailableSecondary();
	for (int x = 0; x < numberOfPolyMetaDataPoints; x++)	// the number of used points in the input parameter, in_polyMetaData
	{
		PointSearchData pointData = checkIfPointExists(in_wrappedFan.vertices[x]);	// does a search
		short indexOflocalVertexArrayPointToUse = 0;	// the index of the point to use, from the block's struct array
		if (pointData.isPointFound == 0)	// if it was never found, add a new point
		{
			checkForVertexExpansion();		// SAFETY: check if the number of POINTS exceeds threshold 
			indexOflocalVertexArrayPointToUse = addNewPoint(in_wrappedFan.vertices[x]);
		}
		else if (pointData.isPointFound == 1)
		{
			indexOflocalVertexArrayPointToUse = pointData.foundPointIndex;	// set the 
		}

		// for each point of the input poly, put the found/new index into the appropriate nibble in the EnclaveBlockTriangle
		currentTriangleRef->fillPointIndex(x, indexOflocalVertexArrayPointToUse);
	}

	currentTriangleRef->numberOfTertiaries = numberOfPolyMetaDataPoints - 2;		// set the number of tertiaries
	currentTriangleRef->materialID = in_wrappedFan.poly.materialID;
	currentTriangleRef->emptyNormal = in_wrappedFan.poly.emptyNormal;
	currentTriangleRef->faceAlignment = in_wrappedFan.poly.faceAlignment;


	// be sure to increment the number of secondary polys
	totalFans++;
}

int FanManager::addNewPoint(EnclaveBlockVertex in_blockVertex)
{
	int addedDataPointIndex;	// the index of the added point
	EnclaveBlockVertex* arrayPtr = NULL;
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS) { arrayPtr = localVertexArray; }
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS) { arrayPtr = expandedVertexArray.get(); }

	arrayPtr[totalPoints] = in_blockVertex;
	addedDataPointIndex = totalPoints;	// return the index we inserted at
	totalPoints++;						// be sure to increment the totalPoints.

	return addedDataPointIndex;
}

ECBPolyPoint FanManager::convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex)
{
	ECBPolyPoint pointToReturn;
	pointToReturn.x = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.getVertexX());
	pointToReturn.y = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.getVertexY());
	pointToReturn.z = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.getVertexZ());
	return pointToReturn;
}

PointSearchData FanManager::checkIfPointExists(EnclaveBlockVertex in_blockVertex)
{
	PointSearchData searchData;
	EnclaveBlockVertex* arrayPtr = NULL;
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS) { arrayPtr = localVertexArray; }
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS) { arrayPtr = expandedVertexArray.get(); }
	for (int x = 0; x < totalPoints; x++)
	{
		if (arrayPtr[x] == in_blockVertex)
		{
			searchData.isPointFound = 1;
			searchData.foundPointIndex = x;
		}
	}
	return searchData;
}

PointSearchData FanManager::checkIfNearbyPointExists(EnclaveBlockVertex in_blockVertex)
{
	PointSearchData searchData;
	EnclaveBlockVertex* arrayPtr = NULL;
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS) { arrayPtr = localVertexArray; }
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS) { arrayPtr = expandedVertexArray.get(); }
	bool wasFoundExactly = false;
	for (int x = 0; x < totalPoints; x++)		// usedPoints = total number of used points in the block
	{
		if
			(
			(arrayPtr[x] == in_blockVertex)
				)
		{
			wasFoundExactly = true;
			searchData.isPointFound = 1;		// point was found, set this value in return data
			searchData.foundPointIndex = x;	// indicate the index it was found at
		}
	}

	// if we haven't found it exactly yet, continue.
	if (wasFoundExactly == false)
	{
		for (int y = 0; y < totalPoints; y++)
		{
			EnclaveBlockVertex currentVertex = in_blockVertex;	// copy the current vertex
			int searchArmLength = 3;
			// x floats
			float x_pos = float(currentVertex.getVertexX() + searchArmLength);
			float x_current = currentVertex.getVertexX();
			float x_neg = float(currentVertex.getVertexX() - searchArmLength);

			// y floats
			float y_pos = float(currentVertex.getVertexY() + searchArmLength);
			float y_current = currentVertex.getVertexY();
			float y_neg = float(currentVertex.getVertexY() - searchArmLength);

			// z floats
			float z_pos = float(currentVertex.getVertexZ() + searchArmLength);
			float z_current = currentVertex.getVertexZ();
			float z_neg = float(currentVertex.getVertexZ() - searchArmLength);

			if
				(
					// x checks
				(
					(x_pos == arrayPtr[y].getVertexX())
					||
					(x_current == arrayPtr[y].getVertexX())
					||
					(x_neg == arrayPtr[y].getVertexX())
					)

					&&

					// y checks
					(
					(y_pos == arrayPtr[y].getVertexY())
						||
						(y_current == arrayPtr[y].getVertexY())
						||
						(y_neg == arrayPtr[y].getVertexY())
						)

					&&

					// z checks
					(
					(z_pos == arrayPtr[y].getVertexZ())
						||
						(z_current == arrayPtr[y].getVertexZ())
						||
						(z_neg == arrayPtr[y].getVertexZ())
						)
					)
			{
				searchData.isPointFound = 1;		// point was found, set this value in return data
				searchData.foundPointIndex = y;	// indicate the index it was found at
			}
		}
	}
	return searchData;
}

BlockSearchMeta FanManager::checkForExactPoint(ECBPolyPoint in_point, int in_debugFlag)
{
	BlockSearchMeta returnMeta;
	EnclaveBlockVertex baseVertex = IndependentUtils::convertPolyPointToBlockVertex(in_point);
	PointSearchData searchResults = checkIfPointExists(baseVertex);
	if (searchResults.isPointFound == 1)
	{
		returnMeta.foundPoint = convertVertexToPolyPoint(fetchPoint(searchResults.foundPointIndex));	// checks for the point in either LOCAL or OUTSOURCED data
		returnMeta.searchResult = 1;
		if (in_debugFlag == 1)
		{
			std::cout << "!!! Exact match found... " << std::endl;
		}
	}
	return returnMeta;
}

BlockSearchMeta FanManager::checkIfNearbyPointExistsOnLine(ECBPolyPoint in_point, EnclaveKeyDef::EnclaveKey in_key, int in_debugFlag)
{
	BlockSearchMeta currentSearchMeta;
	EnclaveBlockVertex* arrayPtr = NULL;
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS) { arrayPtr = localVertexArray; }
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS) { arrayPtr = expandedVertexArray.get(); }
	EnclaveBlockVertex baseVertex = IndependentUtils::convertPolyPointToBlockVertex(in_point);
	int numberOfIncrementTicks = 1;		// this needs to be fixed.
	if (in_key.x == 0)
	{
		EnclaveBlockVertex posSearch = baseVertex;
		EnclaveBlockVertex negSearch = baseVertex;
		for (int x = 0; x < numberOfIncrementTicks; x++)
		{
			//posSearch.x += 1;
			posSearch.incrementX(1);
			PointSearchData posResults = checkIfPointExists(posSearch);
			if (posResults.isPointFound == 1)
			{
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[posResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}

			//negSearch.x -= 1;
			negSearch.decrementX(1);
			PointSearchData negResults = checkIfPointExists(negSearch);
			if (negResults.isPointFound == 1)
			{
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[negResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}
		}
	}
	if (in_key.y == 0)
	{
		EnclaveBlockVertex posSearch = baseVertex;
		EnclaveBlockVertex negSearch = baseVertex;
		for (int x = 0; x < numberOfIncrementTicks; x++)
		{
			//posSearch.y += 1;
			posSearch.incrementY(1);
			PointSearchData posResults = checkIfPointExists(posSearch);
			if (posResults.isPointFound == 1)
			{
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[posResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}

			//negSearch.y -= 1;
			negSearch.decrementY(1);
			PointSearchData negResults = checkIfPointExists(negSearch);
			if (negResults.isPointFound == 1)
			{
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[negResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}
		}
	}
	if (in_key.z == 0)
	{
		// search positive z
		EnclaveBlockVertex posSearch = baseVertex;
		EnclaveBlockVertex negSearch = baseVertex;
		for (int x = 0; x < numberOfIncrementTicks; x++)
		{
			//posSearch.z += 1;
			posSearch.incrementZ(1);
			PointSearchData posResults = checkIfPointExists(posSearch);
			if (posResults.isPointFound == 1)
			{
				//std::cout << "Nearby point found! " << std::endl;
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[posResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}

			//negSearch.z -= 1;
			posSearch.decrementZ(1);
			PointSearchData negResults = checkIfPointExists(negSearch);
			if (negResults.isPointFound == 1)
			{
				currentSearchMeta.foundPoint = convertVertexToPolyPoint(arrayPtr[negResults.foundPointIndex]);
				currentSearchMeta.searchResult = 1;
				break;
			}
		}
	}
	return currentSearchMeta;
}

EnclaveBlockVertex FanManager::fetchPoint(int in_pointIndex)
{
	EnclaveBlockVertex returnVertex;
	EnclaveBlockVertex* arrayPtr = NULL;
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS) { arrayPtr = localVertexArray; }
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS) { arrayPtr = expandedVertexArray.get(); }
	return arrayPtr[in_pointIndex];
}

int FanManager::getTotalNumberOfTertiariesInBlock()
{
	int totalCount = 0;
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		for (int x = 0; x < totalFans; x++)
		{
			totalCount += localFanArray[x].numberOfTertiaries;
		}
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		for (int x = 0; x < totalFans; x++)
		{
			totalCount += expandedThinFanArray[x].numberOfTertiaries;
		}
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		for (int x = 0; x < totalFans; x++)
		{
			totalCount += expandedFatFanArray[x].numberOfTertiaries;
		}
	}
	return totalCount;
}

void FanManager::listSecondaries()
{
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		std::cout << ">>> Printing LOCALIZED mode array...; totalFans value is: " << totalFans << std::endl;
		for (int x = 0; x < totalFans; x++)
		{
			std::cout << "Printing points/data for fan at " << x << std::endl;
			//localFanArray[x].printPoints();
			auto targetFanPtr = &localFanArray[x];
			std::cout << "Orientation: " << targetFanPtr->faceAlignment.getPrintableIndicatorValue() << std::endl;
			auto currentFanData = targetFanPtr->getFanData();
			std::cout << "Normal: " << targetFanPtr->emptyNormal.x << ", " << targetFanPtr->emptyNormal.y << ", " << targetFanPtr->emptyNormal.z << std::endl;
			int totalPointsToPrint = currentFanData.numberOfTertiaries + 2;
			for (int y = 0; y < totalPointsToPrint; y++)
			{
				unsigned int currentPointIndex = currentFanData.pointArray[y];
				auto fetchedVertex = fetchPoint(currentPointIndex);
				auto vertexConvertedToPoint = convertVertexToPolyPoint(fetchedVertex);
				std::cout << "Point at triangle index [" << y << "], referencing unique point index " << currentPointIndex <<  ", (" << vertexConvertedToPoint.x << ", " << vertexConvertedToPoint.y << ", " << vertexConvertedToPoint.z << ") " << std::endl;

			}

		}
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		std::cout << ">>> Printing THIN mode array..." << std::endl;
		for (int x = 0; x < totalFans; x++)
		{
			std::cout << "THIN array index: " << x << std::endl;
			expandedThinFanArray[x].printPoints();
		}
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		std::cout << ">>> Printing FAT mode array..." << std::endl;
		for (int x = 0; x < totalFans; x++)
		{
			expandedFatFanArray[x].printPoints();
		}
	}
}

std::string FanManager::generateFanManagerHash()
{
	std::string generatedHash = "";

	// convert the initial value of 0 to a string
	std::string currentHashInput = HashUtils::sha256("0");

	switch (currentFanStorageMode)
	{
		case FanArrayMode::LOCALIZED:
		{
			// For each fan, get each of it's points and put it through SHA256.
			for (int x = 0; x < totalFans; x++)
			{
				auto targetFanPtr = &localFanArray[x];
				auto currentFanData = targetFanPtr->getFanData();
				int totalPointsToPrint = currentFanData.numberOfTertiaries + 2;
				for (int y = 0; y < totalPointsToPrint; y++)
				{
					unsigned int currentPointIndex = currentFanData.pointArray[y];
					auto fetchedVertex = fetchPoint(currentPointIndex);

					// for each vertex dimension, use that as the input for redetermining currentHashInput.
					currentHashInput = HashUtils::sha256(currentHashInput + std::to_string(fetchedVertex.getVertexX()));
					currentHashInput = HashUtils::sha256(currentHashInput + std::to_string(fetchedVertex.getVertexY()));
					currentHashInput = HashUtils::sha256(currentHashInput + std::to_string(fetchedVertex.getVertexZ()));
				}
			}
			break;
		}

		case FanArrayMode::THIN:
		{
			break;
		}

		case FanArrayMode::FAT:
		{
			break;
		}
	}
	return generatedHash = currentHashInput;
}

void FanManager::listPoints()
{
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS)
	{
		for (int x = 0; x < totalPoints; x++)
		{
			std::cout << "(LOCAL_POINTS) Point at index: [" << x << "]: " << int(localVertexArray[x].getVertexX()) << ", " << int(localVertexArray[x].getVertexY()) << ", " << int(localVertexArray[x].getVertexZ()) << std::endl;
		}
	}
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS)
	{
		for (int x = 0; x < totalPoints; x++)
		{
			std::cout << "(NONLOCAL_POINTS) Point at index: [" << x << "]: " << int(expandedVertexArray[x].getVertexX()) << ", " << int(expandedVertexArray[x].getVertexY()) << ", " << int(expandedVertexArray[x].getVertexZ()) << std::endl;
		}
	}
}

int FanManager::getNumberOfTotalFans()
{
	return totalFans;
}

int FanManager::getNumberOfTotalTriangles()
{
	int total = 0;
	for (int x = 0; x < totalFans; x++)
	{
		switch (currentFanStorageMode)
		{
			case FanArrayMode::LOCALIZED: { total += localFanArray[x].getNumberOfTrianglesInFan(); break; };
			case FanArrayMode::THIN: { total += expandedThinFanArray[x].getNumberOfTrianglesInFan(); break; };
			case FanArrayMode::FAT: { total += expandedFatFanArray[x].getNumberOfTrianglesInFan(); break; };
		};
	}
	return total;
}

FanBase* FanManager::retrieveNextAvailableSecondary()
{
	// remember, the current value of totalFans also represents the nextIndex to retrieve from.
	FanBase* returnPtr = NULL;
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		returnPtr = &localFanArray[totalFans];
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		returnPtr = &expandedThinFanArray[totalFans];
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		returnPtr = &expandedFatFanArray[totalFans];
	}
	return returnPtr;
}

FanBase* FanManager::retrieveSecondaryFromIndex(int in_index)
{
	FanBase* returnPtr = NULL;
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		returnPtr = &localFanArray[in_index];
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		returnPtr = &expandedThinFanArray[in_index];
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		returnPtr = &expandedFatFanArray[in_index];
	}
	return returnPtr;
}

int FanManager::getNumberOfTertiariesInTriangleAtIndex(int in_index)
{
	int numberOfTertiaries = 0;
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		numberOfTertiaries += localFanArray[in_index].numberOfTertiaries;
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		numberOfTertiaries += expandedThinFanArray[in_index].numberOfTertiaries;
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		numberOfTertiaries += expandedFatFanArray[in_index].numberOfTertiaries;
	}
	return numberOfTertiaries;
}

ECBPolyPoint FanManager::getEmptyNormalFromTriangle(int in_index)
{
	ECBPolyPoint returnNormal;
	if (currentFanStorageMode == FanArrayMode::LOCALIZED)
	{
		returnNormal = localFanArray[in_index].emptyNormal;
	}
	else if (currentFanStorageMode == FanArrayMode::THIN)
	{
		returnNormal = expandedThinFanArray[in_index].emptyNormal;
	}
	else if (currentFanStorageMode == FanArrayMode::FAT)
	{
		returnNormal = expandedFatFanArray[in_index].emptyNormal;
	}
	return returnNormal;
}

void FanManager::checkForTriangleExpansion()
{
	// check if the number of triangles is the same as expandedFanArraySize
	if
		(
		(totalFans == expandedFanArraySize)
			&&
			(totalFans >= 16)						// always expand if we are above 16.
			)
	{
		currentFanStorageMode = FanArrayMode::THIN;			// set to THIN mode, since we are no longer localized.
		expandTriangleArray();
	}
}

void FanManager::checkForVertexExpansion()
{
	if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS)
	{
		// staying at THIN
		if (totalPoints == 16)	// if we are already at 16 points, we've reached the max, and we need to expand.
		{
			expandedVertexArraySize += 16;	// expand by 16, so we can allocate accordingly.
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			currentPointStorageMode = PointArrayMode::NONLOCAL_POINTS;
			for (int x = 0; x < 16; x++)
			{
				expandedVertexArray[x] = localVertexArray[x];
			}
		}
	}
	else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS)
	{
		// as long as we're at < 256, stay at THIN.
		if (totalPoints < 256)
		{
			std::unique_ptr<EnclaveBlockVertex[]> tempOldVertexArray;
			int oldArraySize = expandedVertexArraySize;
			tempOldVertexArray.reset(new EnclaveBlockVertex[oldArraySize]);
			for (int x = 0; x < oldArraySize; x++)
			{
				tempOldVertexArray[x] = expandedVertexArray[x];
			}

			expandedVertexArraySize += 16;
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < oldArraySize; x++)
			{
				expandedVertexArray[x] = tempOldVertexArray[x];
			}

		}
	}
}

void FanManager::expandTriangleArray()
{
	if (currentFanStorageMode == FanArrayMode::THIN)
	{
		// if the expandedFanArraySize is 16, we need to move from LOCALIZED (local arrays) to THIN (unique_ptr<ThinFan[]>)
		if (expandedFanArraySize == 16)
		{
			//std::unique_ptr<ThinFan[]> tempThinArray;
			//tempThinArray.reset(new ThinFan[expandedFanArraySize]);		// create a temporary array, that is equal to expandedFanArraySize BEFORE we add 16 to it.

			expandedFanArraySize += 16;										// increment the fan array size.
			expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);	// allocate the array.
			isExpandedFanArrayActive = true;								// set expanded array flag.
			for (int x = 0; x < 16; x++)									// we only need to increment 16 times, as local only stores 16.
			{
				expandedThinFanArray[x] = localFanArray[x];
			}

		}

		// otherwise, we're just expanding by 16 every time.
		else if (expandedFanArraySize > 16)
		{
			std::unique_ptr<ThinFan[]> tempThinArray;
			int oldArraySize = expandedFanArraySize;
			tempThinArray.reset(new ThinFan[oldArraySize]);		// create a temporary array, that is equal to expandedFanArraySize BEFORE we add 16 to it.
			for (int x = 0; x < oldArraySize; x++)
			{
				tempThinArray[x] = expandedThinFanArray[x];				// load the contents over.
			}


			expandedFanArraySize += 16;	// now, expand the array size.
			expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);	// allocate the array.
			isExpandedFanArrayActive = true;								// set expanded array flag.
			for (int x = 0; x < oldArraySize; x++)							// we only need to copy oldArraySize elements; the rest of the newly expanded array will be blank/uninitialized.
			{
				expandedThinFanArray[x] = tempThinArray[x];
			}

		}
	}
}

Message FanManager::writeFanManagerToBDMFormat()
{
	Message fanManagerToMsg;	// this Message gets appended to another one, in the call to EnclaveBlock::writeEnclaveBlockToBDMMessage(), 
								// so specifying the type is pointless.

	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	// PART 1: Point data
	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	//
	// Step 1.1: Read, translate to int, and store the values of (in this order):
	//	-currentPointStorageMode
	//	-expandedVertexArraySize
	//	-totalPoints 
	fanManagerToMsg.insertInt(int(currentPointStorageMode));
	fanManagerToMsg.insertInt(int(expandedVertexArraySize));
	fanManagerToMsg.insertInt(int(totalPoints));

	// Step 1.2: Based on the value of the currentPointStorageMode enum, determine which point array data to read from
	// (this will be either localVertexArray or expandedVertexArray.
	switch (currentPointStorageMode)
	{
		// Below: for each scenario, produce appropriate Messages from each EnclaveBlockVertex,
		// and then append them into the fanManagertoMsg.
		// 
		// LOCAL_POINTS means using we are using localVertexArray.
		case PointArrayMode::LOCAL_POINTS:	
		{
			for (int x = 0; x < totalPoints; x++)
			{
				// write out the EnclaveBlockVertices from localVertexArray into this
				//fanManagerToMsg.appendOtherMessage(&localVertexArray[x].convertBlockVertexToMessage());	// Works with C++_17

				Message otherBlockVertexMessage = localVertexArray[x].convertBlockVertexToMessage();	// Works with C++_20
				fanManagerToMsg.appendOtherMessage(&otherBlockVertexMessage);
			}
			break;
		}

		case PointArrayMode::NONLOCAL_POINTS:
		{
			for (int x = 0; x < totalPoints; x++)
			{
				// write out the EnclaveBlockVertices from expandedVertexArray into this
				//fanManagerToMsg.appendOtherMessage(&expandedVertexArray[x].convertBlockVertexToMessage());

				Message otherBlockVertexMessage = expandedVertexArray[x].convertBlockVertexToMessage();	// Works with C++_20
				fanManagerToMsg.appendOtherMessage(&otherBlockVertexMessage);
			}
			break;
		}
	}

	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	// PART 2: Fan data
	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	//
	// Step 2.1: Read, translate to int, and store the values of (in this order):
	//	-currentFanStorageMode
	//	-expandedFanArraySize
	//	-totalFans
	fanManagerToMsg.insertInt(int(currentFanStorageMode));
	fanManagerToMsg.insertInt(int(expandedFanArraySize));
	fanManagerToMsg.insertInt(int(totalFans));
	fanManagerToMsg.insertInt(int(isExpandedFanArrayActive));

	// Step 2.2: Based on the value of currentFanStorageMode, determine where to read the fan array data that we
	// will be writing for.
	switch (currentFanStorageMode)
	{
		// For each case below, we will fetch fans and have their data written to Message format; 
		// we do this by reading from the appropriate array.
		//

		// If LOCALIZED, fetch fans from the localFanArray
		case FanArrayMode::LOCALIZED:
		{
			for (int x = 0; x < totalFans; x++)
			{
				//fanManagerToMsg.appendOtherMessage(&localFanArray[x].getFanData().convertFanDataToMessage());

				Message otherMessageToAppend = localFanArray[x].getFanData().convertFanDataToMessage();
				fanManagerToMsg.appendOtherMessage(&otherMessageToAppend);
			}
			break;
		}

		// If THIN, fetch fans from expandedThinFanArray.
		case FanArrayMode::THIN:
		{
			for (int x = 0; x < totalFans; x++)
			{
				//fanManagerToMsg.appendOtherMessage(&expandedThinFanArray[x].getFanData().convertFanDataToMessage());	// Works in C++_17

				Message ohterMessageToAppend = expandedThinFanArray[x].getFanData().convertFanDataToMessage();	// Must be this way for C++_20
				fanManagerToMsg.appendOtherMessage(&ohterMessageToAppend);
			}
			break;
		}

		// <<<<<<<<<<<<< FAT mode needs testing implemenation still >>>>>>>>>>>>>>>
		case FanArrayMode::FAT:
		{
			for (int x = 0; x < totalFans; x++)
			{

			}
			break;
		}
	}

	return fanManagerToMsg;
}

void FanManager::constructManagerFromMessage(Message* in_managerDataMessage)
{
	// Remember, this function assumes that the Message has already been opened. (we will likely be reading from a big Message that contains more data outside of this)

	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	// PART 1: Point data
	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	//
	// Step 1.1: Read and translate the values of (in this order);
	// the resulting values will be copied directly into the member variables of this class.
	//
	//	-currentPointStorageMode (must be casted back to enum)
	//	-expandedVertexArraySize
	//	-totalPoints 
	//
	currentPointStorageMode = PointArrayMode(in_managerDataMessage->readInt());
	expandedVertexArraySize = unsigned short(in_managerDataMessage->readInt());
	totalPoints = unsigned short(in_managerDataMessage->readInt());

	// Step 1.2: Based on the currentPointStorageMode and totalPoints, determine where the points will be stored.
	switch (currentPointStorageMode)
	{
		case PointArrayMode::LOCAL_POINTS:
		{
			// Points must be read into the localVertexArray.
			for (int x = 0; x < totalPoints; x++)
			{
				unsigned char current_vertex_x = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_y = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_z = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_usesUV = unsigned char(in_managerDataMessage->readInt());
				float current_vertex_U = in_managerDataMessage->readFloat();
				float current_vertex_V = in_managerDataMessage->readFloat();

				EnclaveBlockVertexTextureModeEnum textureMode = EnclaveBlockVertexTextureModeEnum::TILED_COORDS;
				if (current_vertex_usesUV == 1)
				{
					textureMode = EnclaveBlockVertexTextureModeEnum::TEXTURE_COORDS;
				}

				EnclaveBlockVertex constructedVertex(current_vertex_x, current_vertex_y, current_vertex_z, textureMode, current_vertex_U, current_vertex_V);
				localVertexArray[x] = constructedVertex;
			}
			break;
		}

		case PointArrayMode::NONLOCAL_POINTS:
		{
			// Before anything else: reset the expandedVertexArray unique_ptr. The length of the array 
			// should be equal to the value of expandedVertexArraySize.
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);

			// Points must be read into the expandedVertexArray.
			for (int x = 0; x < totalPoints; x++)
			{
				unsigned char current_vertex_x = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_y = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_z = unsigned char(in_managerDataMessage->readInt());
				unsigned char current_vertex_usesUV = unsigned char(in_managerDataMessage->readInt());
				float current_vertex_U = in_managerDataMessage->readFloat();
				float current_vertex_V = in_managerDataMessage->readFloat();

				EnclaveBlockVertexTextureModeEnum textureMode = EnclaveBlockVertexTextureModeEnum::TILED_COORDS;
				if (current_vertex_usesUV == 1)
				{
					textureMode = EnclaveBlockVertexTextureModeEnum::TEXTURE_COORDS;
				}

				EnclaveBlockVertex constructedVertex(current_vertex_x, current_vertex_y, current_vertex_z, textureMode, current_vertex_U, current_vertex_V);
				expandedVertexArray[x] = constructedVertex;
			}
			break;
		}
	}

	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	// PART 2: Fan data
	// |||||||||||||||||||||||||||||||||||||||||||||||||||||
	//
	// Step 2.1: Read and translate the values of (in this order);
	// the resulting values will be copied directly into the member variables of this class.
	//
	//	-currentFanStorageMode
	//	-expandedFanArraySize
	//	-totalFans
	//	-isExpandedFanArrayActive
	currentFanStorageMode = FanArrayMode(in_managerDataMessage->readInt());
	expandedFanArraySize = in_managerDataMessage->readInt();
	totalFans = in_managerDataMessage->readInt();
	isExpandedFanArrayActive = in_managerDataMessage->readInt();

	// Step 2.2: Based on the value of currentFanStorageMode, determine where to write the fan array data to.
	switch (currentFanStorageMode)
	{
		// If LOCALIZED, construct and write fans to the localFanArray
		case FanArrayMode::LOCALIZED:
		{
			for (int x = 0; x < totalFans; x++)
			{
				//fanManagerToMsg.appendOtherMessage(&localFanArray[x].getFanData().convertFanDataToMessage());
				FanData streamedFanData(in_managerDataMessage);
				ThinFan newFan;
				newFan.buildFromFanData(streamedFanData);
				localFanArray[x] = newFan;
			}
			break;
		}

		// If THIN, construct and write fans to the  expandedThinFanArray.
		case FanArrayMode::THIN:
		{
			for (int x = 0; x < totalFans; x++)
			{
				//fanManagerToMsg.appendOtherMessage(&expandedThinFanArray[x].getFanData().convertFanDataToMessage());
				FanData streamedFanData(in_managerDataMessage);
				ThinFan newFan;
				newFan.buildFromFanData(streamedFanData);
				expandedThinFanArray[x] = newFan;
			}
			break;
		}

		// <<<<<<<<<<<<< FAT mode needs testing implemenation still >>>>>>>>>>>>>>>
		case FanArrayMode::FAT:
		{
			for (int x = 0; x < totalFans; x++)
			{

			}
			break;
		}
	}
}
