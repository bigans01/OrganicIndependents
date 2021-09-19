#include "stdafx.h"
#include "PrimaryLineT1Array.h"
#include "IndependentUtils.h"

PrimaryLineT1Array::PrimaryLineT1Array()
{
	// set linked lines
	linkArray[0].IDofLinkedLine = 2;	// link to the endpoint of line 2
	linkArray[1].IDofLinkedLine = 0;	// " of line 0 
	linkArray[2].IDofLinkedLine = 1;	// " of line 1

	// set next lines
	linkArray[0].IDofNextLine = 1;
	linkArray[1].IDofNextLine = 2;
	linkArray[2].IDofNextLine = 0;
}

void PrimaryLineT1Array::addNewPrimaryLine(PrimaryLineT1 in_Link)
{
	//linkArray[in_Link.IDofLinkedLine].IDofLinkedLine = in_Link.IDofLinkedLine;

	linkArray[in_Link.IDofLine] = in_Link;

	/*
	linkArray[in_Link.IDofLine].beginPointMeta = in_Link.beginPointMeta;
	linkArray[in_Link.IDofLine].endPointMeta = in_Link.endPointMeta;
	linkArray[in_Link.IDofLine].beginPointRealXYZ = in_Link.beginPointRealXYZ;
	linkArray[in_Link.IDofLine].endPointRealXYZ = in_Link.endPointRealXYZ;
	linkArray[in_Link.IDofLine].thirdPointRealXYZ = in_Link.thirdPointRealXYZ;
	linkArray[in_Link.IDofLine].intendedFaces = in_Link.intendedFaces;
	linkArray[in_Link.IDofLine].switchIntended = in_Link.switchIntended;
	linkArray[in_Link.IDofLine].perfectClampValue = in_Link.perfectClampValue;
	linkArray[in_Link.IDofLine].x_int = in_Link.x_int;
	linkArray[in_Link.IDofLine].y_int = in_Link.y_int;
	linkArray[in_Link.IDofLine].z_int = in_Link.z_int;
	linkArray[in_Link.IDofLine].x_clamp_value = in_Link.x_clamp_value;
	linkArray[in_Link.IDofLine].y_clamp_value = in_Link.y_clamp_value;
	linkArray[in_Link.IDofLine].z_clamp_value = in_Link.z_clamp_value;
	linkArray[in_Link.IDofLine].IDofLine = in_Link.IDofLine;
	linkArray[in_Link.IDofLine].isLineActiveInPoly = 1;
	*/

	//std::cout << "Line Data:" << std::endl;
	//std::cout << "ID of line: " << int(in_Link.IDofLine) << std::endl;
	//std::cout << "ID of linked line: " << int(linkArray[in_Link.IDofLine].IDofLinkedLine) << std::endl;
	//std::cout << "Line Active: " << linkArray[in_Link.IDofLinkedLine].isLineActiveInPoly << std::endl;
	//std::cout << "Begin point | Enclave key: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.z << std::endl;
	//std::cout << "            | Block Key: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.z << std::endl;
	//std::cout << "            | Precise coord: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "End point   | Enclave key: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.enclaveKey.z << std::endl;
	//std::cout << "            | Block Key: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.blockKey.z << std::endl;
	//std::cout << "            | Precise coord: " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.x << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.y << ", " << linkArray[in_Link.IDofLinkedLine].endPointMeta.preciseCoord.z << std::endl;
	//std::cout << "Primary line x-int: " << linkArray[in_Link.IDofLine].x_int.x << ", " << linkArray[in_Link.IDofLine].x_int.y << ", " << linkArray[in_Link.IDofLine].x_int.z << ", " << std::endl;
}

int PrimaryLineT1Array::checkForTrianglePoints(ECBTrianglePointArray* in_trianglePointArrayPtr)
{
	/*
	Note on detecting corner poitns: the ID of the element in the link array, is the same as the ID of the triangle point that we are analyzing.
	For instance, the beginning point of line 0 (the first element in linkArray) is point 0; line 2's endpoint is equal to point 0.

	*/
	int pointCount = 0;
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "checking points...(line " << x << ") " << std::endl;
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			//std::cout << "Triangle corner detected!!!" << std::endl;
			//std::cout << "Line ID: " << x << std::endl;
			int linkedLineID = linkArray[x].IDofLinkedLine;	// get ID of linkedLine
			//std::cout << "Begin point data---- " << std::endl; 
			//std::cout << "Precise coord: " << linkArray[x].beginPointMeta.preciseCoord.x << ", " << linkArray[x].beginPointMeta.preciseCoord.y << ", " << linkArray[x].beginPointMeta.preciseCoord.z << std::endl;
			//std::cout << "Block coord: " << linkArray[x].beginPointMeta.blockKey.x << ", " << linkArray[x].beginPointMeta.blockKey.y << ", " << linkArray[x].beginPointMeta.blockKey.z << std::endl;
			//std::cout << "Chunk coord: " << linkArray[x].beginPointMeta.enclaveKey.x << ", " << linkArray[x].beginPointMeta.enclaveKey.y << ", " << linkArray[x].beginPointMeta.enclaveKey.z << std::endl;

			//std::cout << "End point data---- " << std::endl;
			//std::cout << "Precise coord: " << linkArray[linkedLineID].endPointMeta.preciseCoord.x << ", " << linkArray[linkedLineID].endPointMeta.preciseCoord.y << ", " << linkArray[linkedLineID].endPointMeta.preciseCoord.z << std::endl;
			//std::cout << "Block coord: " << linkArray[linkedLineID].endPointMeta.blockKey.x << ", " << linkArray[linkedLineID].endPointMeta.blockKey.y << ", " << linkArray[linkedLineID].endPointMeta.blockKey.z << std::endl;
			//std::cout << "Chunk coord: " << linkArray[linkedLineID].endPointMeta.enclaveKey.x << ", " << linkArray[linkedLineID].endPointMeta.enclaveKey.y << ", " << linkArray[linkedLineID].endPointMeta.enclaveKey.z << std::endl;

			if (linkArray[linkedLineID].isLineActiveInPoly == 1)	// if its active, do a comparison
			{
				// std::cout << "line " << x << " is active" << std::endl;
				if (
					linkArray[x].beginPointMeta.preciseCoord.x == linkArray[linkedLineID].endPointMeta.preciseCoord.x
					&&
					linkArray[x].beginPointMeta.preciseCoord.y == linkArray[linkedLineID].endPointMeta.preciseCoord.y
					&&
					linkArray[x].beginPointMeta.preciseCoord.z == linkArray[linkedLineID].endPointMeta.preciseCoord.z
					&&
					linkArray[x].beginPointMeta.blockKey.x == linkArray[linkedLineID].endPointMeta.blockKey.x
					&&
					linkArray[x].beginPointMeta.blockKey.y == linkArray[linkedLineID].endPointMeta.blockKey.y
					&&
					linkArray[x].beginPointMeta.blockKey.z == linkArray[linkedLineID].endPointMeta.blockKey.z
					&&
					linkArray[x].beginPointMeta.enclaveKey.x == linkArray[linkedLineID].endPointMeta.enclaveKey.x
					&&
					linkArray[x].beginPointMeta.enclaveKey.y == linkArray[linkedLineID].endPointMeta.enclaveKey.y
					&&
					linkArray[x].beginPointMeta.enclaveKey.z == linkArray[linkedLineID].endPointMeta.enclaveKey.z
					)
				{
					// std::cout << "(Match) Triangle corner detected!!!!! (point " << x << ") " << std::endl;
					in_trianglePointArrayPtr->pointArray[x].isPointActive = 1;
					in_trianglePointArrayPtr->pointArray[x].pointLocationArray.blockKey = linkArray[x].endPointMeta.blockKey;
					in_trianglePointArrayPtr->pointArray[x].pointLocationArray.enclaveKey = linkArray[x].endPointMeta.enclaveKey;
					pointCount++;
				}
			}
		}
	}
	return pointCount;
}

void PrimaryLineT1Array::adjustForEnclavePerfectClamping(short in_perfectClampValue)
{
	//std::cout << "!!! Attempting adjust for perfect clamping... " << std::endl;
	switch (in_perfectClampValue)
	{
		// perfect clamp on X
		case 1: 
		{
			if (linkArray[0].beginPointMeta.enclaveKey.x == 1)				// if the enclaveKey y is 1, it means that the enclave keys have been "pushed" into the next enclave via Y. 
			{
				//std::cout << "!! X adjustment qualification met. " << std::endl;
				for (int x = 0; x < 3; x++)
				{
					linkArray[x].beginPointMeta.enclaveKey.x = 0;
					linkArray[x].endPointMeta.enclaveKey.x = 0;

					linkArray[x].beginPointMeta.blockKey.x = 3;
					linkArray[x].endPointMeta.blockKey.x = 3;

					linkArray[x].beginPointMeta.preciseCoord.x = 1.0f;
					linkArray[x].endPointMeta.preciseCoord.x = 1.0f;
				}
			}
			break;
		}

		// perfect clamp on Y
		case 2:
		{
			if (linkArray[0].beginPointMeta.enclaveKey.y == 1)				// if the enclaveKey y is 1, it means that the enclave keys have been "pushed" into the next enclave via Y. 
			{
				//std::cout << "!! Y adjustment qualification met. " << std::endl;
				for (int x = 0; x < 3; x++)
				{
					linkArray[x].beginPointMeta.enclaveKey.y = 0;
					linkArray[x].endPointMeta.enclaveKey.y = 0;

					linkArray[x].beginPointMeta.blockKey.y = 3;
					linkArray[x].endPointMeta.blockKey.y = 3;

					linkArray[x].beginPointMeta.preciseCoord.y = 1.0f;
					linkArray[x].endPointMeta.preciseCoord.y = 1.0f;
				}
			}
			break;
		}

		// perfect clamp on Z
		case 3:
		{
			if (linkArray[0].beginPointMeta.enclaveKey.z == 1)				// if the enclaveKey y is 1, it means that the enclave keys have been "pushed" into the next enclave via Y. 
			{
				//std::cout << "!! Z adjustment qualification met. " << std::endl;
				for (int x = 0; x < 3; x++)
				{
					linkArray[x].beginPointMeta.enclaveKey.z = 0;
					linkArray[x].endPointMeta.enclaveKey.z = 0;

					linkArray[x].beginPointMeta.blockKey.z = 3;
					linkArray[x].endPointMeta.blockKey.z = 3;

					linkArray[x].beginPointMeta.preciseCoord.z = 1.0f;
					linkArray[x].endPointMeta.preciseCoord.z = 1.0f;
				}
			}
			break;
		}
	}
}

void PrimaryLineT1Array::produceT2Lines(EnclaveKeyDef::EnclaveKey in_key, int in_perfectClampFlag, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef, BorderDataMap* in_borderDataMapRef)
{
	borderDataMapRef = in_borderDataMapRef;	// set the border data map

	// do analysis for primary T1 line 0
	//std::cout << "!!!! Producing T2 lines..." << std::endl;
	int numberOfActiveLines = 0;		// keeps track of number of active lines:
										// --1 active line will be a standard trace (one line's endpoint goes until it hits its  begin point's plane)
										// --2 or more lines will be a multi-line trace (one line's end point goes until it hits the same plane of another line's begin point)
	if (linkArray[0].isLineActiveInPoly == 1)
	{
		//std::cout << "T2 primary production: line 0 is active...analyzing..." << std::endl;
		//std::cout << "Beginpoint is: " << linkArray[0].beginPointRealXYZ.x << ", " << linkArray[0].beginPointRealXYZ.y << ", " << linkArray[0].beginPointRealXYZ.z << ", " << std::endl;
		//std::cout << "Endpoint is: " << linkArray[0].endPointRealXYZ.x << ", " << linkArray[0].endPointRealXYZ.y << ", " << linkArray[0].endPointRealXYZ.z << ", " << std::endl;
		int noOfT1PointsCheck = checkIfLineHasTwoPrimaryT1Points(0);
		if (noOfT1PointsCheck == 1) // a value of 1 indicates that it does have two primary t1 points
		{
			//std::cout << "Line 0 has two primary t1 points! " << std::endl;
			linkArray[0].doesLineHaveTwoPrimaryT1Points = 1;
		}
		numberOfActiveLines++;
	}

	// " " T1 line 1
	if (linkArray[1].isLineActiveInPoly == 1)
	{
		//std::cout << "T2 primary production: line 1 is active...analyzing..." << std::endl;
		//std::cout << "Beginpoint is: " << linkArray[1].beginPointRealXYZ.x << ", " << linkArray[1].beginPointRealXYZ.y << ", " << linkArray[1].beginPointRealXYZ.z << ", " << std::endl;
		//std::cout << "Endpoint is: " << linkArray[1].endPointRealXYZ.x << ", " << linkArray[1].endPointRealXYZ.y << ", " << linkArray[1].endPointRealXYZ.z << ", " << std::endl;
		int noOfT1PointsCheck = checkIfLineHasTwoPrimaryT1Points(1);
		if (noOfT1PointsCheck == 1) // a value of 1 indicates that it does have two primary t1 points
		{
			//std::cout << "Line 1 has two primary t1 points! " << std::endl;
			linkArray[1].doesLineHaveTwoPrimaryT1Points = 1;
		}
		numberOfActiveLines++;
	}

	// " " T1 line 2
	if (linkArray[2].isLineActiveInPoly == 1)
	{
		//std::cout << "T2 primary production: line 2 is active...analyzing..." << std::endl;
		//std::cout << "Beginpoint is: " << linkArray[2].beginPointRealXYZ.x << ", " << linkArray[2].beginPointRealXYZ.y << ", " << linkArray[2].beginPointRealXYZ.z << ", " << std::endl;
		//std::cout << "Endpoint is: " << linkArray[2].endPointRealXYZ.x << ", " << linkArray[2].endPointRealXYZ.y << ", " << linkArray[2].endPointRealXYZ.z << ", " << std::endl;
		int noOfT1PointsCheck = checkIfLineHasTwoPrimaryT1Points(2);
		if (noOfT1PointsCheck == 1) // a value of 1 indicates that it does have two primary t1 points
		{
			//std::cout << "Line 2 has two primary t1 points! " << std::endl;
			linkArray[2].doesLineHaveTwoPrimaryT1Points = 1;
		}
		numberOfActiveLines++;
	}

	if (numberOfActiveLines == 1)	// standard line trace
	{
		runT2LinesStandard(in_key, in_polyLineSlopesPtr, in_blueprintBorderLines, in_primaryLineT2ArrayRef);
	}
	else if (numberOfActiveLines > 1) // multi-line trace
	{
		//std::cout << "!!!! Performing multi line T2 trace..." << std::endl;
		runT2LinesMulti(in_key, in_polyLineSlopesPtr, in_blueprintBorderLines, in_primaryLineT2ArrayRef, in_perfectClampFlag);
	}

	//std::cout << "Produce t2 lines completed..." << std::endl;
}

void PrimaryLineT1Array::runT2LinesStandard(EnclaveKeyDef::EnclaveKey in_key, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef)
{
	//std::cout << "++++++++++STANDARD LINE CALL, FOR T2 LINES" << std::endl;
	//ECBBorderLineList pointABorderLineList;
	//pointABorderLineList = IndependentUtils::determineBorderLines(in_key);
	ECBPolyPoint primaryLineT1PointA, primaryLineT1PointB, primaryLineT1PointC;
	PrimaryLineT1* primaryLineRef = NULL;
	int lineID = 0;					// will be changed to appropriate value
	int clampValueToCheck;			// the clamp value to pass during the creation of the T2 line

	ECBPolyLineSlopesArray testArray;

	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)	// find the active line; there will only be 1
		{
			primaryLineRef = &linkArray[x];
			lineID = x;
			//primaryLineT1PointA = IndependentUtils::roundPolyPointToHundredths(linkArray[x].beginPointRealXYZ);	// get the rounded version
			//primaryLineT1PointB = IndependentUtils::roundPolyPointToHundredths(linkArray[x].endPointRealXYZ);	// "
			//primaryLineT1PointC = IndependentUtils::roundPolyPointToHundredths(linkArray[x].thirdPointRealXYZ);	// "

			//std::cout << "## (Line " << lineID << ") Current intended faces: " << linkArray[x].intendedFaces.x << ", " << linkArray[x].intendedFaces.y << ", " << linkArray[x].intendedFaces.z << std::endl;

			primaryLineT1PointA = linkArray[x].beginPointRealXYZ;
			primaryLineT1PointB = linkArray[x].endPointRealXYZ;
			primaryLineT1PointC = linkArray[x].thirdPointRealXYZ;
			//std::cout << "~~~~ Values of passed in slopes for this line: " << std::endl;
			//std::cout << in_polyLineSlopesPtr->slopesArray[x].x_intercept_slope.x << ", " << in_polyLineSlopesPtr->slopesArray[x].x_intercept_slope.y << ", " << in_polyLineSlopesPtr->slopesArray[x].x_intercept_slope.z << std::endl;
			//std::cout << in_polyLineSlopesPtr->slopesArray[x].y_intercept_slope.x << ", " << in_polyLineSlopesPtr->slopesArray[x].y_intercept_slope.y << ", " << in_polyLineSlopesPtr->slopesArray[x].y_intercept_slope.z << std::endl;
			//std::cout << in_polyLineSlopesPtr->slopesArray[x].z_intercept_slope.x << ", " << in_polyLineSlopesPtr->slopesArray[x].z_intercept_slope.y << ", " << in_polyLineSlopesPtr->slopesArray[x].z_intercept_slope.z << std::endl;
			//std::cout << "~~~~ Values in correct array: " << std::endl;
			//std::cout << linkArray[x].x_int.x << ", " << linkArray[x].x_int.y << ", " << linkArray[x].x_int.z << std::endl;
			//std::cout << linkArray[x].y_int.x << ", " << linkArray[x].y_int.y << ", " << linkArray[x].y_int.z << std::endl;
			//std::cout << linkArray[x].z_int.x << ", " << linkArray[x].z_int.y << ", " << linkArray[x].z_int.z << std::endl;

			testArray.slopesArray[x].x_intercept_slope = linkArray[x].x_int;
			testArray.slopesArray[x].y_intercept_slope = linkArray[x].y_int;
			testArray.slopesArray[x].z_intercept_slope = linkArray[x].z_int;



			clampValueToCheck = linkArray[x].perfectClampValue;
			if (clampValueToCheck == 1)
			{
				//std::cout << "::::produced secondary line will be clamped to x" << std::endl;
			}
			else if (clampValueToCheck == 2)
			{
				//std::cout << "::::produced secondary line will be clamped to y" << std::endl;
			}
			else if (clampValueToCheck == 3)
			{
				//std::cout << "::::produced secondary line will be clamped to z" << std::endl;
			}
		}
	}
	ECBPPOrientationResults primaryLineT1PointAOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLineT1PointA, in_blueprintBorderLines);
	ECBPPOrientationResults primaryLineT1PointBOrientation = IndependentUtils::GetBlueprintPointOrientation(primaryLineT1PointB, in_blueprintBorderLines);
	//ECBPolyPoint intendedFaces = IndependentUtils::determineIntendedFaces(primaryLineRef->beginPointRealXYZ, primaryLineRef->endPointRealXYZ, primaryLineRef->thirdPointRealXYZ);
	ECBPolyPoint intendedFaces = primaryLineRef->intendedFaces;
	//ECBPolyPoint intendedFaces = IndependentUtils::determineIntendedFacesV2(primaryLineRef->beginPointRealXYZ, primaryLineRef->endPointRealXYZ, primaryLineRef->thirdPointRealXYZ, primaryLineRef->x_int, primaryLineRef->y_int, primaryLineRef->z_int);
	PrimaryLineT2IsolatedTracer primaryLineT2Tracer(
		lineID,
		in_key,
		primaryLineT1PointAOrientation,
		primaryLineT1PointBOrientation,
		primaryLineT1PointA,
		primaryLineT1PointB,
		primaryLineT1PointC,
		in_blueprintBorderLines,
		//in_polyLineSlopesPtr,
		&testArray,
		borderDataMapRef,
		intendedFaces,
		in_primaryLineT2ArrayRef,
		primaryLineRef->perfectClampValue,
		primaryLineRef->x_clamp_value,
		primaryLineRef->y_clamp_value,
		primaryLineRef->z_clamp_value
	);

	// degenerate run check
	if (primaryLineT2Tracer.isRunValid == false)
	{
		areLinesValid = false;
	}
	//std::cout << "::::: Post secondary line complete. (4) " << std::endl;
}

void PrimaryLineT1Array::runT2LinesMulti(EnclaveKeyDef::EnclaveKey in_key, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef, int in_perfectClampFlag)
{
	//std::cout << "++++++++++++++MULTI LINE CALL" << std::endl;
	ECBBorderLineList pointABorderLineList;
	pointABorderLineList = IndependentUtils::determineBorderLines(in_key);
	PrimaryLineT2MultiTracer multiTracer(in_primaryLineT2ArrayRef, in_blueprintBorderLines, borderDataMapRef, in_key, in_perfectClampFlag);
	for (int x = 0; x < 3; x++)		// add each active line to the multiTracer
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			//std::cout << "Line active in poly! " << std::endl;
			multiTracer.primaryLinkArray.insertNewLink(linkArray[x]);	// insert a new primary link into the tracer
		}
	}
	multiTracer.primaryLinkArray.setNextLinks();	// when we are all done, set the next links
	multiTracer.calculateFaceMatchCount();			// call ONLY when the segments have been loaded.
	//std::cout << "Preparing to begin run!!! " << std::endl;
	multiTracer.beginRun();
	//std::cout << "Begin run call completed..." << std::endl;
}

int PrimaryLineT1Array::checkIfLineHasTwoPrimaryT1Points(int in_lineID)
{
	// determine which points to check
	int beginPointLine = -1;	// the line that contains the begin point to be checked; if left at -1, it would indicate the line to compare isn't active
	int endPointLine = -1;	// the line that contains the end point to be checked;	""
	int result = 0;
	if (in_lineID == 0)
	{
		//std::cout << "line 0 entry" << std::endl;
		if (linkArray[1].isLineActiveInPoly == 1)	// do this only if line 1 is active
		{
			beginPointLine = 1;
		}
		if (linkArray[2].isLineActiveInPoly == 1)	// " " line 2 is active
		{
			endPointLine = 2;
		}
	}
	else if (in_lineID == 1)
	{
		//std::cout << "line 1 entry" << std::endl;
		if (linkArray[2].isLineActiveInPoly == 1)	// do this only if line 2 is active 
		{
			beginPointLine = 2;
		}
		if (linkArray[0].isLineActiveInPoly == 1)	// " " line 0 is active
		{
			endPointLine = 0;
		}
	}
	else if (in_lineID == 2)
	{
		//std::cout << "line 2 entry" << std::endl;
		if (linkArray[0].isLineActiveInPoly == 1)
		{
			//std::cout << "test: line 0 is active (compared to line 2)" << std::endl;
			beginPointLine = 0;
		}
		if (linkArray[1].isLineActiveInPoly == 1)
		{
			//std::cout << "test: line 1 is active (compared to line 2)" << std::endl;
			endPointLine = 1;
		}
	}

	if ((beginPointLine != -1) && (endPointLine != -1))		// only do this if there are actually other lines to compare to
	{
		//std::cout << "||||| other  lines active, comparing..." << std::endl;
		result = compareLineEndpointsForCommonT1Points(in_lineID, &linkArray[beginPointLine].beginPointRealXYZ, &linkArray[endPointLine].endPointRealXYZ);
	}
	return result;
}

int PrimaryLineT1Array::compareLineEndpointsForCommonT1Points(int in_lineID, ECBPolyPoint* in_beginPolyPointARef, ECBPolyPoint* in_endPolyPointBRef)
{
	int doesLineHaveTwoSharedPoints = 0;	// default is 0 (does not contain two shared points)


	ECBPolyPoint* lineEndpoint = &linkArray[in_lineID].endPointRealXYZ;	// set the endpoint to compare to
	ECBPolyPoint* pointToCompareToEndpoint = in_beginPolyPointARef;	// set the point that will be compared against the endpoint
	int isEndPointShared = checkIfPointsMatch(lineEndpoint, pointToCompareToEndpoint);		// compare the points

	ECBPolyPoint* lineBeginpoint = &linkArray[in_lineID].beginPointRealXYZ;
	ECBPolyPoint* pointToCompareToBeginpoint = in_endPolyPointBRef;
	int isBeginPointShared = checkIfPointsMatch(lineBeginpoint, pointToCompareToBeginpoint);

	if ((isEndPointShared == 1) && (isBeginPointShared == 1))
	{
		//std::cout << "Line contains two shared primary T1 points! " << std::endl;
		doesLineHaveTwoSharedPoints = 1;
	}

	return doesLineHaveTwoSharedPoints;
}

int PrimaryLineT1Array::checkIfPointsMatch(ECBPolyPoint* in_pointA, ECBPolyPoint* in_pointB)
{
	int isShared = 0;	// 0 by default
	//std::cout << "Point A: " << in_pointA->x << ", " << in_pointA->y << ", " << in_pointA->z << std::endl;
	//std::cout << "Point B: " << in_pointB->x << ", " << in_pointB->y << ", " << in_pointB->z << std::endl;
	if (
		(in_pointA->x == in_pointB->x)
		&&
		(in_pointA->y == in_pointB->y)
		&&
		(in_pointA->z == in_pointB->z)
		)
	{
		//std::cout << "Point match detected." << std::endl;
		isShared = 1;
	}

	return isShared;
}

void PrimaryLineT1Array::swapLinesForBlueprintTracing()
{
	struct LineTracker		// will track which lines are active
	{
		int lineIDArray[3];
		int lineIndex = 0;
	};
	LineTracker lineTracker;

	// check which lines are active, store the number
	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			lineTracker.lineIDArray[lineTracker.lineIndex++] = x;
			//std::cout << "LINE " << x << " IS ACTIVE! " << std::endl;
		}
	}

	// perform swaps
	int totalLines = lineTracker.lineIndex;
	if (totalLines == 1) // For 1 line
	{
		//std::cout << ">> SWAP CASE 1" << std::endl;
		//reverseLinePointsAndIntendedFace(0, in_borderLinesRef);
		reverseLinePointsAndIntendedFaceForBlueprintTracing(lineTracker.lineIDArray[0]);
	}
	else if (totalLines == 2)		// For 2 lines
	{
		//std::cout << ">> SWAP CASE 2" << std::endl;
		PrimaryLineT1 tempLine1 = linkArray[lineTracker.lineIDArray[0]];	// get whatever line number is stored at index 0
		PrimaryLineT1 tempLine2 = linkArray[lineTracker.lineIDArray[1]];	// " index 1

		// perform swap
		linkArray[lineTracker.lineIDArray[0]] = tempLine2;
		linkArray[lineTracker.lineIDArray[1]] = tempLine1;

		// reverse each line
		int numberOfLines = lineTracker.lineIndex;
		for (int x = 0; x < numberOfLines; x++)
		{
			reverseLinePointsAndIntendedFaceForBlueprintTracing(lineTracker.lineIDArray[x]);
		}
	}
	else if (totalLines == 3)		// For 3 lines
	{
		//std::cout << ">> SWAP CASE 3" << std::endl;
		PrimaryLineT1 tempLine1 = linkArray[lineTracker.lineIDArray[0]];	// get whatever line number is stored at index 0
		PrimaryLineT1 tempLine2 = linkArray[lineTracker.lineIDArray[2]];	// " index 1

		// perform swap of first and last lines
		linkArray[lineTracker.lineIDArray[0]] = tempLine2;
		linkArray[lineTracker.lineIDArray[2]] = tempLine1;

		// reverse each line
		int numberOfLines = lineTracker.lineIndex;
		for (int x = 0; x < numberOfLines; x++)
		{
			reverseLinePointsAndIntendedFaceForBlueprintTracing(lineTracker.lineIDArray[x]);
		}
	}
}

void PrimaryLineT1Array::reverseLinePointsAndIntendedFaceForBlueprintTracing(int in_x)
{
	// 1: swap points A and B
	ECBPolyPoint pointASwap = linkArray[in_x].beginPointRealXYZ;
	ECBPolyPoint pointBSwap = linkArray[in_x].endPointRealXYZ;

	EnclaveKeyDef::EnclaveKey keyASwap = linkArray[in_x].beginPointBlueprintKey;
	EnclaveKeyDef::EnclaveKey keyBSwap = linkArray[in_x].endPointBlueprintKey;

	linkArray[in_x].beginPointRealXYZ = pointBSwap;
	linkArray[in_x].endPointRealXYZ = pointASwap;

	linkArray[in_x].beginPointBlueprintKey = keyBSwap;
	linkArray[in_x].endPointBlueprintKey = keyASwap;

	// 2: get new locations
	//ECBPolyPointLocation pointALocation = IndependentUtils::getPolyPointLocation(linkArray[in_x].beginPointRealXYZ, *in_borderLinesRef);		// get enclave key, block key, precise key for point A; (2nd point passed is for the case when input value is 0)
	//ECBPolyPointLocation pointBLocation = IndependentUtils::getPolyPointLocation(linkArray[in_x].endPointRealXYZ, *in_borderLinesRef);		// "" point B

	// 3: re-calculate intended faces; swap if a pair of intended faces match exactly
	ECBPolyPoint oldIntendedFaces = linkArray[in_x].intendedFaces;
	linkArray[in_x].intendedFaces = IndependentUtils::determineIntendedFaces(linkArray[in_x].beginPointRealXYZ, linkArray[in_x].endPointRealXYZ, linkArray[in_x].thirdPointRealXYZ);
	invertSlopesDuringSwap(in_x, oldIntendedFaces);
	//std::cout << "-------------Re calc'd intended faces: " << linkArray[in_x].intendedFaces.x << ", " << linkArray[in_x].intendedFaces.y << ", " << linkArray[in_x].intendedFaces.z << std::endl;

	// 4: set new endpoint meta
	//linkArray[in_x].beginPointMeta = pointALocation;
	//linkArray[in_x].endPointMeta = pointBLocation;

	// 5: perform calibration
	//std::cout << "Points, pre-calibrate: " << std::endl;
	//std::cout << "A: (" << linkArray[in_x].beginPointRealXYZ.x << ", " << linkArray[in_x].beginPointRealXYZ.y << ", " << linkArray[in_x].beginPointRealXYZ.z << ") " << std::endl;
	//std::cout << "B: (" << linkArray[in_x].endPointRealXYZ.x << ", " << linkArray[in_x].endPointRealXYZ.y << ", " << linkArray[in_x].endPointRealXYZ.z << ") " << std::endl;
	linkArray[in_x].calibrate(linkArray[in_x].thirdPointRealXYZ);

	if (linkArray[in_x].switchIntended.x == 1.0f)
	{
		//std::cout << "@ performed x-swap..." << std::endl;
		linkArray[in_x].intendedFaces.x *= -1.0f;
	}
	if (linkArray[in_x].switchIntended.y == 1.0f)
	{
		linkArray[in_x].intendedFaces.y *= -1.0f;
	}
	if (linkArray[in_x].switchIntended.z == 1.0f)
	{
		linkArray[in_x].intendedFaces.z *= -1.0f;
	}
}

void PrimaryLineT1Array::swapLines(ECBBorderValues* in_borderLinesRef)
{

	struct LineTracker		// will track which lines are active
	{
		int lineIDArray[3];
		int lineIndex = 0;
	};
	LineTracker lineTracker;

	// check which lines are active, store the number
	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			lineTracker.lineIDArray[lineTracker.lineIndex++] = x;
			//std::cout << "LINE " << x << " IS ACTIVE! " << std::endl;
		}
	}

	// perform swaps
	int totalLines = lineTracker.lineIndex;
	if (totalLines == 1) // For 1 line
	{
		//std::cout << ">> SWAP CASE 1" << std::endl;
		//reverseLinePointsAndIntendedFace(0, in_borderLinesRef);
		reverseLinePointsAndIntendedFace(lineTracker.lineIDArray[0], in_borderLinesRef);
	}
	else if (totalLines == 2)		// For 2 lines
	{
		//std::cout << ">> SWAP CASE 2" << std::endl;
		PrimaryLineT1 tempLine1 = linkArray[lineTracker.lineIDArray[0]];	// get whatever line number is stored at index 0
		PrimaryLineT1 tempLine2 = linkArray[lineTracker.lineIDArray[1]];	// " index 1

		// perform swap
		linkArray[lineTracker.lineIDArray[0]] = tempLine2;
		linkArray[lineTracker.lineIDArray[1]] = tempLine1;

		// reverse each line
		int numberOfLines = lineTracker.lineIndex;
		for (int x = 0; x < numberOfLines; x++)
		{
			reverseLinePointsAndIntendedFace(lineTracker.lineIDArray[x], in_borderLinesRef);
		}
	}
	else if (totalLines == 3)		// For 3 lines
	{
		//std::cout << ">> SWAP CASE 3" << std::endl;
		PrimaryLineT1 tempLine1 = linkArray[lineTracker.lineIDArray[0]];	// get whatever line number is stored at index 0
		PrimaryLineT1 tempLine2 = linkArray[lineTracker.lineIDArray[2]];	// " index 1

		// perform swap of first and last lines
		linkArray[lineTracker.lineIDArray[0]] = tempLine2;
		linkArray[lineTracker.lineIDArray[2]] = tempLine1;

		// reverse each line
		int numberOfLines = lineTracker.lineIndex;
		for (int x = 0; x < numberOfLines; x++)
		{
			reverseLinePointsAndIntendedFace(lineTracker.lineIDArray[x], in_borderLinesRef);
		}
	}

}

void PrimaryLineT1Array::reverseLinePointsAndIntendedFace(int in_x, ECBBorderValues* in_borderLinesRef)
{
	// 1: swap points A and B
	ECBPolyPoint pointASwap = linkArray[in_x].beginPointRealXYZ;
	ECBPolyPoint pointBSwap = linkArray[in_x].endPointRealXYZ;
	linkArray[in_x].beginPointRealXYZ = pointBSwap;
	linkArray[in_x].endPointRealXYZ = pointASwap;

	// 2: get new locations
	ECBPolyPointLocation pointALocation = IndependentUtils::getPolyPointLocation(linkArray[in_x].beginPointRealXYZ, *in_borderLinesRef);		// get enclave key, block key, precise key for point A; (2nd point passed is for the case when input value is 0)
	ECBPolyPointLocation pointBLocation = IndependentUtils::getPolyPointLocation(linkArray[in_x].endPointRealXYZ, *in_borderLinesRef);		// "" point B

	// 3: re-calculate intended faces; swap if a pair of intended faces match exactly
	ECBPolyPoint oldIntendedFaces = linkArray[in_x].intendedFaces;
	linkArray[in_x].intendedFaces = IndependentUtils::determineIntendedFaces(linkArray[in_x].beginPointRealXYZ, linkArray[in_x].endPointRealXYZ, linkArray[in_x].thirdPointRealXYZ);
	invertSlopesDuringSwap(in_x, oldIntendedFaces);
	//std::cout << "-------------Re calc'd intended faces: " << linkArray[in_x].intendedFaces.x << ", " << linkArray[in_x].intendedFaces.y << ", " << linkArray[in_x].intendedFaces.z << std::endl;

	// 4: set new endpoint meta
	linkArray[in_x].beginPointMeta = pointALocation;
	linkArray[in_x].endPointMeta = pointBLocation;

	// 5: perform calibration
	//std::cout << "Points, pre-calibrate: " << std::endl;
	//std::cout << "A: (" << linkArray[in_x].beginPointRealXYZ.x << ", " << linkArray[in_x].beginPointRealXYZ.y << ", " << linkArray[in_x].beginPointRealXYZ.z << ") " << std::endl;
	//std::cout << "B: (" << linkArray[in_x].endPointRealXYZ.x << ", " << linkArray[in_x].endPointRealXYZ.y << ", " << linkArray[in_x].endPointRealXYZ.z << ") " << std::endl;
	linkArray[in_x].calibrate(linkArray[in_x].thirdPointRealXYZ);

	if (linkArray[in_x].switchIntended.x == 1.0f)
	{
		//std::cout << "@ performed x-swap..." << std::endl;
		linkArray[in_x].intendedFaces.x *= -1.0f;
	}
	if (linkArray[in_x].switchIntended.y == 1.0f)
	{
		linkArray[in_x].intendedFaces.y *= -1.0f;
	}
	if (linkArray[in_x].switchIntended.z == 1.0f)
	{
		linkArray[in_x].intendedFaces.z *= -1.0f;
	}
}

void PrimaryLineT1Array::invertSlopesDuringSwap(int in_index, ECBPolyPoint in_oldFaces)
{
	ECBPolyPoint currentIntendedFaces = linkArray[in_index].intendedFaces;
	if (in_oldFaces.x == currentIntendedFaces.x)
	{
		linkArray[in_index].x_int = IndependentUtils::invertSlope(linkArray[in_index].x_int);
	}
	if (in_oldFaces.y == currentIntendedFaces.y)
	{
		linkArray[in_index].y_int = IndependentUtils::invertSlope(linkArray[in_index].y_int);
	}
	if (in_oldFaces.z == currentIntendedFaces.z)
	{
		linkArray[in_index].z_int = IndependentUtils::invertSlope(linkArray[in_index].z_int);
		//std::cout << "!!! NOTICE: z-int had to be inverted for swap!" << std::endl;
	}
}

void PrimaryLineT1Array::printActiveLines()
{
	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			std::cout << "Line " << x << "is active: " << linkArray[x].beginPointRealXYZ.x << ", " << linkArray[x].beginPointRealXYZ.y << ", " << linkArray[x].beginPointRealXYZ.z << " | " << linkArray[x].endPointRealXYZ.x << ", " << linkArray[x].endPointRealXYZ.y << ", " << linkArray[x].endPointRealXYZ.z << std::endl;
		}
	}
}

void PrimaryLineT1Array::printBlueprintTracingLines()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << "|||||| Line " << x << std::endl;
		std::cout << "Point A -> Location (" << linkArray[x].beginPointRealXYZ.x << ",  " << linkArray[x].beginPointRealXYZ.y << ",  " << linkArray[x].beginPointRealXYZ.z << ") | Blueprint Key: (" << linkArray[x].beginPointBlueprintKey.x << ", " << linkArray[x].beginPointBlueprintKey.y << ", " << linkArray[x].beginPointBlueprintKey.z << ") " << std::endl;
		std::cout << "Point B -> Location (" << linkArray[x].endPointRealXYZ.x << ",  " << linkArray[x].endPointRealXYZ.y << ",  " << linkArray[x].endPointRealXYZ.z << ") | Blueprint Key: (" << linkArray[x].endPointBlueprintKey.x << ", " << linkArray[x].endPointBlueprintKey.y << ", " << linkArray[x].endPointBlueprintKey.z << ") " << std::endl;
	}
}

int PrimaryLineT1Array::getNumberOfActiveLines()
{
	int numberOfActives = 0;
	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			numberOfActives++;
		}
	}
	return numberOfActives;
}

void PrimaryLineT1Array::loadPrimaryT1LinesIntoMap(std::map<int, PrimaryLineT1>* in_map)
{
	//in_map->insert(std::pair<int, PrimaryLineT2>(x, type2LineArray[x]));
	for (int x = 0; x < 3; x++)
	{
		if (linkArray[x].isLineActiveInPoly == 1)
		{
			in_map->insert(std::pair<int, PrimaryLineT1>(x, linkArray[x]));
		}
	}
}