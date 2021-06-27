#include "stdafx.h"
#include "EnclaveTriangleInteriorRunner.h"

EnclaveTriangleInteriorRunner::EnclaveTriangleInteriorRunner(ECBPolyPoint in_beginPoint,
	ECBPolyPoint in_endPoint,
	PrimaryLineT1 in_primaryLine,
	BlockBorderLineList* in_blockBorderLineListRef,
	BorderDataMap* in_borderDataMapRef,
	std::map<int, OrganicWrappedBBFan>* in_wrappedBBFanMapRef,
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* in_exteriorTracedBlocksRef,
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>* in_allTracedBlocksRef,
	short in_materialID,
	short in_isPolyPerfectlyClamped,
	ECBPolyPoint in_emptyNormal,
	EnclaveKeyDef::EnclaveKey in_blockKey,
	int in_segmentCount,
	bool in_debugModeFlag
)
{
	// set debug mode (if needed)
	debugModeFlag = in_debugModeFlag;

	//std::cout << "!! EnclaveTriangleInteriorRunner initialized. " << std::endl;

	// set the line meta
	lineMeta = in_primaryLine;

	// set the line slope (which will be constant)
	lineSlope.x = in_endPoint.x - in_beginPoint.x;
	lineSlope.y = in_endPoint.y - in_beginPoint.y;
	lineSlope.z = in_endPoint.z - in_beginPoint.z;

	// set the block border data refs
	blockBorderLineListRef = in_blockBorderLineListRef;
	borderDataMapRef = in_borderDataMapRef;

	// set the map refs
	wrappedBBFanMapRef = in_wrappedBBFanMapRef;
	exteriorTracedBlocksMapRef = in_exteriorTracedBlocksRef;
	allTracedBlocksMapRef = in_allTracedBlocksRef;

	// begin points



	currentIterationBeginPoint = in_beginPoint;
	//result = OrganicUtils::getEnclaveTracingEndpointFromSlope(enclaveLocalPointA, calculatedLineSlope, &lineList);
	currentEndpointMeta = IndependentUtils::getBlockTracingEndpointMeta(currentIterationBeginPoint, lineSlope, blockBorderLineListRef);
	//currentEndpointMeta = OrganicUtils::getBlock
	currentIterationEndPoint = in_endPoint;

	initialBeginPoint = in_beginPoint;
	initialEndPoint = in_endPoint;
	initialBlockKey = in_blockKey;

	// normalized slope
	normalizedPrimaryLineSlope = IndependentUtils::findNormalizedSlope(currentIterationBeginPoint, currentIterationEndPoint);

	// point orientations
	currentBeginOrientation = IndependentUtils::GetPointOrientation(currentIterationBeginPoint, blockBorderLineListRef);
	currentEndOrientation = IndependentUtils::GetPointOrientation(currentIterationEndPoint, blockBorderLineListRef);

	//intendedFaces = in_primaryLine.intendedFaces;
	intendedFaces = IndependentUtils::determineIntendedFacesV2(in_primaryLine.beginPointRealXYZ, in_primaryLine.endPointRealXYZ, in_primaryLine.thirdPointRealXYZ, in_primaryLine.x_int, in_primaryLine.y_int, in_primaryLine.z_int);

	//std::cout << "!!! EnclaveTriangleInteriorRunner Constructor, intended faces are: " << intendedFaces.x << ", " << intendedFaces.y << ", " << intendedFaces.z << std::endl;

	materialID = in_materialID;
	isPolyPerfectlyClamped = in_isPolyPerfectlyClamped;
	emptyNormal = in_emptyNormal;

	// set the block key.
	currentBlockKey = in_blockKey;

	// 
	segmentCount = in_segmentCount;

	// run attempt iteration
	attemptIteration();
}

void EnclaveTriangleInteriorRunner::attemptIteration()
{
	EnclaveKeyDef::EnclaveKey tempMoveVals = IndependentUtils::retrieveBorderDirection(currentEndOrientation, borderDataMapRef);
	ECBPolyPoint commonValsToFind;
	commonValsToFind.x = float(tempMoveVals.x);
	commonValsToFind.y = float(tempMoveVals.y);
	commonValsToFind.z = float(tempMoveVals.z);
	ECBPolyPoint resultantMoveVals = IndependentUtils::findCommonMoveValues(commonValsToFind, normalizedPrimaryLineSlope);
	performCornerCheck(resultantMoveVals);			// don't start the run if the begin point is in a corner and has neightbors that complete.
	applyMovementToEnclaveKey(resultantMoveVals);	// apply the key, make sure its valid as well
	//checkBlockKeyValidity();			// only start the run if the block key is valid.

	bool wasTraced;
	wasTraced = checkIfBBFanWasTraced(currentBlockKey);
	if (wasTraced == false)
	{

		while (isRunComplete == false)
		{

			currentIterationBeginPoint = IndependentUtils::applyEnclaveResetValuesToPoint(currentIterationEndPoint, currentEndpointMeta.dimensionResetValues, currentEndpointMeta.moveDirectionValues);	// reset the previous end point with the correct new values
			currentEndpointMeta = IndependentUtils::getBlockTracingEndpointMeta(currentIterationBeginPoint, lineSlope, blockBorderLineListRef);
			currentIterationEndPoint = currentEndpointMeta.pointLocation;

			// code for inserting block goes here...
			// ...
			// ...
			// ...
						// work with the current block key here, to insert a new block poly.
			//			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			auto blockCheck = wrappedBBFanMapRef->find(convertedBlockCoords);
			if (blockCheck == wrappedBBFanMapRef->end())	// it wasn't found; insert.
			{
				//std::cout << "!!! Block wasn't found...inserting! Block key: (" << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
					//<< ") || Point A: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z 
					//<< " || Point B: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << ", " << std::endl;
				(*wrappedBBFanMapRef)[convertedBlockCoords].blockSegmentTracker.addNewSegment(currentIterationBeginPoint, currentIterationEndPoint, lineMeta.IDofLine);

				// NEW CODE BEGIN
				BlockCircuit circuit(blockBorderLineListRef, borderDataMapRef, isPolyPerfectlyClamped, PolyDebugLevel::NONE);
				for (int x = 0; x < (*wrappedBBFanMapRef)[convertedBlockCoords].blockSegmentTracker.currentLineSegmentIndex; x++)
				{
					PrimarySegmentMeta sMeta;

					/*
					for (int y = 0; y < 3; y++)
					{
						if (in_linkArrayRef->linkArray[y].IDofLine == secondaryBegin->second.blockSegmentTracker.lineSegments[x].lineID)
						{
							sMeta.x_slope = in_linkArrayRef->linkArray[y].x_int;
							sMeta.y_slope = in_linkArrayRef->linkArray[y].y_int;
							sMeta.z_slope = in_linkArrayRef->linkArray[y].z_int;
							sMeta.intendedFaces = in_linkArrayRef->linkArray[y].intendedFaces;
						}
					}
					*/

					sMeta.x_slope = lineMeta.x_int;
					sMeta.y_slope = lineMeta.y_int;
					sMeta.z_slope = lineMeta.z_int;
					//sMeta.intendedFaces = lineMeta.intendedFaces;
					sMeta.intendedFaces = intendedFaces;

					//std::cout << " Line Meta intended faces: " << sMeta.intendedFaces.x << ", " << sMeta.intendedFaces.y << ", " << sMeta.intendedFaces.z << std::endl;

					//std::cout << "--- X-int: " << sMeta.x_slope.x << ", " << sMeta.x_slope.y << ", " << sMeta.x_slope.z << std::endl;
					//std::cout << "--- Y-int: " << sMeta.y_slope.x << ", " << sMeta.y_slope.y << ", " << sMeta.y_slope.z << std::endl;
					//std::cout << "--- Z-int: " << sMeta.z_slope.x << ", " << sMeta.z_slope.y << ", " << sMeta.z_slope.z << std::endl;

					ECBPolyPoint segmentBeginPoint = currentIterationBeginPoint;
					ECBPolyPoint segmentEndPoint = currentIterationEndPoint;

					ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, blockBorderLineListRef);
					ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, blockBorderLineListRef);

					//BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, &borderDataMap);
					//BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, &borderDataMap);

					BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, borderDataMapRef);
					BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, borderDataMapRef);

					sMeta.startPoint = segmentBeginPoint;
					sMeta.startFaces = segmentBeginFaceList;
					sMeta.endPoint = segmentEndPoint;
					sMeta.endFaces = segmentEndFaceList;

					circuit.insertNewSegment(sMeta);
					fillCircuitMetaData(&circuit, blockBorderLineListRef, borderDataMapRef);
					populateOrganicWrappedBBFanWithCircuitData(&circuit, convertedBlockCoords);
				}
				//populateOrganicWrappedBBFanWithCircuitData(&circuit, convertedBlockCoords);
				// NEW CODE END

			}
			//secondaryMap.triangleMap[convertedBlockCoords].blockSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);

			// ...
			// ...
			// ...


			currentEndOrientation = IndependentUtils::GetPointOrientation(currentIterationEndPoint, blockBorderLineListRef);
			tempMoveVals = IndependentUtils::retrieveBorderDirection(currentEndOrientation, borderDataMapRef);
			commonValsToFind.x = float(tempMoveVals.x);
			commonValsToFind.y = float(tempMoveVals.y);
			commonValsToFind.z = float(tempMoveVals.z);
			resultantMoveVals = IndependentUtils::findCommonMoveValues(commonValsToFind, normalizedPrimaryLineSlope);

			performCornerCheck(resultantMoveVals);
			applyMovementToEnclaveKey(resultantMoveVals);
			checkBlockKeyValidity();								// terminate the run if the block key isn't valid
			wasTraced = checkIfBBFanWasTraced(currentBlockKey);

		}

	}
}

void EnclaveTriangleInteriorRunner::fillCircuitMetaData(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	int matchRequiredCount = 1;	// the number of faces that will have to match (the threshold); the default value is 1, but may be set to 2, if the triangle is perfectly clamped and the points of the segment exist on a shared face.
	int segmentToDetermineMatchThreshold = findIndexOfSegmentWithNonFreeEndpoint(in_circuitRef);
	ECBPolyPoint newPoint = getTraceResultForEndpointSegment(in_circuitRef, segmentToDetermineMatchThreshold, in_blockBorderLineList, in_borderDataMap);
	ECBPPOrientationResults newPointOrientation = IndependentUtils::GetPointOrientation(newPoint, in_blockBorderLineList);
	BorderMDFaceList newFaceList = IndependentUtils::getFaceList(newPointOrientation, in_borderDataMap);

	if (isPolyPerfectlyClamped >= 1)	// only do the below if it's perfectly clamped to a dimension.
	{
		int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startFaces, in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endFaces, newFaceList, 0);
		if (hasCommonFaces == 1)
		{
			matchRequiredCount = 2;
			//std::cout << "!!! NOTICE: all points share a common face!! " << std::endl;
		}
	}
	//in_circuitRef->fillSegmentArrayMetaData(matchRequiredCount);
	in_circuitRef->fillSegmentArrayMetaDataWithPointsV1(matchRequiredCount, lineMeta.beginPointRealXYZ, lineMeta.endPointRealXYZ, lineMeta.thirdPointRealXYZ, "EnclaveTriangleInteriorRunner::fillCircuitMetaData");
}

int EnclaveTriangleInteriorRunner::findIndexOfSegmentWithNonFreeEndpoint(BlockCircuit* in_circuitRef)
{
	int indexVal = 0;	// default is 0
	for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)
	{
		if (in_circuitRef->segmentArray[x].segment.isEndpointNotFree() == true)		// find the segment that has no free endpoint.
		{
			//std::cout << "Found non-free point at index: " << x << std::endl;
			indexVal = x;
		}
	}
	return indexVal;
}

ECBPolyPoint EnclaveTriangleInteriorRunner::getTraceResultForEndpointSegment(BlockCircuit* in_circuitRef, int in_segmentIndex, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	ECBPPOrientationResults enclaveOrientationResults = IndependentUtils::GetPointOrientation(in_circuitRef->segmentArray[in_segmentIndex].segment.endPoint, in_blockBorderLineList);
	PrimarySegmentMeta* metaRef = &in_circuitRef->segmentArray[in_segmentIndex].segment;
	EnclaveKeyDef::EnclaveKey tempMoveVals = IndependentUtils::retrieveBorderDirection(enclaveOrientationResults, in_borderDataMap);	// get the move vals from the current end point orientation
	ECBPolyPoint slopeToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, enclaveOrientationResults, metaRef->x_slope, metaRef->y_slope, metaRef->z_slope, tempMoveVals, isPolyPerfectlyClamped, 0, in_circuitRef->segmentArray[in_segmentIndex].segment.intendedFaces);
	ECBPolyPoint newEndPoint = IndependentUtils::getBlockTracingEndpoint(in_circuitRef->segmentArray[in_segmentIndex].segment.endPoint, slopeToUse, in_blockBorderLineList);
	//ECBPolyPoint returnPoint = newEndPointMeta.pointLocation;
	//std::cout << "### New point is: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	return newEndPoint;
}

void EnclaveTriangleInteriorRunner::populateOrganicWrappedBBFanWithCircuitData(BlockCircuit* in_circuitRef, int in_secondaryID)
{
	if (in_circuitRef->isCircuitValid == true)	// only add if the circuit contains a valid BB fan that can be constructed.
	{
		(*wrappedBBFanMapRef)[in_secondaryID].buildBBFan(in_circuitRef, materialID, emptyNormal);
	}
}

void EnclaveTriangleInteriorRunner::applyMovementToEnclaveKey(ECBPolyPoint in_moveVals)
{
	if (in_moveVals.x != 0)
	{
		currentBlockKey.x += int(in_moveVals.x);
	}
	if (in_moveVals.y != 0)
	{
		currentBlockKey.y += int(in_moveVals.y);
	}
	if (in_moveVals.z != 0)
	{
		currentBlockKey.z += int(in_moveVals.z);
	}

	if (debugModeFlag == true)
	{
		std::cout << "(EnclaveTriangleInteriorRunner): new value of currentBlockKey is: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
	}
}

bool EnclaveTriangleInteriorRunner::checkIfBBFanWasTraced(EnclaveKeyDef::EnclaveKey in_enclavePolyKey)
{

	//int blockIndex = PolyUtils::convertBlockCoordsToSingle(in_enclavePolyKey.x, in_enclavePolyKey.y, in_enclavePolyKey.z);


	auto keySearchResult = exteriorTracedBlocksMapRef->find(in_enclavePolyKey);
	if (keySearchResult != exteriorTracedBlocksMapRef->end())
	{
		isRunComplete = true;
	}
	else
	{
		if (debugModeFlag == true)
		{
			std::cout << "(EnclaveTriangleInteriorRunner): No BB fan found in block: (" << in_enclavePolyKey.x << ", " << in_enclavePolyKey.y << ", " << in_enclavePolyKey.z << ") " << std::endl;
		}
	}
	return isRunComplete;
}

void EnclaveTriangleInteriorRunner::checkBlockKeyValidity()
{
	if
		(
		(currentBlockKey.x > 3)
			||
			(currentBlockKey.x < 0)
			||
			(currentBlockKey.y > 3)
			||
			(currentBlockKey.y < 0)
			||
			(currentBlockKey.z > 3)
			||
			(currentBlockKey.z < 0)
			)
	{

		/*
		if
			(
			(currentBlockKey.x == 3)
				&&
				(currentBlockKey.y == 2)
				&&
				(currentBlockKey.z == -1)
				)
		{
		*/
		/*

			std::cout << "!!! BlockKeyValidity halt, point values are: " << std::endl;
			std::cout << "Begin: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
			std::cout << "End: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
			std::cout << "Invalid BlockKey is: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
			std::cout << "+++++++Primary Line values+++++++++++++ " << std::endl;
			std::cout << "--> Begin point: " << lineMeta.beginPointRealXYZ.x << ", " << lineMeta.beginPointRealXYZ.y << ", " << lineMeta.beginPointRealXYZ.z << std::endl;
			std::cout << "--> End point  : " << lineMeta.endPointRealXYZ.x << ", " << lineMeta.endPointRealXYZ.y << ", " << lineMeta.endPointRealXYZ.z << std::endl;
			std::cout << "--> Third point: " << lineMeta.thirdPointRealXYZ.x << ", " << lineMeta.thirdPointRealXYZ.y << ", " << lineMeta.thirdPointRealXYZ.z << std::endl;

			std::cout << "+++++++Initial values++++++" << std::endl;
			std::cout << "Initial block key wwas  : " << initialBlockKey.x << ", " << initialBlockKey.y << ", " << initialBlockKey.z << std::endl;
			std::cout << "Initial begin point was : " << initialBeginPoint.x << ", " << initialBeginPoint.y << ", " << initialBeginPoint.z << std::endl;
			std::cout << "Initial end point was   : " << initialEndPoint.x << ", " << initialEndPoint.y << ", " << initialEndPoint.z << std::endl;
			std::cout << "Slope was: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;

			std::cout << "++++++++Corner terminating key value:--> " << cornerTerminatingKey.x << ", " << cornerTerminatingKey.y << ", " << cornerTerminatingKey.z << std::endl;
			std::cout << "++++++++Was corner determined?       -->" << wasCornerDetermined << std::endl;
		*/
		//}


		//int someVal = 3;
		//std::cin >> someVal;

		isRunComplete = true;
	}
}

void EnclaveTriangleInteriorRunner::performCornerCheck(ECBPolyPoint in_moveVals)
{

	if (currentEndOrientation.otype == ECBPPOrientations::CORNER)
	{
		wasCornerDetermined = 1;
		bool terminationFlag = false;

		if (in_moveVals.x != 0)	// check on x
		{
			EnclaveKeyDef::EnclaveKey x_keyToCheck = currentBlockKey;  // copy the current key, so we can then just add x to it
			x_keyToCheck.x += int(in_moveVals.x);
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(x_keyToCheck.x, x_keyToCheck.y, x_keyToCheck.z);
			auto blockCheck = wrappedBBFanMapRef->find(convertedBlockCoords);
			if (blockCheck != wrappedBBFanMapRef->end())	// it was found, which means we should terminate.
			{
				terminationFlag = true;
				cornerTerminatingKey = x_keyToCheck;
			}
		}

		if (in_moveVals.y != 0)
		{
			EnclaveKeyDef::EnclaveKey y_keyToCheck = currentBlockKey;	// copy the current key, so we can then just add y to it
			y_keyToCheck.y += int(in_moveVals.y);
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(y_keyToCheck.x, y_keyToCheck.y, y_keyToCheck.z);
			auto blockCheck = wrappedBBFanMapRef->find(convertedBlockCoords);
			if (blockCheck != wrappedBBFanMapRef->end())	// it was found, which means we should terminate.
			{
				terminationFlag = true;
				cornerTerminatingKey = y_keyToCheck;
			}
		}

		if (in_moveVals.z != 0)
		{
			EnclaveKeyDef::EnclaveKey z_keyToCheck = currentBlockKey;  // copy the current key, so we can then just add z to it
			z_keyToCheck.z += int(in_moveVals.z);
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(z_keyToCheck.x, z_keyToCheck.y, z_keyToCheck.z);
			auto blockCheck = wrappedBBFanMapRef->find(convertedBlockCoords);
			if (blockCheck != wrappedBBFanMapRef->end())	// it was found, which means we should terminate.
			{
				terminationFlag = true;
				cornerTerminatingKey = z_keyToCheck;
			}
		}

		if (terminationFlag == true)
		{
			isRunComplete = true;
		}
	}
}