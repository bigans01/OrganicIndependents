#include "stdafx.h"
#include "EnclaveTriangle.h"
#include "IndependentUtils.h"
#include "PrimaryLineT1Array.h"

void EnclaveTriangle::executeRun(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	// prepare poly data
	currentEnclaveKey = in_key;

	//std::cout << "############### Executing run for triangle, with points: [" << lineArray[0].pointA.x << ", " << lineArray[0].pointA.y << ", " << lineArray[0].pointA.z << "] [ "
																			 // << lineArray[1].pointA.x << ", " << lineArray[1].pointA.y << ", " << lineArray[1].pointA.z << "] [ " 
																			  //<< lineArray[2].pointA.x << ", " << lineArray[2].pointA.y << ", " << lineArray[2].pointA.z << "]" << std::endl;

	//std::cout << "!!!!!!!!!!!!!!!!!!!! !!! Producing forward array " << std::endl;
	PrimaryLineT1Array forwardPrimaryLineArray = generatePrimaryLineT1Array(PolyRunDirection::NORMAL);						// generate the forward (NORMAL) run array
	//std::cout << "!!!!!!!!!!!!!!!!!!!! !!! Producing reverse array " << std::endl;
	PrimaryLineT1Array reversePrimaryLineArray = generatePrimaryLineT1Array(PolyRunDirection::REVERSE);						// " " the (REVERSE) run array

	forwardPrimaryLineArray.adjustForEnclavePerfectClamping(isEnclaveTrianglePolyPerfectlyClamped);
	reversePrimaryLineArray.adjustForEnclavePerfectClamping(isEnclaveTrianglePolyPerfectlyClamped);

	//runPolyLinesThroughEnclave(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);							// generate the exterior segments using the forward array
	//runPolyLinesForReverseTerminatingContainer(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);			// run the exterior segments using the reverse array, to produce the reverse terminating set

	executeForwardRunTest(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);
	executeForwardRunTest(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);

	//std::cout << "##### Passed run tests... " << std::endl;

	/*
	if
	(
		(in_key.x == 0)
		&&
		(in_key.y == 0)
		&&
		(in_key.z == 1)
	)
	{
		std::cout << "+++ Enclave to be analyzed found: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
	}
	*/

	if (isTriangleValid == true)	// don't bother running if its invalid
	{
		//std::cout << "######## Run of poly lines complete.. " << std::endl;
		runPolyLinesThroughEnclave(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);							// generate the exterior segments using the forward array
		runPolyLinesForReverseTerminatingContainer(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);			// run the exterior segments using the reverse array, to produce the reverse terminating set

		//runPolyLinesThroughEnclaveReverse(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);

		forwardTerminatingContainer.buildTerminatingSets();		// build the terminating sets (required before an interior run.)
		reverseTerminatingContainer.buildTerminatingSets();

		generateExteriorLineSegments(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap, in_key);						// build the exterior segments, fill the circuit data for them, using the forward array
		//std::cout << "######## Exterior line segment generation complete.. " << std::endl;
		if (isTriangleValid == true)	// don't bother running if its invalid
		{

			runInteriorRunners(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap, PolyRunDirection::NORMAL);
			//std::cout << "######## Normal direction interior run complete. " << std::endl;

			runInteriorRunners(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap, PolyRunDirection::REVERSE);
		}
		//std::cout << "######## Reverse direction interior run complete. " << std::endl;
	}
	else if (isTriangleValid == false)
	{
		std::cout << "!!! Notice, invalid EnclaveTriangle detected, in Enclave with key: " << currentEnclaveKey.x << ", " << currentEnclaveKey.y << ", " << currentEnclaveKey.z << std::endl;
		int invalid = 3;
		std::cin >> invalid;
	}
	//runInteriorRunnerReverse(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap, PolyRunDirection::REVERSE);

	//printExteriorBlocks();
	//performCentroidBlockCheck(forwardPrimaryLineArray.linkArray[0].beginPointRealXYZ, forwardPrimaryLineArray.linkArray[1].beginPointRealXYZ, forwardPrimaryLineArray.linkArray[2].beginPointRealXYZ);
}

void EnclaveTriangle::executeRunNoInteriorFill(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	//  ************ used for debug only *****************
	// prepare poly data
	currentEnclaveKey = in_key;

	//std::cout << "############### Executing run for triangle, with points: [" << lineArray[0].pointA.x << ", " << lineArray[0].pointA.y << ", " << lineArray[0].pointA.z << "] [ "
																			 // << lineArray[1].pointA.x << ", " << lineArray[1].pointA.y << ", " << lineArray[1].pointA.z << "] [ " 
																			  //<< lineArray[2].pointA.x << ", " << lineArray[2].pointA.y << ", " << lineArray[2].pointA.z << "]" << std::endl;

	//std::cout << "!!!!!!!!!!!!!!!!!!!! !!! Producing forward array " << std::endl;
	PrimaryLineT1Array forwardPrimaryLineArray = generatePrimaryLineT1Array(PolyRunDirection::NORMAL);						// generate the forward (NORMAL) run array
	//std::cout << "!!!!!!!!!!!!!!!!!!!! !!! Producing reverse array " << std::endl;
	PrimaryLineT1Array reversePrimaryLineArray = generatePrimaryLineT1Array(PolyRunDirection::REVERSE);						// " " the (REVERSE) run array

	forwardPrimaryLineArray.adjustForEnclavePerfectClamping(isEnclaveTrianglePolyPerfectlyClamped);
	reversePrimaryLineArray.adjustForEnclavePerfectClamping(isEnclaveTrianglePolyPerfectlyClamped);

	//runPolyLinesThroughEnclave(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);							// generate the exterior segments using the forward array
	//runPolyLinesForReverseTerminatingContainer(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);			// run the exterior segments using the reverse array, to produce the reverse terminating set

	executeForwardRunTest(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);
	executeForwardRunTest(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);

	//std::cout << "##### Passed run tests... " << std::endl;

	/*
	if
	(
		(in_key.x == 0)
		&&
		(in_key.y == 0)
		&&
		(in_key.z == 1)
	)
	{
		std::cout << "+++ Enclave to be analyzed found: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
	}
	*/

	if (isTriangleValid == true)	// don't bother running if its invalid
	{
		//std::cout << "######## Run of poly lines complete.. " << std::endl;
		runPolyLinesThroughEnclave(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap);							// generate the exterior segments using the forward array
		runPolyLinesForReverseTerminatingContainer(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);			// run the exterior segments using the reverse array, to produce the reverse terminating set

		//runPolyLinesThroughEnclaveReverse(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap);

		forwardTerminatingContainer.buildTerminatingSets();		// build the terminating sets (required before an interior run.)
		reverseTerminatingContainer.buildTerminatingSets();

		generateExteriorLineSegments(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap, in_key);						// build the exterior segments, fill the circuit data for them, using the forward array
		//std::cout << "######## Exterior line segment generation complete.. " << std::endl;
		if (isTriangleValid == true)	// don't bother running if its invalid
		{

			runInteriorRunnersDebug(&forwardPrimaryLineArray, in_blockBorderLineList, in_borderDataMap, PolyRunDirection::NORMAL);
			//std::cout << "######## Normal direction interior run complete. " << std::endl;

			runInteriorRunnersDebug(&reversePrimaryLineArray, in_blockBorderLineList, in_borderDataMap, PolyRunDirection::REVERSE);
		}
		//std::cout << "######## Reverse direction interior run complete. " << std::endl;
	}
	else if (isTriangleValid == false)
	{
		std::cout << "!!! Notice, invalid EnclaveTriangle detected, in Enclave with key: " << currentEnclaveKey.x << ", " << currentEnclaveKey.y << ", " << currentEnclaveKey.z << std::endl;
		int invalid = 3;
		std::cin >> invalid;
	}

}

PrimaryLineT1Array EnclaveTriangle::generatePrimaryLineT1Array(PolyRunDirection in_polyRunDirection)
{
	PrimaryLineT1Array primaryLineArray;
	// produce the dummy blueprint borders
	EnclaveKeyDef::EnclaveKey dummyKey;
	ECBBorderValues borderVals = IndependentUtils::getBlueprintLimits(dummyKey);



	for (int x = 0; x < 3; x++)
	{
		//std::cout << "################################################################ (( Line " << x << ")) ######## " << std::endl;

		//ECBPolyPoint pointA = lineArray[x].pointA;
		//ECBPolyPoint pointB = lineArray[x].pointB;
		//ECBPolyPoint pointC = lineArray[x].pointC;


		ECBPolyPoint pointA = lineArray[x].pointA;
		ECBPolyPoint pointB = lineArray[x].pointB;
		ECBPolyPoint pointC = lineArray[x].pointC;

		//std::cout << "##### This line's points are: " << std::endl;
		//std::cout << "0: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
		//std::cout << "1: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
		//std::cout << "2: " << pointC.x << ", " << pointC.y << ", " << pointC.z << std::endl;

		//ECBPolyPointLocation pointALocation = OrganicUtils::getPolyPointLocation(pointA, borderVals);
		//ECBPolyPointLocation pointBLocation = OrganicUtils::getPolyPointLocation(pointB, borderVals);
		ECBPolyPointLocation pointALocation = IndependentUtils::getEnclavePolyPointLocation(pointA, borderVals);
		ECBPolyPointLocation pointBLocation = IndependentUtils::getEnclavePolyPointLocation(pointB, borderVals);

		//std::cout << "PointALocation (Enclave): " << pointALocation.enclaveKey.x << ", " << pointALocation.enclaveKey.y << ", " << pointALocation.enclaveKey.z << std::endl;
		//std::cout << "PointALocation (Block): " << pointALocation.blockKey.x << ", " << pointALocation.blockKey.y << ", " << pointALocation.blockKey.z << std::endl;

		PrimaryLineT1 newPrimaryLine;
		//newPrimaryLine.IDofLine = lineArray[x].lineID;	// set the line ID 
		newPrimaryLine.IDofLine = x;	// set the line ID 
		newPrimaryLine.perfectClampValue = isEnclaveTrianglePolyPerfectlyClamped;		// set the perfect clamp value
		newPrimaryLine.isLineActiveInPoly = 1;					// mark the line as being active
		newPrimaryLine.beginPointRealXYZ = pointA;				// store actual XYZ values of point A
		newPrimaryLine.endPointRealXYZ = pointB;				// store actual XYZ values of point B
		newPrimaryLine.thirdPointRealXYZ = pointC;				// store actual XYZ values of point C (not always needed)
		newPrimaryLine.intendedFaces = IndependentUtils::determineIntendedFacesV2(pointA, pointB, pointC, lineArray[x].x_interceptSlope, lineArray[x].y_interceptSlope, lineArray[x].z_interceptSlope);
		newPrimaryLine.beginPointMeta = pointALocation;
		newPrimaryLine.endPointMeta = pointBLocation;
		newPrimaryLine.x_int = lineArray[x].x_interceptSlope;
		newPrimaryLine.y_int = lineArray[x].y_interceptSlope;
		newPrimaryLine.z_int = lineArray[x].z_interceptSlope;

		//std::cout << "################## Checking calibrations..." << std::endl;
		if (in_polyRunDirection == PolyRunDirection::NORMAL)
		{
			//newPrimaryLine.calibrateWithInverseChecks(pointC);					// needs current blueprint borders to be passed down; pass in point C for when clamping occurs
			newPrimaryLine.calibrate(pointC);
		}
		else if (in_polyRunDirection == PolyRunDirection::REVERSE)
		{
			//std::cout << "---- Calibrating inverse checks for line: " << x << " || Points are: " << std::endl;
			//std::cout << "0: " << newPrimaryLine.beginPointRealXYZ.x << ", " << newPrimaryLine.beginPointRealXYZ.y << ", " << newPrimaryLine.beginPointRealXYZ.z << std::endl;
			//std::cout << "1: " << newPrimaryLine.endPointRealXYZ.x << ", " << newPrimaryLine.endPointRealXYZ.y << ", " << newPrimaryLine.endPointRealXYZ.z << std::endl;
			//std::cout << "2: " << newPrimaryLine.thirdPointRealXYZ.x << ", " << newPrimaryLine.thirdPointRealXYZ.y << ", " << newPrimaryLine.thirdPointRealXYZ.z << std::endl;
			ECBPolyPoint checkingInverts = newPrimaryLine.calibrateWithInverseChecks(pointC);  // calibrate with inversions
			//std::cout << "NPL faces, Line " << currentPrimaryLine->first <<" : " << newPrimaryLine.intendedFaces.x << ", " << newPrimaryLine.intendedFaces.y << ", " << newPrimaryLine.intendedFaces.z << std::endl;
		}
		primaryLineArray.addNewPrimaryLine(newPrimaryLine);					// add link to the link array
		//std::cout << "################## Printing meta for line " << x << std::endl;
		//std::cout << "Line ID was: " << lineArray[x].lineID << std::endl;
		//std::cout << "Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
		//std::cout << "Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
		//std::cout << "Point C: " << pointC.x << ", " << pointC.y << ", " << pointC.z << std::endl;
		//std::cout << "Begin point meta: (( Block: " << newPrimaryLine.beginPointMeta.blockKey.x << ", " << newPrimaryLine.beginPointMeta.blockKey.y << ", " << newPrimaryLine.beginPointMeta.blockKey.z <<
		//			" | Enclave: " << newPrimaryLine.beginPointMeta.enclaveKey.x << ", " << newPrimaryLine.beginPointMeta.enclaveKey.y << ", " << newPrimaryLine.beginPointMeta.enclaveKey.y << std::endl;

		//std::cout << "End pointe meta: (( Block " << newPrimaryLine.endPointMeta.blockKey.x << ", " << newPrimaryLine.endPointMeta.blockKey.y << ", " << newPrimaryLine.endPointMeta.blockKey.z <<
		//	" | Enclave: " << newPrimaryLine.endPointMeta.enclaveKey.x << ", " << newPrimaryLine.endPointMeta.enclaveKey.y << ", " << newPrimaryLine.endPointMeta.enclaveKey.y << std::endl;

	}

	// check for line swap
	if (in_polyRunDirection == PolyRunDirection::REVERSE)
	{
		//std::cout << "REVERSING LINES AND POINTS" << std::endl;
		primaryLineArray.swapLines(&borderVals);	// swap lines when in reverse
	}

	/*
	std::cout << "Point and slopes for this array are (JEV3): " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		std::cout << "[ " << x << "] 0: " << primaryLineArray.linkArray[x].beginPointRealXYZ.x << ", " << primaryLineArray.linkArray[x].beginPointRealXYZ.y << ", " << primaryLineArray.linkArray[x].beginPointRealXYZ.z << std::endl;
		std::cout << "[ " << x << "] 1: " << primaryLineArray.linkArray[x].endPointRealXYZ.x << ", " << primaryLineArray.linkArray[x].endPointRealXYZ.y << ", " << primaryLineArray.linkArray[x].endPointRealXYZ.z << std::endl;
		std::cout << "[ " << x << "] 2: " << primaryLineArray.linkArray[x].thirdPointRealXYZ.x << ", " << primaryLineArray.linkArray[x].thirdPointRealXYZ.y << ", " << primaryLineArray.linkArray[x].thirdPointRealXYZ.z << std::endl;
		std::cout << "[ " << x << "] Intended faces: " << primaryLineArray.linkArray[x].intendedFaces.x << ", " << primaryLineArray.linkArray[x].intendedFaces.y << ", " << primaryLineArray.linkArray[x].intendedFaces.z << std::endl;

		std::cout << "[ " << x << "] X: " << primaryLineArray.linkArray[x].x_int.x << ", " << primaryLineArray.linkArray[x].x_int.y << ", " << primaryLineArray.linkArray[x].x_int.z << std::endl;
		std::cout << "[ " << x << "] Y: " << primaryLineArray.linkArray[x].y_int.x << ", " << primaryLineArray.linkArray[x].y_int.y << ", " << primaryLineArray.linkArray[x].y_int.z << std::endl;
		std::cout << "[ " << x << "] Z: " << primaryLineArray.linkArray[x].z_int.x << ", " << primaryLineArray.linkArray[x].z_int.y << ", " << primaryLineArray.linkArray[x].z_int.z << std::endl;
		std::cout << std::endl;
	}
	*/
	return primaryLineArray;
}

void EnclaveTriangle::printExteriorBlocks()
{
	std::cout << "Printing external blocks for this EnclaveTriangle: " << std::endl;
	auto exteriorBegin = exteriorTracedBlocks.begin();
	auto exteriorEnd = exteriorTracedBlocks.end();
	for (exteriorBegin; exteriorBegin != exteriorEnd; exteriorBegin++)
	{
		std::cout << "( " << exteriorBegin->x << ", " << exteriorBegin->y << ", " << exteriorBegin->z << ") " << std::endl;
	}
}

void EnclaveTriangle::printPoints()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << x << ": " << lineArray[x].pointA.x << ", " << lineArray[x].pointA.y << ", " << lineArray[x].pointA.z << std::endl;
	}
}

void EnclaveTriangle::printBlockKeys()
{
	auto blocksBegin = secondaryMap.triangleMap.begin();
	auto blocksEnd = secondaryMap.triangleMap.end();
	for (; blocksBegin != blocksEnd; blocksBegin++)
	{
		auto currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
		std::cout << "Found block key: "; currentKey.printKey(); std::cout << std::endl;
	}

	printExteriorBlocks();
}

void EnclaveTriangle::executeForwardRunTest(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	for (int x = 0; x < 3; x++)
	{
		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);
		int limitCount = 0;	// if this goes beyond 16, flag as invalid
		while (currentLineRunner.isRunComplete == 0)
		{
			currentLineRunner.checkForCompletion();
			currentLineRunner.iterateToNextBlock();
			limitCount++;
			if
				(
					//(limitCount > 16)											// it surpassed the tick count
					//||
				(currentLineRunner.runState == LineRunnerState::INVALID)	// it was flagged as INVALID
					)
			{
				std::cout << "!!! ::RunTest:: Warning, anomalous EnclaveTriangle detected. " << std::endl;
				//int someVal = 3;
				//std::cin >> someVal;
				isTriangleValid = false;
			}
			//std::cout << "end loop... " << std::endl;
		}

		//std::cout << "current line complete state: " << currentLineRunner.isRunComplete << std::endl;
	}
}

void EnclaveTriangle::runPolyLinesThroughEnclave(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	//BlockBorderLineList borderLineList;


	//std::cout << "###################################################################################!!!! Running poly lines through enclave..." << std::endl;
	for (int x = 0; x < 3; x++)
	{


		//std::cout << "####################### >> Running line " << x << std::endl;
		//EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], &borderLineList, x);
		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);

		//std::cout << ">> Line's begin point stats from Array: " << std::endl;
		//std::cout << ": Block: " << in_linkArrayRef->linkArray[x].beginPointMeta.blockKey.x << ", " << in_linkArrayRef->linkArray[x].beginPointMeta.blockKey.y << ", " << in_linkArrayRef->linkArray[x].beginPointMeta.blockKey.z << std::endl;


		//currentLineRunner.checkForCompletion();
		//std::cout << "## Begin point stats: " << std::endl;
		//std::cout << "Enclave: " << currentLineRunner.lineMeta.beginPointMeta.enclaveKey.x << ", " << currentLineRunner.lineMeta.beginPointMeta.enclaveKey.y << ", " << currentLineRunner.lineMeta.beginPointMeta.enclaveKey.z
		//	<< " | Block: " << currentLineRunner.lineMeta.beginPointMeta.blockKey.x << ", " << currentLineRunner.lineMeta.beginPointMeta.blockKey.y << ", " << currentLineRunner.lineMeta.beginPointMeta.blockKey.z << std::endl;

		//std::cout << "## End point stats: " << std::endl;
		//std::cout << "Enclave: " << currentLineRunner.lineMeta.endPointMeta.enclaveKey.x << ", " << currentLineRunner.lineMeta.endPointMeta.enclaveKey.y << ", " << currentLineRunner.lineMeta.endPointMeta.enclaveKey.z
		//	<< " | Block: " << currentLineRunner.lineMeta.endPointMeta.blockKey.x << ", " << currentLineRunner.lineMeta.endPointMeta.blockKey.y << ", " << currentLineRunner.lineMeta.endPointMeta.blockKey.z << std::endl;

		int limitCount = 0;	// if this goes beyond 16, flag as invalid
		while (currentLineRunner.isRunComplete == 0)
		{
			//std::cout << "begin loop... " << std::endl;
			currentLineRunner.checkForCompletion();
			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using
			forwardTerminatingContainer.insertKeyIntoTracingSet(x, currentBlockKey);								// make sure to register for termination later

			//std::cout << "Inserted key into tracing set: (" << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;

			// BELOW LINE WILL CAUSE CRASH (6/25/2020), when running in REVERSE AND ORIGINAL mode (buffer overflow.
			// take the found index to access the appropriate Secondary, and put the segment into it.
			//std::cout << "(Inserting segment into block ID: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
			//std::cout << "::> Begin: " << currentLineRunner.currentIterationBeginPoint.x << ", " << currentLineRunner.currentIterationBeginPoint.y << ", " << currentLineRunner.currentIterationBeginPoint.z
			//		  << " | End " << currentLineRunner.currentIterationEndPoint.x << ", " << currentLineRunner.currentIterationEndPoint.y << ", " << currentLineRunner.currentIterationEndPoint.z << std::endl;
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			secondaryMap.triangleMap[convertedBlockCoords].blockSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);

			/*
			if
			(
				(currentBlockKey.x == 1)
				&&
				(currentBlockKey.y == 0)
				&&
				(currentBlockKey.z == 2)
			)
			{
				std::cout << std::fixed << std::setprecision(7);
				std::cout << "==> begin point: " << currentLineRunner.currentIterationBeginPoint.x << ", " << currentLineRunner.currentIterationBeginPoint.y << ", " << currentLineRunner.currentIterationBeginPoint.z
					<< " | end point: " << currentLineRunner.currentIterationEndPoint.x << ", " << currentLineRunner.currentIterationEndPoint.y << ", " << currentLineRunner.currentIterationEndPoint.z << std::endl;

			}
			*/

			//secondaryMap.triangleMap[PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z)].blockSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);
			exteriorTracedBlocks.insert(currentBlockKey);	// add a record of the external trace
			allTracedBlocks.insert(currentBlockKey);		// add a record to the set of all traces
			currentLineRunner.iterateToNextBlock();
			limitCount++;




			if
				(
					//(limitCount > 16)											// it surpassed the tick count
					//||
				(currentLineRunner.runState == LineRunnerState::INVALID)	// it was flagged as INVALID
					)
			{
				std::cout << "!!! Warning, anomalous EnclaveTriangle detected. " << std::endl;
				//int someVal = 3;
				//std::cin >> someVal;
				isTriangleValid = false;
			}
			//std::cout << "end loop... " << std::endl;
		}


	}
	//std::cout << "!! Done running lines. " << std::endl;
}

void EnclaveTriangle::runPolyLinesThroughEnclaveReverse(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)	// run the exterior lines
{
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "####### spawning EnclaveLineRunner from EnclaveTriangle::runPolyLinesThroughEnclaveReverse" << std::endl;
		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);
		while (currentLineRunner.isRunComplete == 0)
		{

			currentLineRunner.checkForCompletion();
			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using
			//forwardTerminatingContainer.insertKeyIntoTracingSet(x, currentBlockKey);								// make sure to register for termination later

			// BELOW LINE WILL CAUSE CRASH (6/25/2020), when running in REVERSE AND ORIGINAL mode (buffer overflow.
			// take the found index to access the appropriate Secondary, and put the segment into it.
			//std::cout << "(Inserting segment into block ID: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
			//std::cout << "::> Begin: " << currentLineRunner.currentIterationBeginPoint.x << ", " << currentLineRunner.currentIterationBeginPoint.y << ", " << currentLineRunner.currentIterationBeginPoint.z
			//		  << " | End " << currentLineRunner.currentIterationEndPoint.x << ", " << currentLineRunner.currentIterationEndPoint.y << ", " << currentLineRunner.currentIterationEndPoint.z << std::endl;
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			//secondaryMap.triangleMap[convertedBlockCoords].reverseSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);											
			secondaryMap.triangleMap[convertedBlockCoords].reverseSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, in_linkArrayRef->linkArray[x].IDofLine);		// the last argument should not be the value of x (see previous line),
																																																										// it should be the ID of the line that is from the corresponding element in the array; 
																																																										// the value of X and the corresponding ID of the line are not the same when a PrimaryLineT1Array is in REVERSE mode, although they are the same in NORMAL run.
			//secondaryMap.triangleMap[PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z)].reverseSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);
			//exteriorTracedBlocks.insert(currentBlockKey);	// add a record of the external trace
			//allTracedBlocks.insert(currentBlockKey);		// add a record to the set of all traces
			currentLineRunner.iterateToNextBlock();
			//std::cout << "One loop complete..." << std::endl;
		}
	}
}

void EnclaveTriangle::runInteriorRunnerReverse(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PolyRunDirection in_polyRunDirection)
{
	TerminatingSetContainer* terminatingSetRef = &reverseTerminatingContainer;	// will be one of two values below
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "####### spawning EnclaveLineRunner from EnclaveTriangle::runInteriorRunnerReverse" << std::endl;
		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);

		while (currentLineRunner.isRunComplete == 0)
		{
			currentLineRunner.checkForCompletion();
			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			OrganicWrappedBBFan* bbFanRef = &secondaryMap.triangleMap[convertedBlockCoords];		// get a ref to the fan.
			/*
			if (bbFanRef->reverseSegmentTracker.currentLineSegmentIndex == 1)	// if it only contains 1 segment, do the run.
			{
				//if (in_polyRunDirection == PolyRunDirection::REVERSE)
				//{
					//std::cout << "!!! Attempting run for REVERSE! " << std::endl;
				//}

				//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
				ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
				ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
				EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

				//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
				//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
													 //<< in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;
				ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
				PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);

				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &exteriorTracedBlocks, &allTracedBlocks, materialID, IsEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey);
				EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &terminatingSetRef->terminatingSetMap[x].set, &allTracedBlocks, materialID, IsEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey);
				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, &in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap);
			}
			*/



			//if (bbFanRef->blockSegmentTracker.currentLineSegmentIndex == 2)
			if (bbFanRef->reverseSegmentTracker.currentLineSegmentIndex == 2)
			{
				//if (in_polyRunDirection == PolyRunDirection::NORMAL)
				//{
				int isRunValid = bbFanRef->checkIfRunIsValidForTwoSegmentsReverse(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap);
				//int isRunValid = bbFanRef->checkIfRunIsValidForTwoSegmentsViaCopy(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap, bbFanRef->blockSegmentTracker, in_polyRunDirection);
				if
					(
					(isRunValid == 1)
						//&&
						//currentBlockKey.x == 0
						//&&
						//currentBlockKey.y == 2
						//&&
						//currentBlockKey.z == 2
						)
				{
					//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;

					//std::cout << "> Line runner's current begin: " << currentLineRunner.currentIterationBeginPoint.x << ", " << currentLineRunner.currentIterationBeginPoint.y << ", " << currentLineRunner.currentIterationBeginPoint.z << std::endl;
					//std::cout << "> Line runner's current end : " << currentLineRunner.currentIterationEndPoint.x << ", " << currentLineRunner.currentIterationEndPoint.y << ", " << currentLineRunner.currentIterationEndPoint.z << std::endl;

					ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
					ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
					EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

					//std::cout << "!!! +++++ current begin point is: " << currentBeginPoint.x << ", " << currentBeginPoint.y << ", " << currentBeginPoint.z << std::endl;
					//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
					//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
					//									 << in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;

					//std::cout << "X slope is: " << in_linkArrayRef->linkArray[x].x_int.x << ", " << in_linkArrayRef->linkArray[x].x_int.y << ", " << in_linkArrayRef->linkArray[x].x_int.z << std::endl;
					//std::cout << "Y slope is: " << in_linkArrayRef->linkArray[x].y_int.x << ", " << in_linkArrayRef->linkArray[x].y_int.y << ", " << in_linkArrayRef->linkArray[x].y_int.z << std::endl;
					//std::cout << "Z slope is: " << in_linkArrayRef->linkArray[x].z_int.x << ", " << in_linkArrayRef->linkArray[x].z_int.y << ", " << in_linkArrayRef->linkArray[x].z_int.z << std::endl;

					//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
					ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
					if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
					{
						//std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunnerReverse; flagging as INVALID. " << std::endl;
						isTriangleValid = false;
						break;
					}


					PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);
					EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &terminatingSetRef->terminatingSetMap[x].set, &allTracedBlocks, materialID, isEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey, 2, false);
				}
				//}
			}


			currentLineRunner.iterateToNextBlock();
		}
	}
}

void EnclaveTriangle::runPolyLinesForReverseTerminatingContainer(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	int limitCount = 0;
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "####### spawning EnclaveLineRunner from EnclaveTriangle::runPolyLinesForReverseTerminatingContainer" << std::endl;
		//std::cout << "### Line: " << x << std::endl;
		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);
		while (currentLineRunner.isRunComplete == 0)
		{
			currentLineRunner.checkForCompletion();
			//std::cout << "Checked for completion... " << std::endl;

			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using
			reverseTerminatingContainer.insertKeyIntoTracingSet(x, currentBlockKey);
			currentLineRunner.iterateToNextBlock();
			limitCount++;
			//std::cout << "Loop #2 begin... " << std::endl;
			if
				(
				(limitCount > 16)											// it surpassed the tick count
					||
					(currentLineRunner.runState == LineRunnerState::INVALID)	// it was flagged as INVALID
					)
			{
				//std::cout << "!!! Warning, anomalous EnclaveTriangle detected. " << std::endl;
				//int someVal = 3;
				//std::cin >> someVal;
				isTriangleValid = false;
			}
			//std::cout << "Loop #2 end... " << std::endl;
			//std::cout << "----------------------" << std::endl;
		}
	}
}

void EnclaveTriangle::runInteriorRunners(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PolyRunDirection in_polyRunDirection)
{
	//std::cout << "Poly perfect clamp value is: " << IsEnclaveTrianglePolyPerfectlyClamped << std::endl;

	//std::cout << "#################### spawning EnclaveLineRunners from EnclaveTriangle::runInteriorRunners" << std::endl;

	TerminatingSetContainer* terminatingSetRef = NULL;	// will be one of two values below
	if (in_polyRunDirection == PolyRunDirection::NORMAL)
	{
		terminatingSetRef = &forwardTerminatingContainer;
	}
	else if (in_polyRunDirection == PolyRunDirection::REVERSE)
	{
		terminatingSetRef = &reverseTerminatingContainer;
	}

	for (int x = 0; x < 3; x++)
	{
		//std::cout << "################## Attempting interior runner from line: " << x << std::endl;

		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);

		while 
		(
			(currentLineRunner.isRunComplete == 0)				// only run interior tracing if the run isn't complete, 
			&&													// --AND--
			(currentLineRunner.isRunSingleIteration == 0)		// as long as the run is NOT a single iteration 
																// (single iteration means the line's begin and end points are confined to the same block)
		)
		{
			currentLineRunner.checkForCompletion();
			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using
			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			OrganicWrappedBBFan* bbFanRef = &secondaryMap.triangleMap[convertedBlockCoords];		// get a ref to the fan.



			if (bbFanRef->blockSegmentTracker.currentLineSegmentIndex == 1)	// if it only contains 1 segment, do the run.
			{

				//std::cout << "Performing interior run from block: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
				//if (in_polyRunDirection == PolyRunDirection::REVERSE)
				//{
					//std::cout << "!!! Attempting run for REVERSE! " << std::endl;
				//}

				//std::cout << "Terminating set contents: " << std::endl;
				//auto termSetBegin = terminatingSetRef->terminatingSetMap[x].set.begin();
				//auto termSetEnd = terminatingSetRef->terminatingSetMap[x].set.end();
				//for (termSetBegin; termSetBegin != termSetEnd; termSetBegin++)
				//{
					//std::cout << "(" << termSetBegin->x << ", " << termSetBegin->y << ", " << termSetBegin->z << ") " << std::endl;
				//}

				//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
				ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
				ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
				EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

				//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
				//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
													 //<< in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;
				//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
				//if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
				//{
					//std::cout << "!! Warning, slope invalid! " << std::endl;
				//}


				ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
				if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
				{
					std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunners (1 segment); flagging as INVALID. " << std::endl;
					isTriangleValid = false;
					break;
				}


				PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);

				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &exteriorTracedBlocks, &allTracedBlocks, materialID, IsEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey);
				if (in_polyRunDirection == PolyRunDirection::REVERSE)
				{
					//std::cout << "Beginning interior run from block: " << currentBlockKey.x << ",  " << currentBlockKey.y << ",  " << currentBlockKey.z << std::endl;
					//std::cout << "Current begin point is: " << currentBeginPoint.x << ",  " << currentBeginPoint.y << ",  " << currentBeginPoint.z << std::endl;
					//std::cout << "Current end point is: " << tracingResult.resultingEndPoint.x << ",  " << tracingResult.resultingEndPoint.y << ",  " << tracingResult.resultingEndPoint.z << std::endl;

					ECBPolyPoint testIntended = IndependentUtils::determineIntendedFacesV2(in_linkArrayRef->linkArray[x].beginPointRealXYZ, in_linkArrayRef->linkArray[x].endPointRealXYZ, in_linkArrayRef->linkArray[x].thirdPointRealXYZ, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int);
					//std::cout << "Test of intended faces: " << testIntended.x << ", " << testIntended.y << ", " << testIntended.z << std::endl;
				}

				EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &terminatingSetRef->terminatingSetMap[x].set, &allTracedBlocks, materialID, isEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey, 1, false);
				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, &in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap);

			}



			if (bbFanRef->blockSegmentTracker.currentLineSegmentIndex == 2)
			{

				if (in_polyRunDirection == PolyRunDirection::NORMAL)
				{
					//int isRunValid = bbFanRef->checkIfRunIsValidForTwoSegments(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap);
					//if (isRunValid == 1)


					SegmentResult result = bbFanRef->checkIfRunIsValidForTwoSegmentsSpecial(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap);
					if (result.isValid == 1)
					{
						//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
						ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
						ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
						EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

						//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
						//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
															 //<< in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;
						//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);

						ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
						if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
						{
							std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunners (2 segments); flagging as INVALID. " << std::endl;
							//isTriangleValid = false;
							//break;
						}


						bool finder = isSegmentAndSlopeParallel(result.otherSegment, interceptToUse);
						std::map<int, TerminatingSet> tempSet;

						if
							(
							(finder == true)						// only do this if there is a parallel detection.
								&&
								(isEnclaveTrianglePolyPerfectlyClamped == 0)			// it's unnecessary to do this check if the triangle is in true 2 dimensions, and not 3d. 
								)
						{


							//std::cout << "!! +++++Entering true branch. +++++" << std::endl;
							//std::cout << "!! This segment's LINE id is: " << int(in_linkArrayRef->linkArray[x].IDofLine) << std::endl;
							//std::cout << "!! Other segment ID was: " << int(result.otherSegment.lineID) << std::endl;
							tempSet = terminatingSetRef->constructSpecialSet(in_linkArrayRef->linkArray[x].IDofLine, int(result.otherSegment.lineID));
							auto firstVal = tempSet.begin();
							auto secondVal = firstVal->second.set.begin();

							//std::cout << "!! The new map has the following line as a terminating line: " << firstVal->first << std::endl;
							PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);
							EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint,
								tracingResult.resultingEndPoint,
								in_linkArrayRef->linkArray[x],
								in_blockBorderLineList,
								in_borderDataMap,
								&secondaryMap.triangleMap,
								&tempSet[in_linkArrayRef->linkArray[x].IDofLine].set,
								&allTracedBlocks,
								materialID,
								isEnclaveTrianglePolyPerfectlyClamped,
								emptyNormal,
								currentBlockKey,
								2,
								false
							);
						}
						else
						{
							PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);
							EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint,
								tracingResult.resultingEndPoint,
								in_linkArrayRef->linkArray[x],
								in_blockBorderLineList,
								in_borderDataMap,
								&secondaryMap.triangleMap,
								&terminatingSetRef->terminatingSetMap[x].set,
								&allTracedBlocks, materialID,
								isEnclaveTrianglePolyPerfectlyClamped,
								emptyNormal,
								currentBlockKey,
								2,
								false);
						}
						//}
					}
				}
			}

			// new code goes here (for interior runner)
			// ...
			// ...
			// ...
			currentLineRunner.iterateToNextBlock();
		}
	}
}

void EnclaveTriangle::runInteriorRunnersDebug(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PolyRunDirection in_polyRunDirection)
{
	//std::cout << "Poly perfect clamp value is: " << IsEnclaveTrianglePolyPerfectlyClamped << std::endl;

	//std::cout << "#################### spawning EnclaveLineRunners from EnclaveTriangle::runInteriorRunners" << std::endl;

	TerminatingSetContainer* terminatingSetRef = NULL;	// will be one of two values below
	if (in_polyRunDirection == PolyRunDirection::NORMAL)
	{
		terminatingSetRef = &forwardTerminatingContainer;
	}
	else if (in_polyRunDirection == PolyRunDirection::REVERSE)
	{
		terminatingSetRef = &reverseTerminatingContainer;
	}

	for (int x = 0; x < 3; x++)
	{
		std::cout << "(EnclaveTriangle::runInteriorRunnersDebug) ################## Attempting interior runner from line: " << x << std::endl;

		EnclaveLineRunner currentLineRunner(in_linkArrayRef->linkArray[x], in_blockBorderLineList, x);

		while
		(
			(currentLineRunner.isRunComplete == 0)				// only run interior tracing if the run isn't complete, 
			&&													// --AND--
			(currentLineRunner.isRunSingleIteration == 0)		// as long as the run is NOT a single iteration 
																// (single iteration means the line's begin and end points are confined to the same block)
		)
		{
			currentLineRunner.checkForCompletion();
			EnclaveKeyDef::EnclaveKey currentBlockKey = currentLineRunner.currentIterationMeta.blockKey;		// get the current block key the runner is using

			if (currentBlockKey.y == 0)
			{
				std::cout << "!! Notice, special debug value found, printing keys...." << std::endl;

				std::cout << "Enclave triangle empty normal: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;
				std::cout << "Perfect clamp value: " << isEnclaveTrianglePolyPerfectlyClamped << std::endl;

				std::cout << "Begin point enclave key: "; in_linkArrayRef->linkArray[x].beginPointMeta.enclaveKey.printKey(); std::cout << std::endl;
				std::cout << "Begin point block key: "; in_linkArrayRef->linkArray[x].beginPointMeta.blockKey.printKey(); std::cout << std::endl;

				std::cout << "End point enclave key: "; in_linkArrayRef->linkArray[x].endPointMeta.enclaveKey.printKey(); std::cout << std::endl;
				std::cout << "End point block key: "; in_linkArrayRef->linkArray[x].endPointMeta.blockKey.printKey(); std::cout << std::endl;

				int waitVal = 3;
				std::cin >> waitVal;
			}

			int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
			OrganicWrappedBBFan* bbFanRef = &secondaryMap.triangleMap[convertedBlockCoords];		// get a ref to the fan.



			if (bbFanRef->blockSegmentTracker.currentLineSegmentIndex == 1)	// if it only contains 1 segment, do the run.
			{

				//std::cout << "Performing interior run from block: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
				//if (in_polyRunDirection == PolyRunDirection::REVERSE)
				//{
					//std::cout << "!!! Attempting run for REVERSE! " << std::endl;
				//}

				//std::cout << "Terminating set contents: " << std::endl;
				//auto termSetBegin = terminatingSetRef->terminatingSetMap[x].set.begin();
				//auto termSetEnd = terminatingSetRef->terminatingSetMap[x].set.end();
				//for (termSetBegin; termSetBegin != termSetEnd; termSetBegin++)
				//{
					//std::cout << "(" << termSetBegin->x << ", " << termSetBegin->y << ", " << termSetBegin->z << ") " << std::endl;
				//}

				//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
				ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
				ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
				EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

				//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
				//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
													 //<< in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;
				//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
				//if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
				//{
					//std::cout << "!! Warning, slope invalid! " << std::endl;
				//}


				ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
				if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
				{
					std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunners (1 segment); flagging as INVALID. " << std::endl;
					isTriangleValid = false;
					break;
				}


				PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);

				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &exteriorTracedBlocks, &allTracedBlocks, materialID, IsEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey);
				if (in_polyRunDirection == PolyRunDirection::REVERSE)
				{
					//std::cout << "Beginning interior run from block: " << currentBlockKey.x << ",  " << currentBlockKey.y << ",  " << currentBlockKey.z << std::endl;
					//std::cout << "Current begin point is: " << currentBeginPoint.x << ",  " << currentBeginPoint.y << ",  " << currentBeginPoint.z << std::endl;
					//std::cout << "Current end point is: " << tracingResult.resultingEndPoint.x << ",  " << tracingResult.resultingEndPoint.y << ",  " << tracingResult.resultingEndPoint.z << std::endl;

					ECBPolyPoint testIntended = IndependentUtils::determineIntendedFacesV2(in_linkArrayRef->linkArray[x].beginPointRealXYZ, in_linkArrayRef->linkArray[x].endPointRealXYZ, in_linkArrayRef->linkArray[x].thirdPointRealXYZ, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int);
					//std::cout << "Test of intended faces: " << testIntended.x << ", " << testIntended.y << ", " << testIntended.z << std::endl;
				}

				EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap, &terminatingSetRef->terminatingSetMap[x].set, &allTracedBlocks, materialID, isEnclaveTrianglePolyPerfectlyClamped, emptyNormal, currentBlockKey, 1, false);
				//EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint, tracingResult.resultingEndPoint, &in_linkArrayRef->linkArray[x], in_blockBorderLineList, in_borderDataMap, &secondaryMap.triangleMap);

			}



			if (bbFanRef->blockSegmentTracker.currentLineSegmentIndex == 2)
			{

				if (in_polyRunDirection == PolyRunDirection::NORMAL)
				{
					//int isRunValid = bbFanRef->checkIfRunIsValidForTwoSegments(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap);
					//if (isRunValid == 1)


					SegmentResult result = bbFanRef->checkIfRunIsValidForTwoSegmentsSpecial(in_linkArrayRef->linkArray[x].IDofLine, in_blockBorderLineList, in_borderDataMap);
					if (result.isValid == 1)
					{
						//std::cout << "!!! Attempting interior run from: " << currentBlockKey.x << ", " << currentBlockKey.y << ", " << currentBlockKey.z << std::endl;
						ECBPolyPoint currentBeginPoint = currentLineRunner.currentIterationEndPoint;	// get the current endpoint.
						ECBPPOrientationResults currentBeginOrientation = IndependentUtils::GetPointOrientation(currentBeginPoint, in_blockBorderLineList);
						EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, in_borderDataMap);

						//std::cout << "!!! Intended faces are: " << in_linkArrayRef->linkArray[x].intendedFaces.x << ", " << in_linkArrayRef->linkArray[x].intendedFaces.y << ", " << in_linkArrayRef->linkArray[x].intendedFaces.z << std::endl;
						//std::cout << "!!! ++ Line points are: " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << " | "
															 //<< in_linkArrayRef->linkArray[x].endPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].endPointRealXYZ.z << std::endl;
						//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);

						ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, currentBeginOrientation, in_linkArrayRef->linkArray[x].x_int, in_linkArrayRef->linkArray[x].y_int, in_linkArrayRef->linkArray[x].z_int, moveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_linkArrayRef->linkArray[x].intendedFaces);
						if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
						{
							std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunners (2 segments); flagging as INVALID. " << std::endl;
							//isTriangleValid = false;
							//break;
						}


						bool finder = isSegmentAndSlopeParallel(result.otherSegment, interceptToUse);
						std::map<int, TerminatingSet> tempSet;

						if
							(
							(finder == true)						// only do this if there is a parallel detection.
								&&
								(isEnclaveTrianglePolyPerfectlyClamped == 0)			// it's unnecessary to do this check if the triangle is in true 2 dimensions, and not 3d. 
								)
						{


							//std::cout << "!! +++++Entering true branch. +++++" << std::endl;
							//std::cout << "!! This segment's LINE id is: " << int(in_linkArrayRef->linkArray[x].IDofLine) << std::endl;
							//std::cout << "!! Other segment ID was: " << int(result.otherSegment.lineID) << std::endl;
							tempSet = terminatingSetRef->constructSpecialSet(in_linkArrayRef->linkArray[x].IDofLine, int(result.otherSegment.lineID));
							auto firstVal = tempSet.begin();
							auto secondVal = firstVal->second.set.begin();

							//std::cout << "!! The new map has the following line as a terminating line: " << firstVal->first << std::endl;
							PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);
							EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint,
								tracingResult.resultingEndPoint,
								in_linkArrayRef->linkArray[x],
								in_blockBorderLineList,
								in_borderDataMap,
								&secondaryMap.triangleMap,
								&tempSet[in_linkArrayRef->linkArray[x].IDofLine].set,
								&allTracedBlocks,
								materialID,
								isEnclaveTrianglePolyPerfectlyClamped,
								emptyNormal,
								currentBlockKey,
								2,
								false
							);
						}
						else
						{
							PLTracingResult tracingResult = IndependentUtils::getBlockTracingResult(currentBeginPoint, interceptToUse, in_blockBorderLineList, in_borderDataMap, 0);
							EnclaveTriangleInteriorRunner interiorRunner(currentBeginPoint,
								tracingResult.resultingEndPoint,
								in_linkArrayRef->linkArray[x],
								in_blockBorderLineList,
								in_borderDataMap,
								&secondaryMap.triangleMap,
								&terminatingSetRef->terminatingSetMap[x].set,
								&allTracedBlocks, materialID,
								isEnclaveTrianglePolyPerfectlyClamped,
								emptyNormal,
								currentBlockKey,
								2,
								false);
						}
						//}
					}
				}
			}

			// new code goes here (for interior runner)
			// ...
			// ...
			// ...
			currentLineRunner.iterateToNextBlock();
		}
	}
}

bool EnclaveTriangle::isSegmentAndSlopeParallel(SegmentMeta in_segment, ECBPolyPoint in_slope)
{
	bool result = false;

	// get normalized slope of the segment.
	ECBPolyPoint normalizedSegment = IndependentUtils::findNormalizedSlope(in_segment.endVertex, in_segment.beginVertex);
	if
		((normalizedSegment.x == 0) && (in_slope.x == 0))
	{
		//std::cout << "!! Segment and slopes are parallel on X. " << std::endl;
		result = true;
	}

	if
		((normalizedSegment.y == 0) && (in_slope.y == 0))
	{
		//std::cout << "!! Segment and slopes are parallel on Y. " << std::endl;
		result = true;
	}

	if
		((normalizedSegment.z == 0) && (in_slope.z == 0))
	{
		//std::cout << "!! Segment and slopes are parallel on Z. " << std::endl;
		result = true;
	}

	return result;
}

bool EnclaveTriangle::doBlocksExistAtY(int in_y)
{
	bool blocksExistAtY = false;
	auto existingTertiaryBlocksBegin = secondaryMap.triangleMap.begin();
	auto existingTertiaryBlocksEnd = secondaryMap.triangleMap.end();
	for (; existingTertiaryBlocksBegin != existingTertiaryBlocksEnd; existingTertiaryBlocksBegin++)
	{
		EnclaveKeyDef::EnclaveKey convertedKey = PolyUtils::convertSingleToBlockKey(existingTertiaryBlocksBegin->first);
		if (convertedKey.y == in_y)
		{
			std::cout << "! Found key at y " << in_y; convertedKey.printKey(); std::cout << std::endl;
			blocksExistAtY = true;
		}
	}
	return blocksExistAtY;
}

void EnclaveTriangle::reform(ECBPolyPoint in_polyPoint0, ECBPolyPoint in_polyPoint1, ECBPolyPoint in_polyPoint2)
{
	TriangleLine line1, line2, line3;

	line1.pointA = in_polyPoint0; // roundedPoint1;
	line1.pointB = in_polyPoint1; // roundedPoint2;

	line2.pointA = in_polyPoint1; // roundedPoint2;
	line2.pointB = in_polyPoint2; // roundedPoint3;

	line3.pointA = in_polyPoint2; // roundedPoint3;
	line3.pointB = in_polyPoint0; // roundedPoint1;

	lineArray[0] = PolyUtils::convertToECBPolyLine(line1, in_polyPoint2);	// roundedPoint3
	lineArray[1] = PolyUtils::convertToECBPolyLine(line2, in_polyPoint0);	// roundedPoint1
	lineArray[2] = PolyUtils::convertToECBPolyLine(line3, in_polyPoint1);	// roundedPoint2

	// everything else -- materialID, clamping, etc...stays the same; we're only reforming the points.
}

void EnclaveTriangle::generateExteriorLineSegments(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	//std::cout << "############ Generating exterior segments: " << std::endl;

	//BlockBorderLineList lineList;
	//BorderDataMap borderDataMap;

	/*
	std::cout << "####### Points ########## " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		std::cout << x << ": " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.x << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.y << ", " << in_linkArrayRef->linkArray[x].beginPointRealXYZ.z << std::endl;
	}
	*/

	//std::cout << "####### Slope output ######### " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		//std::cout << ":::: " << x  << std::endl;
		//std::cout << "X:: " << in_linkArrayRef->linkArray[x].x_int.x << ", " << in_linkArrayRef->linkArray[x].x_int.y << ", " << in_linkArrayRef->linkArray[x].x_int.z << std::endl;
		//std::cout << "Y:: " << in_linkArrayRef->linkArray[x].y_int.x << ", " << in_linkArrayRef->linkArray[x].y_int.y << ", " << in_linkArrayRef->linkArray[x].y_int.z << std::endl;
		//std::cout << "Z:: " << in_linkArrayRef->linkArray[x].z_int.x << ", " << in_linkArrayRef->linkArray[x].z_int.y << ", " << in_linkArrayRef->linkArray[x].z_int.z << std::endl;
	}
	/*
	auto secondaryBegin = secondaryMap.triangleMap.begin();
	auto secondaryEnd = secondaryMap.triangleMap.end();
	for (secondaryBegin; secondaryBegin != secondaryEnd; secondaryBegin++)
	{
		//short perfectClamp = secondaryBegin->second.
		//BlockCircuit circuit(&lineList, &borderDataMap, IsEnclaveTrianglePolyPerfectlyClamped, PolyDebugLevel::NONE);

		EnclaveKeyDef::EnclaveKey convertedKey = PolyUtils::convertSingleToBlockKey(secondaryBegin->first);
		std::cout << "Inserting data for Block: " << convertedKey.x << ", " << convertedKey.y << ", " << convertedKey.z << " | Segment index: " << int(secondaryBegin->second.blockSegmentTracker.currentLineSegmentIndex) << std::endl;

		BlockCircuit circuit(in_blockBorderLineList, in_borderDataMap, IsEnclaveTrianglePolyPerfectlyClamped, PolyDebugLevel::NONE);
		for (int x = 0; x < secondaryBegin->second.blockSegmentTracker.currentLineSegmentIndex; x++)
		{
			//circuit.insertNewSegment(
			PrimarySegmentMeta sMeta;

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

			ECBPolyPoint segmentBeginPoint = secondaryBegin->second.blockSegmentTracker.lineSegments[x].beginVertex;
			ECBPolyPoint segmentEndPoint = secondaryBegin->second.blockSegmentTracker.lineSegments[x].endVertex;
			//ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, &lineList);
			//ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, &lineList);

			ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, in_blockBorderLineList);
			ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, in_blockBorderLineList);

			//BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, &borderDataMap);
			//BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, &borderDataMap);

			BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, in_borderDataMap);
			BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, in_borderDataMap);

			sMeta.startPoint = segmentBeginPoint;
			sMeta.startFaces = segmentBeginFaceList;
			sMeta.endPoint = segmentEndPoint;
			sMeta.endFaces = segmentEndFaceList;

			circuit.insertNewSegment(sMeta);

			//std::cout << "New segment data: " << std::endl;
			//std::cout << "begin point : " << segmentBeginPoint.x << ", " << segmentBeginPoint.y << ", " << segmentBeginPoint.z << std::endl;
			//std::cout << "end point   : " << segmentEndPoint.x << ", " << segmentEndPoint.y << ", " << segmentEndPoint.z << std::endl;
			//std::cout << "X-slope     : " << sMeta.x_slope.x << ", " << sMeta.x_slope.y << ", " << sMeta.x_slope.z << std::endl;
			//std::cout << "Y-slope     : " << sMeta.y_slope.x << ", " << sMeta.y_slope.y << ", " << sMeta.y_slope.z << std::endl;
			//std::cout << "Z-slope     : " << sMeta.z_slope.x << ", " << sMeta.z_slope.y << ", " << sMeta.z_slope.z << std::endl;

		}
		fillCircuitMetaData(&circuit, in_blockBorderLineList, in_borderDataMap);
		populateOrganicWrappedBBFanWithCircuitData(&circuit, secondaryBegin->first);
	}
	*/

	auto exteriorBegin = exteriorTracedBlocks.begin();
	auto exteriorEnd = exteriorTracedBlocks.end();
	for (exteriorBegin; exteriorBegin != exteriorEnd; exteriorBegin++)
	{
		//std::cout << "!! Exterior segment add for: " << exteriorBegin->x << ", " << exteriorBegin->y << ", " << exteriorBegin->z << std::endl;
			//std::cout << "Poly perfect clamp value is: " << IsEnclaveTrianglePolyPerfectlyClamped << std::endl;


		BlockCircuit circuit(in_blockBorderLineList, in_borderDataMap, isEnclaveTrianglePolyPerfectlyClamped, PolyDebugLevel::NONE);
		int convertedToSingle = PolyUtils::convertBlockCoordsToSingle(exteriorBegin->x, exteriorBegin->y, exteriorBegin->z);
		auto secondaryBegin = secondaryMap.triangleMap.find(convertedToSingle);
		for (int x = 0; x < secondaryBegin->second.blockSegmentTracker.currentLineSegmentIndex; x++)
		{
			//circuit.insertNewSegment(
			PrimarySegmentMeta sMeta;

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

			ECBPolyPoint segmentBeginPoint = secondaryBegin->second.blockSegmentTracker.lineSegments[x].beginVertex;
			ECBPolyPoint segmentEndPoint = secondaryBegin->second.blockSegmentTracker.lineSegments[x].endVertex;
			//ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, &lineList);
			//ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, &lineList);

			ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, in_blockBorderLineList);
			ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, in_blockBorderLineList);

			//BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, &borderDataMap);
			//BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, &borderDataMap);

			BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, in_borderDataMap);
			BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, in_borderDataMap);

			sMeta.startPoint = segmentBeginPoint;
			sMeta.startFaces = segmentBeginFaceList;
			sMeta.endPoint = segmentEndPoint;
			sMeta.endFaces = segmentEndFaceList;

			circuit.insertNewSegment(sMeta);


			/*
			if (segmentBeginPoint.x == 2)
			{
				std::cout << "::: Block: " << exteriorBegin->x << ", " << exteriorBegin->y << ", " << exteriorBegin->z << std::endl;

				std::cout << ">>>>>>>>>>>>>New segment data: " << std::endl;
				std::cout << "begin point : " << segmentBeginPoint.x << ", " << segmentBeginPoint.y << ", " << segmentBeginPoint.z << std::endl;
				std::cout << "end point   : " << segmentEndPoint.x << ", " << segmentEndPoint.y << ", " << segmentEndPoint.z << std::endl;
			}
			*/
			//std::cout << "X-slope     : " << sMeta.x_slope.x << ", " << sMeta.x_slope.y << ", " << sMeta.x_slope.z << std::endl;
			//std::cout << "Y-slope     : " << sMeta.y_slope.x << ", " << sMeta.y_slope.y << ", " << sMeta.y_slope.z << std::endl;
			//std::cout << "Z-slope     : " << sMeta.z_slope.x << ", " << sMeta.z_slope.y << ", " << sMeta.z_slope.z << std::endl;

		}
		//std::cout << "#################################### ||||||||||||| Building circuit for key: (" << exteriorBegin->x << ", " << exteriorBegin->y << ", " << exteriorBegin->z << ") " << std::endl;
		//fillCircuitMetaData(&circuit, in_blockBorderLineList, in_borderDataMap, in_key);
		//fillCircuitMetaDataSpecialDebug(&circuit, in_blockBorderLineList, in_borderDataMap, in_key);
		fillCircuitMetaDataV2(&circuit, in_blockBorderLineList, in_borderDataMap, in_key);
		if (isTriangleValid == false)
		{
			//std::cout << "Triangle detected as invalid, stopping run of EnclaveTriangle. " << std::endl;
			break;
		}

		//std::cout << "### Circuit fill complete...  " << std::endl;
		populateOrganicWrappedBBFanWithCircuitData(&circuit, secondaryBegin->first);
		//std::cout << "### BB fan build complete... " << std::endl;
	}
}

SecondarySearchResult EnclaveTriangle::checkIfSecondaryExists(EnclaveKeyDef::EnclaveKey in_key)
{

	SecondarySearchResult result;
	/*
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	for (int x = 0; x < numberOfSecondaries; x++)
	{
		if (organicSecondaryArray[x].keyID == keyToSingle)
		{
			result.wasFound = true;
			result.foundIndex = x;
		}
	}
	*/
	return result;

}

void EnclaveTriangle::insertNewSecondary(EnclaveKeyDef::EnclaveKey in_key)
{
	/*
	OrganicWrappedBBFan newSecondary;
	newSecondary.keyID = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	organicSecondaryArray[numberOfSecondaries++] = newSecondary;
	*/
}

void EnclaveTriangle::fillCircuitMetaData(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	/*
	if
		(
		(in_key.x == 0)
			&&
			(in_key.y == 6)
			&&
			(in_key.z == 6)
			)
	{
	*/
	std::cout << "!!! (EnclaveTriangle) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
	int numberOfSegments = in_circuitRef->currentSegmentIndex;
	std::cout << ":: Number of segments: " << numberOfSegments << std::endl;
	for (int x = 0; x < numberOfSegments; x++)
	{
		std::cout << x << ": " << in_circuitRef->segmentArray[x].segment.startPoint.x << ", " << in_circuitRef->segmentArray[x].segment.startPoint.y << ", " << in_circuitRef->segmentArray[x].segment.startPoint.z << " ||| "
			<< in_circuitRef->segmentArray[x].segment.endPoint.x << ", " << in_circuitRef->segmentArray[x].segment.endPoint.y << ", " << in_circuitRef->segmentArray[x].segment.endPoint.z << std::endl;
	}
	//}





	int matchRequiredCount = 1;	// the number of faces that will have to match (the threshold); the default value is 1, but may be set to 2, if the triangle is perfectly clamped and the points of the segment exist on a shared face.
	int segmentToDetermineMatchThreshold = findIndexOfSegmentWithNonFreeEndpoint(in_circuitRef, in_key);
	ECBPolyPoint newPoint = getTraceResultForEndpointSegment(in_circuitRef, segmentToDetermineMatchThreshold, in_blockBorderLineList, in_borderDataMap);
	ECBPPOrientationResults newPointOrientation = IndependentUtils::GetPointOrientation(newPoint, in_blockBorderLineList);
	BorderMDFaceList newFaceList = IndependentUtils::getFaceList(newPointOrientation, in_borderDataMap);

	if (isEnclaveTrianglePolyPerfectlyClamped >= 1)	// only do the below if it's perfectly clamped to a dimension.
	{
		int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startFaces, in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endFaces, newFaceList, 0);
		if (hasCommonFaces == 1)
		{
			matchRequiredCount = 2;

			if
				(
				(in_key.x == 0)
					&&
					(in_key.y == 6)
					&&
					(in_key.z == 6)
					)
			{

				std::cout << "!!! (EnclaveTriangle) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
				int numberOfSegments = in_circuitRef->currentSegmentIndex;
				std::cout << ":: Number of segments: " << numberOfSegments << std::endl;
				for (int x = 0; x < numberOfSegments; x++)
				{
					std::cout << x << ": " << in_circuitRef->segmentArray[x].segment.startPoint.x << ", " << in_circuitRef->segmentArray[x].segment.startPoint.y << ", " << in_circuitRef->segmentArray[x].segment.startPoint.z << " ||| "
						<< in_circuitRef->segmentArray[x].segment.endPoint.x << ", " << in_circuitRef->segmentArray[x].segment.endPoint.y << ", " << in_circuitRef->segmentArray[x].segment.endPoint.z << std::endl;
				}

				std::cout << "####### Points compared to get the match threshold were: " << std::endl;
				std::cout << "Segment start: " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.z << std::endl;
				std::cout << "Segment end  : " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.z << std::endl;
				std::cout << "new traced point : " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			}
			//std::cout << "!!! (EnclaveTriangle) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
		}
		else if (hasCommonFaces == 0)
		{
			//std::cout << "EnclaveTriangle was perfectly clamped, but had a threshold of 1. " << std::endl;
		}
	}

	if
		(
		(in_key.x == 0)
			&&
			(in_key.y == 6)
			&&
			(in_key.z == 6)
			)
	{

		//in_circuitRef->fillSegmentArrayMetaData(matchRequiredCount, in_key);
	}
}

void EnclaveTriangle::fillCircuitMetaDataSpecialDebug(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	if
		(
		(in_key.x == 0)
			&&
			(in_key.y == 6)
			&&
			(in_key.z == 6)
			&&

			(in_circuitRef->segmentArray[2].segment.startPoint.x == 1)
			//&&
			//(in_circuitRef->segmentArray[2].segment.startPoint.z == .9)
			)
	{
		std::cout << "!!! (EnclaveTriangle [1]) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
		int numberOfSegments = in_circuitRef->currentSegmentIndex;
		std::cout << ":: Number of segments: " << numberOfSegments << std::endl;
		for (int x = 0; x < numberOfSegments; x++)
		{
			std::cout << x << ": " << in_circuitRef->segmentArray[x].segment.startPoint.x << ", " << in_circuitRef->segmentArray[x].segment.startPoint.y << ", " << in_circuitRef->segmentArray[x].segment.startPoint.z << " ||| "
				<< in_circuitRef->segmentArray[x].segment.endPoint.x << ", " << in_circuitRef->segmentArray[x].segment.endPoint.y << ", " << in_circuitRef->segmentArray[x].segment.endPoint.z << std::endl;
		}
		//}





		int matchRequiredCount = 1;	// the number of faces that will have to match (the threshold); the default value is 1, but may be set to 2, if the triangle is perfectly clamped and the points of the segment exist on a shared face.
		int segmentToDetermineMatchThreshold = findIndexOfSegmentWithNonFreeEndpoint(in_circuitRef, in_key);

		std::wcout << "DEBUG: chosen segment begin: " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.z << std::endl;
		std::wcout << "DEBUG: chosen segment   end: " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.z << std::endl;

		ECBPolyPoint newPoint = getTraceResultForEndpointSegment(in_circuitRef, segmentToDetermineMatchThreshold, in_blockBorderLineList, in_borderDataMap);
		ECBPPOrientationResults newPointOrientation = IndependentUtils::GetPointOrientation(newPoint, in_blockBorderLineList);
		BorderMDFaceList newFaceList = IndependentUtils::getFaceList(newPointOrientation, in_borderDataMap);

		if (isEnclaveTrianglePolyPerfectlyClamped >= 1)	// only do the below if it's perfectly clamped to a dimension.
		{
			int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startFaces, in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endFaces, newFaceList, 0);
			if (hasCommonFaces == 1)
			{
				matchRequiredCount = 2;

				if
					(
					(in_key.x == 0)
						&&
						(in_key.y == 6)
						&&
						(in_key.z == 6)
						)
				{

					std::cout << "!!! (EnclaveTriangle [2]) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
					int numberOfSegments = in_circuitRef->currentSegmentIndex;
					std::cout << ":: Number of segments: " << numberOfSegments << std::endl;
					for (int x = 0; x < numberOfSegments; x++)
					{
						std::cout << x << ": " << in_circuitRef->segmentArray[x].segment.startPoint.x << ", " << in_circuitRef->segmentArray[x].segment.startPoint.y << ", " << in_circuitRef->segmentArray[x].segment.startPoint.z << " ||| "
							<< in_circuitRef->segmentArray[x].segment.endPoint.x << ", " << in_circuitRef->segmentArray[x].segment.endPoint.y << ", " << in_circuitRef->segmentArray[x].segment.endPoint.z << std::endl;
					}

					std::cout << "####### Points compared to get the match threshold were: " << std::endl;
					std::cout << "Segment start: " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startPoint.z << std::endl;
					std::cout << "Segment end  : " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.x << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.y << ", " << in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endPoint.z << std::endl;
					std::cout << "new traced point : " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
				}
				//std::cout << "!!! (EnclaveTriangle) NOTICE: all points share a common face; Enclave Key is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") " << std::endl;
			}
			else if (hasCommonFaces == 0)
			{
				//std::cout << "EnclaveTriangle was perfectly clamped, but had a threshold of 1. " << std::endl;
			}
		}

		if
			(
			(in_key.x == 0)
				&&
				(in_key.y == 6)
				&&
				(in_key.z == 6)
				)
		{

			ECBPolyPoint point_0 = lineArray[0].pointA;
			ECBPolyPoint point_1 = lineArray[1].pointA;
			ECBPolyPoint point_2 = lineArray[2].pointA;
			isTriangleValid = in_circuitRef->fillSegmentArrayMetaDataFromCircuit(matchRequiredCount, in_key, point_0, point_1, point_2);
		}
	}
}

void EnclaveTriangle::fillCircuitMetaDataV2(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key)
{
	int matchRequiredCount = 1;
	// determine the appropriate match threshold, based on perfect clamp value.
	if (isEnclaveTrianglePolyPerfectlyClamped >= 1)	// only do the below if it's perfectly clamped to a dimension.
	{
		// resulting bool array
		bool boolArray[3];
		for (int x = 0; x < 3; x++)
		{
			boolArray[x] = false;
		}

		// index of next line array
		int nextLineArray[3];

		//int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.startFaces, in_circuitRef->segmentArray[segmentToDetermineMatchThreshold].segment.endFaces, newFaceList, 0);
		int currentMaxIndex = in_circuitRef->currentSegmentCount - 1;
		for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)
		{
			if (x != currentMaxIndex)
			{
				nextLineArray[x] = x + 1;
				int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[x].segment.startFaces, in_circuitRef->segmentArray[x].segment.endFaces, in_circuitRef->segmentArray[nextLineArray[x]].segment.startFaces, 0);
				if (hasCommonFaces == 1)
				{
					boolArray[x] = true;
				}
				//segmentArray[x].indexOfNextSegment = x + 1;
				//ECBPolyPoint targetPointToAdd = segmentArray[x + 1].segment.startPoint;
				//segmentArray[x].targetPoint = targetPointToAdd;								// insert it
			}
			else if (x == currentMaxIndex)	// otherwise, the next segment's index is 0 (completing the circuit)
			{
				nextLineArray[x] = 0;
				int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(in_circuitRef->segmentArray[x].segment.startFaces, in_circuitRef->segmentArray[x].segment.endFaces, in_circuitRef->segmentArray[nextLineArray[x]].segment.startFaces, 0);
				if (hasCommonFaces == 1)
				{
					boolArray[x] = true;
				}
				//segmentArray[x].indexOfNextSegment = 0;
				//ECBPolyPoint targetPointToAdd = segmentArray[0].segment.startPoint;			// "" 
				//segmentArray[x].targetPoint = targetPointToAdd;								// ""
			}
		}

		// loop and check for all true; if they are all true, its perfectly clamped.
		int boolMatchCounter = 0;
		for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)
		{
			if (boolArray[x] == true)
			{
				boolMatchCounter++;
			}
		}

		if (boolMatchCounter == in_circuitRef->currentSegmentCount)	// if the counter of "trues" equals the number of segments, it's perfectly clamped.
		{
			matchRequiredCount = 2;
		}
	}

	ECBPolyPoint point_0 = lineArray[0].pointA;
	ECBPolyPoint point_1 = lineArray[1].pointA;
	ECBPolyPoint point_2 = lineArray[2].pointA;
	//in_circuitRef->fillSegmentArrayMetaData(matchRequiredCount, in_key, point_0, point_1, point_2);
	isTriangleValid = in_circuitRef->fillSegmentArrayMetaDataFromCircuit(matchRequiredCount, in_key, point_0, point_1, point_2);
}

void EnclaveTriangle::populateOrganicWrappedBBFanWithCircuitData(BlockCircuit* in_circuitRef, int in_secondaryID)
{
	if (in_circuitRef->isCircuitValid == true)	// only add if the circuit contains a valid BB fan that can be constructed.
	{
		secondaryMap.triangleMap[in_secondaryID].buildBBFan(in_circuitRef, materialID, emptyNormal);
	}
}

int EnclaveTriangle::findIndexOfSegmentWithNonFreeEndpoint(BlockCircuit* in_circuitRef, EnclaveKeyDef::EnclaveKey in_key)
{
	int indexVal = 0;	// default is 0
	for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)
	{
		if (in_circuitRef->segmentArray[x].segment.isEndpointNotFree() == true)		// find the segment that has no free endpoint.
		//if (isEndpointValidForFreeSelection(x, in_circuitRef) == true)
		{
			//std::cout << "Found non-free point at index: " << x << std::endl;
			indexVal = x;
		}
	}
	return indexVal;
}

bool EnclaveTriangle::isEndpointValidForFreeSelection(int in_lineID, BlockCircuit* in_circuitRef)
{
	bool result = false;



	//int nextSegmentID = circuit.segmentArray[in_lineID].indexOfNextSegment;
	int currentMaxIndex = in_circuitRef->currentSegmentCount - 1;		// get the max index.
	int tempLinkIdArray[3];										// will store the IDs we find
	for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)		// cycle through the segments, determine what the next line to check would be.
	{
		if (x != currentMaxIndex)
		{
			tempLinkIdArray[x] = x + 1;
		}
		else if (x == currentMaxIndex)
		{
			tempLinkIdArray[x] = 0;
		}
	}

	int nextLineID = tempLinkIdArray[in_lineID];				// get the value of the next line to check

	if (in_lineID == 2)
	{
		for (int x = 0; x < in_circuitRef->currentSegmentCount; x++)		// cycle through the segments, determine what the next line to check would be.
		{
			std::cout << x << ": " << tempLinkIdArray[x] << std::endl;
		}
	}

	if
		(
		(in_circuitRef->segmentArray[in_lineID].segment.endPoint.x != in_circuitRef->segmentArray[nextLineID].segment.startPoint.x)
			&&
			(in_circuitRef->segmentArray[in_lineID].segment.endPoint.y != in_circuitRef->segmentArray[nextLineID].segment.startPoint.y)
			&&
			(in_circuitRef->segmentArray[in_lineID].segment.endPoint.z != in_circuitRef->segmentArray[nextLineID].segment.startPoint.z)
			)
	{
		result = true;
	}

	return result;
}

ECBPolyPoint EnclaveTriangle::getTraceResultForEndpointSegment(BlockCircuit* in_circuitRef, int in_segmentIndex, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	ECBPPOrientationResults enclaveOrientationResults = IndependentUtils::GetPointOrientation(in_circuitRef->segmentArray[in_segmentIndex].segment.endPoint, in_blockBorderLineList);
	PrimarySegmentMeta* metaRef = &in_circuitRef->segmentArray[in_segmentIndex].segment;
	EnclaveKeyDef::EnclaveKey tempMoveVals = IndependentUtils::retrieveBorderDirection(enclaveOrientationResults, in_borderDataMap);	// get the move vals from the current end point orientation
	//ECBPolyPoint slopeToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(in_borderDataMap, enclaveOrientationResults, metaRef->x_slope, metaRef->y_slope, metaRef->z_slope, tempMoveVals, IsEnclaveTrianglePolyPerfectlyClamped, 0, in_circuitRef->segmentArray[in_segmentIndex].segment.intendedFaces);
	ECBPolyPoint slopeToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(in_borderDataMap, enclaveOrientationResults, metaRef->x_slope, metaRef->y_slope, metaRef->z_slope, tempMoveVals, isEnclaveTrianglePolyPerfectlyClamped, 0, in_circuitRef->segmentArray[in_segmentIndex].segment.intendedFaces);
	if (IndependentUtils::checkIfInterceptIsValid(slopeToUse) == false)
	{
		std::cout << "Triangle flagged as invalid, in EnclaveTriangle::runInteriorRunnerReverse; flagging as INVALID. " << std::endl;
		isTriangleValid = false;
		//break;
	}

	ECBPolyPoint newEndPoint = IndependentUtils::getBlockTracingEndpoint(in_circuitRef->segmentArray[in_segmentIndex].segment.endPoint, slopeToUse, in_blockBorderLineList);
	//std::cout << "### New point is: " << newEndPoint.x << ", " << newEndPoint.y << ", " << newEndPoint.z << std::endl;
	return newEndPoint;
}

void EnclaveTriangle::performCentroidBlockCheck(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC)
{
	ECBPolyPoint centroid = IndependentUtils::determineTriangleCentroid(in_pointA, in_pointB, in_pointC);
	EnclaveKeyDef::EnclaveKey dummyKey;	// dummy for testing.
	ECBBorderValues borderVals = IndependentUtils::getBlueprintLimits(dummyKey);
	ECBPolyPointLocation pointALocation = IndependentUtils::getPolyPointLocation(centroid, borderVals);
	//std::cout << "Centroid block key is: " << pointALocation.blockKey.x << ", " << pointALocation.blockKey.y << ", " << pointALocation.blockKey.z << std::endl;

	//int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(currentBlockKey.x, currentBlockKey.y, currentBlockKey.z);
	//secondaryMap.triangleMap[convertedBlockCoords].reverseSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, x);											
	//secondaryMap.triangleMap[convertedBlockCoords].reverseSegmentTracker.addNewSegment(currentLineRunner.currentIterationBeginPoint, currentLineRunner.currentIterationEndPoint, in_linkArrayRef->linkArray[x].IDofLine);

	int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(pointALocation.blockKey.x, pointALocation.blockKey.y, pointALocation.blockKey.z);
	auto wasFound = secondaryMap.triangleMap.find(convertedBlockCoords);
	if (wasFound == secondaryMap.triangleMap.end())
	{
		std::cout << "<< CENTROID not detected! " << pointALocation.blockKey.x << ", " << pointALocation.blockKey.y << ", " << pointALocation.blockKey.z << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
	}
}