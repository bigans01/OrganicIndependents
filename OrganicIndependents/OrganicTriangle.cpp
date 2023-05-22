#include "stdafx.h"
#include "OrganicTriangle.h"

OrganicTriangle::OrganicTriangle()
{

}

OrganicTriangle::OrganicTriangle(ECBPoly* in_ecbPolyRef, PrimaryLineT1Array in_forwardPrimaryLineCopy, PrimaryLineT1Array in_reversePrimaryLineCopy, EnclaveKeyDef::EnclaveKey in_blueprintKey, BorderDataMap* in_borderDataMapRef, BlockBorderLineList* in_blockBorderLineListRef, int in_polyID)
{
	//std::cout << "ORGANICTRIANGLE BLUEPRINT KEY: " << in_blueprintKey.x << ", " << in_blueprintKey.y << ", " << in_blueprintKey.z << std::endl;
	//blueprintKey = in_blueprintKey;								// set the blueprint key.	
	//forwardPolyLineArray = in_forwardPrimaryLineCopy;							// a copy of the line array for a forward direction (may not be needed later)
	//reversePolyLineArray = in_reversePrimaryLineCopy;							// " " reverse direction run
	borderDataMapRef = in_borderDataMapRef;						// border data ref
	//blockBorderLineListRef = in_blockBorderLineListRef;
	ecbPolyRef = in_ecbPolyRef;
	polyID = in_polyID;
	//isPolyPerfectlyClamped = in_ecbPolyRef->isPolyPerfectlyClamped;
	//materialID = in_ecbPolyRef->materialID;
	//emptyNormal = in_ecbPolyRef->emptyNormal;

	//std::cout << "### Organic Triangle Material ID is: " << ecbPolyRef->materialID << std::endl;
	//std::cout << "### Organic triangle points are: " << std::endl;
	//std::cout << "0: " << forwardPolyLineArray.linkArray[0].beginPointRealXYZ.x << ", " << forwardPolyLineArray.linkArray[0].beginPointRealXYZ.y << ", " << forwardPolyLineArray.linkArray[0].beginPointRealXYZ.z << std::endl;
	//std::cout << "1: " << forwardPolyLineArray.linkArray[1].beginPointRealXYZ.x << ", " << forwardPolyLineArray.linkArray[1].beginPointRealXYZ.y << ", " << forwardPolyLineArray.linkArray[1].beginPointRealXYZ.z << std::endl;
	//std::cout << "2: " << forwardPolyLineArray.linkArray[2].beginPointRealXYZ.x << ", " << forwardPolyLineArray.linkArray[2].beginPointRealXYZ.y << ", " << forwardPolyLineArray.linkArray[2].beginPointRealXYZ.z << std::endl;

	// ----------------------------- Part 1: external tracing.
	/*
	for (int x = 0; x < 3; x++)
	{

		//std::cout << "!!!!!!!!! Attempting run for line: " << x << std::endl;
		EnclaveKeyDef::EnclaveKey beginEnclaveKey = in_t1LineArrayCopy.linkArray[x].beginPointMeta.enclaveKey;
		EnclaveKeyDef::EnclaveKey endEnclaveKey = in_t1LineArrayCopy.linkArray[x].endPointMeta.enclaveKey;
		if (OrganicUtils::checkIfKeysMatch(beginEnclaveKey, endEnclaveKey) == false)
		{
			OrganicTriangleExteriorRunner runner(in_t1LineArrayCopy.linkArray[x]);	// run each line through the ePolyMap (exterior)
			while (runner.isRunComplete == 0)
			{
				runner.checkIfRunComplete();		// check if the run is complete, so we know how to return the points below

				ECBPolyPointPair pair = runner.retrieveCurrentBeginAndEndPoints();
				// things to do go here
				//std::cout << std::endl;
				EnclaveKeyDef::EnclaveKey currentKey = runner.retrieveCurrentIterationEnclaveKey();
				polyMap.addExternalEPolyData(currentKey, pair, x, &forwardPolyLineArray, &enclaveBorderLines, borderDataMapRef, isPolyPerfectlyClamped);	// add external data

				//std::cout << "|| Current line points: " << std::endl;
				//std::cout << "|| A: " << pair.pointA.x << ", " << pair.pointA.y << ", " << pair.pointA.z << std::endl;
				//std::cout << "|| B: " << pair.pointB.x << ", " << pair.pointB.y << ", " << pair.pointB.z << std::endl;
				//std::cout << "|| Enclave Key: " << currentKey.x << ", " << currentKey.y << ", " << currentKey.z << std::endl;

				runner.traverseLineOnce();

			}

			//int someVal;
			//std::cout << "#### run complete, enter value to continue..." << std::endl;
			//std::cin >> someVal;
		}
		else if (OrganicUtils::checkIfKeysMatch(beginEnclaveKey, endEnclaveKey) == true)
		{
			//std::cout << "!!! TRUE Value reached... " << std::endl;
			OrganicTriangleExteriorRunner runner(in_t1LineArrayCopy.linkArray[x]);	// run each line through the ePolyMap (exterior)
			runner.checkIfRunComplete();
			ECBPolyPointPair pair = runner.retrieveCurrentBeginAndEndPoints();
			EnclaveKeyDef::EnclaveKey currentKey = runner.retrieveCurrentIterationEnclaveKey();
			polyMap.addExternalEPolyData(currentKey, pair, x, &forwardPolyLineArray, &enclaveBorderLines, borderDataMapRef, isPolyPerfectlyClamped);		// add external data
		}

		// else if true, do things here
	}

	//polyMap.printExternalKeys();


	// ----------------------------- Part 2: build the exterior enclave poly circuits.
	polyMap.determineExteriorEnclavePolyCircuits();
	*/

	//fillExteriorEnclavePolyCircuits();		// step 1: build the exterior EnclavePolys, their circuits, and segments; generate the poly for these enclaves.
	//runExteriorReverse(&in_reversePrimaryLineCopy);


	//forwardTerminatingContainer.buildTerminatingSets();		// build the terminating sets (required before an interior run.)
	//reverseTerminatingContainer.buildTerminatingSets();


	//runInteriorFillerOriginalDirection();	// step 2: run interior filling in the original direction.
	//runInteriorFiller(&in_forwardPrimaryLineCopy);
	//runInteriorFiller(&in_reversePrimaryLineCopy);
	produceEnclaveTriangles();				// step 3: after the interior filler has run (both original and reverse), go to each EnclavePoly and produce the EnclaveTriangles.

	EnclaveKeyDef::EnclaveKey debugKey;
	debugKey.x = 3;
	debugKey.y = 2;
	debugKey.z = 3;
	//debugCheckIfKeyExists(debugKey);
	//int someVal;
	//std::cout << "#### run complete, enter value to continue..." << std::endl;
	//std::cin >> someVal;
}

void OrganicTriangle::produceEnclaveTriangles()
{
	// ||||||||||||||||||||||||||||||||||||||||||||||| START OPTION 1: The old school way.
	auto enclaveBegin = polyMap.enclavePolyMap.begin();
	auto enclaveEnd = polyMap.enclavePolyMap.end();
	for (enclaveBegin; enclaveBegin != enclaveEnd; enclaveBegin++)
	{
		/*
		if
			(
			(enclaveBegin->first.x == 5)
				&&
				(enclaveBegin->first.y == 7)
				&&
				(enclaveBegin->first.z == 0)
				)
		{
		*/
		//std::cout << "::::: Producing enclave Triangles for: " << enclaveBegin->first.x << ", " << enclaveBegin->first.y << ", " << enclaveBegin->first.z << std::endl;
		//enclaveBegin->second.epProduceEnclaveTriangles(enclaveBegin->first);
	//}
	}
	// ||||||||||||||||||||||||||||||||||||||||||||| END OPTION 1.



	// ||||||||||||||||||||||||||||||||||||||||||||||| START OPTION 2: Use the entire ECBPoly to create an FTriangle, and do it that way.
	//
	// NOTE: as of 1/31/2023, "OPTION 2" still has anomalous edge cases detected when it runs some FTriangles.
	// Use caution, and stick to "OPTION 1" above unless testing this explicitly.

	// ecbPolyRef->materialID, ecbPolyRef->isPolyPerfectlyClamped, ecbPolyRef->emptyNormal

	originalPoints[0] = ecbPolyRef->lineMap[0].pointA;
	originalPoints[1] = ecbPolyRef->lineMap[1].pointA;
	originalPoints[2] = ecbPolyRef->lineMap[2].pointA;

	//std::cout << "Start: FTriangle processing. " << std::endl;
	FTriangle fPoly(ecbPolyRef->lineMap[0].pointA,
		ecbPolyRef->lineMap[1].pointA,
		ecbPolyRef->lineMap[2].pointA,
		FTriangleType::BLUEPRINT,
		ecbPolyRef->emptyNormal,
		ecbPolyRef->polyBoundaryOrientation,
		ecbPolyRef->isPolyPerfectlyClamped,
		ecbPolyRef->materialID);

	fPoly.fracture();

	auto outputsMap = fPoly.fetchOutputContainerRef();
	//std::cout << "Start: FTriangle processing. " << std::endl;
	for (auto& currentContainer : *outputsMap)
	{
		//polyMap.enclavePolyMap[currentContainer.first].epBorderDataMapRef = borderDataMapRef;	// why do we need to do this? WHY?
		polyMap.enclavePolyMap[currentContainer.first].produceEnclaveTrianglesFromFTriangleContainer(&currentContainer.second, currentContainer.first);
		polyMap.allTracedEnclaves.insert(currentContainer.first);
	}
	//std::cout << "End: FTriangle processing. " << std::endl;	

	// ||||||||||||||||||||||||||||||||||||||||||||| END OPTION 2.

}

void OrganicTriangle::debugCheckIfKeyExists(EnclaveKeyDef::EnclaveKey in_key)
{
	auto keyFind = polyMap.enclavePolyMap.find(in_key);
	if (keyFind != polyMap.enclavePolyMap.end())
	{
		std::cout << "! Key was found: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") | In OrganicTriangle for PolyID: " << polyID << std::endl;
	}
}

void OrganicTriangle::checkIfBlockWasTraced(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	//auto findBlock = polyMap.enclavePolyMap[in_enclaveKey].
}

bool OrganicTriangle::checkCurrentPointAgainstSegmentCorrelation(ECBPolyPoint in_currentPoint, PrimarySegmentMeta in_segment)
{
	bool isWithinBounds = false;

	// check against segment's start point:
	if
		(
		(abs(in_segment.startPoint.x - in_currentPoint.x) <= .02f)
			&&
			(abs(in_segment.startPoint.y - in_currentPoint.y) <= .02f)
			&&
			(abs(in_segment.startPoint.z - in_currentPoint.z) <= .02f)
			)
	{
		isWithinBounds = true;
	}

	// check against segment's end point:
	if
		(
		(abs(in_segment.endPoint.x - in_currentPoint.x) <= .02f)
			&&
			(abs(in_segment.endPoint.y - in_currentPoint.y) <= .02f)
			&&
			(abs(in_segment.endPoint.z - in_currentPoint.z) <= .02f)
			)
	{
		isWithinBounds = true;
	}
	return isWithinBounds;
}