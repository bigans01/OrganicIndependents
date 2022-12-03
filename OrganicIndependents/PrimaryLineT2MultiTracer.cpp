#include "stdafx.h"
#include "PrimaryLineT2MultiTracer.h"

PrimaryLineT2MultiTracer::PrimaryLineT2MultiTracer(PrimaryLineT2Array* in_PLT2ArrayRef, 
													ECBBorderLineList* in_blueprintBorderLines, 
													BorderDataMap* in_borderDataMapRef, 
													EnclaveKeyDef::EnclaveKey in_blueprintKey, 
													PerfectClampEnum in_perfectClampFlag)
{
	PLT2ArrayRef = in_PLT2ArrayRef;
	blueprintBorderLinesRef = in_blueprintBorderLines;
	borderDataMapRef = in_borderDataMapRef;
	blueprintKey = in_blueprintKey;
	currentPerfectClampValue = in_perfectClampFlag;
	//calculateFaceMatchCount();
	if
		(
		(blueprintKey.x == 7)
			&&
			(blueprintKey.y == -6)
			&&
			(blueprintKey.z == -1)
			)
	{
		//std::cout << " PLT2 DEBUG SET!" << std::endl;
		//debugFlag = 1;
	}
}

void PrimaryLineT2MultiTracer::calculateFaceMatchCount()
{
	//std::cout << "-----------------------------------------------------!!!!! Attempting calculation of face match count... " << std::endl;
	if
	(
		(currentPerfectClampValue != PerfectClampEnum::NONE)
			//&&
			//(primaryLinkArray.totalNumberOfLinks != 0)
	)
	{
		//std::cout << "Perfect clamp value pass... " << std::endl;
		// resulting bool array
		bool boolArray[3];
		for (int x = 0; x < 3; x++)
		{
			boolArray[x] = false;
		}

		// index of next line array
		int nextLineArray[3];

		int currentMaxIndex = primaryLinkArray.totalNumberOfLinks - 1;	// get the current max index value

		for (int x = 0; x < primaryLinkArray.totalNumberOfLinks; x++)
		{
			if (x != currentMaxIndex)
			{
				nextLineArray[x] = x + 1;
				//static ECBPPOrientationResults GetBlueprintPointOrientation(ECBPolyPoint in_pointToCheck, ECBBorderLineList* in_borderLineList);
				ECBPPOrientationResults beginOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[x].primaryLineData.beginPointRealXYZ, blueprintBorderLinesRef);
				ECBPPOrientationResults endOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[x].primaryLineData.endPointRealXYZ, blueprintBorderLinesRef);
				ECBPPOrientationResults nextBeginOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[nextLineArray[x]].primaryLineData.beginPointRealXYZ, blueprintBorderLinesRef);

				BorderMDFaceList arg1FaceList = IndependentUtils::getFaceList(beginOrientation, borderDataMapRef);
				BorderMDFaceList arg2FaceList = IndependentUtils::getFaceList(endOrientation, borderDataMapRef);
				BorderMDFaceList arg3FaceList = IndependentUtils::getFaceList(nextBeginOrientation, borderDataMapRef);

				//int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(primaryLinkArray.linkArray[x].primaryLineData.
				int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(arg1FaceList, arg2FaceList, arg3FaceList, 0);
				if (hasCommonFaces == 1)
				{
					boolArray[x] = true;
				}

			}

			else if (x == currentMaxIndex)
			{
				nextLineArray[x] = 0;

				ECBPPOrientationResults beginOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[x].primaryLineData.beginPointRealXYZ, blueprintBorderLinesRef);
				ECBPPOrientationResults endOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[x].primaryLineData.endPointRealXYZ, blueprintBorderLinesRef);
				ECBPPOrientationResults nextBeginOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLinkArray.linkArray[nextLineArray[x]].primaryLineData.beginPointRealXYZ, blueprintBorderLinesRef);

				BorderMDFaceList arg1FaceList = IndependentUtils::getFaceList(beginOrientation, borderDataMapRef);
				BorderMDFaceList arg2FaceList = IndependentUtils::getFaceList(endOrientation, borderDataMapRef);
				BorderMDFaceList arg3FaceList = IndependentUtils::getFaceList(nextBeginOrientation, borderDataMapRef);

				//int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(primaryLinkArray.linkArray[x].primaryLineData.
				int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(arg1FaceList, arg2FaceList, arg3FaceList, 0);
				if (hasCommonFaces == 1)
				{
					boolArray[x] = true;
				}
			}
		}

		// loop and check for all true; if they are all true, its perfectly clamped.
		int boolMatchCounter = 0;
		for (int x = 0; x < primaryLinkArray.totalNumberOfLinks; x++)
		{
			if (boolArray[x] == true)
			{
				boolMatchCounter++;
			}
		}

		if (boolMatchCounter == primaryLinkArray.totalNumberOfLinks)	// if the counter of "trues" equals the number of segments, it's perfectly clamped.
		{
			//std::cout << "########### required face match count is now 2. " << std::endl;
			faceMatchCount = 2;
		}
	}
}

void PrimaryLineT2MultiTracer::beginRun()
{
	//std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Beginning MULTI run! " << std::endl;
	int numberOfPrimariesToTrace = primaryLinkArray.totalNumberOfLinks;		// how many times will we have to loop?
	//std::cout << "number of primaries to trace: " << numberOfPrimariesToTrace << std::endl;
	ECBPolyPointPair currentEndAndNextBegin;
	ECBPolyPointPair stopTest;
	for (int x = 0; x < numberOfPrimariesToTrace; x++)
	{
		int isCurrentPrimaryTraceComplete = 0;

		while (isCurrentPrimaryTraceComplete == 0)	// has the current primary line finished spawning all necessary Primary T2 lines?
		{
			currentEndAndNextBegin = primaryLinkArray.getCurrentEndAndNextBeginPoints();	// let's get the points to compare with
			stopTest = primaryLinkArray.getCurrentEndAndNextBeginPoints();
			recentIntendedFaces = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces;
			//std::cout << "TEST, value of current pointB: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
			//std::cout << "stopTest point A is: " << stopTest.pointA.x << ", " << stopTest.pointA.y << ", " << stopTest.pointA.z << std::endl;
			//int stopFlag = checkIfFaceListsMatch(currentPointB, stopTest.pointA);
			int stopFlag = 0;
			if (wasBSet == 1)	// F-007
			{
				stopFlag = checkIfFaceListsMatch(currentPointB, stopTest.pointB);
			}

			if (stopFlag == 1)
			{
				//std::cout << "WHEYYYYYY we got a match. " << std::endl;
				currentTracingMethod = SecondaryLineType::NONSEGMENT;
			}
			//std::cout << "Current line points A/B: " << std::endl;
			//std::cout << "A: " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ.x << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ.y << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ.z << std::endl;
			//std::cout << "B: " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ.x << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ.y << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ.z << std::endl;
			//std::cout << "Current Z slope is: " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int.x << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int.y << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int.z << std::endl;
			//std::cout << "Current End: " << currentEndAndNextBegin.pointA.x << ", " << currentEndAndNextBegin.pointA.y << ", " << currentEndAndNextBegin.pointA.z << std::endl;
			//std::cout << "Next Begin: " << currentEndAndNextBegin.pointB.x << ", " << currentEndAndNextBegin.pointB.y << ", " << currentEndAndNextBegin.pointB.z << std::endl;
			//std::cout << "Intended faces: " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces.x << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces.y << ", " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces.z << std::endl;
			int doFaceListsMatch = checkIfFaceListsMatch(currentEndAndNextBegin.pointA, currentEndAndNextBegin.pointB);

			// SEGMENT mode checks
			if (currentTracingMethod == SecondaryLineType::SEGMENT)
			{
				//std::cout << "Entered SEGMENT mode " << std::endl;
				if      // points are equivalent, do nothing
					(
						checkIfPointsMatch(currentEndAndNextBegin.pointA, currentEndAndNextBegin.pointB) == 1
						)
				{
					//std::cout << "SEGMENT: condition 1 hit. " << std::endl;
					isCurrentPrimaryTraceComplete = 1;
				}
				else if // points are not same, but are on same face, so we will end after this run
					(
						checkIfFaceListsMatch(currentEndAndNextBegin.pointA, currentEndAndNextBegin.pointB) == 1
						)
				{
					//std::cout << "SEGMENT: condition 2 hit. " << std::endl;
					createPT2LineViaSegmentEnd();	// create one new line here
					isCurrentPrimaryTraceComplete = 1;
				}
				else if // points are not the same, and are not on same face, so switch to nonsegment mode
					(
						checkIfFaceListsMatch(currentEndAndNextBegin.pointA, currentEndAndNextBegin.pointB) == 0
						)
				{
					// switch to nonsegment mode to begin trace
					//std::cout << "SEGMENT: condition 3 hit. " << std::endl;
					createFirstOfMultiplePT2Lines();
					currentTracingMethod = SecondaryLineType::NONSEGMENT;
					//std::cout << "Create first of Multiple..." << std::endl;
				}
			}
			else if (currentTracingMethod == SecondaryLineType::NONSEGMENT)
			{
				//std::cout << "Entering NONSEGMENT logic..." << std::endl;
				//std::cout << "!! Stop flag is: " << stopFlag << std::endl;
				if (stopFlag == 1)
				{
					//std::cout << "Entered Nonsegment case 1... " << std::endl;
					if (checkIfPointsMatch(currentPointB, stopTest.pointA) == 0)
					{
						//std::cout << "Nonsegment case 1: points do NOT match! " << std::endl;
						createLastOfMultiplePT2Lines();
					}
					isCurrentPrimaryTraceComplete = 1;
				}
				else if
					(
						checkIfFaceListsMatch(currentEndAndNextBegin.pointB, currentPointB)		// if the begin point of the next line, and the end point of the current T2 line are on same face...
						)
				{
					//std::cout << "Entered Nonsegment case 2... " << std::endl;
					createLastOfMultiplePT2Lines();
					isCurrentPrimaryTraceComplete = 1;
				}
				else
				{
					//std::cout << "Entered Nonsegment case 3... " << std::endl;
					createNextOfMultiplePT2Lines();
				}
			}
			//std::cout << "!!!! End of pass, current point B is: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
		}
		primaryLinkArray.switchToNextLink();
		currentTracingMethod = SecondaryLineType::SEGMENT;		// reset back to segment for next primary line iteration
		//std::cout << "!!!!!!! While loop complete, heading to next line..." << std::endl;
	}
	//primaryLinkArray.switchToNextLink();
	//primaryLinkArray.getCurrentEndAndNextBeginPoints();
	//primaryLinkArray.switchToNextLink();
	//primaryLinkArray.getCurrentEndAndNextBeginPoints();
	//std::cout << "Multi-primary run complete.....>>>" << std::endl;
}

int PrimaryLineT2MultiTracer::checkIfPointsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	int result = 0;		// default is 0
	if
		(
		(in_pointA.x == in_pointB.x)
			&&
			(in_pointA.y == in_pointB.y)
			&&
			(in_pointA.z == in_pointB.z)
			)
	{
		result = 1;
	}
	return result;
}

int PrimaryLineT2MultiTracer::checkIfFaceListsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	int result = 0;
	ECBPPOrientationResults pointAOrientation = IndependentUtils::GetBlueprintPointOrientation(in_pointA, blueprintBorderLinesRef);
	ECBPPOrientationResults pointBOrientation = IndependentUtils::GetBlueprintPointOrientation(in_pointB, blueprintBorderLinesRef);
	BorderMDFaceList pointAFaceList = IndependentUtils::getFaceList(pointAOrientation, borderDataMapRef);
	BorderMDFaceList pointBFaceList = IndependentUtils::getFaceList(pointBOrientation, borderDataMapRef);
	result = IndependentUtils::checkIfFaceListsMatch(pointAFaceList, pointBFaceList, faceMatchCount);
	return result;
}

void PrimaryLineT2MultiTracer::createPT2LineViaSegmentEnd()
{
	//std::cout << "Creating via segment end... (so only one line produced) " << std::endl;
	// see PrimaryLineT2IsolatedTracer.cpp, line 123
	// setup variables for creation of new primary T2 line
	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	int indexOfNextLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].indexOfNextLink;


	// new points
	ECBPolyPoint pointA = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ;		// get first point
	ECBPolyPoint pointB = primaryLinkArray.linkArray[indexOfNextLine].primaryLineData.beginPointRealXYZ;						// get second point
	ECBPolyPoint pointC = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ;		// get third point
	x_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.x_int;		// get x-slope
	y_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.y_int;		// "" y
	z_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int;		// "" z
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(pointA, PT2blueprintLimits);		// 
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(pointB, PT2blueprintLimits);		// 
	PrimaryLineT2 newPT2Line;
	int lineID = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.IDofLine;
	newPT2Line.IDofParentLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.IDofLine;		// set up PT2 line
	newPT2Line.perfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;
	newPT2Line.beginPointRealXYZ = pointA;
	newPT2Line.endPointRealXYZ = pointB;
	newPT2Line.thirdPointRealXYZ = pointC;
	newPT2Line.intendedFaces = IndependentUtils::determineIntendedFaces(pointA, pointB, pointC);
	newPT2Line.beginPointMeta = PT2pointALocation;
	newPT2Line.endPointMeta = PT2pointBLocation;
	newPT2Line.calibrate(pointC);
	newPT2Line.applySlopes(primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces, x_int_slope, y_int_slope, z_int_slope, pointA, pointB, pointC);
	//std::cout << "New line intended faces are: " << newPT2Line.intendedFaces.x << ", " << newPT2Line.intendedFaces.y << ", " << newPT2Line.intendedFaces.z << std::endl;
	PLT2ArrayRef->addNewPrimaryT2Line(newPT2Line);
	// whoa thats new
	//std::cout << "Line ID is: " << lineID << std::endl;

	//ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentBeginPoint, PT2blueprintLimits);		// 
	//ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentTraceResults.resultingEndPoint, PT2blueprintLimits);		// 
}

void PrimaryLineT2MultiTracer::createFirstOfMultiplePT2Lines()
{
	//std::cout << "!!!! Creating first of multiples:: " << std::endl;
	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	if (debugFlag == 1)
	{
		std::cout << "Blueprint borders are: " << std::endl;
		std::cout << "neg X: " << PT2blueprintLimits.negXlimit << std::endl;
		std::cout << "pos X: " << PT2blueprintLimits.posXlimit << std::endl;
		std::cout << "neg Y: " << PT2blueprintLimits.negYlimit << std::endl;
		std::cout << "pos Y: " << PT2blueprintLimits.posYlimit << std::endl;
		std::cout << "neg Z: " << PT2blueprintLimits.negZlimit << std::endl;
		std::cout << "pos Z: " << PT2blueprintLimits.posZlimit << std::endl;
	}

	//std::cout << "Spawning line points are: " << std::endl;
	ECBPolyPoint origPointA = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ;
	ECBPolyPoint origPointB = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ;
	ECBPolyPoint origPointC = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.thirdPointRealXYZ;

	//std::cout << "##############" << std::endl;
	//std::cout << "Blueprint borders are: " << std::endl;
	//std::cout << "neg X: " << PT2blueprintLimits.negXlimit << std::endl;
	//std::cout << "pos X: " << PT2blueprintLimits.posXlimit << std::endl;
	//std::cout << "neg Y: " << PT2blueprintLimits.negYlimit << std::endl;
	//std::cout << "pos Y: " << PT2blueprintLimits.posYlimit << std::endl;
	//std::cout << "neg Z: " << PT2blueprintLimits.negZlimit << std::endl;
	//std::cout << "pos Z: " << PT2blueprintLimits.posZlimit << std::endl;
	//std::cout << "##############" << std::endl;



	//std::cout << "orig. point A: " << origPointA.x << ", " << origPointA.y << ", " << origPointA.z << std::endl;
	//std::cout << "orig. point B: " << origPointB.x << ", " << origPointB.y << ", " << origPointB.z << std::endl;
	//std::cout << "orig. point C: " << origPointC.x << ", " << origPointC.y << ", " << origPointC.z << std::endl;

	currentPointA = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.endPointRealXYZ;	// get the end point of the line (will be the first point in the new triangle)
	ECBPPOrientationResults pointAOrientation = IndependentUtils::GetBlueprintPointOrientation(currentPointA, blueprintBorderLinesRef);	// get the orientation of the end of the primary line
	moveVals = IndependentUtils::retrieveBorderDirection(pointAOrientation, borderDataMapRef);	// get the move vals from the current end point orientation
	//std::cout << "PLT2Multi: inital move vals are: " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
	x_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.x_int;	// get x int slope
	y_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.y_int;	// "" 
	z_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int;	// ""
	currentPerfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;	// current value of the perfect clamp
	ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUse(borderDataMapRef, pointAOrientation, x_int_slope, y_int_slope, z_int_slope, moveVals, currentPerfectClampValue, debugFlag);
	//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(borderDataMapRef, pointAOrientation, x_int_slope, y_int_slope, z_int_slope, moveVals, currentPerfectClampValue, debugFlag, recentIntendedFaces);
	//std::cout << ">>>> calling trace result from first of multiple...." << std::endl;
	PLTracingResult currentTraceResult = getBlueprintTracingResult(interceptToUse);
	//std::cout << "HELLO??!? " << std::endl;
	currentPointB = currentTraceResult.resultingEndPoint;		// the second point in the initial line, is equal to the newly found point
	wasBSet = 1;
	//std::cout << "First new line's point b is: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
	if (debugFlag == 1)
	{
		std::cout << "First new line's point b is: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
	}
	currentPointC = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.beginPointRealXYZ;	// the third and final point in this first line, is equivalent to the first point of the originating primary T1 line.
	//std::cout << "First new line's point b is: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentPointA, PT2blueprintLimits);		// 
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentPointB, PT2blueprintLimits);
	PrimaryLineT2 newPT2Line;
	newPT2Line.IDofParentLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.IDofLine;		// set up PT2 line
	newPT2Line.perfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;
	newPT2Line.beginPointRealXYZ = currentPointA;
	newPT2Line.endPointRealXYZ = currentPointB;
	newPT2Line.thirdPointRealXYZ = currentPointC;
	newPT2Line.intendedFaces = IndependentUtils::determineIntendedFaces(currentPointA, currentPointB, currentPointC);
	newPT2Line.beginPointMeta = PT2pointALocation;
	newPT2Line.endPointMeta = PT2pointBLocation;
	newPT2Line.calibrate(currentPointC);
	newPT2Line.applySlopes(primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces, x_int_slope, y_int_slope, z_int_slope, currentPointA, currentPointB, currentPointC);
	//std::cout << "OKAY WE ARE HERE " << std::endl;
	//std::cout << "#### New line's slopes will be: " << std::endl;
	//std::cout << "X: " << newPT2Line.x_int_slope.x << ", " << newPT2Line.x_int_slope.y << ", " << newPT2Line.x_int_slope.z << std::endl;
	//std::cout << "Y: " << newPT2Line.y_int_slope.x << ", " << newPT2Line.y_int_slope.y << ", " << newPT2Line.y_int_slope.z << std::endl;
	//std::cout << "Z: " << newPT2Line.z_int_slope.x << ", " << newPT2Line.z_int_slope.y << ", " << newPT2Line.z_int_slope.z << std::endl;
	PLT2ArrayRef->addNewPrimaryT2Line(newPT2Line);

	//std::cout << "!!! New PrimaryLine T2 Begin Point: " << newPT2Line.beginPointRealXYZ.x << ", " << newPT2Line.beginPointRealXYZ.y << ", " << newPT2Line.beginPointRealXYZ.z << std::endl;
	//std::cout << "!!! New PrimaryLine T2 End Point: " << newPT2Line.endPointRealXYZ.x << ", " << newPT2Line.endPointRealXYZ.y << ", " << newPT2Line.endPointRealXYZ.z << std::endl;
	//std::cout << "AND WE ARE HERE " << std::endl;

	// make sure to set the intended faces, if a next line is used.
	//recentIntendedFaces = newPT2Line.intendedFaces;
}

void PrimaryLineT2MultiTracer::createNextOfMultiplePT2Lines()
{
	//std::cout << "!!!! Creating next of multiples..." << std::endl;
	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	currentPointC = currentPointA;	// first: load A into C
	currentPointA = currentPointB;	// second: load B into A
	ECBPPOrientationResults pointAOrientation = IndependentUtils::GetBlueprintPointOrientation(currentPointA, blueprintBorderLinesRef);	// get the orientation of the end of the primary line
	moveVals = IndependentUtils::retrieveBorderDirection(pointAOrientation, borderDataMapRef);	// get the move vals from the current end point orientation
	//std::cout << "PLT2Multi: next move vals are: " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
	x_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.x_int;	// get x int slope
	y_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.y_int;	// "" 
	z_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int;	// ""
	currentPerfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;	// current value of the perfect clamp
	//std::cout << "y clamp value is: " << primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.y_clamp_value << std::endl;
	//ECBPolyPoint interceptToUse = OrganicUtils::getAppropriateSlopeToUse(borderDataMapRef, pointAOrientation, x_int_slope, y_int_slope, z_int_slope, moveVals, currentPerfectClampValue, debugFlag);
	ECBPolyPoint interceptToUse = IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(borderDataMapRef, pointAOrientation, x_int_slope, y_int_slope, z_int_slope, moveVals, currentPerfectClampValue, debugFlag, recentIntendedFaces);
	//std::cout << ">>>>> calling getBlueprintTracingResult..." << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;
	PLTracingResult currentTraceResult = getBlueprintTracingResult(interceptToUse);
	currentPointB = currentTraceResult.resultingEndPoint;		// the second point in the initial line, is equal to the newly found point
	wasBSet = 1;
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentPointA, PT2blueprintLimits);		// 
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentPointB, PT2blueprintLimits);
	PrimaryLineT2 newPT2Line;
	newPT2Line.IDofParentLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.IDofLine;		// set up PT2 line
	newPT2Line.perfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;
	newPT2Line.beginPointRealXYZ = currentPointA;
	newPT2Line.endPointRealXYZ = currentPointB;
	newPT2Line.thirdPointRealXYZ = currentPointC;
	newPT2Line.intendedFaces = IndependentUtils::determineIntendedFaces(currentPointA, currentPointB, currentPointC);
	newPT2Line.beginPointMeta = PT2pointALocation;
	newPT2Line.endPointMeta = PT2pointBLocation;
	newPT2Line.calibrate(currentPointC);
	newPT2Line.applySlopes(primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces, x_int_slope, y_int_slope, z_int_slope, currentPointA, currentPointB, currentPointC);
	//std::cout << "New PT2 line intended faces: " << newPT2Line.intendedFaces.x << ", " << newPT2Line.intendedFaces.y << ", " << newPT2Line.intendedFaces.z << std::endl;
	PLT2ArrayRef->addNewPrimaryT2Line(newPT2Line);

	// make sure to set the intended faces, if a next line is used.
	//recentIntendedFaces = newPT2Line.intendedFaces;
}

void PrimaryLineT2MultiTracer::createLastOfMultiplePT2Lines()
{
	//std::cout << "!!!! Creating LAST of Multiples..." << std::endl;
	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	int indexOfNextLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].indexOfNextLink;
	currentPointC = currentPointA;	// load A into C
	currentPointA = currentPointB;	// load B into A
	currentPointB = primaryLinkArray.linkArray[indexOfNextLine].primaryLineData.beginPointRealXYZ;
	wasBSet = 1;
	//std::cout << "!! Point A is: " << currentPointA.x << ", " << currentPointA.y << ", " << currentPointA.z << std::endl;
	//std::cout << "!! Point B is: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
	ECBPPOrientationResults pointAOrientation = IndependentUtils::GetBlueprintPointOrientation(currentPointA, blueprintBorderLinesRef);	// get the orientation of the end of the primary line
	moveVals = IndependentUtils::retrieveBorderDirection(pointAOrientation, borderDataMapRef);	// get the move vals from the current end point orientation
	x_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.x_int;	// get x int slope
	y_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.y_int;	// "" 
	z_int_slope = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.z_int;	// ""
	currentPerfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;	// current value of the perfect clamp
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentPointA, PT2blueprintLimits);		// 
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentPointB, PT2blueprintLimits);
	PrimaryLineT2 newPT2Line;
	newPT2Line.IDofParentLine = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.IDofLine;		// set up PT2 line
	newPT2Line.perfectClampValue = primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.perfectClampValue;
	newPT2Line.beginPointRealXYZ = currentPointA;
	newPT2Line.endPointRealXYZ = currentPointB;
	newPT2Line.thirdPointRealXYZ = currentPointC;
	newPT2Line.intendedFaces = IndependentUtils::determineIntendedFaces(currentPointA, currentPointB, currentPointC);
	newPT2Line.beginPointMeta = PT2pointALocation;
	newPT2Line.endPointMeta = PT2pointBLocation;
	newPT2Line.calibrate(currentPointC);
	newPT2Line.applySlopes(primaryLinkArray.linkArray[primaryLinkArray.currentLinkIndex].primaryLineData.intendedFaces, x_int_slope, y_int_slope, z_int_slope, currentPointA, currentPointB, currentPointC);
	PLT2ArrayRef->addNewPrimaryT2Line(newPT2Line);

	//std::cout << "!!! New PrimaryLine T2 Begin Point: " << newPT2Line.beginPointRealXYZ.x << ", " << newPT2Line.beginPointRealXYZ.y << ", " << newPT2Line.beginPointRealXYZ.z << std::endl;
	//std::cout << "!!! New PrimaryLine T2 End Point: " << newPT2Line.endPointRealXYZ.x << ", " << newPT2Line.endPointRealXYZ.y << ", " << newPT2Line.endPointRealXYZ.z << std::endl;
}

PLTracingResult PrimaryLineT2MultiTracer::getBlueprintTracingResult(ECBPolyPoint in_interceptToUse)
{
	PLTracingResult resultToReturn;
	//std::cout << ">>> MultiTracer call..." << std::endl;
	ECBPolyPoint resultEndpoint = IndependentUtils::getBlueprintTracingEndpointForIsolatedPrimaryT2(currentPointA, in_interceptToUse, blueprintBorderLinesRef, blueprintKey);
	//std::cout << ">>> MultiTracer call end..." << std::endl;
	ECBPPOrientationResults orientationResults = IndependentUtils::GetBlueprintPointOrientation(resultEndpoint, blueprintBorderLinesRef);
	BorderMDFaceList resultFaceList = IndependentUtils::getFaceList(orientationResults, borderDataMapRef);					// get resultant face list by looking up the map with the orientation
	resultToReturn.resultingEndPoint = resultEndpoint;
	resultToReturn.resultingFaceList = resultFaceList;
	resultToReturn.resultingOrientation = orientationResults;
	return resultToReturn;
}

void PrimaryLineT2MultiTracer::getEndpoint()
{
	//  was created to fix compilation bug; please edit at will
	//std::cout << "Retrieving endpoint" << std::endl;
}