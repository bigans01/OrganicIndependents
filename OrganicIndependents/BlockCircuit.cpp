#include "stdafx.h"
#include "BlockCircuit.h"
#include "IndependentUtils.h"

BlockCircuit::BlockCircuit(BlockBorderLineList* in_blockBorderLineListRef, BorderDataMap* in_borderDataMapRef, int in_perfectClampValue, PolyDebugLevel in_debugLevel)
{
	//std::cout << "############### !!! Block Circuit constructor call!! " << std::endl;

	borderDataMapRef = in_borderDataMapRef;
	blockBorderLineListRef = in_blockBorderLineListRef;
	perfectClampValue = in_perfectClampValue;
	debugLevel = in_debugLevel;
	logger.setDebugLevel(debugLevel);			// set the BlockCircuit's debug logging level
}

void BlockCircuit::insertNewSegment(PrimarySegmentMeta in_primarySegment)
{
	SCircuitSegment newSegment;
	newSegment.segment = in_primarySegment;							// only the primary segment is appended to the SCircuitSegment initially (other values computed later)
	newSegment.borderDataMapRef = borderDataMapRef;					// set the pointers (no computation is needed)
	newSegment.blockBorderLineListRef = blockBorderLineListRef;
	newSegment.perfectClampValue = perfectClampValue;
	newSegment.setDebugMode(debugLevel);							// set this new segment's debug logging level
	segmentArray[currentSegmentIndex++] = newSegment;	// insert the new segment
	currentSegmentCount++;
	currentMaxIndex++;
}

bool BlockCircuit::fillSegmentArrayMetaDataWithPointsV2(int in_faceMatchThreshold, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2)
{
	currentMaxIndex--;	 // this function gets called once per segment, so just decrement this one time. (Example, 2 segments would mean that the final index = 1)
	calculateSegmentLinks(in_faceMatchThreshold);
	//std::cout << "SEGMENT COUNT: -> " << currentSegmentCount << std::endl;
	if (currentSegmentCount > 1)
	{
		//std::cout << "SEGMENT COUNT: -> " << currentSegmentCount << std::endl;
		// only determine final types when there is more than one segment. (its not necessary for STANDALONE type)
		determineFinalTypeForFirstSegment();
		determineFinalTypeForLastSegment();
	}

	/*
	std::cout << "##### Segment count is: " << currentSegmentCount << std::endl;
	for (int x = 0; x < currentSegmentCount; x++)
	{
		std::cout << x << ": Start: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << " | End: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
	}
	*/

	bool validityResult = gatherCircuitPointsWithString("fillSegmentArrayData #1", in_point0, in_point1, in_point2);	// gather points from the segment(s)
	if (validityResult == false)
	{
		//std::cout << "(BlockCircuit): bad circuit detected in call to function, fillSegmentArrayMetaDataWithPointsV2" << std::endl;
	}
	return validityResult;
	//gatherCircuitPoints();
}

bool BlockCircuit::fillSegmentArrayMetaDataWithPointsV1(int in_faceMatchThreshold, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, std::string in_optionalString)	// debugging only
{
	currentMaxIndex--;	 // this function gets called once per segment, so just decrement this one time. (Example, 2 segments would mean that the final index = 1)
	calculateSegmentLinks(in_faceMatchThreshold);
	if (currentSegmentCount > 1)
	{
		//std::cout << "SEGMENT COUNT: -> " << currentSegmentCount << std::endl;
		// only determine final types when there is more than one segment. (its not necessary for STANDALONE type)
		determineFinalTypeForFirstSegment();
		determineFinalTypeForLastSegment();
	}

	/*
	std::cout << "##### Segment count is: " << currentSegmentCount << std::endl;
	for (int x = 0; x < currentSegmentCount; x++)
	{
		std::cout << x << ": Start: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << " | End: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
	}
	*/

	//gatherCircuitPoints(in_point0, in_point1, in_point2, in_optionalString);	// gather points from the segment(s)
	bool validityResult = gatherCircuitPointsWithString(in_optionalString, in_point0, in_point1, in_point2);
	if (validityResult == false)
	{
		//std::cout << "(BlockCircuit): bad circuit detected in call to function, fillSegmentArrayMetaDataWithPointsV1" << std::endl;
	}
	return validityResult;
}

bool BlockCircuit::fillSegmentArrayMetaDataFromCircuit(int in_faceMatchThreshold, EnclaveKeyDef::EnclaveKey in_enclaveKey, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2)	// for debugging
{
	circuitEnclaveKey = in_enclaveKey;
	currentMaxIndex--;	 // this function gets called once per segment, so just decrement this one time. (Example, 2 segments would mean that the final index = 1)
	calculateSegmentLinks(in_faceMatchThreshold);
	//std::cout << "SEGMENT COUNT: -> " << currentSegmentCount << std::endl;

	if (currentSegmentCount > 1)
	{
		//std::cout << "SEGMENT COUNT: -> " << currentSegmentCount << std::endl;
		// only determine final types when there is more than one segment. (its not necessary for STANDALONE type)
		determineFinalTypeForFirstSegment();
		determineFinalTypeForLastSegment();
	}

	/*
	std::cout << "##### Segment count is: " << currentSegmentCount << std::endl;
	for (int x = 0; x < currentSegmentCount; x++)
	{
		std::cout << x << ": Start: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << " | End: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
	}
	*/

	bool validityResult = gatherCircuitPointsWithString("fillSegmentArrayData #2", in_point0, in_point1, in_point2);	// gather points from the segment(s)
	if (validityResult == false)
	{
		//std::cout << "(BlockCircuit): bad circuit detected in call to function, fillSegmentArrayMetaDataFromCircuit" << std::endl;
	}
	return validityResult;
	//gatherCircuitPoints();
}

void BlockCircuit::calculateSegmentLinks(int in_faceMatchThreshold)
{
	// first, set the next link chain values in each SCircuitSegment, along with the corresponding targetPoint
	for (int x = 0; x < currentSegmentCount; x++)
	{
		if (x != currentMaxIndex)	// as long as we are not in the last element of the circuit, set the link to be the next segment
		{
			segmentArray[x].indexOfNextSegment = x + 1;
			ECBPolyPoint targetPointToAdd = segmentArray[x + 1].segment.startPoint;		// fetch the point from the next segment in the circuit
			segmentArray[x].targetPoint = targetPointToAdd;								// insert it

			//if (segmentArray[x].targetPoint.x == 2)
			if (segmentArray[x].targetPoint.x > 1)
			{

				std::cout << "####### Segment data: " << std::endl;
				std::cout << "Begin point: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << std::endl;
				std::cout << "End point: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
				std::cout << "Target point: " << segmentArray[x].targetPoint.x << ", " << segmentArray[x].targetPoint.y << ", " << segmentArray[x].targetPoint.z << std::endl;

				std::cout << "!!! data of next segment is: " << std::endl;
				std::cout << "Begin point: " << segmentArray[x + 1].segment.startPoint.x << ", " << segmentArray[x + 1].segment.startPoint.y << ", " << segmentArray[x + 1].segment.startPoint.z << std::endl;
				std::cout << "End point: " << segmentArray[x + 1].segment.endPoint.x << ", " << segmentArray[x + 1].segment.endPoint.y << ", " << segmentArray[x + 1].segment.endPoint.z << std::endl;
				std::cout << "Target point: " << segmentArray[x + 1].targetPoint.x << ", " << segmentArray[x + 1].targetPoint.y << ", " << segmentArray[x + 1].targetPoint.z << std::endl;
			}
		}
		else if (x == currentMaxIndex)	// otherwise, the next segment's index is 0 (completing the circuit)
		{
			segmentArray[x].indexOfNextSegment = 0;
			ECBPolyPoint targetPointToAdd = segmentArray[0].segment.startPoint;			// "" 
			segmentArray[x].targetPoint = targetPointToAdd;								// ""
		}
	}

	if (currentSegmentCount == 1)
	{
		// *** do the following only if there is just one segment. ***
		//std::cout << "SEGMENT COUNT (STANDALONE) : -> " << currentSegmentCount << std::endl;
		segmentArray[0].faceMatchThreshold = in_faceMatchThreshold;
		segmentArray[0].setAsStandaloneSegment();

	}

	else if (currentSegmentCount > 1)
	{
		// next, calculate the initial values for the CircuitSegmentType (default value is NOVAL, but always changes)
		for (int x = 0; x < currentSegmentCount; x++)
		{
			segmentArray[x].faceMatchThreshold = in_faceMatchThreshold;		// must be set before the initial type is calculated
			segmentArray[x].determineInitialType();		// determine initial values for CLOSED, OPEN_SINGLE, OPEN_MULTI...
		}
	}
}

void BlockCircuit::determineFinalTypeForFirstSegment()
{


	SCircuitSegment* segmentRef = &segmentArray[0];		// point to the first segment

	//std::cout << "(Begin segment) segment (( point A: " << segmentRef->segment.startPoint.x << ", " << segmentRef->segment.startPoint.y << ", " << segmentRef->segment.startPoint.z << " | point B: " << segmentRef->segment.endPoint.x << ", " << segmentRef->segment.endPoint.y << ", " << segmentRef->segment.endPoint.z << ")) " <<std::endl;

	CircuitSegmentType* typeRef = &segmentRef->type;
	if (*typeRef == CircuitSegmentType::CLOSED)
	{
		*typeRef = CircuitSegmentType::CLOSED_FIRST;
	}
	if (*typeRef == CircuitSegmentType::OPEN_SINGLE)
	{
		*typeRef = CircuitSegmentType::OPEN_SINGLE_FIRST;
	}
	if (*typeRef == CircuitSegmentType::OPEN_MULTI)
	{
		*typeRef = CircuitSegmentType::OPEN_MULTI_FIRST;
	}
}
void BlockCircuit::determineFinalTypeForLastSegment()
{
	SCircuitSegment* segmentRef = &segmentArray[currentMaxIndex];		// point to the first segment
	//std::cout << "(End segment) (( point A: " << segmentRef->segment.startPoint.x << ", " << segmentRef->segment.startPoint.y << ", " << segmentRef->segment.startPoint.z << " | point B: " << segmentRef->segment.endPoint.x << ", " << segmentRef->segment.endPoint.y << ", " << segmentRef->segment.endPoint.z << ")) " << std::endl;
	CircuitSegmentType* typeRef = &segmentRef->type;
	if (*typeRef == CircuitSegmentType::CLOSED)
	{
		*typeRef = CircuitSegmentType::CLOSED_LAST;
	}
	if (*typeRef == CircuitSegmentType::OPEN_SINGLE)
	{
		*typeRef = CircuitSegmentType::OPEN_SINGLE_LAST;
	}
	if (*typeRef == CircuitSegmentType::OPEN_MULTI)
	{
		*typeRef = CircuitSegmentType::OPEN_MULTI_LAST;
	}
}

void BlockCircuit::printSegmentMetaData()
{
	for (int x = 0; x < currentSegmentCount; x++)
	{

		//std::cout << "Segment [" << x << "]: | Point A: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z
		//	<< " | Point B: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z
		//	<< " | Next segment: " << segmentArray[x].indexOfNextSegment
		//	<< " | Type: ";

		logger.log("Segment [", x, "]: | Point A: ", segmentArray[x].segment.startPoint.x, ", ", segmentArray[x].segment.startPoint.y, ", ", segmentArray[x].segment.startPoint.z);
		logger.log(" | Point B: ", segmentArray[x].segment.endPoint.x, ", ", segmentArray[x].segment.endPoint.y, ", ", segmentArray[x].segment.endPoint.z);
		logger.log(" | Next segment: ", segmentArray[x].indexOfNextSegment);
		logger.log(" | Type: ");

		if (segmentArray[x].type == CircuitSegmentType::CLOSED_FIRST)
		{
			//std::cout << "CLOSED_FIRST " << std::endl;
			logger.log("CLOSED_FIRST", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::CLOSED)
		{
			//std::cout << "CLOSED " << std::endl;
			logger.log("CLOSED", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::CLOSED_LAST)
		{
			//std::cout << "CLOSED_LAST " << std::endl;
			logger.log("CLOSED_LAST", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_SINGLE_FIRST)
		{
			//std::cout << "OPEN_SINGLE_FIRST " << std::endl;
			logger.log("OPEN_SINGLE_FIRST", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_SINGLE)
		{
			//std::cout << "OPEN_SINGLE " << std::endl;
			logger.log("OPEN_SINGLE", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_SINGLE_LAST)
		{
			//std::cout << "OPEN_SINGLE_LAST " << std::endl;
			logger.log("OPEN_SINGLE_LAST", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_MULTI_FIRST)
		{
			//std::cout << "OPEN_MULTI_FIRST " << std::endl;
			logger.log("OPEN_MULTI_FIRST", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_MULTI)
		{
			//std::cout << "OPEN_MULTI " << std::endl;
			logger.log("OPEN_MULTI", "\n");
		}

		else if (segmentArray[x].type == CircuitSegmentType::OPEN_MULTI_LAST)
		{
			//std::cout << "OPEN_MULTI_LAST " << std::endl;
			logger.log("OPEN_MULTI_LAST", "\n");
		}

		else
		{
			//std::cout << "NOVAL" << std::endl;
			logger.log("NOVAL", "\n");
		}
	}
}

bool BlockCircuit::gatherCircuitPointsWithString(std::string in_optionalString, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2)
{
	//std::cout << "!!! (BlockCircuit) Attempting gather..." << std::endl;
	for (int x = 0; x < currentSegmentCount; x++)
	{
		//std::cout << "!! Current segment: " << x << std::endl;

		//MicroPolyPointContainer currentContainer = segmentArray[x].getCircuitPoints(circuitEnclaveKey, circuitBlockKey, in_optionalString);
		MicroPolyPointContainer currentContainer = segmentArray[x].getCircuitPoints(circuitEnclaveKey, circuitBlockKey, in_point0, in_point1, in_point2, in_optionalString);
		//(circuitEnclaveKey, circuitBlockKey, in_point0, in_point1, in_point2, "BlockCircuit::gatherCircuitPoints (with points 1)");

		// only do the following if the container was valid
		if (currentContainer.isContainerValid == true)
		{
			// let's just print whats in there, shall we?
			if (currentContainer.numberOfPoints != 0)
			{
				//std::cout << "####### Container points for container " << x << ": " << std::endl;
				logger.log("####### Container points for container ", x, ": ", "\n");
				for (int x = 0; x < currentContainer.numberOfPoints; x++)
				{
					//std::cout << x << ": " << currentContainer.pointArray[x].x << ", " << currentContainer.pointArray[x].y << ", " << currentContainer.pointArray[x].z << std::endl;
					logger.log(x, ": ", currentContainer.pointArray[x].x, ", ", currentContainer.pointArray[x].y, ", ", currentContainer.pointArray[x].z, "\n");
				}
			}

			// ?????? why was this deleted?
			finalCircuitPoints.loadPointsFromOtherContainer(&currentContainer);
		}
		else if (currentContainer.isContainerValid == false)
		{
			//std::cout << "(BlockCircuit): circuit is not valid; called by function gatherCircuitPointsWithString " << std::endl;
			isCircuitValid = false;
		}
	}
	//std::cout << "!!! (BlockCircuit) Gather complete... " << std::endl;
	if (finalCircuitPoints.numberOfPoints == 2)
	{
		/*
		std::cout << "!!!! WARNING: MicroPolyPointContainer contains only two points; halting. " << std::endl;
		std::cout << "! Current segment count was: " << currentSegmentCount << std::endl;

		for (int x = 0; x < currentSegmentCount; x++)
		{
			std::cout << "Segment    : " << x << " data: " << std::endl;
			std::cout << "Begin point: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << std::endl;
			std::cout << "End   point: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
		}
		*/

		//int someVal = 3;
		//std::cin >> someVal;

		isCircuitValid = false;
	}

	// Check for duplicate points; if there are, the circuit is bad.
	/*
	bool duplicateCheck = checkForDuplicatePoints(&finalCircuitPoints);
	if (duplicateCheck == true)
	{
		isCircuitValid = false;
	}
	*/

	return isCircuitValid;
}

void BlockCircuit::gatherCircuitPointsWithStringV2(std::string in_optionalString, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2)
{
	//std::cout << "!!! (BlockCircuit) Attempting gather..." << std::endl;
	for (int x = 0; x < currentSegmentCount; x++)
	{
		MicroPolyPointContainer currentContainer = segmentArray[x].getCircuitPoints(circuitEnclaveKey, circuitBlockKey, in_point0, in_point1, in_point2, in_optionalString);


		// let's just print whats in there, shall we?
		if (currentContainer.numberOfPoints != 0)
		{
			//std::cout << "####### Container points for container " << x << ": " << std::endl;
			logger.log("####### Container points for container ", x, ": ", "\n");
			for (int x = 0; x < currentContainer.numberOfPoints; x++)
			{
				//std::cout << x << ": " << currentContainer.pointArray[x].x << ", " << currentContainer.pointArray[x].y << ", " << currentContainer.pointArray[x].z << std::endl;
				logger.log(x, ": ", currentContainer.pointArray[x].x, ", ", currentContainer.pointArray[x].y, ", ", currentContainer.pointArray[x].z, "\n");
			}
		}

		// ?????? why was this deleted?
		finalCircuitPoints.loadPointsFromOtherContainer(&currentContainer);
	}
	//std::cout << "!!! (BlockCircui) Gather complete... " << std::endl;
	if (finalCircuitPoints.numberOfPoints == 2)
	{
		std::cout << "!!!! WARNING: MicroPolyPointContainer contains only two points; halting. " << std::endl;
		std::cout << "! Current segment count was: " << currentSegmentCount << std::endl;

		for (int x = 0; x < currentSegmentCount; x++)
		{
			std::cout << "Segment    : " << x << " data: " << std::endl;
			std::cout << "Begin point: " << segmentArray[x].segment.startPoint.x << ", " << segmentArray[x].segment.startPoint.y << ", " << segmentArray[x].segment.startPoint.z << std::endl;
			std::cout << "End   point: " << segmentArray[x].segment.endPoint.x << ", " << segmentArray[x].segment.endPoint.y << ", " << segmentArray[x].segment.endPoint.z << std::endl;
		}

		//int someVal = 3;
		//std::cin >> someVal;

		isCircuitValid = false;
	}

	// Check for duplicate points; if there are, the circuit is bad.
	/*
	bool duplicateCheck = checkForDuplicatePoints(&finalCircuitPoints);
	if (duplicateCheck == true)
	{
		isCircuitValid = false;
	}
	*/
}

bool BlockCircuit::checkForDuplicatePoints(MicroPolyPointContainer* in_containerRef)
{
	bool duplicatesFound = false;
	int numberOfPointsToScan = in_containerRef->numberOfPoints;
	std::map<int, ECBPolyPoint> uniquePointMap;

	/*
	for (int x = 0; x < numberOfPointsToScan; x++)
	{
		// fetch the point
		ECBPolyPoint currentPoint = in_containerRef->pointArray[x];

		// scan to see if the point exists.
		if (uniquePointMap.size() == 0)	// for the very first point
		{
			uniquePointMap[uniquePointMap.size()] = currentPoint;
		}
		else
		{
			// check to see if the point matches.
			bool matchFound = false;
			auto currentPointsBegin = uniquePointMap.begin();
			auto currentPointsEnd = uniquePointMap.end();
			for (; currentPointsBegin != currentPointsEnd; currentPointsBegin++)
			{
				// if the below holds true, a duplicate was found.
				if (currentPoint == currentPointsBegin->second)
				{
					matchFound = true;
					duplicatesFound = true;
					break;
				}
			}

			// if we had no match, insert the point.
			if (matchFound == false)
			{
				uniquePointMap[uniquePointMap.size()] = currentPoint;
			}
		}
	}
	*/

	// HARD CODED check: all 1,1,1 points.
	int allOneCount = 0;
	for (int x = 0; x < numberOfPointsToScan; x++)
	{
		// fetch the point
		ECBPolyPoint currentPoint = in_containerRef->pointArray[x];
		if
		(
			currentPoint.x == 1.0f
			&&
			currentPoint.y == 1.0f
			&&
			currentPoint.z == 1.0f
		)
		{
			allOneCount++;
		}
	}
	if (allOneCount == numberOfPointsToScan)
	{
		duplicatesFound = true;
	}


	return duplicatesFound;
}

TertiaryTriangleContainer BlockCircuit::produceAndReturnTertiaryTriangles(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	//std::cout << "!!!!!! Final circuit number of points: " << finalCircuitPoints.numberOfPoints << std::endl;
	//std::cout << "Enclave Key: " << in_enclaveKey.x << ", " << in_enclaveKey.y << ", " << in_enclaveKey.z << std::endl;
	//std::cout << "Block Key: " << in_blockKey.x << ", " << in_blockKey.y << ", " << in_blockKey.z << std::endl;

	logger.log("!!!!!! Final circuit number of points: ", finalCircuitPoints.numberOfPoints, "\n");
	logger.log("Enclave Key: ", in_enclaveKey.x, ", ", in_enclaveKey.y, ", ", in_enclaveKey.z, "\n");
	logger.log("Block Key: ", in_blockKey.x, ", ", in_blockKey.y, ", ", in_blockKey.z, "\n");

	int numberOfTriangles = finalCircuitPoints.numberOfPoints - 2;		// the number of triangles in the circuit
	TertiaryTriangleContainer tertiaryData;
	//tertiaryData.triangleVertices[numberOfPoints++] = OrganicUtils::convertPolyPointToBlockVertex(snappedRoot);
	//tertiaryData.triangleVertices[numberOfPoints++] = OrganicUtils::convertPolyPointToBlockVertex(snappedCurrentBegin);
	tertiaryData.triangleVertices[0] = IndependentUtils::convertPolyPointToBlockVertex(finalCircuitPoints.pointArray[0]);
	tertiaryData.triangleVertices[1] = IndependentUtils::convertPolyPointToBlockVertex(finalCircuitPoints.pointArray[1]);
	int loopStartIndex = 2;
	for (int x = 0; x < numberOfTriangles; x++)
	{
		tertiaryData.triangleVertices[loopStartIndex] = IndependentUtils::convertPolyPointToBlockVertex(finalCircuitPoints.pointArray[loopStartIndex]);
		tertiaryData.numberOfTertiaryTriangles++;
		loopStartIndex++;
	}

	//std::cout << "!!! number of Triangles: " << tertiaryData.numberOfTertiaryTriangles << std::endl;
	logger.log("!!! number of Triangles: ", tertiaryData.numberOfTertiaryTriangles);
	if (debugLevel != PolyDebugLevel::NONE)
	{
		tertiaryData.listPoints();
	}
	return tertiaryData;
}