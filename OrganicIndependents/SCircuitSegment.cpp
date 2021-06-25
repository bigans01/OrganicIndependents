#include "stdafx.h"
#include "SCircuitSegment.h"

void SCircuitSegment::setAsStandaloneSegment()
{
	type = CircuitSegmentType::STANDALONE;
}

void SCircuitSegment::determineInitialType()
{
	ECBPolyPoint currentSegmentEnd = segment.endPoint;
	//ECBPolyPoint target = targetPoint;

	ECBPPOrientationResults currentSegmentEndOrientation = IndependentUtils::GetPointOrientation(currentSegmentEnd, blockBorderLineListRef);
	ECBPPOrientationResults targetOrientation = IndependentUtils::GetPointOrientation(targetPoint, blockBorderLineListRef);
	BorderMDFaceList currentSegmentEndFaceList = IndependentUtils::getFaceList(currentSegmentEndOrientation, borderDataMapRef);
	BorderMDFaceList targetFaceList = IndependentUtils::getFaceList(targetOrientation, borderDataMapRef);



	// are the points the same
	if (IndependentUtils::checkIfPolyPointsMatch(currentSegmentEnd, targetPoint) == 1)
	{
		type = CircuitSegmentType::CLOSED;	// it is CLOSED case
	}

	// if not, are they on the same face?
	else if (IndependentUtils::checkIfFaceListsMatch(currentSegmentEndFaceList, targetFaceList, faceMatchThreshold) == 1)	// they aren't the same point, but they exist on the same face
	{
		type = CircuitSegmentType::OPEN_SINGLE;	// it is an OPEN_SINGLE case
	}

	// the points are not the same, and they are not on the same face. More than one line will be produced.
	else
	{
		type = CircuitSegmentType::OPEN_MULTI;
	}

}

MicroPolyPointContainer SCircuitSegment::getCircuitPointsV1(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey, std::string in_optionalString)
{
	MicroPolyPointContainer returnContainer;
	enclaveKey = in_enclaveKey;
	blockKey = in_blockKey;
	callingString = in_optionalString;

	//std::cout << "********* calling getCircuitPoints ******************* " << std::endl;

	// standalone type
	if (type == CircuitSegmentType::STANDALONE)
	{
		//std::cout << "Calling STANDALONE..." << std::endl;
		returnContainer = generatePointsForStandalone();
	}

	// closed types
	if (type == CircuitSegmentType::CLOSED_FIRST)
	{
		//std::cout << "Calling CLOSED_FIRST... " << std::endl;
		returnContainer = generatePointsForClosedFirst();
	}
	else if (type == CircuitSegmentType::CLOSED)
	{
		//std::cout << "Calling CLOSED... " << std::endl;
		returnContainer = generatePointsForClosed();
	}
	else if (type == CircuitSegmentType::CLOSED_LAST)
	{
		//std::cout << "Calling CLOSED_LAST... " << std::endl;
		returnContainer = generatePointsForClosedLast();
	}

	// open_single types
	else if (type == CircuitSegmentType::OPEN_SINGLE_FIRST)
	{
		//std::cout << "Calling OPEN_SINGLE_FIRST... " << std::endl;
		returnContainer = generatePointsForSingleFirst();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE)
	{
		//std::cout << "Calling OPEN_SINGLE... " << std::endl;
		returnContainer = generatePointsForSingle();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE_LAST)
	{
		//std::cout << "Calling OPEN_SINGLE_LAST... " << std::endl;
		returnContainer = generatePointsForSingleLast();
	}

	// open_multi types
	else if (type == CircuitSegmentType::OPEN_MULTI_FIRST)
	{
		//std::cout << "Calling OPEN_MULTI_FIRST... " << std::endl;
		returnContainer = generatePointsForMultiFirst();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI)
	{
		//std::cout << "Calling OPEN_MULTI... " << std::endl;
		returnContainer = generatePointsForMulti();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI_LAST)
	{
		//std::cout << "Calling OPEN_MULTI_LAST... " << std::endl;
		returnContainer = generatePointsForMultiLast();
	}

	return returnContainer;
}

MicroPolyPointContainer SCircuitSegment::getCircuitPoints(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, std::string in_optionalString)
{
	MicroPolyPointContainer returnContainer;

	triangle_point0 = in_point0;		// debug only
	triangle_point1 = in_point1;		// ""
	triangle_point2 = in_point2;		// ""
	callingString = in_optionalString;

	enclaveKey = in_enclaveKey;
	blockKey = in_blockKey;

	//std::cout << "********* calling getCircuitPoints ******************* " << std::endl;

	// standalone type
	if (type == CircuitSegmentType::STANDALONE)
	{
		//std::cout << "Calling STANDALONE..." << std::endl;
		returnContainer = generatePointsForStandalone();
	}

	// closed types
	if (type == CircuitSegmentType::CLOSED_FIRST)
	{
		//std::cout << "Calling CLOSED_FIRST... " << std::endl;
		returnContainer = generatePointsForClosedFirst();
	}
	else if (type == CircuitSegmentType::CLOSED)
	{
		//std::cout << "Calling CLOSED... " << std::endl;
		returnContainer = generatePointsForClosed();
	}
	else if (type == CircuitSegmentType::CLOSED_LAST)
	{
		//std::cout << "Calling CLOSED_LAST... " << std::endl;
		returnContainer = generatePointsForClosedLast();
	}

	// open_single types
	else if (type == CircuitSegmentType::OPEN_SINGLE_FIRST)
	{
		//std::cout << "Calling OPEN_SINGLE_FIRST... " << std::endl;
		returnContainer = generatePointsForSingleFirst();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE)
	{
		//std::cout << "Calling OPEN_SINGLE... " << std::endl;
		returnContainer = generatePointsForSingle();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE_LAST)
	{
		//std::cout << "Calling OPEN_SINGLE_LAST... " << std::endl;
		returnContainer = generatePointsForSingleLast();
	}

	// open_multi types
	else if (type == CircuitSegmentType::OPEN_MULTI_FIRST)
	{
		//std::cout << "Calling OPEN_MULTI_FIRST... " << std::endl;
		returnContainer = generatePointsForMultiFirst();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI)
	{
		//std::cout << "Calling OPEN_MULTI... " << std::endl;
		returnContainer = generatePointsForMulti();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI_LAST)
	{
		//std::cout << "Calling OPEN_MULTI_LAST... " << std::endl;
		returnContainer = generatePointsForMultiLast();
	}

	return returnContainer;
}

MicroPolyPointContainer SCircuitSegment::getCircuitPointsDebug(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	MicroPolyPointContainer returnContainer;
	enclaveKey = in_enclaveKey;
	blockKey = in_blockKey;
	debugFlag = 1;

	// standalone type
	if (type == CircuitSegmentType::STANDALONE)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling STANDALONE..." << std::endl;
		}
		returnContainer = generatePointsForStandalone();
	}

	// closed types
	if (type == CircuitSegmentType::CLOSED_FIRST)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling CLOSED_FIRST... " << std::endl;
		}
		returnContainer = generatePointsForClosedFirst();
	}
	else if (type == CircuitSegmentType::CLOSED)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling CLOSED... " << std::endl;
		}
		returnContainer = generatePointsForClosed();
	}
	else if (type == CircuitSegmentType::CLOSED_LAST)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling CLOSED_LAST... " << std::endl;
		}
		returnContainer = generatePointsForClosedLast();
	}

	// open_single types
	else if (type == CircuitSegmentType::OPEN_SINGLE_FIRST)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_SINGLE_FIRST... " << std::endl;
		}
		returnContainer = generatePointsForSingleFirst();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_SINGLE... " << std::endl;
		}
		returnContainer = generatePointsForSingle();
	}
	else if (type == CircuitSegmentType::OPEN_SINGLE_LAST)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_SINGLE_LAST... " << std::endl;
		}
		returnContainer = generatePointsForSingleLast();
	}

	// open_multi types
	else if (type == CircuitSegmentType::OPEN_MULTI_FIRST)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_MULTI_FIRST... " << std::endl;
		}
		returnContainer = generatePointsForMultiFirst();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI)
	{
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_MULTI... " << std::endl;
		}
		returnContainer = generatePointsForMulti();
	}
	else if (type == CircuitSegmentType::OPEN_MULTI_LAST)
	{
		//std::cout << "Calling OPEN_MULTI_LAST... " << std::endl;
		if (debugFlag == 1)
		{
			std::cout << "Calling OPEN_MULTI_LAST... " << std::endl;
		}
		returnContainer = generatePointsForMultiLast();
	}

	return returnContainer;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForStandalone()
{
	MicroPolyPointContainer container;
	targetPoint = segment.startPoint; // make sure to set the target point to the segment's begin point
	//std::cout << "Stand alone target point is: " << targetPoint.x << ", " << targetPoint.y << ", " << targetPoint.z << std::endl;
	ECBPolyPoint currentSegmentBegin = segment.startPoint;
	ECBPolyPoint currentSegmentEnd = segment.endPoint;
	container.insertNewPoint(currentSegmentBegin);
	container.insertNewPoint(currentSegmentEnd);

	MicroPolyPointContainer subContainer = getNewSegments(segment);
	for (int x = 0; x < (subContainer.numberOfPoints - 1); x++)
	{
		container.insertNewPoint(subContainer.pointArray[x]);
	}
	return container;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForClosedFirst()
{
	// should insert: begin point
	//                end point

	MicroPolyPointContainer closedContainer;
	ECBPolyPoint firstPointToAdd = segment.startPoint;
	ECBPolyPoint secondPointToAdd = segment.endPoint;
	closedContainer.insertNewPoint(firstPointToAdd);
	closedContainer.insertNewPoint(secondPointToAdd);
	return closedContainer;
}
MicroPolyPointContainer SCircuitSegment::generatePointsForClosed()
{
	// should insert: end point

	MicroPolyPointContainer closedContainer;
	ECBPolyPoint firstPointToAdd = segment.endPoint;
	closedContainer.insertNewPoint(firstPointToAdd);
	return closedContainer;
}
MicroPolyPointContainer SCircuitSegment::generatePointsForClosedLast()
{
	// contains no points.

	MicroPolyPointContainer closedContainer;
	return closedContainer;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForSingleFirst()
{
	// should insert: begin point
	//                end point
	//                start point of next segment

	MicroPolyPointContainer openSingleContainer;
	ECBPolyPoint p0 = segment.startPoint;
	ECBPolyPoint p1 = segment.endPoint;
	ECBPolyPoint p2 = targetPoint;

	openSingleContainer.insertNewPoint(p0);
	openSingleContainer.insertNewPoint(p1);
	openSingleContainer.insertNewPoint(p2);

	return openSingleContainer;
}
MicroPolyPointContainer SCircuitSegment::generatePointsForSingle()
{
	// should insert: end point
	//                start point of next segment

	MicroPolyPointContainer openSingleContainer;
	ECBPolyPoint p0 = segment.endPoint;
	ECBPolyPoint p1 = targetPoint;

	openSingleContainer.insertNewPoint(p0);
	openSingleContainer.insertNewPoint(p1);

	return openSingleContainer;
}
MicroPolyPointContainer SCircuitSegment::generatePointsForSingleLast()
{
	// should insert: end point only

	MicroPolyPointContainer openSingleContainer;
	ECBPolyPoint p0 = segment.endPoint;

	openSingleContainer.insertNewPoint(p0);

	return openSingleContainer;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForMultiFirst()
{
	// should insert: begin point
	//                end point
	//                end point of all segments

	MicroPolyPointContainer openMultiContainer;

	// insert the first two points (always inserted first)
	ECBPolyPoint currentSegmentBegin = segment.startPoint;
	ECBPolyPoint currentSegmentEnd = segment.endPoint;

	if (debugFlag == 1)
	{
		//std::cout << ">> segment begin: " << segment.startPoint.x << ", " << segment.startPoint.y << ", " << segment.startPoint.z << ", " << std::endl;
		//std::cout << ">> segment end  : " << segment.endPoint.x << ", " << segment.endPoint.y << ", " << segment.endPoint.z << ", " << std::endl;
	}

	openMultiContainer.insertNewPoint(currentSegmentBegin);
	openMultiContainer.insertNewPoint(currentSegmentEnd);


	MicroPolyPointContainer subContainer = getNewSegments(segment);
	for (int x = 0; x < subContainer.numberOfPoints; x++)
	{
		openMultiContainer.insertNewPoint(subContainer.pointArray[x]);
	}

	return openMultiContainer;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForMulti()
{
	// should insert: end point
	//                end point of all segments

	MicroPolyPointContainer openMultiContainer;

	// insert the end point of the current segment (always inserted first)
	ECBPolyPoint currentSegmentEnd = segment.endPoint;
	openMultiContainer.insertNewPoint(currentSegmentEnd);

	MicroPolyPointContainer subContainer = getNewSegments(segment);
	for (int x = 0; x < subContainer.numberOfPoints; x++)
	{
		openMultiContainer.insertNewPoint(subContainer.pointArray[x]);
	}

	return openMultiContainer;
}

MicroPolyPointContainer SCircuitSegment::generatePointsForMultiLast()
{
	// should insert: end point
	//                end point of all segments EXCEPT the last one

	MicroPolyPointContainer openMultiContainer;

	// insert the end point of the current segment (always inserted first)
	ECBPolyPoint currentSegmentEnd = segment.endPoint;
	openMultiContainer.insertNewPoint(currentSegmentEnd);

	MicroPolyPointContainer subContainer = getNewSegments(segment);
	for (int x = 0; x < (subContainer.numberOfPoints - 1); x++)		// remember, only get all points EXCEPt the last one
	{
		openMultiContainer.insertNewPoint(subContainer.pointArray[x]);
	}

	return openMultiContainer;
}

MicroPolyPointContainer SCircuitSegment::getNewSegments(PrimarySegmentMeta in_segmentMeta)
{
	MicroPolyPointContainer returnContainer;
	//std::cout << ">>>>>>>>>>> Printing PrimarySegmentMeta data: " << std::endl;
	//std::cout << "! Intended Faces: " << in_segmentMeta.intendedFaces.x << ", " << in_segmentMeta.intendedFaces.y << ", " << in_segmentMeta.intendedFaces.z << std::endl;

	segmentLogger.log(">>>>>>>>>>> Printing PrimarySegmentMeta data: ", "\n");
	segmentLogger.log("! Intended Faces: ", in_segmentMeta.intendedFaces.x, ", ", in_segmentMeta.intendedFaces.y, ", ", in_segmentMeta.intendedFaces.z, "\n");

	if (debugFlag == 1)
	{
		//std::cout << "!! Intended faces: " << in_segmentMeta.intendedFaces.x << ", " << in_segmentMeta.intendedFaces.y << ", " << in_segmentMeta.intendedFaces.z << std::endl;
	}

	// set up face lists to terminate the while loop
	//ECBPPOrientationResults segmentBeginOrientation = IndependentUtils::GetPointOrientation(segmentBeginPoint, blockBorderLineListRef);
	//ECBPPOrientationResults segmentEndOrientation = IndependentUtils::GetPointOrientation(segmentEndPoint, blockBorderLineListRef);
	//BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(segmentBeginOrientation, borderDataMapRef);
	//BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(segmentEndOrientation, borderDataMapRef);

	//BorderMDFaceList terminatingFaceList = OrganicUtils::getFaceList
	//BorderMDFaceList currentFace = in_segmentMeta.endFaces;

	// set up the orientation variables
	ECBPPOrientationResults currentOrientation = IndependentUtils::GetPointOrientation(in_segmentMeta.endPoint, blockBorderLineListRef);
	ECBPPOrientationResults terminatingOrientation = IndependentUtils::GetPointOrientation(targetPoint, blockBorderLineListRef);

	BorderMDFaceList currentFaceList = in_segmentMeta.endFaces;
	BorderMDFaceList terminatingFaceList = IndependentUtils::getFaceList(terminatingOrientation, borderDataMapRef);


	EnclaveKeyDef::EnclaveKey moveVals = IndependentUtils::retrieveBorderDirection(currentOrientation, borderDataMapRef);		// get the orientation of the beginning end point.


	// TESTING ONLY (begin)
	ECBPolyPoint normalizedSlope = IndependentUtils::findNormalizedSlope(in_segmentMeta.startPoint, in_segmentMeta.endPoint);
	ECBPolyPoint moveValsFromKey;
	moveValsFromKey.x = float(moveVals.x);
	moveValsFromKey.y = float(moveVals.y);
	moveValsFromKey.z = float(moveVals.z);
	//ECBPolyPoint commonVals = OrganicUtils::findCommonMoveValues(moveValsFromKey, normalizedSlope);
	//std::cout << "## CommonVals are: " << commonVals.x << ", " << commonVals.y << ", " << commonVals.z << std::endl;
	// set new vals:
	//moveVals.x = commonVals.x;
	//moveVals.y = commonVals.y;
	//moveVals.z = commonVals.z;
	// TESTING ONLY (end)

	// ||||||||||||||| Logic for the very first segment

	// grab the points
	ECBPolyPoint leadingPoint = in_segmentMeta.endPoint;		// the point that will produce the resultingPoint
	ECBPolyPoint resultingPoint;
	ECBPolyPoint terminatingPoint = targetPoint;

	// set up the slopes
	ECBPolyPoint current_x_slope = in_segmentMeta.x_slope;
	ECBPolyPoint current_y_slope = in_segmentMeta.y_slope;
	ECBPolyPoint current_z_slope = in_segmentMeta.z_slope;

	// get the intercept to use
	ECBPolyPoint interceptToUse;
	int debugFlag2 = 0;
	//std::cout << ">> Move vals" << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
	//std::cout << ">> Intended faces: " << in_segmentMeta.intendedFaces.x << ", " << in_segmentMeta.intendedFaces.y << ", " << in_segmentMeta.intendedFaces.z << std::endl;
	//std::cout << "!! Attempting grab of appropriate slope... " << std::endl;
	//interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag2, in_segmentMeta.intendedFaces);

	interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag2, in_segmentMeta.intendedFaces);
	if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
	{
		MicroPolyPointContainer resetContainer;	// just copy a new one to the old, don't add
		returnContainer = resetContainer;
		returnContainer.isContainerValid = false;	// flag it as being invalid
		std::cout << "Returng container flagged as invalid... " << std::endl;
		return returnContainer;
	}

	PLTracingResult currentTraceResults = IndependentUtils::getBlockTracingResult(leadingPoint, interceptToUse, blockBorderLineListRef, borderDataMapRef, debugFlag2);
	resultingPoint = currentTraceResults.resultingEndPoint;		// set the new endpoint
	ECBPolyPoint initialResultingPoint = resultingPoint;

	int debugCount = 0;
	ECBPolyPoint debugArray[5];

	segmentLogger.log("! Resulting point is: ", resultingPoint.x, ", ", resultingPoint.y, ", ", resultingPoint.z, "\n");

	//std::cout << "!!! Initial intercept to use is: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;

	//std::cout << "!! Terminating point (begin point ) is:     " << terminatingPoint.x << ", " << terminatingPoint.y << ", " << terminatingPoint.z << std::endl;
	//std::cout << "!! Leading point is: " << leadingPoint.x << ", " << leadingPoint.y << ", " << leadingPoint.z << std::endl;
	//std::cout << "!! First resulting point is: " << resultingPoint.x << ", " << resultingPoint.y << ", " << resultingPoint.z << std::endl;


	if (debugFlag == 1)
	{
		//std::cout << "!! First resulting point is: " << resultingPoint.x << ", " << resultingPoint.y << ", " << resultingPoint.z << std::endl;
		//std::cout << "!! Terminating point is:     " << terminatingPoint.x << ", " << terminatingPoint.y << ", " << terminatingPoint.z << std::endl;
	}

	returnContainer.insertNewPoint(resultingPoint);

	// get the new current face value
	currentOrientation = IndependentUtils::GetPointOrientation(resultingPoint, blockBorderLineListRef);
	currentFaceList = IndependentUtils::getFaceList(currentOrientation, borderDataMapRef);

	// do the face lists match?
	if (IndependentUtils::checkIfFaceListsMatch(currentFaceList, terminatingFaceList, faceMatchThreshold) == 0)
	{
		// they do not... continue!!!!
		// ||||||||||||||||||||||||||||||||||||| Logic for subsequent segments
		//std::cout << "!!!! SCircuitSegment: Warning, face lists do not match, continuing! " << std::endl;
		//std::cout << "Point is: " << resultingPoint.x << ", " << resultingPoint.y << ", " << resultingPoint.z << std::endl;

		segmentLogger.log("!!!! SCircuitSegment: Warning, face lists do not match, continuing! ", "\n");
		segmentLogger.log("-----> Enclave: ", enclaveKey.x, ", ", enclaveKey.y, ", ", enclaveKey.z, "\n");
		segmentLogger.log("-----> Block: ", blockKey.x, ", ", blockKey.y, ", ", blockKey.z, "\n");
		segmentLogger.log("Point is: ", resultingPoint.x, ", ", resultingPoint.y, ", ", resultingPoint.z, "\n");
		segmentLogger.log("Face match threshold is: ", faceMatchThreshold, "\n");

		if (segmentLogMode == PolyDebugLevel::DEBUG)
		{
			//std::cout << "!! Debug mode, please enter a number to continue..." << std::endl;
			//int someVal;
			//std::cin >> someVal;
		}

		ECBPolyPoint currentPoint2 = in_segmentMeta.endPoint;		// initial value for point 2 (for normalized slope
		ECBPolyPoint currentPoint3 = resultingPoint;

		int loopCount = 0;
		while (IndependentUtils::checkIfFaceListsMatch(currentFaceList, terminatingFaceList, faceMatchThreshold) == 0)
		{
			//std::cout << "!!!! (Build #2) (while loop) current Point is: " << resultingPoint.x << ", " << resultingPoint.y << ", " << resultingPoint.z << std::endl;
			segmentLogger.log("!!!! (Build #2) (while loop) current Point is: ", resultingPoint.x, ", ", resultingPoint.y, ", ", resultingPoint.z, "\n");
			segmentLogger.log("-----> Enclave: ", enclaveKey.x, ", ", enclaveKey.y, ", ", enclaveKey.z, "\n");
			segmentLogger.log("-----> Block: ", blockKey.x, ", ", blockKey.y, ", ", blockKey.z, "\n");

			if (segmentLogMode == PolyDebugLevel::DEBUG)
			{
				//std::cout << "!! WARNING, STILL NONE!" << std::endl;
				//int someVal;
				//std::cin >> someVal;
			}
			ECBPolyPoint normalizedSlope = IndependentUtils::findNormalizedSlope(currentPoint2, currentPoint3);

			moveVals = IndependentUtils::retrieveBorderDirection(currentOrientation, borderDataMapRef);	// retrieve move vals of appropriate type

			/*
			moveValsFromKey;
			moveValsFromKey.x = moveVals.x;
			moveValsFromKey.y = moveVals.y;
			moveValsFromKey.z = moveVals.z;
			commonVals = OrganicUtils::findCommonMoveValues(moveValsFromKey, normalizedSlope);
			moveVals.x = commonVals.x;
			moveVals.y = commonVals.y;
			moveVals.z = commonVals.z;
			*/



			//OrganicUtils::checkForSecondarySlopeInversion(in_segmentMeta.intendedFaces, moveVals, &current_x_slope, &current_y_slope, &current_z_slope);
			//interceptToUse = OrganicUtils::getAppropriateSlopeToUse(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag, in_segmentMeta.intendedFaces);
			//interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag2, in_segmentMeta.intendedFaces);

			interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag2, in_segmentMeta.intendedFaces);
			if (IndependentUtils::checkIfInterceptIsValid(interceptToUse) == false)
			{
				MicroPolyPointContainer resetContainer;	// just copy a new one to the old, don't add
				returnContainer = resetContainer;
				returnContainer.isContainerValid = false;	// flag it as being invalid

				std::cout << "Returng container flagged as invalid... " << std::endl;
				return returnContainer;
			}


			//interceptToUse = OrganicUtils::getAppropriateSlopeToUseForMultipleTrace(borderDataMapRef, currentOrientation, current_x_slope, current_y_slope, current_z_slope, moveVals, perfectClampValue, debugFlag, normalizedSlope);
			currentTraceResults = IndependentUtils::getBlockTracingResult(resultingPoint, interceptToUse, blockBorderLineListRef, borderDataMapRef, debugFlag2);
			resultingPoint = currentTraceResults.resultingEndPoint;

			debugArray[debugCount++] = resultingPoint;

			returnContainer.insertNewPoint(resultingPoint);
			currentOrientation = IndependentUtils::GetPointOrientation(resultingPoint, blockBorderLineListRef);
			currentFaceList = IndependentUtils::getFaceList(currentOrientation, borderDataMapRef);

			//std::cout << "Move vals used: " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
			//std::cout << "Slope used was: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;

			segmentLogger.log("Move vals used: ", moveVals.x, ", ", moveVals.y, ", ", moveVals.z, "\n");
			segmentLogger.log("Slope used was: ", interceptToUse.x, ", ", interceptToUse.y, ", ", interceptToUse.z, "\n");



			//int debugIn;
			//std::cin >> debugIn;
			loopCount++;
			//std::cout << "Loop count is: " << loopCount << std::endl;
			if (loopCount >= 5)
			{
				//std::cout << "Enclave key this occurs in is: " << 
				std::cout << "################################################################ Face match threshold is: " << faceMatchThreshold << std::endl;

				std::cout << "++ Parent triangle points are: " << std::endl;
				std::cout << "0: " << triangle_point0.x << ", " << triangle_point0.y << ", " << triangle_point0.z << std::endl;
				std::cout << "1: " << triangle_point1.x << ", " << triangle_point1.y << ", " << triangle_point1.z << std::endl;
				std::cout << "2: " << triangle_point2.x << ", " << triangle_point2.y << ", " << triangle_point2.z << std::endl;
				std::cout << "calling string: " << callingString << std::endl;

				std::cout << "Enclave key is: " << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << std::endl;
				std::cout << "Segment begin: " << in_segmentMeta.startPoint.x << ", " << in_segmentMeta.startPoint.y << ", " << in_segmentMeta.startPoint.z << std::endl;
				std::cout << "Segment end  : " << in_segmentMeta.endPoint.x << ", " << in_segmentMeta.endPoint.y << ", " << in_segmentMeta.endPoint.z << std::endl;
				std::cout << "Intended faces are: " << in_segmentMeta.intendedFaces.x << ", " << in_segmentMeta.intendedFaces.y << ", " << in_segmentMeta.intendedFaces.z << std::endl;
				std::cout << "Initial move vals are: " << moveValsFromKey.x << ", " << moveValsFromKey.y << ", " << moveValsFromKey.z << std::endl;
				std::cout << "X-slope: " << current_x_slope.x << ", " << current_x_slope.y << ", " << current_x_slope.z << std::endl;
				std::cout << "Y-slope: " << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
				std::cout << "Z-slope: " << current_z_slope.x << ", " << current_z_slope.y << ", " << current_z_slope.z << std::endl;
				std::cout << "Terminating point: " << terminatingPoint.x << ", " << terminatingPoint.y << ", " << terminatingPoint.z << std::endl;
				std::cout << "First point found: " << initialResultingPoint.x << ", " << initialResultingPoint.y << ", " << initialResultingPoint.z << std::endl;
				for (int x = 0; x < 5; x++)
				{
					std::cout << "Next point found: " << debugArray[x].x << ", " << debugArray[x].y << ", " << debugArray[x].z << std::endl;
				}

				MicroPolyPointContainer resetContainer;	// just copy a new one to the old, don't add
				returnContainer = resetContainer;
				returnContainer.isContainerValid = false;	// flag it as being invalid
				break;

				//int stopVal = 3;
				//std::cin >> stopVal;
			}
		}

		// Insert the target point as the last point
		returnContainer.insertNewPoint(terminatingPoint);

		/*
		if (IndependentUtils::checkIfFaceListsMatch(currentFaceList, terminatingFaceList, 1) == 1)
		{
			std::cout << "````` FAce lists do match! " << std::endl;
		}
		*/
	}
	else if (IndependentUtils::checkIfFaceListsMatch(currentFaceList, terminatingFaceList, faceMatchThreshold) == 1)
	{
		returnContainer.insertNewPoint(terminatingPoint);
	}

	//std::cout << "####### returning MicroPolyPointContainer..." << std::endl;

	return returnContainer;
}

void SCircuitSegment::setDebugMode(PolyDebugLevel in_debugLevel)
{
	segmentLogger.setDebugLevel(in_debugLevel);
	segmentLogMode = in_debugLevel;
}