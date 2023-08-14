#include "stdafx.h"
#include "EnclaveTriangle.h"

int IndependentUtils::checkIfPointsExistOnSameFace(BorderMDFaceList in_faceListA, BorderMDFaceList in_faceListB, BorderMDFaceList in_faceListC, int in_debugFlag)
{
	// we'll check the number of NOVAL's here
	/*
	short list_a_valids = 0;
	short list_b_valids = 0;
	int list_c_valids = 0;
	for (int x = 0; x < 3; x++)
	{
		if (in_faceListA.faceList[x] != ECBPPOrientations::NOVAL)
		{
			list_a_valids++;
		}
		if (in_faceListB.faceList[x] != ECBPPOrientations::NOVAL)
		{
			list_b_valids++;
		}
		if (in_faceListC.faceList[x] != ECBPPOrientations::NOVAL)
		{
			list_c_valids++;
		}
	}
	std::cout << "Valid counts: " << list_a_valids << ", " << list_b_valids << ", " << list_c_valids << std::endl;
	*/
	// we will compare all 3 faces in list A, to all 3 in list B, and all 3 in list C.

	short haveCommonFaces = 0;
	struct MatchTracker
	{
		ECBPPOrientations faceValue = ECBPPOrientations::NOVAL;	// default,  will be replaced later
		short found_in_second = 0;
		short found_in_third = 0;
	};

	if (in_debugFlag == 1)
	{
		std::cout << "Checking faces in List C... " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			if (in_faceListC.faceList[x] == ECBPPOrientations::BOTTOMFACE)
			{
				std::cout << "List C has BOTTOM face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::TOPFACE)
			{
				std::cout << "List C has TOP face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::EASTFACE)
			{
				std::cout << "List C has EAST face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::WESTFACE)
			{
				std::cout << "List C has WEST face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::NORTHFACE)
			{
				std::cout << "List C has NORTH face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::SOUTHFACE)
			{
				std::cout << "List C has SOUTH face " << std::endl;
			}
			else if (in_faceListC.faceList[x] == ECBPPOrientations::NOVAL)
			{
				std::cout << "List C has a NO VAL!!!! " << std::endl;
			}
		}
	}

	// create the struct, load the faces
	MatchTracker matchArray[3];
	matchArray[0].faceValue = in_faceListA.faceList[0];
	matchArray[1].faceValue = in_faceListA.faceList[1];
	matchArray[2].faceValue = in_faceListA.faceList[2];
	for (int x = 0; x < 3; x++)
	{
		ECBPPOrientations valueToFind = matchArray[x].faceValue; // grab the face value to look for
		for (int b = 0; b < 3; b++)
		{
			if ((in_faceListB.faceList[b] == valueToFind) && (valueToFind != ECBPPOrientations::NOVAL))
			{
				matchArray[x].found_in_second = 1;
			}
			if ((in_faceListC.faceList[b] == valueToFind) && (valueToFind != ECBPPOrientations::NOVAL))
			{
				matchArray[x].found_in_third = 1;
			}
		}
		if (in_debugFlag == 1)
		{
			std::cout << "found in second? -> " << matchArray[x].found_in_second << std::endl;
			std::cout << "found in third? -> " << matchArray[x].found_in_third << std::endl;
		}


		// set appropriate flag if they are found in both
		if (
			(matchArray[x].found_in_second == 1)
			&&
			(matchArray[x].found_in_third == 1)
			)
		{
			haveCommonFaces = 1;
			if (in_debugFlag == 1)
			{
				std::cout << "Faces are common!!" << std::endl;
			}
		}
	}
	return haveCommonFaces;
}

ECBPolyPointLocation IndependentUtils::getPolyPointLocation(ECBPolyPoint in_point, ECBBorderValues in_borderValues)
{
	ECBPolyPointLocation locationToReturn;
	//::cout << ">>>>>>>>>>>>>>>>>>>>>>>> Test call begins here: " << std::endl;
	// check borders for X, 
	//std::cout << "value of x (NEW POLYPOINT CALL): " << in_point.x << std::endl;
	if (in_point.x == in_borderValues.posXlimit)		// East limit hit
	{
		//std::cout << "hey there!!! X hit East border..." << std::endl;
		locationToReturn.enclaveKey.x = 7;
		locationToReturn.blockKey.x = 3;
		locationToReturn.preciseCoord.x = 1.0f;

	}
	else if (in_point.x == in_borderValues.negXlimit)	// West limit hit
	{
		//std::cout << "hey there!!! X hit West border..." << std::endl;
		locationToReturn.enclaveKey.x = 0;
		locationToReturn.blockKey.x = 0;
		locationToReturn.preciseCoord.x = 0.0f;
	}
	else
	{
		//std::cout << "|||| (x) point being passed is: " << in_point.x << std::endl;
		CursorPathTraceContainer xContainer = IndependentUtils::getPreciseCoordinate(in_point.x);
		locationToReturn.enclaveKey.x = xContainer.EnclaveCoord;
		locationToReturn.blockKey.x = xContainer.BlockCoord;
		locationToReturn.preciseCoord.x = xContainer.ExactBlockCoord;
		//std::cout << "|||| value of precise coord: " << locationToReturn.preciseCoord.x << std::endl;
		//std::cout << "Chunk and block data for point (" << in_point.x << ", " << in_point.y << ", " << in_point.z << ") " << std::endl;
		//std::cout << "Chunk location for X: " << locationToReturn.enclaveKey.x << std::endl;
		//std::cout << "Block location for X: " << locationToReturn.blockKey.x << std::endl;

	}

	// check borders for Y,
	if (in_point.y == in_borderValues.posYlimit)
	{
		locationToReturn.enclaveKey.y = 7;
		locationToReturn.blockKey.y = 3;
		locationToReturn.preciseCoord.y = 1.0f;
	}
	else if (in_point.y == in_borderValues.negYlimit)
	{
		locationToReturn.enclaveKey.y = 0;
		locationToReturn.blockKey.y = 0;
		locationToReturn.preciseCoord.y = 0.0f;
	}
	else
	{
		//std::cout << "|||| point being passed is: " << in_point.y << std::endl;
		CursorPathTraceContainer yContainer = IndependentUtils::getPreciseCoordinate(in_point.y);
		locationToReturn.enclaveKey.y = yContainer.EnclaveCoord;
		locationToReturn.blockKey.y = yContainer.BlockCoord;
		locationToReturn.preciseCoord.y = yContainer.ExactBlockCoord;
	}

	// check borders for Z,
	if (in_point.z == in_borderValues.posZlimit)
	{
		locationToReturn.enclaveKey.z = 7;
		locationToReturn.blockKey.z = 3;
		locationToReturn.preciseCoord.z = 1.0f;
	}
	else if (in_point.z == in_borderValues.negZlimit)
	{
		locationToReturn.enclaveKey.z = 0;
		locationToReturn.blockKey.z = 0;
		locationToReturn.preciseCoord.z = 0.0f;
	}
	else
	{
		//std::cout << "|||| point being passed is: " << in_point.z << std::endl;
		CursorPathTraceContainer zContainer = IndependentUtils::getPreciseCoordinate(in_point.z);
		locationToReturn.enclaveKey.z = zContainer.EnclaveCoord;
		locationToReturn.blockKey.z = zContainer.BlockCoord;
		locationToReturn.preciseCoord.z = zContainer.ExactBlockCoord;
	}
	//locationToReturn.preciseCoord = in_point;
	//std::cout << "Chunk and block data for point (" << in_point.x << ", " << in_point.y << ", " << in_point.z << ") " << std::endl;
	//std::cout << "Chunk location: " << locationToReturn.enclaveKey.x << ", " << locationToReturn.enclaveKey.y << ", " << locationToReturn.enclaveKey.z << std::endl;
	//std::cout << "Block location: " << locationToReturn.blockKey.x << ", " << locationToReturn.blockKey.y << ", " << locationToReturn.blockKey.z << std::endl;
	//std::cout << "Remainder amount: " << locationToReturn.preciseCoord.x << ", " << locationToReturn.preciseCoord.y << ", " << locationToReturn.preciseCoord.z << std::endl;

	return locationToReturn;
}

ECBCalibratedPointPair IndependentUtils::compareAndCalibrateDistances(ECBPolyPointTri* in_polyPointTriRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_currentLineSlope, EnclaveKeyDef::EnclaveKey in_currentBlueprintKey)
{
	ECBCalibratedPointPair returnPair;
	ECBBorderValues current_border_values = IndependentUtils::getBlueprintLimits(in_currentBlueprintKey);
	//std::cout << "Pre-normalize: " << in_currentLineSlope.x << ", " << in_currentLineSlope.y << ", " << in_currentLineSlope.z << std::endl;
	ECBPolyPoint normalized_slope = IndependentUtils::findNormalizedPoint(in_currentLineSlope);
	//std::cout << "|||||||||||||||||| beginning calibration comparison |||||||||||||||||||||" << std::endl;
	//std::cout << "Border values are: " << std::endl;
	//std::cout << "Normalized slope is: " << normalized_slope.x << ", " << normalized_slope.y << ", " << normalized_slope.z << ", " << std::endl;
	//std::cout << "Pos X: " << current_border_values.posXlimit << std::endl;
	//std::cout << "Neg X: " << current_border_values.negXlimit << std::endl;
	//std::cout << "Pos Y: " << current_border_values.posYlimit << std::endl;
	//std::cout << "Neg Y: " << current_border_values.negYlimit << std::endl;
	//std::cout << "Pos Z: " << current_border_values.posZlimit << std::endl;
	//std::cout << "Neg Z: " << current_border_values.negZlimit << std::endl;

	//std::cout << ">>   In distance values: " << in_distanceValues.x << ", " << in_distanceValues.y << "," << in_distanceValues.z << std::endl;

	int corner_flag = 0;
	int line_flag = 0;
	// first, check to see if it's a corner
	//float rounded_x_distance = roundToHundredth(in_distanceValues.x);
	//float rounded_y_distance = roundToHundredth(in_distanceValues.y);
	//float rounded_z_distance = roundToHundredth(in_distanceValues.z);

	ECBPolyPoint appropriate_distances = roundXYZInterceptDistancesToAppropriatePrecision(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);			// this will FIX the issue on 2/24/2019 (F-003)
	float rounded_x_distance = appropriate_distances.x;
	float rounded_y_distance = appropriate_distances.y;
	float rounded_z_distance = appropriate_distances.z;

	//float rounded_x_distance = roundToThousandths(in_distanceValues.x);
	//float rounded_y_distance = roundToThousandths(in_distanceValues.y);
	//float rounded_z_distance = roundToThousandths(in_distanceValues.z);
	//std::cout << "Rounded x: " << rounded_x_distance << std::endl;
	//std::cout << "Rounded y: " << rounded_y_distance << std::endl;
	//std::cout << "Rounded z: " << rounded_z_distance << std::endl;
	//std::cout << "+++++intercept point values, pre-calibration+++++" << std::endl;
	//std::cout << "X int: " << in_polyPointTriRef->triPoints[0].x << ", " << in_polyPointTriRef->triPoints[0].y << ", " << in_polyPointTriRef->triPoints[0].z << ", " << std::endl;
	//std::cout << "Y int: " << in_polyPointTriRef->triPoints[1].x << ", " << in_polyPointTriRef->triPoints[1].y << ", " << in_polyPointTriRef->triPoints[1].z << ", " << std::endl;
	//std::cout << "Z int: " << in_polyPointTriRef->triPoints[2].x << ", " << in_polyPointTriRef->triPoints[2].y << ", " << in_polyPointTriRef->triPoints[2].z << ", " << std::endl;


	// ::::::::::::::::::::::::::::::::::::::::::::::::::::: Corner match logic
	if (
		(rounded_x_distance == rounded_y_distance)
		&&
		(rounded_y_distance == rounded_z_distance)
		)
	{
		//std::cout << ":::::: corner threshold by passed, moving all points to corner..." << std::endl;
		corner_flag = 1;	// set the corner flag

		//std::cout << "direction of slope is: " << normalized_slope.x << ", " << normalized_slope.y << ", " << normalized_slope.z << std::endl;

		// set values to appropriate corners 
		// Lower SW:
		if ((normalized_slope.x == -1) && (normalized_slope.y == -1) && (normalized_slope.z == 1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.negXlimit;
			calibratedPoint.y = current_border_values.negYlimit;
			calibratedPoint.z = current_border_values.posZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Lower NW
		else if ((normalized_slope.x == -1) && (normalized_slope.y == -1) && (normalized_slope.z == -1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.negXlimit;
			calibratedPoint.y = current_border_values.negYlimit;
			calibratedPoint.z = current_border_values.negZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Lower NE
		else if ((normalized_slope.x == 1) && (normalized_slope.y == -1) && (normalized_slope.z == -1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.posXlimit;
			calibratedPoint.y = current_border_values.negYlimit;
			calibratedPoint.z = current_border_values.negZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Lower SE
		else if ((normalized_slope.x == 1) && (normalized_slope.y == -1) && (normalized_slope.z == 1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.posXlimit;
			calibratedPoint.y = current_border_values.negYlimit;
			calibratedPoint.z = current_border_values.posZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Upper SW
		else if ((normalized_slope.x == -1) && (normalized_slope.y == 1) && (normalized_slope.z == 1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.negXlimit;
			calibratedPoint.y = current_border_values.posYlimit;
			calibratedPoint.z = current_border_values.posZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Upper NW
		else if ((normalized_slope.x == -1) && (normalized_slope.y == 1) && (normalized_slope.z == -1))
		{
			//std::cout << "Corner point to use found. " << std::endl;
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.negXlimit;
			calibratedPoint.y = current_border_values.posYlimit;
			calibratedPoint.z = current_border_values.negZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Upper NE
		else if ((normalized_slope.x == 1) && (normalized_slope.y == 1) && (normalized_slope.z == -1))
		{
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.posXlimit;
			calibratedPoint.y = current_border_values.posYlimit;
			calibratedPoint.z = current_border_values.negZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}

		// Upper SE
		else if ((normalized_slope.x == 1) && (normalized_slope.y == 1) && (normalized_slope.z == 1))
		{
			ECBPolyPoint calibratedPoint;
			calibratedPoint.x = current_border_values.posXlimit;
			calibratedPoint.y = current_border_values.posYlimit;
			calibratedPoint.z = current_border_values.posZlimit;
			returnPair.calibratedPointTri.triPoints[0] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[1] = calibratedPoint;
			returnPair.calibratedPointTri.triPoints[2] = calibratedPoint;
			ECBPolyPoint calibratedDistance;
			calibratedDistance.x = 1.0f;
			calibratedDistance.y = 1.0f;
			calibratedDistance.z = 1.0f;
			returnPair.calibratedDistance = calibratedDistance;
		}



		else
		{
			std::cout << "Warning, no corner found!!!! " << std::endl;
			std::cout << ">>>> Normalized slope was: " << normalized_slope.x << ", " << normalized_slope.y << ", " << normalized_slope.z << ", " << std::endl;
			std::cout << "Original distances: " << std::endl;
			std::cout << "distance x: " << in_distanceValues.x << std::endl;
			std::cout << "distance y: " << in_distanceValues.y << std::endl;
			std::cout << "distance z: " << in_distanceValues.z << std::endl;
			int stopinput;
			std::cin >> stopinput;
		}

	}

	// ::::::::::::::::::::::::::::::::::::::::::::::::::::: Line match logic

	// Z lines	
	else if
		(
		(rounded_x_distance == rounded_y_distance)
			)
	{
		// std::cout << ":::: Z lines, x/y distance match, calibrating: " << std::endl;
		ECBPolyPoint fusedPoint;
		fusedPoint.x = in_polyPointTriRef->triPoints[0].x;	// get the precise x
		fusedPoint.y = in_polyPointTriRef->triPoints[1].y;	// get the precise y

		returnPair.calibratedPointTri = *in_polyPointTriRef;	// copy the referenced value first
		returnPair.calibratedDistance.x = rounded_x_distance;	// set the new distances
		returnPair.calibratedDistance.y = rounded_y_distance;
		returnPair.calibratedDistance.z = rounded_z_distance;

		returnPair.calibratedPointTri.triPoints[0].y = fusedPoint.y;	// set the y coordinate in the x-intercept
		returnPair.calibratedPointTri.triPoints[1].x = fusedPoint.x;		// set the x coordinate in the y-intercept

		//int whoathere;
		//std::cin >> whoathere;
	}

	// Y lines
	else if
		(
		(rounded_x_distance == rounded_z_distance)
			)
	{
		// std::cout << ":::: Y lines, x/z distance match, calibrating: " << std::endl;
		ECBPolyPoint fusedPoint;
		fusedPoint.x = in_polyPointTriRef->triPoints[0].x;		// get the precise x
		fusedPoint.z = in_polyPointTriRef->triPoints[2].z;		// get the precise z

		returnPair.calibratedPointTri = *in_polyPointTriRef;
		returnPair.calibratedDistance.x = rounded_x_distance;	// set the new distances
		returnPair.calibratedDistance.y = rounded_y_distance;
		returnPair.calibratedDistance.z = rounded_z_distance;

		returnPair.calibratedPointTri.triPoints[0].z = fusedPoint.z;
		returnPair.calibratedPointTri.triPoints[2].x = fusedPoint.x;


	}

	else if
		(
		(rounded_y_distance == rounded_z_distance)
			)
	{
		// std::cout << ":::: X linse, y/z distance match, calibrating: " << std::endl;
		ECBPolyPoint fusedPoint;
		fusedPoint.y = in_polyPointTriRef->triPoints[1].y;		// get the precise y
		fusedPoint.z = in_polyPointTriRef->triPoints[2].z;		// get the precise z

		returnPair.calibratedPointTri = *in_polyPointTriRef;
		returnPair.calibratedDistance.x = rounded_x_distance;	// set the new distances
		returnPair.calibratedDistance.y = rounded_y_distance;
		returnPair.calibratedDistance.z = rounded_z_distance;

		returnPair.calibratedPointTri.triPoints[1].z = fusedPoint.z;
		returnPair.calibratedPointTri.triPoints[2].y = fusedPoint.y;
	}


	// Normal face
	else
	{
		returnPair.calibratedPointTri = *in_polyPointTriRef;
		returnPair.calibratedDistance = in_distanceValues;
	}

	// std::cout << "||||||||||||||||||| ending calibration comparison ||||||||||||||||||||" << std::endl;
	return returnPair;
}

ECBIntersectMeta IndependentUtils::findClosestBlueprintIntersection(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, EnclaveKeyDef::EnclaveKey in_pointAKey, EnclaveKeyDef::EnclaveKey in_pointBKey)
{
	// firstly, calculate the slope (pointB - pointA)
	ECBIntersectMeta intersectMetaReturn;		// for return value
	//std::cout << "Point A Key: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
	//std::cout << "Point B Key: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;
	//std::cout << std::endl << "FCI: PointA " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << ", " << std::endl;
	//std::cout << "FCI: PointB " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << ", " << std::endl;

				// std::cout << "Key A: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
				// std::cout << "Key B: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;


	if (!(in_pointAKey == in_pointBKey))		// do this if the keys are not equal
	{
		//std::cout << "!!! CONDITION 1 entry" << std::endl;
		//std::cout << "FCI: PointA " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << ", " << std::endl;
		//std::cout << "FCI: PointB " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << ", " << std::endl;
		ECBPolyPoint resultantSlope;
		resultantSlope.x = in_pointB.x - in_pointA.x;
		resultantSlope.y = in_pointB.y - in_pointA.y;
		resultantSlope.z = in_pointB.z - in_pointA.z;
		//std::cout << "Resultant slope values are: " << resultantSlope.x << ", " << resultantSlope.y << ", " << resultantSlope.z << std::endl;
		// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
		int x_dir = 0;
		int y_dir = 0;
		int z_dir = 0;

		float x_interceptCoord = 0.0f;
		float y_interceptCoord = 0.0f;
		float z_interceptCoord = 0.0f;

		// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
		float powSlopeX = pow(resultantSlope.x, 2.0f);
		float powSlopeY = pow(resultantSlope.y, 2.0f);
		float powSlopeZ = pow(resultantSlope.z, 2.0f);
		float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

		// create a temporary struct to store tracked data
		struct travelDistances
		{
			float timeToComplete = 0.0f;
			float length = 0.0f;
			int direction = 0;
			ECBPolyPoint point;
		};
		travelDistances travelResults[3];	// first index value (0) = x result
											// second index value (1) = y result
											// third index value (2) = z result

		// ******for this logic, check lines starting at 1891 in OrganicSystem.cpp
		// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
		float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
		float time_to_complete_y_traversal = 0.0f;
		float time_to_complete_z_traversal = 0.0f;

		float time_to_complete_x_traversal_RAW = 0.0f;
		float time_to_complete_y_traversal_RAW = 0.0f;
		float time_to_complete_z_traversal_RAW = 0.0f;

		ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
		ECBPolyPoint calculatedPoint_for_y;
		ECBPolyPoint calculatedPoint_for_z;


		if (resultantSlope.x > 0)
		{
			//std::cout << "POS X entry " << std::endl;
			x_dir = 1;			// going towards positive x 
			x_interceptCoord = float((in_pointAKey.x * 32) + 32);								// x_interceptCoord is the precise location of x at the EAST face border
			float origin_to_border_x_diff = (x_interceptCoord - in_pointA.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
			//std::cout << "X intercept coord: " << x_interceptCoord << std::endl;
			//std::cout << "Origin to border x diff: " << origin_to_border_x_diff << std::endl;
			time_to_complete_x_traversal = origin_to_border_x_diff / resultantSlope.x;	// get the distance that the ray has to travel to get to this value of x
			calculatedPoint_for_x.x = x_interceptCoord;
			calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);
			calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
			travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
			travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[0].direction = x_dir;
			//std::cout << "X-traversal time: " << time_to_complete_x_traversal << std::endl;

		}
		else if (resultantSlope.x < 0)
		{
			x_dir = -1;			// going towards negative x
			x_interceptCoord = float(in_pointAKey.x * 32);
			//std::cout << "x-intercept coord: " << x_interceptCoord << std::endl;
			//std::cout << "point A x: " << in_pointA.x << std::endl;
			float origin_to_border_x_diff = abs(x_interceptCoord - in_pointA.x);					// make sure to get absolute value for these two lines (WEST border)
			time_to_complete_x_traversal = abs(origin_to_border_x_diff / resultantSlope.x);	// ""
			calculatedPoint_for_x.x = x_interceptCoord;
			calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);	// "" 
			calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);	// ""

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
			travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
			travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[0].direction = x_dir;
		}
		//std::cout << "X-traversal value: " << travelResults[0].timeToComplete << std::endl;

		//... for y												********************************************************************************************************************************************************************
		if (resultantSlope.y > 0)
		{
			//std::cout << "POS Y entry " << std::endl;
			y_dir = 1;		// going towards positive y
			y_interceptCoord = float((in_pointAKey.y * 32) + 32);								// y_interceptCoord is the precise location of y at the TOP face border
			float origin_to_border_y_diff = y_interceptCoord - in_pointA.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
			time_to_complete_y_traversal = origin_to_border_y_diff / resultantSlope.y;	// get the distance this ray has to travel to get to this value of y
			//std::cout << "correct value of origin_to_border: " << origin_to_border_y_diff << std::endl;
			calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
			calculatedPoint_for_y.y = y_interceptCoord;
			calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
			travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[1].direction = y_dir;

		}
		else if (resultantSlope.y < 0)
		{
			//std::cout << "NEG Y entry " << std::endl;
			y_dir = -1;

			//std::cout << "BP key A: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
			//std::cout << "BP key B: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;

			y_interceptCoord = float(in_pointAKey.y * 32);
			float origin_to_border_y_diff = abs(y_interceptCoord - in_pointA.y);
			time_to_complete_y_traversal = abs(origin_to_border_y_diff / resultantSlope.y);
			//time_to_complete_y_traversal = roundToThousandths(abs(origin_to_border_y_diff / resultantSlope.y));
			//time_to_complete_y_traversal_RAW = abs(origin_to_border_y_diff / resultantSlope.y);
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
			calculatedPoint_for_y.y = y_interceptCoord;
			calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
			travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[1].direction = y_dir;
		}
		//std::cout << "Y-traversal value: " << travelResults[1].timeToComplete << std::endl;

		//... for z
		if (resultantSlope.z > 0)
		{
			//std::cout << ">> POS Z entry " << std::endl;
			z_dir = 1;
			z_interceptCoord = float((in_pointAKey.z * 32) + 32);
			float origin_to_border_z_diff = z_interceptCoord - in_pointA.z;
			time_to_complete_z_traversal = origin_to_border_z_diff / resultantSlope.z;
			//time_to_complete_z_traversal = roundToThousandths(origin_to_border_z_diff / resultantSlope.z);
			//time_to_complete_z_traversal_RAW = origin_to_border_z_diff / resultantSlope.z;
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
			calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
			calculatedPoint_for_z.z = z_interceptCoord;

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
			travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[2].direction = z_dir;
		}
		else if (resultantSlope.z < 0)
		{
			//std::cout << "NEG Z entry " << std::endl;
			z_dir = -1;
			z_interceptCoord = float(in_pointAKey.z * 32);
			float origin_to_border_z_diff = abs(z_interceptCoord - in_pointA.z);
			time_to_complete_z_traversal = abs(origin_to_border_z_diff / resultantSlope.z);
			//time_to_complete_z_traversal = roundToThousandths(abs(origin_to_border_z_diff / resultantSlope.z));
			//time_to_complete_z_traversal_RAW = abs(origin_to_border_z_diff / resultantSlope.z);
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
			calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
			calculatedPoint_for_z.z = z_interceptCoord;

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
			travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[2].direction = z_dir;
		}
		//std::cout << "Z-traversal value: " << travelResults[2].timeToComplete << std::endl;
		//std::cout << "||||| distance values set " << std::endl;

		// find true key shift value (in case the point is exactly on a border line or border corner of this ECB's x/y/z limits)
		ECBPolyPointTri triPointParam;
		triPointParam.triPoints[0] = calculatedPoint_for_x;
		triPointParam.triPoints[1] = calculatedPoint_for_y;
		triPointParam.triPoints[2] = calculatedPoint_for_z;
		ECBPolyPoint distanceValues;
		distanceValues.x = time_to_complete_x_traversal;
		distanceValues.y = time_to_complete_y_traversal;
		distanceValues.z = time_to_complete_z_traversal;

		//std::cout << "Time to complete traversals: " << std::endl;
		//std::cout << "x: " << time_to_complete_x_traversal << std::endl;
		//std::cout << "y: " << time_to_complete_y_traversal << std::endl;
		//std::cout << "z: " << time_to_complete_z_traversal << std::endl;
		//std::cout << std::fixed << std::setprecision(7);
		//std::cout << "Raw values: " << std::endl;
		//std::cout << "Raw x: " << time_to_complete_x_traversal_RAW << std::endl;
		//std::cout << "Raw y: " << time_to_complete_y_traversal_RAW << std::endl;
		//std::cout << "Raw z: " << time_to_complete_z_traversal_RAW << std::endl;
		//std::cout << "Comparing and calibrate..." << std::endl;
		//std::cout << "!!### Resultant slope used will be: " << resultantSlope.x << ", " << resultantSlope.y << ", " << resultantSlope.z << std::endl;
		ECBCalibratedPointPair newTri = compareAndCalibrateDistances(&triPointParam, distanceValues, resultantSlope, in_pointAKey);
		//std::cout << "Comparing and calibrate complete..." << std::endl;
		ECBPolyPoint slopeDirection;
		slopeDirection.x = float(x_dir);
		slopeDirection.y = float(y_dir);
		slopeDirection.z = float(z_dir);

		triPointParam = newTri.calibratedPointTri;
		distanceValues = newTri.calibratedDistance;

		//std::cout << "Calling BP movemeta..." << std::endl;
		intersectMetaReturn = findBlueprintBorderMoveMeta(in_pointAKey, in_pointA, distanceValues, slopeDirection, triPointParam);			// find key for shifting 
		//std::cout << "||||| Intersect return meta point: " << intersectMetaReturn.intersectedPoint.x << ", " << intersectMetaReturn.intersectedPoint.y << ", " << intersectMetaReturn.intersectedPoint.z << std::endl;
		//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| End of movemeta finding. " << std::endl;
	}

	else if (in_pointAKey == in_pointBKey)
	{
		intersectMetaReturn.originPoint = in_pointA;
		intersectMetaReturn.intersectedPoint = in_pointB;
		//std::cout << "finish entry" << std::endl;
	}
	// cycle through results
	/*
	float currentShortestTime = 0.0f;
	int currentShortestIndex = -1;		// -1 is default (no valid index selected)
	for (int x = 0; x < 3; x++)
	{
		if (travelResults[x].timeToComplete != 0.0f)	// is there an actual valid time for this?
		{
			if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
			{
				currentShortestTime = travelResults[x].timeToComplete;	// set the initial length
				currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
			}
			else if (currentShortestTime != 0.0f)
			{
				if (travelResults[x].timeToComplete < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
				{
					currentShortestTime = travelResults[x].timeToComplete;	// reset the length
					currentShortestIndex = x;							// reset the index
				}
			}
		}
	}

	if (currentShortestIndex == 0)	// x was found
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +X" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -X" << std::endl;
		}
	}
	else if (currentShortestIndex == 1)
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +Y" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -Y" << std::endl;
		}
	}
	else if (currentShortestIndex == 2)
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +Z" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -Z" << std::endl;
		}
	}
	*/
	//intersectResult.faceID = travelResults[currentShortestIndex].direction;
	//intersectResult.intersectedPoint = travelResults[currentShortestIndex].point;
	//intersectResult.lineLength = travelResults[currentShortestIndex].length;
	//intersectResult.originPoint = in_pointA;
	//std::cout << "Preparing to return..." << std::endl;
	return intersectMetaReturn;
}

ECBIntersectMeta IndependentUtils::findBlueprintBorderMoveMeta(EnclaveKeyDef::EnclaveKey in_Key1, ECBPolyPoint in_originPoint, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords)
{
	// variable initialization
	//std::cout << "------------------------------------------------------------------->>>> Origin point for calc is: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << ", " << std::endl;
	ECBIntersectMeta intersectMeta;
	EnclaveKeyDef::EnclaveKey newKey;
	ECBBorderLineList pointABorderLineList;
	ECBBorderValues borderLimits = getBlueprintLimits(in_Key1);		// get the precise border limits
	pointABorderLineList = IndependentUtils::determineBorderLines(in_Key1);			// get the border values for the origin point, based on in_Key1
	int isPointOnALine = 1;
	//std::cout << "Original dist_to_x: " << in_distanceValues.x << std::endl;
	//std::cout << "Original dist_to_y: " << in_distanceValues.y << std::endl;
	//std::cout << "Original dist_to_z: " << in_distanceValues.z << std::endl;
	//std::cout << std::fixed << std::setprecision(7);
	//float dist_to_X = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.x);
	//float dist_to_Y = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.y);
	//float dist_to_Z = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.z);
	//float dist_to_X = OrganicUtils::roundToThousandths(in_distanceValues.x);
	//float dist_to_Y = OrganicUtils::roundToThousandths(in_distanceValues.y);
	//float dist_to_Z = OrganicUtils::roundToThousandths(in_distanceValues.z);
	ECBPolyPoint testXYZenhanced = IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecisionDebug(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = testXYZenhanced.x;
	float dist_to_Y = testXYZenhanced.y;
	float dist_to_Z = testXYZenhanced.z;

	//ECBPolyPoint x_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[0]);	// get x point
	//ECBPolyPoint y_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[1]);	// ...y point
	//ECBPolyPoint z_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[2]);	// ...z point
	//std::cout << "Base intercept coords, before conversion: " << std::endl;
	//std::cout << "X--> " << in_XYZinterceptCoords.triPoints[0].x << ",  " << in_XYZinterceptCoords.triPoints[0].y << ",  " << in_XYZinterceptCoords.triPoints[0].z << std::endl;
	//std::cout << "Y--> " << in_XYZinterceptCoords.triPoints[1].x << ",  " << in_XYZinterceptCoords.triPoints[1].y << ",  " << in_XYZinterceptCoords.triPoints[1].z << std::endl;
	//std::cout << "Z--> " << in_XYZinterceptCoords.triPoints[2].x << ",  " << in_XYZinterceptCoords.triPoints[2].y << ",  " << in_XYZinterceptCoords.triPoints[2].z << std::endl;
	ECBPolyPoint x_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[0], in_Key1);
	ECBPolyPoint y_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[1], in_Key1);
	ECBPolyPoint z_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[2], in_Key1);

	//std::cout << "X int coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
	//std::cout << "Y int coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
	//std::cout << "Z int coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;

	//std::cout << "------------------------" << std::endl;
	//ECBPolyPoint originalX = in_XYZinterceptCoords.triPoints[0];
	//ECBPolyPoint originalY = in_XYZinterceptCoords.triPoints[1];
	//ECBPolyPoint originalZ = in_XYZinterceptCoords.triPoints[2];

	//std::cout << "Original X: " << originalX.x << "," << originalX.y << "," << originalX.z << std::endl;
	//std::cout << "Original Y: " << originalY.x << "," << originalY.y << "," << originalY.z << std::endl;
	//std::cout << "Original Z: " << originalZ.x << "," << originalZ.y << "," << originalZ.z << std::endl;

	//std::cout << "Dist values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	if ((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
	{
		//std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}
	ECBPolyPoint pointToCheck;				// the actual intercept point that will be used when comparing to border lines, border corners, or border faces
	if (dist_to_Y == dist_to_Z)
	{
		//std::cout << "Dist to Y/Z are equal!" << std::endl;
	}
	if ((in_originPoint.x == ((in_Key1.z*32.0f) + 32.0f)) || (in_originPoint.x == in_Key1.z*32.0f))
	{
		//std::cout << "Origin point on bounds! " << std::endl;
	}

	//float resultVal = 2.0f + 0.0f;
	//std::cout << "result val test: " << resultVal << std::endl;

	// Step 1: check if the resulting point is a perfect corner intercept
	// Multiple conditions:
	//		Condition 1: X, Y, -AND- Z intercept distance are the same 
	//		Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
	//      Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
	//		Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
	//      Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)
	//      Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
	//      Condition 7: in_slope.z is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (z intercept coord's x and y are exactly on border)
	if (
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == ((in_Key1.z*32.0f) + 32.0f)) || (in_originPoint.z == in_Key1.z*32.0f))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == ((in_Key1.y*32.0f) + 32.0f)) || (in_originPoint.y == in_Key1.y*32.0f))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == ((in_Key1.x*32.0f) + 32.0f)) || (in_originPoint.x == in_Key1.x*32.0f))		// THIS WAS AN ERROR ON: 5/27/2018! These values were z instead of x!!!!
			&&
			in_slopeDirection.x == 0.0f
			)

		||

		(														//  Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)		
		(dist_to_X != 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((x_intercept_coords.y == ((in_Key1.y*32.0f) + 32.0f)) || (x_intercept_coords.y == in_Key1.y*32.0f))
				&&
				((x_intercept_coords.z == ((in_Key1.z*32.0f) + 32.0f)) || (x_intercept_coords.z == in_Key1.z*32.0f))
				)
			)

		||

		(														//   Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Y != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((y_intercept_coords.x == ((in_Key1.x*32.0f) + 32.0f)) || (y_intercept_coords.x == in_Key1.x*32.0f))
				&&
				((y_intercept_coords.z == ((in_Key1.z*32.0f) + 32.0f)) || (y_intercept_coords.z == in_Key1.z*32.0f))
				)
			)

		||

		(														//   Condition 7: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Z != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(
			((z_intercept_coords.x == ((in_Key1.x*32.0f) + 32.0f)) || (z_intercept_coords.x == in_Key1.x*32.0f))
				&&
				((z_intercept_coords.y == ((in_Key1.y*32.0f) + 32.0f)) || (z_intercept_coords.y == in_Key1.y*32.0f))
				)
			)
		)
	{
		//std::cout << ">>> STEP 1 ENTRY (Corner detected)" << std::endl;
		// need triple if check below
		if (in_slopeDirection.x != 0.0f)
		{
			//std::cout << std::fixed << std::setprecision(7);
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);		// pick any of the 3 points (it won't matter since they all have the same distance...we'll pick x coord here)
			//std::cout << "X point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			//std::cout << "X-intercept, x: " << pointToCheck.x << std::endl;
			//std::cout << "X-intercept, y: " << pointToCheck.y << std::endl;
			//std::cout << "X-intercept, z: " << pointToCheck.z << std::endl;

			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << std::endl;
			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.y << std::endl;
			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;

			if (pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x)
			{
				//std::cout << "x true match found... " << std::endl;
			}
			if (pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y)
			{
				//std::cout << "y true match found..." << std::endl;
			}
			if (pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
			{
				//std::cout << "z true match found..." << std::endl;
			}
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			//std::cout << "Y point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//std::cout << "Z point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x	&&		pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y	&&		pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)		// Lower NW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key
			//std::cout << "Point is at lower NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)			// Lower SW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			//std::cout << "Point is at lower SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSW.cornerPoint.x   &&	pointToCheck.y == pointABorderLineList.corner_UpperSW.cornerPoint.y   &&  pointToCheck.z == pointABorderLineList.corner_UpperSW.cornerPoint.z)			// Upper SW	 
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SW
			//std::cout << "Point is at upper SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNW.cornerPoint.z)			// Upper NW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x	&&  pointToCheck.y == pointABorderLineList.corner_LowerNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerNE.cornerPoint.z)			// Lower NE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSE.cornerPoint.z)			// Lower SE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperSE.cornerPoint.z)			// Upper SE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNE.cornerPoint.z)			// Upper NE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
			//std::cout << "Point is at upper NE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}

	}

	// Step 2: check if the resulting point is a border line intercept
	else if ((dist_to_X == dist_to_Y) || (dist_to_Y == dist_to_Z) || (dist_to_X == dist_to_Z))
	{
		//std::cout << "STEP 2 ENTRY" << std::endl;
		// ||||||||||||||||||||||||||||||||||||||||| condition 1 (Z-lines): X and Y distances match, check for Z ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		if ((dist_to_X == dist_to_Y) && (dist_to_X != 0.0f))
		{
			//std::cout << "STEP 2:1 ENTRY: Z-lines " << std::endl;
			//std::cout << "dist to Z: " << dist_to_Z << std::endl;
			//std::cout << "dist to X: " << dist_to_X << std::endl;
			//std::cout << "in_slopeDirection.z: " << in_slopeDirection.z << std::endl;
			if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))
			{
				//std::cout << "Z is less than X!!! " << std::endl;
			}
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			if (
				(dist_to_Z > dist_to_X) && (in_slopeDirection.z != 0.0f)		//     condition 1: if Z's distance is greater  than X (could also be y here), AND it's slope is not 0, we go with x/y distance point
				||																//		-- OR --
				(in_slopeDirection.z == 0.0f)									//     condition 2: Z's slope is 0, check x and y
				)	// we can compare to either x or y, since they are both the same value; if Z is greater, it will not hit a Z-face before X and Y does
			{
				//pointToCheck = x_intercept_coords;	// make the pointToCheck equal to either x or y intercept coords
				//std::cout << "(Z-line) OPTION 1: entry check" << std::endl;
				//perform comparisons to Zaxis borders -- lowerWest, lowerEast, upperWest, upperEast
				//std::cout << "Lower NW coords: " << pointABorderLineList.corner_LowerNW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.z << std::endl;
				//std::cout << "Lower SW coords: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;

				//std::cout << "Upper SW coords: " << pointABorderLineList.corner_UpperSW.cornerPoint.x << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.y << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.z << std::endl;
				// Lower West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_LowerNW.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_LowerSW.cornerPoint.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// Upper West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_UpperNW.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_UpperNW.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_UpperSW.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// Upper East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_UpperNE.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_UpperNE.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_UpperNE.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_UpperSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
					//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// Lower East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_LowerNE.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_LowerNE.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_LowerSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

			}
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y
			{
				std::cout << "(!! Z-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.z == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
					//std::cout << "X/Y distances match, but Z is hit first (positive z): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
					//std::cout << "X/Y distances match, but Z is hit first (negative z)" << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				//if 
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): X and Z distances match, check for Y ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))
		{
			//std::cout << "STEP 2:2 ENTRY: Y-lines " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);

			if (
				(dist_to_Y > dist_to_X) && (in_slopeDirection.y != 0.0f)			// condition 1:	if Y's distance is greater  than X (could also be z here), AND it's slope is not 0, we go with x/z distance point
				||																	// --OR-- 
				(in_slopeDirection.y == 0.0f)										// condition 2: Y's slope is 0, check x and y	
				)
			{
				//std::cout << "(Y-line) OPTION 1: entry check" << std::endl;
				if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNE.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerNE.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperNE.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// South East line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerSE.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSE.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerSE.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperSE.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// South West line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerSW.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperSW.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// North West line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerNW.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperNW.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

				}

			}
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				//std::cout << "(Y-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.y == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
					//std::cout << "X/Z distances match, but Y is hit first (negative y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 3 (x-lines): Y and Z distances match, check for X ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y == dist_to_Z) && (dist_to_Y != 0.0f))
		{
			//std::cout << "STEP 2:3 ENTRY: X-lines " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//std::cout << "x slope direction " << in_slopeDirection.x << std::endl;
			if (in_slopeDirection.x == 0.0f)
			{
				//std::cout << "x slope is 0!" << std::endl;
			}
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			if (
				(dist_to_X > dist_to_Y) && (in_slopeDirection.x != 0.0f)	// condition 1:	if X's distance is greater  than Y (could also be Z here), AND it's slope is not 0, we go with y/z distance point
				||															// --OR-- 
				(in_slopeDirection.x == 0.0f)								// condition 2: X's slope is 0, check y and z
				)
			{
				//std::cout << "STEP 2:3:1 Entry " << std::endl;
				//std::cout << "Lower NW coords: " << pointABorderLineList.corner_LowerNW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.z << std::endl;
				//std::cout << "Lower SW coords: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;
				//std::cout << "Upper SW coords: " << pointABorderLineList.corner_UpperSW.cornerPoint.x << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.y << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.z << std::endl;
				if ((pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_LowerNW.cornerPoint.x	&&		pointToCheck.x < pointABorderLineList.corner_LowerNE.cornerPoint.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_LowerSW.cornerPoint.x	&&	pointToCheck.x < pointABorderLineList.corner_LowerSE.cornerPoint.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_UpperSW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_UpperSW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_UpperSW.cornerPoint.x	&& pointToCheck.x < pointABorderLineList.corner_UpperSE.cornerPoint.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_UpperNW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_UpperNW.cornerPoint.x	&& pointToCheck.x < pointABorderLineList.corner_UpperNE.cornerPoint.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
			}
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << "STEP 2:3:2 entry " << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.x == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				}
			}
		}

		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.WestFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.EastFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
			}

		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
			if ((pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.BottomFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.TopFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			if ((pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.NorthFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.SouthFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
			}

		}
	}

	// Step 3: else, it's just a normal intercept
	else {
		//std::cout << "|||| Normal Face intercept entry " << std::endl;
		float currentShortestTime = 0.0f;
		int currentShortestIndex = -1;		// -1 is default (no valid index selected)
		float distanceValues[3];			// set up a temp array
		distanceValues[0] = in_distanceValues.x;	// store x distance value
		distanceValues[1] = in_distanceValues.y;	// "" y
		distanceValues[2] = in_distanceValues.z;	// "" z

		for (int x = 0; x < 3; x++)
		{
			if (distanceValues[x] != 0.0f)	// is there an actual valid time for this?
			{
				if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
				{
					currentShortestTime = distanceValues[x];	// set the initial length
					currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
				}
				else if (currentShortestTime != 0.0f)
				{
					if (distanceValues[x] < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
					{
						currentShortestTime = distanceValues[x];	// reset the length
						currentShortestIndex = x;							// reset the index
					}
				}
			}
		}
		if (currentShortestIndex == 0)	// x was found
		{
			//pointToCheck = x_intercept_coords;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			if (in_slopeDirection.x == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +X" << std::endl;
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			//pointToCheck = y_intercept_coords;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
			if (in_slopeDirection.y == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -Y" << std::endl;
			}
		}
		else if (currentShortestIndex == 2)		// z was found
		{
			//std::cout << "Z will intercept in normal face..." << std::endl;
			//std::cout << "Z intercept is: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;
			pointToCheck = z_intercept_coords;
			//std::cout << "Z intercept-Point to check, before conversion" << pointToCheck.x << "," << pointToCheck.y << "," << pointToCheck.z << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
			ECBPPOrientationResults testResults = IndependentUtils::GetBlueprintPointOrientation(pointToCheck, &pointABorderLineList);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
			//std::cout << "Z intercept-Point to check, post conversion" << pointToCheck.x << "," << pointToCheck.y << "," << pointToCheck.z << std::endl;
			if (in_slopeDirection.z == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;

			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -Z" << std::endl;
			}
		}

	}

	intersectMeta.incrementingKey = newKey;
	intersectMeta.originPoint = in_originPoint;
	intersectMeta.intersectedPoint = pointToCheck;
	//std::cout << ">>> values of intersectMeta return: " << std::endl;
	//std::cout << "|||||||| Intersected point values are: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
	//std::cout << "incrementingKey: " << intersectMeta.incrementingKey.x << ", " << intersectMeta.incrementingKey.y << ", " << intersectMeta.incrementingKey.z << std::endl;
//	std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| End of movemeta finding. " << std::endl;
	//std::cout << "+++->>>>Incrementing key: " << intersectMeta.incrementingKey.x << ", " << intersectMeta.incrementingKey.y << ", " << intersectMeta.incrementingKey.z << std::endl;
	//std::cout << "+++->>>>Intersected point: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
	//std::cout << std::endl;
	if (intersectMeta.incrementingKey.x == 0
		&&
		intersectMeta.incrementingKey.y == 0
		&&
		intersectMeta.incrementingKey.z == 0
		)
	{
		std::cout << ">>>> (Blueprint border move meta) Invalid value detected" << std::endl;
		std::cout << "+++++ Debug data: +++++" << std::endl;
		std::cout << "!!!!!!!! (DEBUG) Origin point is: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
		std::cout << "|||||||| (DEBUG) Intersected point values are: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		std::cout << "Key used to produce blueprint borders: " << in_Key1.x << ", " << in_Key1.y << ", " << in_Key1.z << std::endl;
		std::cout << "Slope direction: " << in_slopeDirection.x << ", " << in_slopeDirection.y << ", " << in_slopeDirection.z << std::endl;
		std::cout << "Dist to X: " << dist_to_X << std::endl;
		std::cout << "Dist to Y: " << dist_to_Y << std::endl;
		std::cout << "Dist to Z: " << dist_to_Z << std::endl;

		std::cout << "X int coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
		std::cout << "Y int coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
		std::cout << "Z int coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;
		int in_val;
		std::cin >> in_val;
	}


	return intersectMeta;
}

ECBIntersectMeta IndependentUtils::findCBIv2(ECBPolyPoint in_pointA,
	ECBPolyPoint in_pointB,
	EnclaveKeyDef::EnclaveKey in_pointAKey,
	EnclaveKeyDef::EnclaveKey in_pointBKey,
	TracingLineBoundingBox in_boundingBox)
{
	// firstly, calculate the slope (pointB - pointA)
	ECBIntersectMeta intersectMetaReturn;		// for return value
	//std::cout << "Point A Key: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
	//std::cout << "Point B Key: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;
	//std::cout << std::endl << "FCI: PointA " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << ", " << std::endl;
	//std::cout << "FCI: PointB " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << ", " << std::endl;

				// std::cout << "Key A: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
				// std::cout << "Key B: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;


	if (!(in_pointAKey == in_pointBKey))		// do this if the keys are not equal
	{
		//std::cout << "!!! CONDITION 1 entry" << std::endl;
		//std::cout << "FCI: PointA " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << ", " << std::endl;
		//std::cout << "FCI: PointB " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << ", " << std::endl;
		ECBPolyPoint resultantSlope;
		resultantSlope.x = in_pointB.x - in_pointA.x;
		resultantSlope.y = in_pointB.y - in_pointA.y;
		resultantSlope.z = in_pointB.z - in_pointA.z;
		//std::cout << "Resultant slope values are: " << resultantSlope.x << ", " << resultantSlope.y << ", " << resultantSlope.z << std::endl;
		// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
		int x_dir = 0;
		int y_dir = 0;
		int z_dir = 0;

		float x_interceptCoord = 0.0f;
		float y_interceptCoord = 0.0f;
		float z_interceptCoord = 0.0f;

		// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
		float powSlopeX = pow(resultantSlope.x, 2.0f);
		float powSlopeY = pow(resultantSlope.y, 2.0f);
		float powSlopeZ = pow(resultantSlope.z, 2.0f);
		float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

		// create a temporary struct to store tracked data
		struct travelDistances
		{
			float timeToComplete = 0.0f;
			float length = 0.0f;
			int direction = 0;
			ECBPolyPoint point;
		};
		travelDistances travelResults[3];	// first index value (0) = x result
											// second index value (1) = y result
											// third index value (2) = z result

		// ******for this logic, check lines starting at 1891 in OrganicSystem.cpp
		// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
		float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
		float time_to_complete_y_traversal = 0.0f;
		float time_to_complete_z_traversal = 0.0f;

		float time_to_complete_x_traversal_RAW = 0.0f;
		float time_to_complete_y_traversal_RAW = 0.0f;
		float time_to_complete_z_traversal_RAW = 0.0f;

		ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
		ECBPolyPoint calculatedPoint_for_y;
		ECBPolyPoint calculatedPoint_for_z;


		if (resultantSlope.x > 0)
		{
			//std::cout << "POS X entry " << std::endl;
			x_dir = 1;			// going towards positive x 
			x_interceptCoord = float((in_pointAKey.x * 32) + 32);								// x_interceptCoord is the precise location of x at the EAST face border
			float origin_to_border_x_diff = (x_interceptCoord - in_pointA.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
			//std::cout << "X intercept coord: " << x_interceptCoord << std::endl;
			//std::cout << "Origin to border x diff: " << origin_to_border_x_diff << std::endl;
			time_to_complete_x_traversal = origin_to_border_x_diff / resultantSlope.x;	// get the distance that the ray has to travel to get to this value of x
			calculatedPoint_for_x.x = x_interceptCoord;
			calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);
			calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
			travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
			travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[0].direction = x_dir;
			//std::cout << "X-traversal time: " << time_to_complete_x_traversal << std::endl;

		}
		else if (resultantSlope.x < 0)
		{
			x_dir = -1;			// going towards negative x
			x_interceptCoord = float(in_pointAKey.x * 32);
			//std::cout << "x-intercept coord: " << x_interceptCoord << std::endl;
			//std::cout << "point A x: " << in_pointA.x << std::endl;
			float origin_to_border_x_diff = abs(x_interceptCoord - in_pointA.x);					// make sure to get absolute value for these two lines (WEST border)
			time_to_complete_x_traversal = abs(origin_to_border_x_diff / resultantSlope.x);	// ""
			calculatedPoint_for_x.x = x_interceptCoord;
			calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);	// "" 
			calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);	// ""

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
			travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
			travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[0].direction = x_dir;
		}
		//std::cout << "X-traversal value: " << travelResults[0].timeToComplete << std::endl;

		//... for y												********************************************************************************************************************************************************************
		if (resultantSlope.y > 0)
		{
			//std::cout << "POS Y entry " << std::endl;
			y_dir = 1;		// going towards positive y
			y_interceptCoord = float((in_pointAKey.y * 32) + 32);								// y_interceptCoord is the precise location of y at the TOP face border
			float origin_to_border_y_diff = y_interceptCoord - in_pointA.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
			time_to_complete_y_traversal = origin_to_border_y_diff / resultantSlope.y;	// get the distance this ray has to travel to get to this value of y
			//std::cout << "correct value of origin_to_border: " << origin_to_border_y_diff << std::endl;
			calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
			calculatedPoint_for_y.y = y_interceptCoord;
			calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
			travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[1].direction = y_dir;

		}
		else if (resultantSlope.y < 0)
		{
			//std::cout << "NEG Y entry " << std::endl;
			y_dir = -1;

			//std::cout << "BP key A: " << in_pointAKey.x << ", " << in_pointAKey.y << ", " << in_pointAKey.z << std::endl;
			//std::cout << "BP key B: " << in_pointBKey.x << ", " << in_pointBKey.y << ", " << in_pointBKey.z << std::endl;

			y_interceptCoord = float(in_pointAKey.y * 32);
			float origin_to_border_y_diff = abs(y_interceptCoord - in_pointA.y);
			time_to_complete_y_traversal = abs(origin_to_border_y_diff / resultantSlope.y);
			//time_to_complete_y_traversal = roundToThousandths(abs(origin_to_border_y_diff / resultantSlope.y));
			//time_to_complete_y_traversal_RAW = abs(origin_to_border_y_diff / resultantSlope.y);
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
			calculatedPoint_for_y.y = y_interceptCoord;
			calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
			travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[1].direction = y_dir;
		}
		//std::cout << "Y-traversal value: " << travelResults[1].timeToComplete << std::endl;

		//... for z
		if (resultantSlope.z > 0)
		{
			//std::cout << ">> POS Z entry " << std::endl;
			z_dir = 1;
			z_interceptCoord = float((in_pointAKey.z * 32) + 32);
			float origin_to_border_z_diff = z_interceptCoord - in_pointA.z;
			time_to_complete_z_traversal = origin_to_border_z_diff / resultantSlope.z;
			//time_to_complete_z_traversal = roundToThousandths(origin_to_border_z_diff / resultantSlope.z);
			//time_to_complete_z_traversal_RAW = origin_to_border_z_diff / resultantSlope.z;
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
			calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
			calculatedPoint_for_z.z = z_interceptCoord;

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
			travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[2].direction = z_dir;
		}
		else if (resultantSlope.z < 0)
		{
			//std::cout << "NEG Z entry " << std::endl;
			z_dir = -1;
			z_interceptCoord = float(in_pointAKey.z * 32);
			float origin_to_border_z_diff = abs(z_interceptCoord - in_pointA.z);
			time_to_complete_z_traversal = abs(origin_to_border_z_diff / resultantSlope.z);
			//time_to_complete_z_traversal = roundToThousandths(abs(origin_to_border_z_diff / resultantSlope.z));
			//time_to_complete_z_traversal_RAW = abs(origin_to_border_z_diff / resultantSlope.z);
			//ECBPolyPoint calculatedPoint;
			calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
			calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
			calculatedPoint_for_z.z = z_interceptCoord;

			ECBPolyPoint distanceToCalculatedPoint;
			distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
			distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
			distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
			float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
			float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
			float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
			travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
			travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
			travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
			travelResults[2].direction = z_dir;
		}
		//std::cout << "Z-traversal value: " << travelResults[2].timeToComplete << std::endl;
		//std::cout << "||||| distance values set " << std::endl;

		// find true key shift value (in case the point is exactly on a border line or border corner of this ECB's x/y/z limits)
		ECBPolyPointTri triPointParam;
		triPointParam.triPoints[0] = calculatedPoint_for_x;
		triPointParam.triPoints[1] = calculatedPoint_for_y;
		triPointParam.triPoints[2] = calculatedPoint_for_z;
		ECBPolyPoint distanceValues;
		distanceValues.x = time_to_complete_x_traversal;
		distanceValues.y = time_to_complete_y_traversal;
		distanceValues.z = time_to_complete_z_traversal;

		//std::cout << "Time to complete traversals: " << std::endl;
		//std::cout << "x: " << time_to_complete_x_traversal << std::endl;
		//std::cout << "y: " << time_to_complete_y_traversal << std::endl;
		//std::cout << "z: " << time_to_complete_z_traversal << std::endl;
		//std::cout << std::fixed << std::setprecision(7);
		//std::cout << "Raw values: " << std::endl;
		//std::cout << "Raw x: " << time_to_complete_x_traversal_RAW << std::endl;
		//std::cout << "Raw y: " << time_to_complete_y_traversal_RAW << std::endl;
		//std::cout << "Raw z: " << time_to_complete_z_traversal_RAW << std::endl;
		//std::cout << "Comparing and calibrate..." << std::endl;
		//std::cout << "!!### Resultant slope used will be: " << resultantSlope.x << ", " << resultantSlope.y << ", " << resultantSlope.z << std::endl;
		ECBCalibratedPointPair newTri = compareAndCalibrateDistances(&triPointParam, distanceValues, resultantSlope, in_pointAKey);
		//std::cout << "Comparing and calibrate complete..." << std::endl;
		ECBPolyPoint slopeDirection;
		slopeDirection.x = float(x_dir);
		slopeDirection.y = float(y_dir);
		slopeDirection.z = float(z_dir);

		triPointParam = newTri.calibratedPointTri;
		distanceValues = newTri.calibratedDistance;

		//std::cout << "Calling BP movemeta..." << std::endl;
		intersectMetaReturn = findBBMMv2(in_pointAKey,
										in_pointA,
										distanceValues,
										slopeDirection,
										triPointParam,
										in_boundingBox);			// find key for shifting 			// find key for shifting 
		//std::cout << "||||| Intersect return meta point: " << intersectMetaReturn.intersectedPoint.x << ", " << intersectMetaReturn.intersectedPoint.y << ", " << intersectMetaReturn.intersectedPoint.z << std::endl;
		//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| End of movemeta finding. " << std::endl;
	}

	else if (in_pointAKey == in_pointBKey)
	{
		intersectMetaReturn.originPoint = in_pointA;
		intersectMetaReturn.intersectedPoint = in_pointB;
		//std::cout << "finish entry" << std::endl;
	}
	// cycle through results
	/*
	float currentShortestTime = 0.0f;
	int currentShortestIndex = -1;		// -1 is default (no valid index selected)
	for (int x = 0; x < 3; x++)
	{
		if (travelResults[x].timeToComplete != 0.0f)	// is there an actual valid time for this?
		{
			if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
			{
				currentShortestTime = travelResults[x].timeToComplete;	// set the initial length
				currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
			}
			else if (currentShortestTime != 0.0f)
			{
				if (travelResults[x].timeToComplete < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
				{
					currentShortestTime = travelResults[x].timeToComplete;	// reset the length
					currentShortestIndex = x;							// reset the index
				}
			}
		}
	}

	if (currentShortestIndex == 0)	// x was found
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +X" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -X" << std::endl;
		}
	}
	else if (currentShortestIndex == 1)
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +Y" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -Y" << std::endl;
		}
	}
	else if (currentShortestIndex == 2)
	{
		if (travelResults[currentShortestIndex].direction == 1)
		{
			//std::cout << "Line intersects at +Z" << std::endl;
		}
		else if (travelResults[currentShortestIndex].direction == -1)
		{
			//std::cout << "Line intersects at -Z" << std::endl;
		}
	}
	*/
	//intersectResult.faceID = travelResults[currentShortestIndex].direction;
	//intersectResult.intersectedPoint = travelResults[currentShortestIndex].point;
	//intersectResult.lineLength = travelResults[currentShortestIndex].length;
	//intersectResult.originPoint = in_pointA;
	//std::cout << "Preparing to return..." << std::endl;
	return intersectMetaReturn;
}

ECBIntersectMeta IndependentUtils::findBBMMv2(EnclaveKeyDef::EnclaveKey in_Key1,
	ECBPolyPoint in_originPoint,
	ECBPolyPoint in_distanceValues,
	ECBPolyPoint in_slopeDirection,
	ECBPolyPointTri in_XYZinterceptCoords,
	TracingLineBoundingBox in_boundingBox)
{
	// variable initialization
	std::cout << "------------------------------------------------------------------->>>> Origin point for calc is: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << ", " << std::endl;
	ECBIntersectMeta intersectMeta;
	EnclaveKeyDef::EnclaveKey newKey;
	ECBBorderLineList pointABorderLineList;
	ECBBorderValues borderLimits = getBlueprintLimits(in_Key1);		// get the precise border limits
	pointABorderLineList = IndependentUtils::determineBorderLines(in_Key1);			// get the border values for the origin point, based on in_Key1
	int isPointOnALine = 1;
	//std::cout << "Original dist_to_x: " << in_distanceValues.x << std::endl;
	//std::cout << "Original dist_to_y: " << in_distanceValues.y << std::endl;
	//std::cout << "Original dist_to_z: " << in_distanceValues.z << std::endl;
	//std::cout << std::fixed << std::setprecision(7);
	//float dist_to_X = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.x);
	//float dist_to_Y = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.y);
	//float dist_to_Z = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.z);
	//float dist_to_X = OrganicUtils::roundToThousandths(in_distanceValues.x);
	//float dist_to_Y = OrganicUtils::roundToThousandths(in_distanceValues.y);
	//float dist_to_Z = OrganicUtils::roundToThousandths(in_distanceValues.z);
	ECBPolyPoint testXYZenhanced = IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecisionDebug(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = testXYZenhanced.x;
	float dist_to_Y = testXYZenhanced.y;
	float dist_to_Z = testXYZenhanced.z;

	//ECBPolyPoint x_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[0]);	// get x point
	//ECBPolyPoint y_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[1]);	// ...y point
	//ECBPolyPoint z_intercept_coords = OrganicUtils::roundPolyPointToOneMillionthsSpecial(in_XYZinterceptCoords.triPoints[2]);	// ...z point
	//std::cout << "Base intercept coords, before conversion: " << std::endl;
	//std::cout << "X--> " << in_XYZinterceptCoords.triPoints[0].x << ",  " << in_XYZinterceptCoords.triPoints[0].y << ",  " << in_XYZinterceptCoords.triPoints[0].z << std::endl;
	//std::cout << "Y--> " << in_XYZinterceptCoords.triPoints[1].x << ",  " << in_XYZinterceptCoords.triPoints[1].y << ",  " << in_XYZinterceptCoords.triPoints[1].z << std::endl;
	//std::cout << "Z--> " << in_XYZinterceptCoords.triPoints[2].x << ",  " << in_XYZinterceptCoords.triPoints[2].y << ",  " << in_XYZinterceptCoords.triPoints[2].z << std::endl;
	ECBPolyPoint x_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[0], in_Key1);
	ECBPolyPoint y_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[1], in_Key1);
	ECBPolyPoint z_intercept_coords = IndependentUtils::roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[2], in_Key1);

	//std::cout << "X int coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
	//std::cout << "Y int coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
	//std::cout << "Z int coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;

	//std::cout << "------------------------" << std::endl;
	//ECBPolyPoint originalX = in_XYZinterceptCoords.triPoints[0];
	//ECBPolyPoint originalY = in_XYZinterceptCoords.triPoints[1];
	//ECBPolyPoint originalZ = in_XYZinterceptCoords.triPoints[2];

	//std::cout << "Original X: " << originalX.x << "," << originalX.y << "," << originalX.z << std::endl;
	//std::cout << "Original Y: " << originalY.x << "," << originalY.y << "," << originalY.z << std::endl;
	//std::cout << "Original Z: " << originalZ.x << "," << originalZ.y << "," << originalZ.z << std::endl;

	//std::cout << "Dist values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	if ((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
	{
		//std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}
	ECBPolyPoint pointToCheck;				// the actual intercept point that will be used when comparing to border lines, border corners, or border faces
	if (dist_to_Y == dist_to_Z)
	{
		//std::cout << "Dist to Y/Z are equal!" << std::endl;
	}
	if ((in_originPoint.x == ((in_Key1.z*32.0f) + 32.0f)) || (in_originPoint.x == in_Key1.z*32.0f))
	{
		//std::cout << "Origin point on bounds! " << std::endl;
	}

	//float resultVal = 2.0f + 0.0f;
	//std::cout << "result val test: " << resultVal << std::endl;

	// Step 1: check if the resulting point is a perfect corner intercept
	// Multiple conditions:
	//		Condition 1: X, Y, -AND- Z intercept distance are the same 
	//		Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
	//      Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
	//		Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
	//      Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)
	//      Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
	//      Condition 7: in_slope.z is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (z intercept coord's x and y are exactly on border)
	if (
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == ((in_Key1.z*32.0f) + 32.0f)) || (in_originPoint.z == in_Key1.z*32.0f))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == ((in_Key1.y*32.0f) + 32.0f)) || (in_originPoint.y == in_Key1.y*32.0f))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == ((in_Key1.x*32.0f) + 32.0f)) || (in_originPoint.x == in_Key1.x*32.0f))		// THIS WAS AN ERROR ON: 5/27/2018! These values were z instead of x!!!!
			&&
			in_slopeDirection.x == 0.0f
			)

		||

		(														//  Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)		
		(dist_to_X != 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((x_intercept_coords.y == ((in_Key1.y*32.0f) + 32.0f)) || (x_intercept_coords.y == in_Key1.y*32.0f))
				&&
				((x_intercept_coords.z == ((in_Key1.z*32.0f) + 32.0f)) || (x_intercept_coords.z == in_Key1.z*32.0f))
				)
			)

		||

		(														//   Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Y != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((y_intercept_coords.x == ((in_Key1.x*32.0f) + 32.0f)) || (y_intercept_coords.x == in_Key1.x*32.0f))
				&&
				((y_intercept_coords.z == ((in_Key1.z*32.0f) + 32.0f)) || (y_intercept_coords.z == in_Key1.z*32.0f))
				)
			)

		||

		(														//   Condition 7: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Z != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(
			((z_intercept_coords.x == ((in_Key1.x*32.0f) + 32.0f)) || (z_intercept_coords.x == in_Key1.x*32.0f))
				&&
				((z_intercept_coords.y == ((in_Key1.y*32.0f) + 32.0f)) || (z_intercept_coords.y == in_Key1.y*32.0f))
				)
			)
		)
	{
		//std::cout << ">>> STEP 1 ENTRY (Corner detected)" << std::endl;
		// need triple if check below
		if (in_slopeDirection.x != 0.0f)
		{
			//std::cout << std::fixed << std::setprecision(7);
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);		// pick any of the 3 points (it won't matter since they all have the same distance...we'll pick x coord here)
			//std::cout << "X point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			//std::cout << "X-intercept, x: " << pointToCheck.x << std::endl;
			//std::cout << "X-intercept, y: " << pointToCheck.y << std::endl;
			//std::cout << "X-intercept, z: " << pointToCheck.z << std::endl;

			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << std::endl;
			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.y << std::endl;
			//std::cout << "True corner point to match to: " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;

			if (pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x)
			{
				//std::cout << "x true match found... " << std::endl;
			}
			if (pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y)
			{
				//std::cout << "y true match found..." << std::endl;
			}
			if (pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
			{
				//std::cout << "z true match found..." << std::endl;
			}
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			//std::cout << "Y point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//std::cout << "Z point chosen... " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x	&&		pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y	&&		pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)		// Lower NW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key
			//std::cout << "Point is at lower NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)			// Lower SW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			//std::cout << "Point is at lower SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSW.cornerPoint.x   &&	pointToCheck.y == pointABorderLineList.corner_UpperSW.cornerPoint.y   &&  pointToCheck.z == pointABorderLineList.corner_UpperSW.cornerPoint.z)			// Upper SW	 
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SW
			//std::cout << "Point is at upper SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNW.cornerPoint.z)			// Upper NW
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x	&&  pointToCheck.y == pointABorderLineList.corner_LowerNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerNE.cornerPoint.z)			// Lower NE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSE.cornerPoint.z)			// Lower SE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperSE.cornerPoint.z)			// Upper SE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNE.cornerPoint.z)			// Upper NE
		{
			newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
			//std::cout << "Point is at upper NE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}

	}

	// Step 2: check if the resulting point is a border line intercept
	else if ((dist_to_X == dist_to_Y) || (dist_to_Y == dist_to_Z) || (dist_to_X == dist_to_Z))
	{
		//std::cout << "STEP 2 ENTRY" << std::endl;
		// ||||||||||||||||||||||||||||||||||||||||| condition 1 (Z-lines): X and Y distances match, check for Z ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		if ((dist_to_X == dist_to_Y) && (dist_to_X != 0.0f))
		{
			//std::cout << "STEP 2:1 ENTRY: Z-lines " << std::endl;
			//std::cout << "dist to Z: " << dist_to_Z << std::endl;
			//std::cout << "dist to X: " << dist_to_X << std::endl;
			//std::cout << "in_slopeDirection.z: " << in_slopeDirection.z << std::endl;
			if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))
			{
				//std::cout << "Z is less than X!!! " << std::endl;
			}
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			if (
				(dist_to_Z > dist_to_X) && (in_slopeDirection.z != 0.0f)		//     condition 1: if Z's distance is greater  than X (could also be y here), AND it's slope is not 0, we go with x/y distance point
				||																//		-- OR --
				(in_slopeDirection.z == 0.0f)									//     condition 2: Z's slope is 0, check x and y
				)	// we can compare to either x or y, since they are both the same value; if Z is greater, it will not hit a Z-face before X and Y does
			{
				//pointToCheck = x_intercept_coords;	// make the pointToCheck equal to either x or y intercept coords
				//std::cout << "(Z-line) OPTION 1: entry check" << std::endl;
				//perform comparisons to Zaxis borders -- lowerWest, lowerEast, upperWest, upperEast
				//std::cout << "Lower NW coords: " << pointABorderLineList.corner_LowerNW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.z << std::endl;
				//std::cout << "Lower SW coords: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;

				//std::cout << "Upper SW coords: " << pointABorderLineList.corner_UpperSW.cornerPoint.x << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.y << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.z << std::endl;
				// Lower West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_LowerNW.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_LowerSW.cornerPoint.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// Upper West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_UpperNW.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_UpperNW.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_UpperSW.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// Upper East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_UpperNE.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_UpperNE.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_UpperNE.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_UpperSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
					//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// Lower East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x)
					&&
					(pointToCheck.y == pointABorderLineList.corner_LowerNE.cornerPoint.y)
					&&
					(pointToCheck.z > pointABorderLineList.corner_LowerNE.cornerPoint.z	&& pointToCheck.z < pointABorderLineList.corner_LowerSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

			}
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y
			{
				//std::cout << "(Z-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.z == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
					//std::cout << "X/Y distances match, but Z is hit first (positive z): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
					//std::cout << "X/Y distances match, but Z is hit first (negative z)" << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				//if 
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): X and Z distances match, check for Y ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))
		{
			//std::cout << "STEP 2:2 ENTRY: Y-lines " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);

			if (
				(dist_to_Y > dist_to_X) && (in_slopeDirection.y != 0.0f)			// condition 1:	if Y's distance is greater  than X (could also be z here), AND it's slope is not 0, we go with x/z distance point
				||																	// --OR-- 
				(in_slopeDirection.y == 0.0f)										// condition 2: Y's slope is 0, check x and y	
				)
			{
				//std::cout << "(Y-line) OPTION 1: entry check" << std::endl;
				if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNE.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerNE.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperNE.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// South East line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerSE.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSE.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerSE.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperSE.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// South West line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerSW.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperSW.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// North West line
				else if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)
					&&
					(pointToCheck.y > pointABorderLineList.corner_LowerNW.cornerPoint.y	&& pointToCheck.y < pointABorderLineList.corner_UpperNW.cornerPoint.y)
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

				}

			}
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				//std::cout << "(Y-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.y == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
					//std::cout << "X/Z distances match, but Y is hit first (negative y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 3 (x-lines): Y and Z distances match, check for X ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y == dist_to_Z) && (dist_to_Y != 0.0f))
		{
			//std::cout << "STEP 2:3 ENTRY: X-lines " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//std::cout << "x slope direction " << in_slopeDirection.x << std::endl;
			if (in_slopeDirection.x == 0.0f)
			{
				//std::cout << "x slope is 0!" << std::endl;
			}
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			if (
				(dist_to_X > dist_to_Y) && (in_slopeDirection.x != 0.0f)	// condition 1:	if X's distance is greater  than Y (could also be Z here), AND it's slope is not 0, we go with y/z distance point
				||															// --OR-- 
				(in_slopeDirection.x == 0.0f)								// condition 2: X's slope is 0, check y and z
				)
			{
				//std::cout << "STEP 2:3:1 Entry " << std::endl;
				//std::cout << "Lower NW coords: " << pointABorderLineList.corner_LowerNW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerNW.cornerPoint.z << std::endl;
				//std::cout << "Lower SW coords: " << pointABorderLineList.corner_LowerSW.cornerPoint.x << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.y << ", " << pointABorderLineList.corner_LowerSW.cornerPoint.z << std::endl;
				//std::cout << "Upper SW coords: " << pointABorderLineList.corner_UpperSW.cornerPoint.x << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.y << ", " << pointABorderLineList.corner_UpperSW.cornerPoint.z << std::endl;
				if ((pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_LowerNW.cornerPoint.x	&&		pointToCheck.x < pointABorderLineList.corner_LowerNE.cornerPoint.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_LowerSW.cornerPoint.x	&&	pointToCheck.x < pointABorderLineList.corner_LowerSE.cornerPoint.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_UpperSW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_UpperSW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_UpperSW.cornerPoint.x	&& pointToCheck.x < pointABorderLineList.corner_UpperSE.cornerPoint.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
				// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y)
					&&
					(pointToCheck.z == pointABorderLineList.corner_UpperNW.cornerPoint.z)
					&&
					(pointToCheck.x > pointABorderLineList.corner_UpperNW.cornerPoint.x	&& pointToCheck.x < pointABorderLineList.corner_UpperNE.cornerPoint.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
					)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
			}
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << "STEP 2:3:2 entry " << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
				if (in_slopeDirection.x == 1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				}
			}
		}

		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.WestFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.EastFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
			}

		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
			if ((pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.BottomFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.TopFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			if ((pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.NorthFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.SouthFace);
				newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
			}

		}
	}

	// Step 3: else, it's just a normal intercept
	else {
		//std::cout << "|||| Normal Face intercept entry " << std::endl;
		float currentShortestTime = 0.0f;
		int currentShortestIndex = -1;		// -1 is default (no valid index selected)
		float distanceValues[3];			// set up a temp array
		distanceValues[0] = in_distanceValues.x;	// store x distance value
		distanceValues[1] = in_distanceValues.y;	// "" y
		distanceValues[2] = in_distanceValues.z;	// "" z

		for (int x = 0; x < 3; x++)
		{
			if (distanceValues[x] != 0.0f)	// is there an actual valid time for this?
			{
				if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
				{
					currentShortestTime = distanceValues[x];	// set the initial length
					currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
				}
				else if (currentShortestTime != 0.0f)
				{
					if (distanceValues[x] < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
					{
						currentShortestTime = distanceValues[x];	// reset the length
						currentShortestIndex = x;							// reset the index
					}
				}
			}
		}
		if (currentShortestIndex == 0)	// x was found
		{
			//pointToCheck = x_intercept_coords;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits);
			if (in_slopeDirection.x == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +X" << std::endl;
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			//pointToCheck = y_intercept_coords;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits);
			if (in_slopeDirection.y == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -Y" << std::endl;
			}
		}
		else if (currentShortestIndex == 2)		// z was found
		{
			//std::cout << "Z will intercept in normal face..." << std::endl;
			//std::cout << "Z intercept is: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;
			pointToCheck = z_intercept_coords;
			//std::cout << "Z intercept-Point to check, before conversion" << pointToCheck.x << "," << pointToCheck.y << "," << pointToCheck.z << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
			ECBPPOrientationResults testResults = IndependentUtils::GetBlueprintPointOrientation(pointToCheck, &pointABorderLineList);
			pointToCheck = IndependentUtils::roundToNearestBlueprintLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits);
			IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
			//std::cout << "Z intercept-Point to check, post conversion" << pointToCheck.x << "," << pointToCheck.y << "," << pointToCheck.z << std::endl;
			if (in_slopeDirection.z == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;

			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				newKey = IndependentUtils::getBlueprintCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -Z" << std::endl;
			}
		}

	}

	// Once the newKey value has been determined, it must go through the TracingLineBoundingBox instance to check
	// for any corrections.
	intersectMeta.incrementingKey = in_boundingBox.applyBoundingCorrectionToKeys(in_Key1, newKey);

	intersectMeta.originPoint = in_originPoint;
	intersectMeta.intersectedPoint = pointToCheck;
	//std::cout << ">>> values of intersectMeta return: " << std::endl;
	//std::cout << "|||||||| Intersected point values are: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
	//std::cout << "incrementingKey: " << intersectMeta.incrementingKey.x << ", " << intersectMeta.incrementingKey.y << ", " << intersectMeta.incrementingKey.z << std::endl;
//	std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| End of movemeta finding. " << std::endl;
	//std::cout << "+++->>>>Incrementing key: " << intersectMeta.incrementingKey.x << ", " << intersectMeta.incrementingKey.y << ", " << intersectMeta.incrementingKey.z << std::endl;
	//std::cout << "+++->>>>Intersected point: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
	//std::cout << std::endl;
	if (intersectMeta.incrementingKey.x == 0
		&&
		intersectMeta.incrementingKey.y == 0
		&&
		intersectMeta.incrementingKey.z == 0
		)
	{
		std::cout << ">>>> (Blueprint border move meta) Invalid value detected" << std::endl;
		std::cout << "+++++ Debug data: +++++" << std::endl;
		std::cout << "!!!!!!!! (DEBUG) Origin point is: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
		std::cout << "|||||||| (DEBUG) Intersected point values are: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		std::cout << "Key used to produce blueprint borders: " << in_Key1.x << ", " << in_Key1.y << ", " << in_Key1.z << std::endl;
		std::cout << "Slope direction: " << in_slopeDirection.x << ", " << in_slopeDirection.y << ", " << in_slopeDirection.z << std::endl;
		std::cout << "Dist to X: " << dist_to_X << std::endl;
		std::cout << "Dist to Y: " << dist_to_Y << std::endl;
		std::cout << "Dist to Z: " << dist_to_Z << std::endl;

		std::cout << "X int coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
		std::cout << "Y int coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
		std::cout << "Z int coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;
		int in_val;
		std::cin >> in_val;
	}


	return intersectMeta;
}

ECBPolyPoint IndependentUtils::roundToAppropriatePrecisionForHundredths(ECBPolyPoint in_polyPoint, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	ECBBorderValues borderValues = IndependentUtils::getBlueprintLimits(in_blueprintKey);
	ECBPolyPoint calibratedPoint;
	calibratedPoint.x = in_polyPoint.x;
	calibratedPoint.y = in_polyPoint.y;
	calibratedPoint.y = in_polyPoint.y;
	calibratedPoint.z = in_polyPoint.z;
	//std::cout << std::fixed << std::setprecision(4);		// set precision level for testing
	//std::cout << "(new) Pre-change point values: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << std::endl;
	//std::cout << "/////////////// Testing of rounding to appropriate precision ///////////////" << std::endl;
	//std::cout << "pos x: " << borderValues.posXlimit << std::endl;
	//std::cout << "neg x: " << borderValues.negXlimit << std::endl;
	//std::cout << "pos z: " << borderValues.posZlimit << std::endl;
	//std::cout << "neg z: " << borderValues.negZlimit << std::endl;
	//std::cout << "pos y: " << borderValues.posYlimit << std::endl;
	//std::cout << "neg y: " << borderValues.negYlimit << std::endl;
	/*
	else if ((in_polyPoint.x <= -1000.0f) || (in_polyPoint.x >= 1000.0f))
	{
	returnPoint.x = .999f;
	}
	else if ((in_polyPoint.x <= -100.0f) || (in_polyPoint.x >= 100.0f))
	{
	returnPoint.x = .9999f;
	}
	else
	{
	returnPoint.x = .9999f;
	}
	*/

	// check for x:
	if (in_polyPoint.x <= -10000.0f || in_polyPoint.x >= 10000.0f)
	{
		//std::cout << "*****x value needs special logic check for rounding to hundredths..." << std::endl;
		calibratedPoint.x = roundToHundredthSpecial(calibratedPoint.x, borderValues.negXlimit, borderValues.posXlimit);
	}
	else
	{
		calibratedPoint.x = roundToHundredth(calibratedPoint.x);
	}

	// check for y:
	if (in_polyPoint.y <= -10000.0f || in_polyPoint.y >= 10000.0f)
	{
		calibratedPoint.y = roundToHundredthSpecial(calibratedPoint.y, borderValues.negYlimit, borderValues.posYlimit);
	}
	else
	{
		calibratedPoint.y = roundToHundredth(calibratedPoint.y);
	}

	// check for z:
	if (in_polyPoint.z <= -10000.0f || in_polyPoint.z >= 10000.0f)
	{
		calibratedPoint.z = roundToHundredthSpecial(calibratedPoint.z, borderValues.negZlimit, borderValues.posZlimit);
	}
	else
	{
		calibratedPoint.z = roundToHundredth(calibratedPoint.z);
	}
	//std::cout << "(new) Post calibrate x: " << calibratedPoint.x << std::endl;
	//std::cout << "(new) Post calibrate y: " << calibratedPoint.y << std::endl;
	//std::cout << "(new) Post calibrate z: " << calibratedPoint.z << std::endl;

	return calibratedPoint;
}

float IndependentUtils::roundToHundredthSpecial(float in_float, float in_lowerLimit, float in_upperLimit)
{
	float returnFloat = in_float;
	float negative_threshold = (in_lowerLimit + .01f);	// i.e., 32000.01f would be the lower threshold
	float positive_threshold = (in_upperLimit - .01f);	// i.e., 32031.99f would be the upper threshold
	//std::cout << "Float to round: " << in_float << std::endl;
	//std::cout << "negative threshold: " << negative_threshold << std::endl;
	//std::cout << "positive threshold: " << positive_threshold << std::endl;
	if (in_float <= negative_threshold)
	{
		//std::cout << "****rounding to negative threshold...new value is: ";
		returnFloat = in_lowerLimit;
		//std::cout << returnFloat << std::endl;
	}
	else if (in_float >= positive_threshold)
	{
		//std::cout << "****rounding to positive threshold...new value is: ";
		returnFloat = in_upperLimit;
		//std::cout << returnFloat << std::endl;
	}
	else
	{
		//std::cout << "****no special rounding required...reducing to nearest floor: ";
		returnFloat = (floor(in_float * 100)) / 100;
		//std::cout << returnFloat << std::endl;
		//if (returnFloat == 32000.01f)
		//{
		//	std::cout << "precision check validated for 32000.01f..." << std::endl;
		//};
	}
	//std::cout << "Return float value is: " << returnFloat << std::endl;
	return returnFloat;
}

ECBPolyPoint IndependentUtils::roundToNearestBlueprintLineOrCorner(int in_xoryorz, ECBPolyPoint in_polyPoint, int in_lineOrCorner, ECBBorderValues* in_blueprintBorderValuesRef)
{
	ECBPolyPoint calibratedPoint = in_polyPoint;
	// calibrate points, to remove any over shot -- i.e., 32.001f becomes 32.0f, -0.001f becomes 0.0f (safety precaution)
	//std::cout << "===============> Test float values: " << std::endl;
	// std::cout << std::fixed << std::setprecision(7);
	//std::cout << "test x: " << in_polyPoint.x << std::endl;
	//std::cout << "test y: " << in_polyPoint.y << std::endl;
	//std::cout << "test z: " << in_polyPoint.z << std::endl;

	// calibrate X
	if (calibratedPoint.x < in_blueprintBorderValuesRef->negXlimit)	// check if x is less than the negative x limit
	{
		calibratedPoint.x = in_blueprintBorderValuesRef->negXlimit;
	}
	else if (calibratedPoint.x > in_blueprintBorderValuesRef->posXlimit)	// check if x is greater than the positive x limit
	{
		calibratedPoint.x = in_blueprintBorderValuesRef->posXlimit;
	}

	// calibrate Y
	if (calibratedPoint.y < in_blueprintBorderValuesRef->negYlimit)
	{
		calibratedPoint.y = in_blueprintBorderValuesRef->negYlimit;
	}
	else if (calibratedPoint.y > in_blueprintBorderValuesRef->posYlimit)
	{
		calibratedPoint.y = in_blueprintBorderValuesRef->posYlimit;
	}

	// calibrate Z
	if (calibratedPoint.z < in_blueprintBorderValuesRef->negZlimit)
	{
		calibratedPoint.z = in_blueprintBorderValuesRef->negZlimit;
	}
	else if (calibratedPoint.z > in_blueprintBorderValuesRef->posZlimit)
	{
		calibratedPoint.z = in_blueprintBorderValuesRef->posZlimit;
	}

	// determine precision limits
	ECBPolyPoint precisionLimit = IndependentUtils::determinePolyPointPrecisionLimits(calibratedPoint);
	float precisePosXLimit = (in_blueprintBorderValuesRef->posXlimit - 1.0f) + precisionLimit.x;
	float preciseNegXLimit = (in_blueprintBorderValuesRef->negXlimit) + (1.0f - precisionLimit.x);
	float precisePosYLimit = (in_blueprintBorderValuesRef->posYlimit - 1.0f) + precisionLimit.y;
	float preciseNegYLimit = (in_blueprintBorderValuesRef->negYlimit) + (1.0f - precisionLimit.y);
	float precisePosZLimit = (in_blueprintBorderValuesRef->posZlimit - 1.0f) + precisionLimit.z;
	float preciseNegZLimit = (in_blueprintBorderValuesRef->negZlimit) + (1.0f - precisionLimit.z);

	//std::cout << "Limits are: " << std::endl;
	//std::cout << "pos x: " << precisePosXLimit << std::endl;
	//std::cout << "neg x: " << preciseNegXLimit << std::endl;
	//std::cout << "pos y: " << precisePosYLimit << std::endl;
	//std::cout << "neg y: " << preciseNegYLimit << std::endl;
	//std::cout << "pos z: " << precisePosZLimit << std::endl;
	//std::cout << "neg z: " << preciseNegZLimit << std::endl;


	if (in_lineOrCorner == 0)		// if the value is 0, it's a corner. Just round to the nearest border in this case. All point's should be at a border
	{
		//std::cout << "|||||||||| WARNING: corner detected " << std::endl;
		//std::cout << in_xoryorz << std::endl;
		// Round other points to X
		//if (in_xoryorz == 0)
		//{
			//OrganicUtils::roundToAppropriatePrecision(in_XYZinterceptCoords.triPoints[0], in_Key1);


			// calc for x
		if (in_polyPoint.x > precisePosXLimit)
		{
			calibratedPoint.x = in_blueprintBorderValuesRef->posXlimit;
		}
		else if (in_polyPoint.x < preciseNegXLimit)
		{
			calibratedPoint.x = in_blueprintBorderValuesRef->negXlimit;
		}

		// calc for y
		if (in_polyPoint.y > precisePosYLimit)	// is y greater than 0.5f? if yes, then round to 1.0f
		{
			calibratedPoint.y = in_blueprintBorderValuesRef->posYlimit;
		}
		else if (in_polyPoint.y < preciseNegYLimit)
		{
			calibratedPoint.y = in_blueprintBorderValuesRef->negYlimit;
		}

		// calc for z
		if (in_polyPoint.z > precisePosZLimit) // is z greater than 0.5f? if yes, then round to 1.0f
		{
			calibratedPoint.z = in_blueprintBorderValuesRef->posZlimit;
		}
		else if (in_polyPoint.z < preciseNegZLimit)
		{
			calibratedPoint.z = in_blueprintBorderValuesRef->negZlimit;
		}

		//}

		// Round other points to Y
		//else if (in_xoryorz == 1)
		//{

		//}

		// Round other point to Z
		//else if (in_xoryorz == 2)
		//{

		//}

	}
	else if (in_lineOrCorner == 1)	// if the value is 1, it means the point is on a line. That also means one point doesn't exist precisely on the line; we must round only one point
									// to either the x or y or z. For example, if the point SHOULD be at x = 32.0f, y = 16.0f, and z = 32.0f, x and z must be similiar. However,
									// due to multiplying of floats not being exact, this will likely be something like 32.0f, 15.9999f, and 31.99999f (if we were checking the point's location from the results of an x-intercept slope). 
									// This will cause the if/then logic to fail when this data is returned, if the value to check is not precisely 32.f.
	{
		// Round a second point to be "like" X
		if (in_xoryorz == 0)
		{
			if (in_polyPoint.y > precisePosYLimit)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = in_blueprintBorderValuesRef->posYlimit;
			}
			else if (in_polyPoint.y < preciseNegYLimit)
			{
				calibratedPoint.y = in_blueprintBorderValuesRef->negYlimit;
			}

			// check for z
			if (in_polyPoint.z > precisePosZLimit) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.z = in_blueprintBorderValuesRef->posZlimit;
			}
			else if (in_polyPoint.z < preciseNegZLimit)
			{
				calibratedPoint.z = in_blueprintBorderValuesRef->negZlimit;
			}
		}

		// Round a second point to be "like" Y
		else if (in_xoryorz == 1)
		{
			// check for x
			if (in_polyPoint.x > precisePosXLimit)
			{
				calibratedPoint.x = in_blueprintBorderValuesRef->posXlimit;
			}
			else if (in_polyPoint.x < preciseNegXLimit)
			{
				calibratedPoint.x = in_blueprintBorderValuesRef->negXlimit;
			}

			// check for z
			if (in_polyPoint.z > precisePosZLimit)
			{
				calibratedPoint.z = in_blueprintBorderValuesRef->posZlimit;
			}
			else if (in_polyPoint.z < preciseNegZLimit)
			{
				calibratedPoint.z = in_blueprintBorderValuesRef->negZlimit;
			}
		}

		// Round a second poin tto be "like" z
		else if (in_xoryorz == 2)
		{
			//std::cout << "Z entry detected " << std::endl;
			if (in_polyPoint.x > precisePosXLimit)
			{
				calibratedPoint.x = in_blueprintBorderValuesRef->posXlimit;
			}
			else if (in_polyPoint.x < preciseNegXLimit)
			{
				calibratedPoint.x = in_blueprintBorderValuesRef->negXlimit;
			}

			// check for y
			if (in_polyPoint.y > precisePosYLimit)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = in_blueprintBorderValuesRef->posYlimit;
			}
			else if (in_polyPoint.y < preciseNegYLimit)
			{
				calibratedPoint.y = in_blueprintBorderValuesRef->negYlimit;
			}
		}
	}

	//std::cout << "|||| Calibrated values are: " << std::endl;
	//std::cout << "x: " << calibratedPoint.x << std::endl;
	//std::cout << "y: " << calibratedPoint.y << std::endl;
	//std::cout << "z: " << calibratedPoint.z << std::endl;

	//if (calibratedPoint.z == 32032.00f)
	//{
		//std::cout << "Hey!!" << std::endl;
	//}
	return calibratedPoint;
}

ECBPolyPoint IndependentUtils::determinePolyPointPrecisionLimits(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint returnPoint;
	// check for x
	if ((in_polyPoint.x <= -10000.00f) || (in_polyPoint.x >= 10000.00f))
	{
		returnPoint.x = .99f;
	}
	else if ((in_polyPoint.x <= -1000.0f) || (in_polyPoint.x >= 1000.0f))
	{
		returnPoint.x = .999f;
	}
	else if ((in_polyPoint.x <= -100.0f) || (in_polyPoint.x >= 100.0f))
	{
		returnPoint.x = .9999f;
	}
	else
	{
		returnPoint.x = .9999f;
	}

	// check for y
	if ((in_polyPoint.y <= -10000.00f) || (in_polyPoint.y >= 10000.00f))
	{
		returnPoint.y = .99f;
	}
	else if ((in_polyPoint.y <= -1000.0f) || (in_polyPoint.y >= 1000.0f))
	{
		returnPoint.y = .999f;
	}
	else if ((in_polyPoint.y <= -100.0f) || (in_polyPoint.y >= 100.0f))
	{
		returnPoint.y = .9999f;
	}
	else
	{
		returnPoint.y = .9999f;
	}

	// check for z
	if ((in_polyPoint.z <= -10000.00f) || (in_polyPoint.z >= 10000.00f))
	{
		returnPoint.z = .99f;
	}
	else if ((in_polyPoint.z <= -1000.0f) || (in_polyPoint.z >= 1000.0f))
	{
		returnPoint.z = .999f;
	}
	else if ((in_polyPoint.z <= -100.0f) || (in_polyPoint.z >= 100.0f))
	{
		returnPoint.z = .9999f;
	}
	else
	{
		returnPoint.z = .9999f;
	}


	return returnPoint;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::getBlueprintCalibratedKey(ECBPolyPoint in_originPoint, ECBPolyPoint in_interceptPoint, ECBBorderLineList* in_borderLineListRef)
{
	EnclaveKeyDef::EnclaveKey valueToReturn;	// for returning the appropriate move key
	ECBPPOrientationResults results = IndependentUtils::GetBlueprintPointOrientation(in_interceptPoint, in_borderLineListRef);	// get the point's orientation
	valueToReturn = lookupBlueprintBorderKey(results, in_borderLineListRef, in_originPoint, in_interceptPoint);									// get the move key
	return valueToReturn;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::lookupBlueprintBorderKey(ECBPPOrientationResults in_results, ECBBorderLineList* in_borderLineListRef, ECBPolyPoint in_originPoint, ECBPolyPoint in_interceptPoint)
{
	EnclaveKeyDef::EnclaveKey returnKey;
	if (in_results.otype == ECBPPOrientations::CORNER)	// corner lookups
	{
		//std::cout << "CORNER HIT" << std::endl;
		if (in_results.osubtype == ECBPPOrientations::CORNER_LOWERNE)
		{
			//std::cout << "LOWER NE" << std::endl;
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_LowerNE.borderValues);
			//returnKey = in_borderLineListRef->corner_LowerNE.cornerAmpValues.getKey();
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_LowerNE.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_LOWERSE)
		{
			//std::cout << "LOWER SE discovered!!! " << std::endl;
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_LowerSE.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_LowerSE.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_LOWERSW)
		{
			//std::cout << "LOWER SW" << std::endl;
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_LowerSW.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_LowerSW.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_LOWERNW)
		{
			//std::cout << "LOWER NW" << std::endl;
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_LowerNW.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_LowerNW.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_UPPERNE)
		{
			//std::cout << "UPPER NE" << std::endl;
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_UpperNE.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_UpperNE.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_UPPERSE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_UpperSE.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_UpperSE.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_UPPERSW)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_UpperSW.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_UpperSW.cornerAmpValues, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::CORNER_UPPERNW)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->corner_UpperNW.borderValues);
			returnKey = getBorderShiftResult(in_borderLineListRef->corner_UpperNW.cornerAmpValues, in_originPoint, in_interceptPoint);
		}

	}
	else if (in_results.otype == ECBPPOrientations::LINE)
	{
		// X AXIS
		if (in_results.osubtype == ECBPPOrientations::XAXIS_LOWERNORTH)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Xaxis_lowerNorth);
			returnKey = getBorderShiftResult(in_borderLineListRef->Xaxis_lowerNorth, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::XAXIS_LOWERSOUTH)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Xaxis_lowerSouth);
			returnKey = getBorderShiftResult(in_borderLineListRef->Xaxis_lowerSouth, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::XAXIS_UPPERNORTH)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Xaxis_upperNorth);
			returnKey = getBorderShiftResult(in_borderLineListRef->Xaxis_upperNorth, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::XAXIS_UPPERSOUTH)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Xaxis_upperSouth);
			returnKey = getBorderShiftResult(in_borderLineListRef->Xaxis_upperSouth, in_originPoint, in_interceptPoint);
		}

		// Y AXIS
		else if (in_results.osubtype == ECBPPOrientations::YAXIS_NORTHEAST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Yaxis_northEast);
			returnKey = getBorderShiftResult(in_borderLineListRef->Yaxis_northEast, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::YAXIS_NORTHWEST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Yaxis_northWest);
			returnKey = getBorderShiftResult(in_borderLineListRef->Yaxis_northWest, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::YAXIS_SOUTHEAST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Yaxis_southEast);
			//returnKey = in_borderLineListRef->Yaxis_southEast.getKey();
			returnKey = getBorderShiftResult(in_borderLineListRef->Yaxis_southEast, in_originPoint, in_interceptPoint);
			//std::cout << ":::: Return key here will be: " << returnKey.x << ", " << returnKey.y << ", " << returnKey.z << std::endl;
		}
		else if (in_results.osubtype == ECBPPOrientations::YAXIS_SOUTHWEST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Yaxis_southWest);
			returnKey = getBorderShiftResult(in_borderLineListRef->Yaxis_southWest, in_originPoint, in_interceptPoint);
		}

		// Z AXIS
		else if (in_results.osubtype == ECBPPOrientations::ZAXIS_LOWEREAST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Zaxis_lowerEast);
			returnKey = getBorderShiftResult(in_borderLineListRef->Zaxis_lowerEast, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::ZAXIS_LOWERWEST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Zaxis_lowerWest);
			returnKey = getBorderShiftResult(in_borderLineListRef->Zaxis_lowerWest, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::ZAXIS_UPPEREAST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Zaxis_upperEast);
			returnKey = getBorderShiftResult(in_borderLineListRef->Zaxis_upperEast, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::ZAXIS_UPPERWEST)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->Zaxis_upperWest);
			returnKey = getBorderShiftResult(in_borderLineListRef->Zaxis_upperWest, in_originPoint, in_interceptPoint);
		}
	}

	else if (in_results.otype == ECBPPOrientations::FACE)
	{
		if (in_results.osubtype == ECBPPOrientations::NORTHFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->NorthFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->NorthFace, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::SOUTHFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->SouthFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->SouthFace, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::WESTFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->WestFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->WestFace, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::EASTFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->EastFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->EastFace, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::TOPFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->TopFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->TopFace, in_originPoint, in_interceptPoint);
		}
		else if (in_results.osubtype == ECBPPOrientations::BOTTOMFACE)
		{
			//metaToReturn.applyBlockBorder(in_lineListRef->BottomFace);
			returnKey = getBorderShiftResult(in_borderLineListRef->BottomFace, in_originPoint, in_interceptPoint);
		}
	}
	return returnKey;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::getBorderShiftResult(ECBBorder in_Border, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	EnclaveKeyDef::EnclaveKey shiftKey;
	//std::cout << "BorderShift point A: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;
	//std::cout << "BorderShift point B: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;

	//std::cout << "Border shift data: " << in_Border.move_x << ", " << in_Border.move_y << ", " << in_Border.move_z << std::endl;

	float x_diff = in_pointB.x - in_pointA.x;
	float y_diff = in_pointB.y - in_pointA.y;
	float z_diff = in_pointB.z - in_pointA.z;

	// check for if x_diff, y_diff, z_diff is zero (would indicate zero slope)
	float normalized_x_diff = 0.0f;
	if (x_diff != 0.0f)
	{
		normalized_x_diff = float(x_diff / abs(x_diff));
	}

	float normalized_y_diff = 0.0f;
	if (y_diff != 0.0f)
	{
		normalized_y_diff = float(y_diff / abs(y_diff));
	}

	float normalized_z_diff = 0.0f;
	if (z_diff != 0.0f)
	{
		normalized_z_diff = float(z_diff / abs(z_diff));
	}
	//std::cout << "normal x: " << normalized_x_diff << std::endl;
	//std::cout << "normal y: " << normalized_y_diff << std::endl;
	//std::cout << "normal z: " << normalized_z_diff << std::endl;

	//std::cout << in_Border.move_x << std::endl;
	if (normalized_x_diff == in_Border.move_x)
	{
		shiftKey.x += int(in_Border.move_x);
		//std::cout << "shiftKey: move_X" << std::endl;
	}
	if (normalized_y_diff == in_Border.move_y)
	{
		shiftKey.y += int(in_Border.move_y);
		//std::cout << "shiftKey: move_Y" << std::endl;
	}
	if (normalized_z_diff == in_Border.move_z)
	{
		shiftKey.z += int(in_Border.move_z);
		//std::cout << "shiftKey: move_Z" << std::endl;
	}

	//std::cout << "Shift key x is: " << shiftKey.x << std::endl;
	//std::cout << "Shift key y is: " << shiftKey.y << std::endl;
	//std::cout << "Shift key z is: " << shiftKey.z << std::endl;

	return shiftKey;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::getUncalibratedBlueprintKeyForPoint(ECBPolyPoint in_point)
{
	EnclaveKeyDef::EnclaveKey rawKey(0,0,0);

	// check x
	if (in_point.x != 0.0f)
	{
		rawKey.x = floor(in_point.x / 32);
	}

	// check y
	if (in_point.y != 0.0f)
	{
		rawKey.y = floor(in_point.y / 32);
	}

	// check z
	if (in_point.z != 0.0f)
	{
		rawKey.z = floor(in_point.z / 32);
	}

	return rawKey;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::getUncalibratedBlueprintKeyForPoint(DoublePoint in_point)
{
	EnclaveKeyDef::EnclaveKey rawKey(0, 0, 0);

	// check x
	if (in_point.x != 0.0f)
	{
		rawKey.x = floor(in_point.x / 32);
	}

	// check y
	if (in_point.y != 0.0f)
	{
		rawKey.y = floor(in_point.y / 32);
	}

	// check z
	if (in_point.z != 0.0f)
	{
		rawKey.z = floor(in_point.z / 32);
	}

	return rawKey;
}

ECBPolyPoint IndependentUtils::getBlueprintTracingEndpointForIsolatedPrimaryT2(ECBPolyPoint in_pointA, ECBPolyPoint in_slope, ECBBorderLineList* in_borderLineList, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	//std::cout << "*********** calling for primary t2 isolation; Key is: " << in_blueprintKey.x << ", " << in_blueprintKey.y << ", " << in_blueprintKey.z << std::endl;
	//std::cout << "### Slope is: " << in_slope.x << ", " << in_slope.y << ", " << in_slope.z << std::endl;
	//std::cout << "PT2 x: " << in_pointA.x << std::endl;
	//std::cout << "PT2 y: " << in_pointA.y << std::endl;
	//std::cout << "PT2 z: " << in_pointA.z << std::endl;
	ECBPolyPoint returnPoint;
	ECBIntersectMeta tempIntersectMeta;
	ECBPolyPoint resultantSlope;
	resultantSlope.x = in_slope.x;
	resultantSlope.y = in_slope.y;
	resultantSlope.z = in_slope.z;

	//std::cout << "Chosen slope is: " << resultantSlope.x << ", " << resultantSlope.y << ", " << resultantSlope.z << std::endl;
	// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
	int x_dir = 0;
	int y_dir = 0;
	int z_dir = 0;

	float x_interceptCoord = 0.0f;
	float y_interceptCoord = 0.0f;
	float z_interceptCoord = 0.0f;

	// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
	float powSlopeX = pow(resultantSlope.x, 2.0f);
	float powSlopeY = pow(resultantSlope.y, 2.0f);
	float powSlopeZ = pow(resultantSlope.z, 2.0f);
	float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

	// create a temporary struct to store tracked data
	struct travelDistances
	{
		float timeToComplete = 0.0f;
		float length = 0.0f;
		int direction = 0;
		ECBPolyPoint point;
	};
	travelDistances travelResults[3];	// first index value (0) = x result
										// second index value (1) = y result
										// third index value (2) = z result

										// ******for this logic, check lines starting at 1891 in OrganicSystem.cpp
										// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	ECBPolyPoint calculatedPoint_for_y;
	ECBPolyPoint calculatedPoint_for_z;


	if (resultantSlope.x > 0)
	{
		x_dir = 1;			// going towards positive x 
		x_interceptCoord = float((in_blueprintKey.x * 32) + 32);								// x_interceptCoord is the precise location of x at the EAST face border
		float origin_to_border_x_diff = (x_interceptCoord - in_pointA.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
		time_to_complete_x_traversal = origin_to_border_x_diff / resultantSlope.x;	// get the distance that the ray has to travel to get to this value of x
																					//ECBPolyPoint calculatedPoint_for_x;					// the actual point as it would exist on the EAST face
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
		travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
		travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[0].direction = x_dir;

	}
	else if (resultantSlope.x < 0)
	{
		x_dir = -1;			// going towards negative x
		x_interceptCoord = float(in_blueprintKey.x * 32);
		float origin_to_border_x_diff = abs(x_interceptCoord - in_pointA.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / resultantSlope.x);	// ""
																						//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the WEST face
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = in_pointA.y + (resultantSlope.y * time_to_complete_x_traversal);	// "" 
		calculatedPoint_for_x.z = in_pointA.z + (resultantSlope.z * time_to_complete_x_traversal);	// ""

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[0].timeToComplete = time_to_complete_x_traversal;			// store the distance to get to this point (0 = X direction/slope)
		travelResults[0].point = calculatedPoint_for_x;								// store the actual point for later
		travelResults[0].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[0].direction = x_dir;
	}

	//... for y												********************************************************************************************************************************************************************
	if (resultantSlope.y > 0)
	{
		y_dir = 1;		// going towards positive y
		y_interceptCoord = float((in_blueprintKey.y * 32) + 32);								// y_interceptCoord is the precise location of y at the TOP face border
		float origin_to_border_y_diff = y_interceptCoord - in_pointA.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
		time_to_complete_y_traversal = origin_to_border_y_diff / resultantSlope.y;	// get the distance this ray has to travel to get to this value of y
																					//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the TOP face
		calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
		travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
		travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[1].direction = y_dir;

	}
	else if (resultantSlope.y < 0)
	{
		y_dir = -1;
		y_interceptCoord = float(in_blueprintKey.y * 32);
		float origin_to_border_y_diff = abs(y_interceptCoord - in_pointA.y);
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / resultantSlope.y);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_y.x = in_pointA.x + (resultantSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = in_pointA.z + (resultantSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[1].timeToComplete = time_to_complete_y_traversal;			// store the distance to get to this point (1 = Y direction/slope)
		travelResults[1].point = calculatedPoint_for_y;								// store the actual point for later
		travelResults[1].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[1].direction = y_dir;
	}

	//... for z
	if (resultantSlope.z > 0)
	{
		z_dir = 1;
		z_interceptCoord = float((in_blueprintKey.z * 32) + 32);
		float origin_to_border_z_diff = z_interceptCoord - in_pointA.z;
		time_to_complete_z_traversal = origin_to_border_z_diff / resultantSlope.z;
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
		travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
		travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[2].direction = z_dir;
	}
	else if (resultantSlope.z < 0)
	{
		z_dir = -1;
		z_interceptCoord = float(in_blueprintKey.z * 32);
		float origin_to_border_z_diff = abs(z_interceptCoord - in_pointA.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / resultantSlope.z);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_z.x = in_pointA.x + (resultantSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = in_pointA.y + (resultantSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - in_pointA.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - in_pointA.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - in_pointA.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		travelResults[2].timeToComplete = time_to_complete_z_traversal;			// store the distance to get to this point (1 = Y direction/slope)
		travelResults[2].point = calculatedPoint_for_z;								// store the actual point for later
		travelResults[2].length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		travelResults[2].direction = z_dir;
	}


	// find true key shift value (in case the point is exactly on a border line or border corner of this ECB's x/y/z limits)
	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;

	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;
	//std::cout << "###### Pre calibrate pair call" << std::endl;
	ECBCalibratedPointPair calibratedPair = compareAndCalibrateDistances(&triPointParam, distanceValues, resultantSlope, in_blueprintKey);
	//std::cout << "Post calibrate pair call" << std::endl;

	ECBPolyPoint slopeDirection;
	slopeDirection.x = float(x_dir);
	slopeDirection.y = float(y_dir);
	slopeDirection.z = float(z_dir);

	distanceValues = calibratedPair.calibratedDistance;
	triPointParam = calibratedPair.calibratedPointTri;

	tempIntersectMeta = findBlueprintBorderMoveMeta(in_blueprintKey, in_pointA, distanceValues, slopeDirection, triPointParam);			// find key for shifting 
	//std::cout << "++++++++++++++++++++++++||||| Intersect return meta point: " << tempIntersectMeta.intersectedPoint.x << ", " << tempIntersectMeta.intersectedPoint.y << ", " << tempIntersectMeta.intersectedPoint.z << std::endl;

// cycle through results
/*
float currentShortestTime = 0.0f;
int currentShortestIndex = -1;		// -1 is default (no valid index selected)
for (int x = 0; x < 3; x++)
{
if (travelResults[x].timeToComplete != 0.0f)	// is there an actual valid time for this?
{
if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
{
currentShortestTime = travelResults[x].timeToComplete;	// set the initial length
currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
}
else if (currentShortestTime != 0.0f)
{
if (travelResults[x].timeToComplete < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
{
currentShortestTime = travelResults[x].timeToComplete;	// reset the length
currentShortestIndex = x;							// reset the index
}
}
}
}

if (currentShortestIndex == 0)	// x was found
{
if (travelResults[currentShortestIndex].direction == 1)
{
//std::cout << "Line intersects at +X" << std::endl;
}
else if (travelResults[currentShortestIndex].direction == -1)
{
//std::cout << "Line intersects at -X" << std::endl;
}
}
else if (currentShortestIndex == 1)
{
if (travelResults[currentShortestIndex].direction == 1)
{
//std::cout << "Line intersects at +Y" << std::endl;
}
else if (travelResults[currentShortestIndex].direction == -1)
{
//std::cout << "Line intersects at -Y" << std::endl;
}
}
else if (currentShortestIndex == 2)
{
if (travelResults[currentShortestIndex].direction == 1)
{
//std::cout << "Line intersects at +Z" << std::endl;
}
else if (travelResults[currentShortestIndex].direction == -1)
{
//std::cout << "Line intersects at -Z" << std::endl;
}
}
*/
//intersectResult.faceID = travelResults[currentShortestIndex].direction;
//intersectResult.intersectedPoint = travelResults[currentShortestIndex].point;
//intersectResult.lineLength = travelResults[currentShortestIndex].length;
//intersectResult.originPoint = in_pointA;
	return tempIntersectMeta.intersectedPoint;
}

ECBBorderValues IndependentUtils::getBlueprintLimits(EnclaveKeyDef::EnclaveKey in_Key)
{
	//std::cout << "whoa doggy" << std::endl;
	ECBBorderValues  borderValues;
	borderValues.negXlimit = in_Key.x * 32.0f;			// west border 
	borderValues.posXlimit = (in_Key.x * 32.0f) + 32.0f;	// east border
	borderValues.negYlimit = in_Key.y * 32.0f;			// bottom border 
	borderValues.posYlimit = (in_Key.y * 32.0f) + 32.0f;	// top border
	borderValues.negZlimit = in_Key.z * 32.0f;			// north border
	borderValues.posZlimit = (in_Key.z * 32.0f) + 32.0f;	// south border
	return borderValues;
}

ECBPPOrientationResults IndependentUtils::GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList in_blockBorders)	// default version of GetPointOrientation
{
	ECBPPOrientationResults returnResults;

	// STEP 1: check to see if point is in a corner ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//std::cout << std::fixed << std::setprecision(7);
	//std::cout << "Point to check here is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "debug of x in point is: " << float(in_pointToCheck.x) << std::endl;
	ECBPolyPoint testPoint = IndependentUtils::roundPolyPointToHundredths(in_pointToCheck);
	//std::cout << "converted value of x in point is: " << testPoint.x << ", " << testPoint.y << ", " << testPoint.z << std::endl;
	//std::cout << " debug of x upperSE is: " << float(in_blockBorders.corner_UpperSE.pointXYZ.x) << std::endl;

	/*
	if (float(in_pointToCheck.x) == float(in_blockBorders.corner_LowerNW.pointXYZ.x))
	{
		//std::cout << "debug: X match! " << std::endl;
	}
	else if (!(float(in_pointToCheck.x) == float(in_blockBorders.corner_LowerNW.pointXYZ.x)))
	{
		//std::cout << "Whoa! no match! " << std::endl;
		//std::cout << "int'd value 1 is: " << int(in_pointToCheck.x) << std::endl;
		//std::cout << "int'd value 2 is: " << int(in_blockBorders.corner_LowerNW.pointXYZ.x) << std::endl;
	}

	if (in_pointToCheck.y == in_blockBorders.corner_LowerNW.pointXYZ.y)
	{
		//std::cout << "debug: Y match! " << std::endl;
	}
	if (in_pointToCheck.z == in_blockBorders.corner_LowerNW.pointXYZ.z)
	{
		//std::cout << "debug: Z match! " << std::endl;
	}
	*/

	if (in_pointToCheck.x == in_blockBorders.corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_LowerNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_LowerNW.pointXYZ.z)		// Lower NW
	{
		//std::cout << "||||||||||||| Get Point Orientation: Lower NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_LowerSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_LowerSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_LowerSW.pointXYZ.z)		// Lower SW
	{
		//std::cout << "Get Point Orientation: Lower SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_LowerSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_LowerSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_LowerSE.pointXYZ.z)		// Lower SE
	{
		//std::cout << "Get Point Orientation: Lower SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_LowerNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_LowerNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_LowerNE.pointXYZ.z)		// Lower NE
	{
		//std::cout << "Get Point Orientation: Lower NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_UpperNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_UpperNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_UpperNW.pointXYZ.z)		// Upper NW
	{
		//std::cout << "Get Point Orientation: Upper NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_UpperSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_UpperSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_UpperSW.pointXYZ.z)		// Upper SW
	{
		//std::cout << "Get Point Orientation: Upper SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_UpperSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_UpperSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_UpperSE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders.corner_UpperNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders.corner_UpperNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders.corner_UpperNE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
	}

	// STEP 2: check to see if point is on a line ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	// Z LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// >>>>> Lower West Z line
	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_LowerNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders.corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(blockBorders.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Get Point Orientation: Lower west line hit " << std::endl;
	}

	// >>>>> Upper West line check
	else if ((in_pointToCheck.x == in_blockBorders.corner_UpperNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders.corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders.corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders.Zaxis_upperWest);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Zaxis_upperWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// >>>>> Upper East line check
	else if ((in_pointToCheck.x == in_blockBorders.corner_UpperNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders.corner_UpperNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders.corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders.Zaxis_upperEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Zaxis_upperEast, in_originPoint, in_pointToCheck);	// get the shifting key				
		//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		// std::cout << "|||||||||| Get Point Orientation: Upper east line hit " << std::endl;
	}
	// >>>>> Lower East line check
	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders.corner_LowerNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_LowerNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders.corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders.Zaxis_lowerEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Zaxis_lowerEast, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}









	// Y LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerNE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders.corner_UpperNE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Yaxis_northEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// South East line
	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerSE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerSE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerSE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders.corner_UpperSE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Yaxis_southEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// South West line
	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerSW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerSW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders.corner_UpperSW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Yaxis_southWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// North West line
	else if ((in_pointToCheck.x == in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders.corner_UpperNW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Yaxis_northWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

	}




	// X LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.y == in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.x < in_blockBorders.corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders.corner_LowerSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerSW.pointXYZ.x	&&	in_pointToCheck.x < in_blockBorders.corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders.corner_UpperSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_UpperSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_UpperSW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders.corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders.corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders.corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_UpperNW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders.corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders.Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
	}

	// STEP 3: check to see if point is on a face ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// West face check
	else if ((in_pointToCheck.x == 0.0f)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders.corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders.corner_UpperSW.pointXYZ.z)

		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::WESTFACE;
	}

	// North face check
	else if ((in_pointToCheck.z == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders.corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::NORTHFACE;
	}

	// East face check
	else if ((in_pointToCheck.x == 1.0f)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders.corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders.corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "EAST FACE hit! " << std::endl;
	}

	// South face check
	else if ((in_pointToCheck.z == 1.0f)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders.corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders.corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
	}

	// Top face check
	else if ((in_pointToCheck.y == 1.0f)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders.corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::TOPFACE;
	}

	// Bottom Face check
	else if ((in_pointToCheck.y == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders.corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders.corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders.corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders.corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
	}

	else
	{
		std::cout << "!!!! Warning !!!! point has NO FACE " << std::endl;
	}

	return returnResults;
}

ECBPPOrientationResults IndependentUtils::GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList* in_blockBorders)	// version of GetPointOrientation which takes a pointer instead of a copy of in_blockBorders
{
	ECBPPOrientationResults returnResults;
	// STEP 1: check to see if point is in a corner ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//std::cout << std::fixed << std::setprecision(7);
	//std::cout << "Point to check here is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "debug of x in point is: " << float(in_pointToCheck.x) << std::endl;
	ECBPolyPoint testPoint = IndependentUtils::roundPolyPointToHundredths(in_pointToCheck);
	//std::cout << "converted value of x in point is: " << testPoint.x << ", " << testPoint.y << ", " << testPoint.z << std::endl;
	//std::cout << " debug of x upperSE is: " << float(in_blockBorders->corner_UpperSE.pointXYZ.x) << std::endl;

	/*
	if (float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x))
	{
	//std::cout << "debug: X match! " << std::endl;
	}
	else if (!(float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x)))
	{
	//std::cout << "Whoa! no match! " << std::endl;
	//std::cout << "int'd value 1 is: " << int(in_pointToCheck.x) << std::endl;
	//std::cout << "int'd value 2 is: " << int(in_blockBorders->corner_LowerNW.pointXYZ.x) << std::endl;
	}

	if (in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
	{
	//std::cout << "debug: Y match! " << std::endl;
	}
	if (in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
	{
	//std::cout << "debug: Z match! " << std::endl;
	}
	*/

	if (in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)		// Lower NW
	{
		//std::cout << "||||||||||||| Get Point Orientation: Lower NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)		// Lower SW
	{
		//std::cout << "Get Point Orientation: Lower SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerSE.pointXYZ.z)		// Lower SE
	{
		//std::cout << "Get Point Orientation: Lower SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerNE.pointXYZ.z)		// Lower NE
	{
		//std::cout << "Get Point Orientation: Lower NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperNW.pointXYZ.z)		// Upper NW
	{
		//std::cout << "Get Point Orientation: Upper NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperSW.pointXYZ.z)		// Upper SW
	{
		//std::cout << "Get Point Orientation: Upper SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperSE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperNE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
	}

	// STEP 2: check to see if point is on a line ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	// Z LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// >>>>> Lower West Z line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_LowerNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(blockBorders.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Get Point Orientation: Lower west line hit " << std::endl;
	}

	// >>>>> Upper West line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_UpperNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_upperWest);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_upperWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// >>>>> Upper East line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_UpperNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_UpperNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_upperEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_upperEast, in_originPoint, in_pointToCheck);	// get the shifting key				
		//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "|||||||||| Get Point Orientation: Upper east line hit " << std::endl;
	}
	// >>>>> Lower East line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_LowerNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_LowerNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_lowerEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_lowerEast, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}









	// Y LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperNE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_northEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// South East line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerSE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerSE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperSE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_southEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// South West line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerSW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerSW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperSW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_southWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// North West line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_northWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

	}




	// X LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_LowerSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerSW.pointXYZ.x	&&	in_pointToCheck.x < in_blockBorders->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_UpperSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_UpperSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_UpperSW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_UpperNW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
	}

	// STEP 3: check to see if point is on a face ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// West face check
	else if ((in_pointToCheck.x == 0.0f)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)

		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::WESTFACE;
	}

	// North face check
	else if ((in_pointToCheck.z == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::NORTHFACE;
	}

	// East face check
	else if ((in_pointToCheck.x == 1.0f)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "EAST FACE hit! " << std::endl;
	}

	// South face check
	else if ((in_pointToCheck.z == 1.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
	}

	// Top face check
	else if ((in_pointToCheck.y == 1.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::TOPFACE;
	}

	// Bottom Face check
	else if ((in_pointToCheck.y == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
	}

	else
	{
		returnResults.otype = ECBPPOrientations::FREE;
		returnResults.osubtype = ECBPPOrientations::FREE;

		//std::cout << "!!!! Warning !!!! point has NO FACE " << std::endl;
	}




	return returnResults;
}

ECBPPOrientationResults IndependentUtils::getDynamicPointOrientation(ECBPolyPoint in_pointToCheck, DynamicBorderLineList* in_dynamicBorderLineListRef)
{
	ECBPPOrientationResults returnResults;
	// STEP 1: check to see if point is in a corner ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//std::cout << std::fixed << std::setprecision(7);
	//std::cout << "Point to check here is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "debug of x in point is: " << float(in_pointToCheck.x) << std::endl;
	ECBPolyPoint testPoint = IndependentUtils::roundPolyPointToHundredths(in_pointToCheck);
	//std::cout << "converted value of x in point is: " << testPoint.x << ", " << testPoint.y << ", " << testPoint.z << std::endl;
	//std::cout << " debug of x upperSE is: " << float(in_blockBorders->corner_UpperSE.pointXYZ.x) << std::endl;

	/*
	if (float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x))
	{
	//std::cout << "debug: X match! " << std::endl;
	}
	else if (!(float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x)))
	{
	//std::cout << "Whoa! no match! " << std::endl;
	//std::cout << "int'd value 1 is: " << int(in_pointToCheck.x) << std::endl;
	//std::cout << "int'd value 2 is: " << int(in_blockBorders->corner_LowerNW.pointXYZ.x) << std::endl;
	}

	if (in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
	{
	//std::cout << "debug: Y match! " << std::endl;
	}
	if (in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
	{
	//std::cout << "debug: Z match! " << std::endl;
	}
	*/

	if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.z)		// Lower NW
	{
		//std::cout << "||||||||||||| Get Point Orientation: Lower NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.z)		// Lower SW
	{
		//std::cout << "Get Point Orientation: Lower SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.z)		// Lower SE
	{
		//std::cout << "Get Point Orientation: Lower SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.z)		// Lower NE
	{
		//std::cout << "Get Point Orientation: Lower NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)		// Upper NW
	{
		//std::cout << "Get Point Orientation: Upper NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)		// Upper SW
	{
		//std::cout << "Get Point Orientation: Upper SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
	}
	else if (in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.x	&&		in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.y	&&		in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
	}

	// STEP 2: check to see if point is on a line ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	// Z LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// >>>>> Lower West Z line
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.z	&& in_pointToCheck.z < in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(blockBorders.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Get Point Orientation: Lower west line hit " << std::endl;
	}

	// >>>>> Upper West line check
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z	&& in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_dynamicBorderLineListRef->Zaxis_upperWest);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Zaxis_upperWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// >>>>> Upper East line check
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.z	&& in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_dynamicBorderLineListRef->Zaxis_upperEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Zaxis_upperEast, in_originPoint, in_pointToCheck);	// get the shifting key				
		//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "|||||||||| Get Point Orientation: Upper east line hit " << std::endl;
	}
	// >>>>> Lower East line check
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.z	&& in_pointToCheck.z < in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_dynamicBorderLineListRef->Zaxis_lowerEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Zaxis_lowerEast, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}









	// Y LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.y	&& in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Yaxis_northEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// South East line
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.y	&& in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Yaxis_southEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// South West line
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.y	&& in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Yaxis_southWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// North West line
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y	&& in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Yaxis_northWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

	}




	// X LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerSW.pointXYZ.x	&&	in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.x	&& in_pointToCheck.x < in_dynamicBorderLineListRef->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.x	&& in_pointToCheck.x < in_dynamicBorderLineListRef->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_dynamicBorderLineListRef->Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
	}

	// STEP 3: check to see if point is on a face ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// West face check
	else if ((in_pointToCheck.x == 0.0f)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)

		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::WESTFACE;
	}

	// North face check
	else if ((in_pointToCheck.z == 0.0f)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::NORTHFACE;
	}

	// East face check
	else if ((in_pointToCheck.x == in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "EAST FACE hit! " << std::endl;
	}

	// South face check
	else if ((in_pointToCheck.z == in_dynamicBorderLineListRef->corner_LowerSE.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
	}

	// Top face check
	else if ((in_pointToCheck.y == in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::TOPFACE;
	}

	// Bottom Face check
	else if ((in_pointToCheck.y == 0.0f)
		&&
		(in_pointToCheck.x > in_dynamicBorderLineListRef->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_dynamicBorderLineListRef->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_dynamicBorderLineListRef->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_dynamicBorderLineListRef->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
	}

	else
	{
		returnResults.otype = ECBPPOrientations::FREE;
		returnResults.osubtype = ECBPPOrientations::FREE;

		//std::cout << "!!!! Warning !!!! point has NO FACE " << std::endl;
	}




	return returnResults;
}

ECBPPOrientationResults IndependentUtils::GetEnclavePointOrientation(ECBPolyPoint in_pointToCheck, EnclaveBorderLineList* in_blockBorders)
{
	ECBPPOrientationResults returnResults;
	// STEP 1: check to see if point is in a corner ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//std::cout << std::fixed << std::setprecision(7);
	//std::cout << "### Enclave Point to check here is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "debug of x in point is: " << float(in_pointToCheck.x) << std::endl;
	ECBPolyPoint testPoint = IndependentUtils::roundPolyPointToHundredths(in_pointToCheck);
	//std::cout << "converted value of x in point is: " << testPoint.x << ", " << testPoint.y << ", " << testPoint.z << std::endl;
	//std::cout << " debug of x upperSE is: " << float(in_blockBorders->corner_UpperSE.pointXYZ.x) << std::endl;

	/*
	if (float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x))
	{
	//std::cout << "debug: X match! " << std::endl;
	}
	else if (!(float(in_pointToCheck.x) == float(in_blockBorders->corner_LowerNW.pointXYZ.x)))
	{
	//std::cout << "Whoa! no match! " << std::endl;
	//std::cout << "int'd value 1 is: " << int(in_pointToCheck.x) << std::endl;
	//std::cout << "int'd value 2 is: " << int(in_blockBorders->corner_LowerNW.pointXYZ.x) << std::endl;
	}

	if (in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
	{
	//std::cout << "debug: Y match! " << std::endl;
	}
	if (in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
	{
	//std::cout << "debug: Z match! " << std::endl;
	}
	*/

	if (in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)		// Lower NW
	{
		//std::cout << "||||||||||||| Get Point Orientation: Lower NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)		// Lower SW
	{
		//std::cout << "Get Point Orientation: Lower SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerSE.pointXYZ.z)		// Lower SE
	{
		//std::cout << "Get Point Orientation: Lower SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_LowerNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_LowerNE.pointXYZ.z)		// Lower NE
	{
		//std::cout << "Get Point Orientation: Lower NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperNW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperNW.pointXYZ.z)		// Upper NW
	{
		//std::cout << "Get Point Orientation: Upper NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperSW.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperSW.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperSW.pointXYZ.z)		// Upper SW
	{
		//std::cout << "Get Point Orientation: Upper SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperSE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperSE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperSE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
	}
	else if (in_pointToCheck.x == in_blockBorders->corner_UpperNE.pointXYZ.x	&&		in_pointToCheck.y == in_blockBorders->corner_UpperNE.pointXYZ.y	&&		in_pointToCheck.z == in_blockBorders->corner_UpperNE.pointXYZ.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
	}

	// STEP 2: check to see if point is on a line ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	// Z LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// >>>>> Lower West Z line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_LowerNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(blockBorders.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Get Point Orientation: Lower west line hit " << std::endl;
	}

	// >>>>> Upper West line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_UpperNW.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_upperWest);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_upperWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// >>>>> Upper East line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_UpperNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_UpperNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_upperEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_upperEast, in_originPoint, in_pointToCheck);	// get the shifting key				
		//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "|||||||||| Get Point Orientation: Upper east line hit " << std::endl;
	}
	// >>>>> Lower East line check
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y == in_blockBorders->corner_LowerNE.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_LowerNE.pointXYZ.z	&& in_pointToCheck.z < in_blockBorders->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_blockBorders->Zaxis_lowerEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Zaxis_lowerEast, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}









	// Y LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperNE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_northEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// South East line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerSE.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSE.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerSE.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperSE.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_southEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// South West line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerSW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerSW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperSW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_southWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}
	// North West line
	else if ((in_pointToCheck.x == in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y	&& in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Yaxis_northWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

	}




	// X LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.y == in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x	&&		in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_LowerSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_LowerSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerSW.pointXYZ.x	&&	in_pointToCheck.x < in_blockBorders->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_UpperSW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_UpperSW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_UpperSW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z == in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_UpperNW.pointXYZ.x	&& in_pointToCheck.x < in_blockBorders->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_blockBorders->Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
	}

	// STEP 3: check to see if point is on a face ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// West face check
	else if ((in_pointToCheck.x == 0.0f)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)

		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::WESTFACE;
		//std::cout << "Enclave: WEST FACE hit! " << std::endl;
	}

	// North face check
	else if ((in_pointToCheck.z == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::NORTHFACE;
		//std::cout << "Enclave: NORTH FACE hit! " << std::endl;
	}

	// East face check
	else if ((in_pointToCheck.x == 4.0f)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "Enclave: EAST FACE hit! " << std::endl;
	}

	// South face check
	else if ((in_pointToCheck.z == 4.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.y > in_blockBorders->corner_LowerNW.pointXYZ.y)
		&&
		(in_pointToCheck.y < in_blockBorders->corner_UpperNW.pointXYZ.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
		//std::cout << "Enclave: SOUTH FACE hit! " << std::endl;
	}

	// Top face check
	else if ((in_pointToCheck.y == 4.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::TOPFACE;
		//std::cout << "Enclave: TOP FACE hit! " << std::endl;
	}

	// Bottom Face check
	else if ((in_pointToCheck.y == 0.0f)
		&&
		(in_pointToCheck.x > in_blockBorders->corner_LowerNW.pointXYZ.x)
		&&
		(in_pointToCheck.x < in_blockBorders->corner_LowerNE.pointXYZ.x)
		&&
		(in_pointToCheck.z > in_blockBorders->corner_UpperNW.pointXYZ.z)
		&&
		(in_pointToCheck.z < in_blockBorders->corner_UpperSW.pointXYZ.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
		//std::cout << "Enclave: BOTTOM FACE hit! " << std::endl;
	}

	else
	{
		returnResults.otype = ECBPPOrientations::FREE;
		returnResults.osubtype = ECBPPOrientations::FREE;

		//std::cout << "!!!! Warning !!!! Enclave point has NO FACE " << std::endl;
	}




	return returnResults;
}

ECBPolyPoint IndependentUtils::applyEnclaveResetValuesToPoint(ECBPolyPoint in_pointToApplyTo, ECBPolyPoint in_resetValues, ECBPolyPoint in_moveValues)
{
	ECBPolyPoint returnPoint = in_pointToApplyTo;
	if ((in_resetValues.x != -1) && (in_moveValues.x != 0))
	{
		returnPoint.x = in_resetValues.x;
	}
	if ((in_resetValues.y != -1) && (in_moveValues.y != 0))
	{
		returnPoint.y = in_resetValues.y;
	}
	if ((in_resetValues.z != -1) && (in_moveValues.z != 0))
	{
		returnPoint.z = in_resetValues.z;
	}
	return returnPoint;
}

ECBPolyPointLocation IndependentUtils::getEnclavePolyPointLocation(ECBPolyPoint in_point, ECBBorderValues in_borderValues)
{
	ECBPolyPointLocation locationToReturn;
	//::cout << ">>>>>>>>>>>>>>>>>>>>>>>> Test call begins here: " << std::endl;
	//CursorPathTraceContainer tempContainer = OrganicUtils::testGetPreciseCoordinate(in_point.y);
	// check borders for X, 
	//std::cout << "value of x (NEW POLYPOINT CALL): " << in_point.x << std::endl;
	if (in_point.x == in_borderValues.posXlimit)		// East limit hit
	{
		//std::cout << "hey there!!! X hit East border..." << std::endl;
		locationToReturn.enclaveKey.x = 7;
		locationToReturn.blockKey.x = 3;
		locationToReturn.preciseCoord.x = 1.0f;

	}
	else if (in_point.x == in_borderValues.negXlimit)	// West limit hit
	{
		//std::cout << "hey there!!! X hit West border..." << std::endl;
		locationToReturn.enclaveKey.x = 0;
		locationToReturn.blockKey.x = 0;
		locationToReturn.preciseCoord.x = 0.0f;
	}
	else
	{
		/*
		//std::cout << "|||| (x) point being passed is: " << in_point.x << std::endl;
		CursorPathTraceContainer xContainer = OrganicUtils::getPreciseCoordinate(in_point.x);
		locationToReturn.enclaveKey.x = xContainer.EnclaveCoord;
		locationToReturn.blockKey.x = xContainer.BlockCoord;
		locationToReturn.preciseCoord.x = xContainer.ExactBlockCoord;
		*/

		//std::cout << "|||| (x) point being passed is: " << in_point.x << std::endl;
		if (in_point.x != 4.0f)
		{
			CursorPathTraceContainer xContainer = IndependentUtils::getPreciseCoordinate(in_point.x);
			locationToReturn.enclaveKey.x = xContainer.EnclaveCoord;
			locationToReturn.blockKey.x = xContainer.BlockCoord;
			locationToReturn.preciseCoord.x = xContainer.ExactBlockCoord;
		}
		else if (in_point.x == 4.0f)
		{
			locationToReturn.blockKey.x = 3;
			locationToReturn.preciseCoord.x = 1.0f;
		}
		//std::cout << "|||| value of precise coord: " << locationToReturn.preciseCoord.x << std::endl;
		//std::cout << "Chunk and block data for point (" << in_point.x << ", " << in_point.y << ", " << in_point.z << ") " << std::endl;
		//std::cout << "Chunk location for X: " << locationToReturn.enclaveKey.x << std::endl;
		//std::cout << "Block location for X: " << locationToReturn.blockKey.x << std::endl;

	}

	// check borders for Y,
	if (in_point.y == in_borderValues.posYlimit)
	{
		locationToReturn.enclaveKey.y = 7;
		locationToReturn.blockKey.y = 3;
		locationToReturn.preciseCoord.y = 1.0f;
	}
	else if (in_point.y == in_borderValues.negYlimit)
	{
		locationToReturn.enclaveKey.y = 0;
		locationToReturn.blockKey.y = 0;
		locationToReturn.preciseCoord.y = 0.0f;
	}
	else
	{
		/*
		//std::cout << "|||| point being passed is: " << in_point.y << std::endl;
		CursorPathTraceContainer yContainer = OrganicUtils::getPreciseCoordinate(in_point.y);
		locationToReturn.enclaveKey.y = yContainer.EnclaveCoord;
		locationToReturn.blockKey.y = yContainer.BlockCoord;
		locationToReturn.preciseCoord.y = yContainer.ExactBlockCoord;
		*/


		//std::cout << "|||| (x) point being passed is: " << in_point.x << std::endl;
		//std::cout << "!!! Y branch hit " << std::endl;
		if (in_point.y != 4.0f)
		{
			CursorPathTraceContainer yContainer = IndependentUtils::getPreciseCoordinate(in_point.y);
			locationToReturn.enclaveKey.y = yContainer.EnclaveCoord;
			locationToReturn.blockKey.y = yContainer.BlockCoord;
			locationToReturn.preciseCoord.y = yContainer.ExactBlockCoord;
		}
		else if (in_point.y == 4.0f)
		{
			//std::cout << ">>> set for branch 2 " << std::endl;
			locationToReturn.blockKey.y = 3;
			locationToReturn.preciseCoord.y = 1.0f;
		}

	}

	// check borders for Z,
	if (in_point.z == in_borderValues.posZlimit)
	{
		locationToReturn.enclaveKey.z = 7;
		locationToReturn.blockKey.z = 3;
		locationToReturn.preciseCoord.z = 1.0f;
	}
	else if (in_point.z == in_borderValues.negZlimit)
	{
		locationToReturn.enclaveKey.z = 0;
		locationToReturn.blockKey.z = 0;
		locationToReturn.preciseCoord.z = 0.0f;
	}
	else
	{
		/*
		//std::cout << "|||| point being passed is: " << in_point.z << std::endl;
		CursorPathTraceContainer zContainer = OrganicUtils::getPreciseCoordinate(in_point.z);
		locationToReturn.enclaveKey.z = zContainer.EnclaveCoord;
		locationToReturn.blockKey.z = zContainer.BlockCoord;
		locationToReturn.preciseCoord.z = zContainer.ExactBlockCoord;
		*/

		//std::cout << "|||| (x) point being passed is: " << in_point.x << std::endl;
		if (in_point.z != 4.0f)
		{
			CursorPathTraceContainer zContainer = IndependentUtils::getPreciseCoordinate(in_point.z);
			locationToReturn.enclaveKey.z = zContainer.EnclaveCoord;
			locationToReturn.blockKey.z = zContainer.BlockCoord;
			locationToReturn.preciseCoord.z = zContainer.ExactBlockCoord;
		}
		else if (in_point.z == 4.0f)
		{
			locationToReturn.blockKey.z = 3;
			locationToReturn.preciseCoord.z = 1.0f;
		}

	}
	//locationToReturn.preciseCoord = in_point;
	//std::cout << "Chunk and block data for point (" << in_point.x << ", " << in_point.y << ", " << in_point.z << ") " << std::endl;
	//std::cout << "Chunk location: " << locationToReturn.enclaveKey.x << ", " << locationToReturn.enclaveKey.y << ", " << locationToReturn.enclaveKey.z << std::endl;
	//std::cout << "Block location: " << locationToReturn.blockKey.x << ", " << locationToReturn.blockKey.y << ", " << locationToReturn.blockKey.z << std::endl;
	//std::cout << "Remainder amount: " << locationToReturn.preciseCoord.x << ", " << locationToReturn.preciseCoord.y << ", " << locationToReturn.preciseCoord.z << std::endl;
	return locationToReturn;
}

ECBPolyPoint IndependentUtils::findCommonMoveValues(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB)
{
	ECBPolyPoint returnPoint;
	if (in_polyPointA.x == in_polyPointB.x)		// is x common?
	{
		returnPoint.x = in_polyPointA.x;
	}
	if (in_polyPointA.y == in_polyPointB.y)
	{
		returnPoint.y = in_polyPointA.y;
	}
	if (in_polyPointA.z == in_polyPointB.z)
	{
		returnPoint.z = in_polyPointA.z;
	}
	return returnPoint;
}

ECBPolyPoint IndependentUtils::determineTriangleCentroid(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC)
{
	ECBPolyPoint returnPoint;
	returnPoint.x = ((in_pointA.x) + (in_pointB.x) + (in_pointC.x)) / 3;
	returnPoint.y = ((in_pointA.y) + (in_pointB.y) + (in_pointC.y)) / 3;
	returnPoint.z = ((in_pointA.z) + (in_pointB.z) + (in_pointC.z)) / 3;
	return returnPoint;
}

glm::vec3 IndependentUtils::findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 foundCentroid;
	foundCentroid.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	foundCentroid.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	foundCentroid.z = (in_point0.z + in_point1.z + in_point2.z) / 3;
	//std::cout << "Centroid X: " << foundCentroid.x << std::endl;
	//std::cout << "Centroid y: " << foundCentroid.y << std::endl;
	return foundCentroid;
}

glm::vec3 IndependentUtils::findTriangleNormal(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 returnVec;
	PointTranslationCheck pointTranslator;
	QuatRotationPoints quatPoints;

	// copy points 
	glm::vec3 point0Copy = in_point0;
	glm::vec3 point1Copy = in_point1;
	glm::vec3 point2Copy = in_point2;

	quatPoints.insertPointRefs(&point0Copy, &point1Copy, &point2Copy);

	// check for any translation
	pointTranslator.performCheck(quatPoints.getFirstPoint());
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
		//std::cout << "!!! Points were translated; their values are: " << std::endl;
		//std::cout << "0: " << point0Copy.x << ", " << point0Copy.y << ", " << point0Copy.z << std::endl;
		//std::cout << "1: " << point1Copy.x << ", " << point1Copy.y << ", " << point1Copy.z << std::endl;
		//std::cout << "2: " << point2Copy.x << ", " << point2Copy.y << ", " << point2Copy.z << std::endl;
	}

	// calculate the normal, after translating point 0 of the triangle to point 0 (so that points 1 and 2 become appropriate values to get the cross product from)
	returnVec = glm::normalize(glm::cross(point1Copy, point2Copy));
	//std::cout << "Calculated triangle normal is: " << returnVec.x << ", " << returnVec.y << ", " << returnVec.z << std::endl;

	return returnVec;
}

ECBPPOrientationResults IndependentUtils::GetBlueprintPointOrientation(ECBPolyPoint in_pointToCheck, ECBBorderLineList* in_borderLineList)
{
	ECBPPOrientationResults returnResults;
	// STEP 1: check to see if point is in a corner ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//std::cout << std::fixed << std::setprecision(7);
	//std::cout << "(BLUEPRINT POINT ORIENTATION): Point to check here is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "Lower SE: " << in_borderLineList->corner_LowerSE.cornerPoint.x << ", " << in_borderLineList->corner_LowerSE.cornerPoint.y << ", " << in_borderLineList->corner_LowerSE.cornerPoint.z << std::endl;
	//std::cout << "debug of x in point is: " << float(in_pointToCheck.x) << std::endl;
	ECBPolyPoint testPoint = IndependentUtils::roundPolyPointToHundredths(in_pointToCheck);
	//std::cout << "converted value of x in point is: " << testPoint.x << ", " << testPoint.y << ", " << testPoint.z << std::endl;
	//std::cout << " debug of x upperSE is: " << float(in_borderLineList->corner_UpperSE.cornerPoint.x) << std::endl;

	/*
	if (float(in_pointToCheck.x) == float(in_borderLineList->corner_LowerNW.cornerPoint.x))
	{
	//std::cout << "debug: X match! " << std::endl;
	}
	else if (!(float(in_pointToCheck.x) == float(in_borderLineList->corner_LowerNW.cornerPoint.x)))
	{
	//std::cout << "Whoa! no match! " << std::endl;
	//std::cout << "int'd value 1 is: " << int(in_pointToCheck.x) << std::endl;
	//std::cout << "int'd value 2 is: " << int(in_borderLineList->corner_LowerNW.cornerPoint.x) << std::endl;
	}

	if (in_pointToCheck.y == in_borderLineList->corner_LowerNW.cornerPoint.y)
	{
	//std::cout << "debug: Y match! " << std::endl;
	}
	if (in_pointToCheck.z == in_borderLineList->corner_LowerNW.cornerPoint.z)
	{
	//std::cout << "debug: Z match! " << std::endl;
	}
	*/

	if (in_pointToCheck.x == in_borderLineList->corner_LowerNW.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_LowerNW.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_LowerNW.cornerPoint.z)		// Lower NW
	{
		//std::cout << "||||||||||||| Get Point Orientation: Lower NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_LowerSW.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_LowerSW.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_LowerSW.cornerPoint.z)		// Lower SW
	{
		//std::cout << "Get Point Orientation: Lower SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_LowerSE.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_LowerSE.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_LowerSE.cornerPoint.z)		// Lower SE
	{
		//std::cout << "Get Point Orientation: Lower SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_LowerNE.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_LowerNE.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_LowerNE.cornerPoint.z)		// Lower NE
	{
		//std::cout << "Get Point Orientation: Lower NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_UpperNW.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_UpperNW.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_UpperNW.cornerPoint.z)		// Upper NW
	{
		//std::cout << "Get Point Orientation: Upper NW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_UpperSW.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_UpperSW.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_UpperSW.cornerPoint.z)		// Upper SW
	{
		//std::cout << "Get Point Orientation: Upper SW corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_UpperSE.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_UpperSE.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_UpperSE.cornerPoint.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper SE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
	}
	else if (in_pointToCheck.x == in_borderLineList->corner_UpperNE.cornerPoint.x	&&		in_pointToCheck.y == in_borderLineList->corner_UpperNE.cornerPoint.y	&&		in_pointToCheck.z == in_borderLineList->corner_UpperNE.cornerPoint.z)		// Upper SE
	{
		//std::cout << "||||||||||||| Get Point Orientation: Upper NE corner hit" << std::endl;
		returnResults.otype = ECBPPOrientations::CORNER;
		returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
	}

	// STEP 2: check to see if point is on a line ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	// Z LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// >>>>> Lower West Z line
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.y == in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_LowerNW.cornerPoint.z	&& in_pointToCheck.z < in_borderLineList->corner_LowerSW.cornerPoint.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(blockBorders.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Get Point Orientation: Lower west line hit " << std::endl;
	}

	// >>>>> Upper West line check
	else if ((in_pointToCheck.x == in_borderLineList->corner_UpperNW.cornerPoint.x)
		&&
		(in_pointToCheck.y == in_borderLineList->corner_UpperNW.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNW.cornerPoint.z	&& in_pointToCheck.z < in_borderLineList->corner_UpperSW.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_borderLineList->Zaxis_upperWest);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Zaxis_upperWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}

	// >>>>> Upper East line check
	else if ((in_pointToCheck.x == in_borderLineList->corner_UpperNE.cornerPoint.x)
		&&
		(in_pointToCheck.y == in_borderLineList->corner_UpperNE.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNE.cornerPoint.z	&& in_pointToCheck.z < in_borderLineList->corner_UpperSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_borderLineList->Zaxis_upperEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Zaxis_upperEast, in_originPoint, in_pointToCheck);	// get the shifting key				
		//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "|||||||||| Get Point Orientation: Upper east line hit " << std::endl;
	}
	// >>>>> Lower East line check
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.y == in_borderLineList->corner_LowerNE.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_LowerNE.cornerPoint.z	&& in_pointToCheck.z < in_borderLineList->corner_LowerSE.cornerPoint.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//calculatedEndpointData.applyBlockBorder(in_borderLineList->Zaxis_lowerEast);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Zaxis_lowerEast, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
	}









	// Y LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerNE.cornerPoint.z)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNE.cornerPoint.y	&& in_pointToCheck.y < in_borderLineList->corner_UpperNE.cornerPoint.y)
		)
	{
		//std::cout << "!!! #### Blueprint point is at north east Y line. " << std::endl;
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Yaxis_northEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Point is at north east line " << std::endl;
	}

	// South East line
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerSE.cornerPoint.x)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerSE.cornerPoint.z)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerSE.cornerPoint.y	&& in_pointToCheck.y < in_borderLineList->corner_UpperSE.cornerPoint.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Yaxis_southEast, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Point is at south east line" << std::endl;
	}
	// South West line
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerSW.cornerPoint.x)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerSW.cornerPoint.z)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerSW.cornerPoint.y	&& in_pointToCheck.y < in_borderLineList->corner_UpperSW.cornerPoint.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Yaxis_southWest, in_originPoint, in_pointToCheck);	// get the shifting key
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Point is at south west line" << std::endl;
	}
	// North West line
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerNW.cornerPoint.z)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNW.cornerPoint.y	&& in_pointToCheck.y < in_borderLineList->corner_UpperNW.cornerPoint.y)
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Yaxis_northWest, in_originPoint, in_pointToCheck);	// get the shifting key
		//std::cout << "!!! #### Blueprint point is at north west Y line. " << std::endl;
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
		//std::cout << "Point is at north west line" << std::endl;
	}




	// X LINES |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	else if ((in_pointToCheck.y == in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerNW.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerNW.cornerPoint.x	&&		in_pointToCheck.x < in_borderLineList->corner_LowerNE.cornerPoint.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_borderLineList->corner_LowerSW.cornerPoint.y)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_LowerSW.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerSW.cornerPoint.x	&&	in_pointToCheck.x < in_borderLineList->corner_LowerSE.cornerPoint.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_borderLineList->corner_UpperSW.cornerPoint.y)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_UpperSW.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_UpperSW.cornerPoint.x	&& in_pointToCheck.x < in_borderLineList->corner_UpperSE.cornerPoint.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((in_pointToCheck.y == in_borderLineList->corner_UpperNW.cornerPoint.y)
		&&
		(in_pointToCheck.z == in_borderLineList->corner_UpperNW.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_UpperNW.cornerPoint.x	&& in_pointToCheck.x < in_borderLineList->corner_UpperNE.cornerPoint.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(in_borderLineList->Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		returnResults.otype = ECBPPOrientations::LINE;
		returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
	}

	// STEP 3: check to see if point is on a face ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// West face check
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerSW.cornerPoint.x)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.y < in_borderLineList->corner_UpperNW.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNW.cornerPoint.z)
		&&
		(in_pointToCheck.z < in_borderLineList->corner_UpperSW.cornerPoint.z)

		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::WESTFACE;
	}

	// North face check
	else if ((in_pointToCheck.z == in_borderLineList->corner_LowerNW.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.x < in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.y < in_borderLineList->corner_UpperNW.cornerPoint.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::NORTHFACE;
	}

	// East face check
	else if ((in_pointToCheck.x == in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.y < in_borderLineList->corner_UpperNW.cornerPoint.y)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNW.cornerPoint.z)
		&&
		(in_pointToCheck.z < in_borderLineList->corner_UpperSW.cornerPoint.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "EAST FACE hit! " << std::endl;
	}

	// South face check
	else if ((in_pointToCheck.z == in_borderLineList->corner_LowerSE.cornerPoint.z)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.x < in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.y > in_borderLineList->corner_LowerNW.cornerPoint.y)
		&&
		(in_pointToCheck.y < in_borderLineList->corner_UpperNW.cornerPoint.y)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
	}

	// Top face check
	else if ((in_pointToCheck.y == in_borderLineList->corner_UpperSW.cornerPoint.y)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.x < in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNW.cornerPoint.z)
		&&
		(in_pointToCheck.z < in_borderLineList->corner_UpperSW.cornerPoint.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::TOPFACE;
	}

	// Bottom Face check
	else if ((in_pointToCheck.y == in_borderLineList->corner_LowerSW.cornerPoint.y)
		&&
		(in_pointToCheck.x > in_borderLineList->corner_LowerNW.cornerPoint.x)
		&&
		(in_pointToCheck.x < in_borderLineList->corner_LowerNE.cornerPoint.x)
		&&
		(in_pointToCheck.z > in_borderLineList->corner_UpperNW.cornerPoint.z)
		&&
		(in_pointToCheck.z < in_borderLineList->corner_UpperSW.cornerPoint.z)
		)
	{
		returnResults.otype = ECBPPOrientations::FACE;
		returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
	}

	return returnResults;
}

ECBBorderLineList IndependentUtils::determineBorderLines(EnclaveKeyDef::EnclaveKey in_Key)
{
	ECBBorderLineList newLineList;
	// set z axis values
	/*
	newLineList.Zaxis_lowerWest.pointA.x = in_Key.x * 32;			// Lower West: goes from (west) bottom north to bottom south (+z)
	newLineList.Zaxis_lowerWest.pointA.y = in_Key.y * 32;
	newLineList.Zaxis_lowerWest.pointA.z = in_Key.z * 32;

	newLineList.Zaxis_lowerWest.pointB.x = in_Key.x * 32;
	newLineList.Zaxis_lowerWest.pointB.y = in_Key.y * 32;
	newLineList.Zaxis_lowerWest.pointB.z = (in_Key.z * 32) + 32;

	newLineList.Zaxis_lowerEast.pointA.x = (in_Key.x * 32) + 32;	// Lower east:
	newLineList.Zaxis_lowerEast.pointA.y = in_Key.y * 32;
	newLineList.Zaxis_lowerEast.pointA.z = (in_Key.z * 32) + 32;
	*/

	// set up corner coordinates and values

	// Lower NW coords
	newLineList.corner_LowerNW.cornerPoint.x = float(in_Key.x * 32);
	newLineList.corner_LowerNW.cornerPoint.y = float(in_Key.y * 32);
	newLineList.corner_LowerNW.cornerPoint.z = float(in_Key.z * 32);

	newLineList.corner_LowerNW.cornerAmpValues.move_x = -1.0f;	// "" amp values (see diagram)
	newLineList.corner_LowerNW.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerNW.cornerAmpValues.move_z = -1.0f;


	// Lower SW coords
	newLineList.corner_LowerSW.cornerPoint.x = float(in_Key.x * 32);
	newLineList.corner_LowerSW.cornerPoint.y = float(in_Key.y * 32);
	newLineList.corner_LowerSW.cornerPoint.z = float((in_Key.z * 32) + 32);

	newLineList.corner_LowerSW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_LowerSW.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerSW.cornerAmpValues.move_z = 1.0f;


	// Lower SE coords
	newLineList.corner_LowerSE.cornerPoint.x = float((in_Key.x * 32) + 32);
	newLineList.corner_LowerSE.cornerPoint.y = float(in_Key.y * 32);
	newLineList.corner_LowerSE.cornerPoint.z = float((in_Key.z * 32) + 32);

	newLineList.corner_LowerSE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_LowerSE.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerSE.cornerAmpValues.move_z = 1.0f;


	// Lower NE coords
	newLineList.corner_LowerNE.cornerPoint.x = float((in_Key.x * 32) + 32);
	newLineList.corner_LowerNE.cornerPoint.y = float(in_Key.y * 32);
	newLineList.corner_LowerNE.cornerPoint.z = float(in_Key.z * 32);

	newLineList.corner_LowerNE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_LowerNE.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerNE.cornerAmpValues.move_z = -1.0f;

	// Upper NW coords
	newLineList.corner_UpperNW.cornerPoint.x = float(in_Key.x * 32);
	newLineList.corner_UpperNW.cornerPoint.y = float((in_Key.y * 32) + 32);
	newLineList.corner_UpperNW.cornerPoint.z = float(in_Key.z * 32);

	newLineList.corner_UpperNW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_UpperNW.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperNW.cornerAmpValues.move_z = -1.0f;

	// Upper SW coords
	newLineList.corner_UpperSW.cornerPoint.x = float(in_Key.x * 32);
	newLineList.corner_UpperSW.cornerPoint.y = float((in_Key.y * 32) + 32);
	newLineList.corner_UpperSW.cornerPoint.z = float((in_Key.z * 32) + 32);

	newLineList.corner_UpperSW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_UpperSW.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperSW.cornerAmpValues.move_z = 1.0f;

	// Upper SE coords
	newLineList.corner_UpperSE.cornerPoint.x = float((in_Key.x * 32) + 32);
	newLineList.corner_UpperSE.cornerPoint.y = float((in_Key.y * 32) + 32);
	newLineList.corner_UpperSE.cornerPoint.z = float((in_Key.z * 32) + 32);

	newLineList.corner_UpperSE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_UpperSE.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperSE.cornerAmpValues.move_z = 1.0f;

	// Upper NEW coords
	newLineList.corner_UpperNE.cornerPoint.x = float((in_Key.x * 32) + 32);
	newLineList.corner_UpperNE.cornerPoint.y = float((in_Key.y * 32) + 32);
	newLineList.corner_UpperNE.cornerPoint.z = float(in_Key.z * 32);

	newLineList.corner_UpperNE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_UpperNE.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperNE.cornerAmpValues.move_z = -1.0f;

	// set up line values 

	//start with x lines
	newLineList.Xaxis_lowerNorth.move_y = -1.0f;	// lower North
	newLineList.Xaxis_lowerNorth.move_z = -1.0f;

	newLineList.Xaxis_lowerSouth.move_y = -1.0f;	// lower South
	newLineList.Xaxis_lowerSouth.move_z = 1.0f;

	newLineList.Xaxis_upperSouth.move_y = 1.0f;		// upper South
	newLineList.Xaxis_upperSouth.move_z = 1.0f;

	newLineList.Xaxis_upperNorth.move_y = 1.0f;		// upper North
	newLineList.Xaxis_upperNorth.move_z = -1.0f;

	// ...z lines
	newLineList.Zaxis_lowerWest.move_x = -1.0f;		// lower West
	newLineList.Zaxis_lowerWest.move_y = -1.0f;

	newLineList.Zaxis_upperWest.move_x = -1.0f;		// upper West
	newLineList.Zaxis_upperWest.move_y = 1.0f;

	newLineList.Zaxis_upperEast.move_x = 1.0f;		// upper East
	newLineList.Zaxis_upperEast.move_y = 1.0f;

	newLineList.Zaxis_lowerEast.move_x = 1.0f;		// lower East
	newLineList.Zaxis_lowerEast.move_y = -1.0f;

	// ...y lines
	newLineList.Yaxis_northEast.move_x = 1.0f;		// north East
	newLineList.Yaxis_northEast.move_z = -1.0f;

	newLineList.Yaxis_southEast.move_x = 1.0f;		// south East
	newLineList.Yaxis_southEast.move_z = 1.0f;

	newLineList.Yaxis_southWest.move_x = -1.0f;		// south West
	newLineList.Yaxis_southWest.move_z = 1.0f;

	newLineList.Yaxis_northWest.move_x = -1.0f;		// north West
	newLineList.Yaxis_northWest.move_z = -1.0f;

	// set up faces
	newLineList.WestFace.move_x = -1.0f;
	newLineList.NorthFace.move_z = -1.0f;
	newLineList.EastFace.move_x = 1.0f;
	newLineList.SouthFace.move_z = 1.0f;
	newLineList.TopFace.move_y = 1.0f;
	newLineList.BottomFace.move_y = -1.0f;

	return newLineList;
}

ECBPolyPoint IndependentUtils::roundPolyPointToHundredths(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint calibratedPoint;
	calibratedPoint.x = IndependentUtils::roundToHundredth(in_polyPoint.x);
	calibratedPoint.y = IndependentUtils::roundToHundredth(in_polyPoint.y);
	calibratedPoint.z = IndependentUtils::roundToHundredth(in_polyPoint.z);
	//std::cout << "-----------begin rounding " << std::endl;
	//std::cout << "X rounded would be: " << OrganicUtils::roundToHundredth(in_polyPoint.x) << std::endl;
	//std::cout << "Y rounded would be: " << OrganicUtils::roundToHundredth(in_polyPoint.y) << std::endl;
	//std::cout << "Z rounded would be: " << OrganicUtils::roundToHundredth(in_polyPoint.z) << std::endl;
	//std::cout << "-----------end rounding " << std::endl;
	return calibratedPoint;
}

glm::vec3 IndependentUtils::roundVec3ToHundredths(glm::vec3 in_vec3)
{
	glm::vec3 calibratedPoint;
	calibratedPoint.x = IndependentUtils::roundToHundredth(in_vec3.x);
	calibratedPoint.y = IndependentUtils::roundToHundredth(in_vec3.y);
	calibratedPoint.z = IndependentUtils::roundToHundredth(in_vec3.z);
	return calibratedPoint;
}

ECBPolyPoint IndependentUtils::roundPolyPointToThousandths(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint calibratedPoint;
	calibratedPoint.x = IndependentUtils::roundToThousandths(in_polyPoint.x);
	calibratedPoint.y = IndependentUtils::roundToThousandths(in_polyPoint.y);
	calibratedPoint.z = IndependentUtils::roundToThousandths(in_polyPoint.z);
	return calibratedPoint;
}

ECBPolyPoint IndependentUtils::roundPolyPointToTenThousandths(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint calibratedPoint;
	calibratedPoint.x = IndependentUtils::roundToTenThousandths(in_polyPoint.x);
	calibratedPoint.y = IndependentUtils::roundToTenThousandths(in_polyPoint.y);
	calibratedPoint.z = IndependentUtils::roundToTenThousandths(in_polyPoint.z);
	return calibratedPoint;
}

glm::vec3 IndependentUtils::convertECBPolyPointToVec3(ECBPolyPoint in_pointToConvert)
{
	return glm::vec3(in_pointToConvert.x, in_pointToConvert.y, in_pointToConvert.z);
}

ECBPolyPoint IndependentUtils::convertVec3ToPolyPoint(glm::vec3 in_pointToConvert)
{
	return ECBPolyPoint(in_pointToConvert.x, in_pointToConvert.y, in_pointToConvert.z);
}

float IndependentUtils::roundToHundredth(float in_float)
{
	float calcedFloat = float(floor(in_float * 100 + 0.5) / 100);
	//std::cout << "Original float: " << in_float << std::endl;
	//std::cout << "Calc'ed float is: " << calcedFloat << std::endl;
	return calcedFloat;
}

double IndependentUtils::roundToHundredth(double in_double)
{
	return double(floor(in_double * 100 + 0.5) / 100);
}

float IndependentUtils::roundToThousandths(float in_float)
{
	float calcedFloat = float(floor(in_float * 1000 + 0.5) / 1000);
	return calcedFloat;
}

float IndependentUtils::roundToTenThousandths(float in_float)
{
	float calcedFloat = float(floor(in_float * 10000 + 0.5) / 10000);
	return calcedFloat;
}

EnclaveKeyDef::EnclaveKey IndependentUtils::retrieveBorderDirection(ECBPPOrientationResults in_results, BorderDataMap* in_dataMapRef)
{
	EnclaveKeyDef::EnclaveKey returnPoint;
	// check for faces

	//std::cout << "!!!! -> starting, retrieveBorderDirection. " << std::endl;
	//std::cout << "!!! Size of faceMap is: " << in_dataMapRef->faceMap.size() << std::endl;

	if (in_results.otype == ECBPPOrientations::FACE)
	{
		//std::cout << "!!! Attempting grab for FACE. " << std::endl;
		returnPoint = in_dataMapRef->faceMap[in_results.osubtype].borderLimits;
		//std::cout << "(face) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}
	else if (in_results.otype == ECBPPOrientations::LINE)
	{
		//std::cout << "!!! Attempting grab for LINE. " << std::endl;
		returnPoint = in_dataMapRef->bdLinesMap[in_results.osubtype].borderLimits;
		//std::cout << "(line) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}
	else if (in_results.otype == ECBPPOrientations::CORNER)
	{
		//std::cout << "!!! Attempting grab for CORNER. " << std::endl;
		returnPoint = in_dataMapRef->cornerMap[in_results.osubtype].borderLimits;
		//std::cout << "(corner) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}
	else
	{
		int warningHalt = 3;
		std::cout << "!!!! WARNING: otype is not FACE, LINE, or CORNER!!!" << std::endl;
		std::cin >> warningHalt;
	}

	return returnPoint;
}

ECBPolyPoint IndependentUtils::getAppropriateSlopeToUse(BorderDataMap* in_dataMapRef, 
														ECBPPOrientationResults in_beginOrientationResults,
														ECBPolyPoint in_xIntOrig, 
														ECBPolyPoint in_yIntOrig, 
														ECBPolyPoint in_zIntOrig, 
														EnclaveKeyDef::EnclaveKey in_moveVals, 
														PerfectClampEnum in_perfectClampValue, 
														int in_debugFlag)
{
	ECBPolyPoint interceptToUse;
	FaceListMeta tempStorage;	// set up the face list meta variable

	//std::cout << "x-slope->: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
	//std::cout << "y-slope->: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
	//std::cout << "z-slope->: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;


	if (in_debugFlag == 1)
	{
		std::cout << "x-slope->: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope->: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope->: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;
	}
	if (in_beginOrientationResults.otype == ECBPPOrientations::FACE)	// the hit is a face
	{
		tempStorage.faceList.faceList[0] = in_beginOrientationResults.osubtype;	// set the value to be the face
		tempStorage.numberOfIntercepts = 1;			// 1 intercept only
	}


	else if (in_beginOrientationResults.otype == ECBPPOrientations::LINE)		// ...get the linked faces for the line
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.numberOfIntercepts = 2;
	}
	else if (in_beginOrientationResults.otype == ECBPPOrientations::CORNER)	// ...get the linked faces for the corner
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.faceList.faceList[2] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[2];
		tempStorage.numberOfIntercepts = 3;
	}
	else
	{
		// std::cout << "!!!... WARNING: invalid orientation detected. " << std::endl;
	}

	EnclaveKeyDef::EnclaveKey moveValsToPass;
	ECBPolyPoint in_xInt = in_xIntOrig;
	ECBPolyPoint in_yInt = in_yIntOrig;
	ECBPolyPoint in_zInt = in_zIntOrig;
	// OrganicUtils::checkForSecondarySlopeInversion(in_intendedFaces, in_moveVals, &in_xInt, &in_yInt, &in_zInt);
	// an intercept count of 1 will always mean a face is use
	if (tempStorage.numberOfIntercepts == 1)
	{

		if (in_debugFlag == 1)
		{
			//std::cout << ">>>>> Face encountered! " << std::endl;
		}

		// an intercept count of 1 will always mean a face is used
		ECBPPOrientations slopeToUse = in_dataMapRef->faceInterceptMap[in_beginOrientationResults.osubtype];
		interceptToUse = getSlopeToUse(slopeToUse, in_xInt, in_yInt, in_zInt);								//FIX HERE
																											//std::cout << "|||| (face) Slope to use is: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;
	}

	// an intercept count of 2 means we need to get the appropriate slope from the line
	else if (tempStorage.numberOfIntercepts == 2)
	{
		if (in_debugFlag == 1)
		{
			std::cout << ">>>>>> Line encountered! " << std::endl;
			std::cout << "Ints are: " << std::endl;
			std::cout << "X: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
			std::cout << "Y: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
			std::cout << "Y: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;
		}

		// get the first face value
		ECBPPOrientations face1 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[0]];
		ECBPolyPoint intercept1 = getSlopeToUse(face1, in_xInt, in_yInt, in_zInt);
		//std::cout << "intercept 1 is : " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;

		// get the second face value
		ECBPPOrientations face2 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[1]];
		ECBPolyPoint intercept2 = getSlopeToUse(face2, in_xInt, in_yInt, in_zInt);							//FIX HERE

		if (in_debugFlag == 1)
		{

			std::cout << "Move vals are: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
			std::cout << "Intercept 1 is: " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;
			std::cout << "Intercept 2 is: " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;
		}

		InterceptValidity firstIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept1, in_perfectClampValue);
		InterceptValidity secondIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept2, in_perfectClampValue);
		interceptToUse = getInterceptToUseFromLine(intercept1, intercept2, firstIntValidity, secondIntValidity, in_moveVals);		//FIX HERE
	}

	// an intercept count of 3 means we need to get the appropriate slope from the corner (in theory, there should only be 1 valid slope ever)
	else if (tempStorage.numberOfIntercepts == 3)
	{

		if (in_debugFlag == 1)
		{
			//std::cout << ">>>>>>>> Corner encountered! " << std::endl;
		}

		moveValsToPass = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].borderLimits;
		ECBPolyPoint polyPointToPass;
		polyPointToPass.x = float(moveValsToPass.x);
		polyPointToPass.y = float(moveValsToPass.y);
		polyPointToPass.z = float(moveValsToPass.z);
		interceptToUse = getInterceptToUseFromCorner(in_xInt, in_yInt, in_zInt, in_perfectClampValue, in_moveVals);			//FIX HERE
	}

	if (in_debugFlag == 1)
	{
		//std::cout << "Intercept to use will be: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << ", " << std::endl;
	}
	return interceptToUse;
}

ECBPolyPoint IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheck(BorderDataMap* in_dataMapRef,
																			ECBPPOrientationResults in_beginOrientationResults, 
																			ECBPolyPoint in_xIntOrig, 
																			ECBPolyPoint in_yIntOrig, 
																			ECBPolyPoint in_zIntOrig, 
																			EnclaveKeyDef::EnclaveKey in_moveVals,
																			PerfectClampEnum in_perfectClampValue, 
																			int in_debugFlag, 
																			ECBPolyPoint in_intendedFaces)
{
	ECBPolyPoint interceptToUse;
	FaceListMeta tempStorage;	// set up the face list meta variable
	//std::cout << "!! ++++++++++++++++++++++++++ (()) Perfect clamp value is: " << in_perfectClampValue << std::endl;

	//std::cout << "---------Slopes, pre inversion checks--------------" << std::endl;
	//std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
	//std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
	//std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;

	if (in_debugFlag == 1)
	{
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;
	}
	if (in_beginOrientationResults.otype == ECBPPOrientations::FACE)	// the hit is a face
	{
		tempStorage.faceList.faceList[0] = in_beginOrientationResults.osubtype;	// set the value to be the face
		tempStorage.numberOfIntercepts = 1;			// 1 intercept only
	}


	else if (in_beginOrientationResults.otype == ECBPPOrientations::LINE)		// ...get the linked faces for the line
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.numberOfIntercepts = 2;
	}
	else if (in_beginOrientationResults.otype == ECBPPOrientations::CORNER)	// ...get the linked faces for the corner
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.faceList.faceList[2] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[2];
		tempStorage.numberOfIntercepts = 3;
	}
	else
	{
		std::cout << "!!!!! WARNING: invalid orientation detected...waiting for input: " << std::endl;
		std::cout << "Slopes: " << std::endl;
		std::cout << "Move vals: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;
		int errorVal;
		std::cin >> errorVal;
	}

	EnclaveKeyDef::EnclaveKey moveValsToPass;
	ECBPolyPoint in_xInt = in_xIntOrig;
	ECBPolyPoint in_yInt = in_yIntOrig;
	ECBPolyPoint in_zInt = in_zIntOrig;

	//std::cout << ">>> Intended faces: " << in_intendedFaces.x << ", " << in_intendedFaces.y << ", " << in_intendedFaces.z << std::endl;
	//std::cout << ">>> Move vals: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;

	IndependentUtils::checkForSecondarySlopeInversion(in_intendedFaces, in_moveVals, &in_xInt, &in_yInt, &in_zInt);

	//std::cout << "---------Slopes, post inversion checks--------------" << std::endl;
	//std::cout << "x-slope: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
	//std::cout << "y-slope: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
	//std::cout << "z-slope: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;

	if (in_debugFlag == 1)
	{
		std::cout << "Inverted-attempt x: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
		std::cout << "Inverted-attempt y: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
		std::cout << "Inverted-attempt z: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;
		std::cout << "Debugged move vals are: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
	}

	// an intercept count of 1 will always mean a face is use
	if (tempStorage.numberOfIntercepts == 1)
	{

		if (in_debugFlag == 1)
		{
			std::cout << ">>>>> Face encountered! " << std::endl;
		}

		// an intercept count of 1 will always mean a face is used
		ECBPPOrientations slopeToUse = in_dataMapRef->faceInterceptMap[in_beginOrientationResults.osubtype];
		interceptToUse = getSlopeToUse(slopeToUse, in_xInt, in_yInt, in_zInt);								//FIX HERE
		//std::cout << "|||| (face) Slope to use is: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;
	}

	// an intercept count of 2 means we need to get the appropriate slope from the line
	else if (tempStorage.numberOfIntercepts == 2)
	{
		if (in_debugFlag == 1)
		{
			std::cout << ">>>>>> Line encountered! " << std::endl;
		}

		// get the first face value
		ECBPPOrientations face1 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[0]];
		ECBPolyPoint intercept1 = getSlopeToUse(face1, in_xInt, in_yInt, in_zInt);
		//std::cout << "intercept 1 is : " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;

		// get the second face value
		ECBPPOrientations face2 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[1]];
		ECBPolyPoint intercept2 = getSlopeToUse(face2, in_xInt, in_yInt, in_zInt);							//FIX HERE

		//InterceptValidity validity = InterceptValidity::NOVAL;
		//std::cout << "!! Slope 1 (intercept 1) is: " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;
		//std::cout << "!! Slope 2 (intercept 2) is: " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;

		InterceptValidity firstIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept1, in_perfectClampValue);
		InterceptValidity secondIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept2, in_perfectClampValue);


		if (firstIntValidity == InterceptValidity::INVALID)
		{
			//std::cout << "#### The first intercept is INVALID: " << std::endl;
		}
		if (secondIntValidity == InterceptValidity::INVALID)
		{
			//std::cout << "#### The second intercept is INVALID: " << std::endl;
		}


		if (in_debugFlag == 0)
		{
			interceptToUse = getInterceptToUseFromLine(intercept1, intercept2, firstIntValidity, secondIntValidity, in_moveVals);		//FIX HERE
		}
		else if (in_debugFlag == 1)
		{
			interceptToUse = getInterceptToUseFromLineDebug(intercept1, intercept2, firstIntValidity, secondIntValidity, in_moveVals);		//FIX HERE
		}

		if (in_debugFlag == 1)
		{
			std::cout << "Move vals are: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
			std::cout << ":::: Intercept 1 is: " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;
			std::cout << ":::: Intercept 2 is: " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;
			std::cout << "Intercept to use is: " << interceptToUse.x << "," << interceptToUse.y << "," << interceptToUse.z << "," << std::endl;
		}
	}

	// an intercept count of 3 means we need to get the appropriate slope from the corner (in theory, there should only be 1 valid slope ever)
	else if (tempStorage.numberOfIntercepts == 3)
	{

		if (in_debugFlag == 1)
		{
			std::cout << ">>>>>>>> Corner encountered! " << std::endl;
		}

		moveValsToPass = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].borderLimits;
		ECBPolyPoint polyPointToPass;
		polyPointToPass.x = float(moveValsToPass.x);
		polyPointToPass.y = float(moveValsToPass.y);
		polyPointToPass.z = float(moveValsToPass.z);
		interceptToUse = getInterceptToUseFromCorner(in_xInt, in_yInt, in_zInt, in_perfectClampValue, in_moveVals);			//FIX HERE
	}

	if (in_debugFlag == 1)
	{
		//std::cout << "Intercept to use will be: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << ", " << std::endl;
	}

	/*
	if
	(
		(interceptToUse.x == 0)
		&&
		(interceptToUse.y == 0)
		&&
		(interceptToUse.z == 0)
	)
	{
		std::cout << "!!!!!!!!!!!!! WARNING, invalid slope detected!!! HALTING  " << std::endl;
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;

		int someVal = 5;
		std::cin >> someVal;

	}
	*/
	return interceptToUse;
}

ECBPolyPoint IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(BorderDataMap* in_dataMapRef, 
																						ECBPPOrientationResults in_beginOrientationResults, 
																						ECBPolyPoint in_xIntOrig, 
																						ECBPolyPoint in_yIntOrig, 
																						ECBPolyPoint in_zIntOrig, 
																						EnclaveKeyDef::EnclaveKey in_moveVals, 
																						PerfectClampEnum in_perfectClampValue, 
																						int in_debugFlag, 
																						ECBPolyPoint in_intendedFaces)
{
	ECBPolyPoint interceptToUse;
	FaceListMeta tempStorage;	// set up the face list meta variable
	//std::cout << "!! ++++++++++++++++++++++++++ (()) Perfect clamp value is: " << in_perfectClampValue << std::endl;

	//std::cout << "---------Slopes, pre inversion checks--------------" << std::endl;
	//std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
	//std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
	//std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;

	if (in_debugFlag == 1)
	{
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;
	}
	if (in_beginOrientationResults.otype == ECBPPOrientations::FACE)	// the hit is a face
	{
		tempStorage.faceList.faceList[0] = in_beginOrientationResults.osubtype;	// set the value to be the face
		tempStorage.numberOfIntercepts = 1;			// 1 intercept only
	}


	else if (in_beginOrientationResults.otype == ECBPPOrientations::LINE)		// ...get the linked faces for the line
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->bdLinesMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.numberOfIntercepts = 2;
	}
	else if (in_beginOrientationResults.otype == ECBPPOrientations::CORNER)	// ...get the linked faces for the corner
	{
		tempStorage.faceList.faceList[0] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[1];
		tempStorage.faceList.faceList[2] = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].linkedFaces[2];
		tempStorage.numberOfIntercepts = 3;
	}
	else
	{
		std::cout << "!!!!! WARNING: invalid orientation detected; will return 0,0,0 " << std::endl;
		std::cout << "Slopes: " << std::endl;
		std::cout << "Move vals: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;
		//int errorVal;
		//std::cin >> errorVal;
	}

	EnclaveKeyDef::EnclaveKey moveValsToPass;
	ECBPolyPoint in_xInt = in_xIntOrig;
	ECBPolyPoint in_yInt = in_yIntOrig;
	ECBPolyPoint in_zInt = in_zIntOrig;

	//std::cout << ">>> Intended faces: " << in_intendedFaces.x << ", " << in_intendedFaces.y << ", " << in_intendedFaces.z << std::endl;
	//std::cout << ">>> Move vals: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;

	checkForSecondarySlopeInversion(in_intendedFaces, in_moveVals, &in_xInt, &in_yInt, &in_zInt);

	//std::cout << "---------Slopes, post inversion checks--------------" << std::endl;
	//std::cout << "x-slope: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
	//std::cout << "y-slope: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
	//std::cout << "z-slope: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;

	if (in_debugFlag == 1)
	{
		std::cout << "Inverted-attempt x: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
		std::cout << "Inverted-attempt y: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
		std::cout << "Inverted-attempt z: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;
		std::cout << "Debugged move vals are: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
	}

	// an intercept count of 1 will always mean a face is use
	if (tempStorage.numberOfIntercepts == 1)
	{

		if (in_debugFlag == 1)
		{
			std::cout << ">>>>> Face encountered! " << std::endl;
		}

		// an intercept count of 1 will always mean a face is used
		ECBPPOrientations slopeToUse = in_dataMapRef->faceInterceptMap[in_beginOrientationResults.osubtype];
		interceptToUse = getSlopeToUse(slopeToUse, in_xInt, in_yInt, in_zInt);								//FIX HERE
		//std::cout << "|||| (face) Slope to use is: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;
	}

	// an intercept count of 2 means we need to get the appropriate slope from the line
	else if (tempStorage.numberOfIntercepts == 2)
	{
		if (in_debugFlag == 1)
		{
			std::cout << ">>>>>> Line encountered! " << std::endl;
		}

		// get the first face value
		ECBPPOrientations face1 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[0]];
		ECBPolyPoint intercept1 = getSlopeToUse(face1, in_xInt, in_yInt, in_zInt);
		//std::cout << "intercept 1 is : " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;

		// get the second face value
		ECBPPOrientations face2 = in_dataMapRef->faceInterceptMap[tempStorage.faceList.faceList[1]];
		ECBPolyPoint intercept2 = getSlopeToUse(face2, in_xInt, in_yInt, in_zInt);							//FIX HERE

		//InterceptValidity validity = InterceptValidity::NOVAL;
		//std::cout << "!! Slope 1 (intercept 1) is: " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;
		//std::cout << "!! Slope 2 (intercept 2) is: " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;

		InterceptValidity firstIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept1, in_perfectClampValue);
		InterceptValidity secondIntValidity = determineInterceptValidity(in_xInt, in_yInt, in_zInt, intercept2, in_perfectClampValue);


		if (firstIntValidity == InterceptValidity::INVALID)
		{
			//std::cout << "#### The first intercept is INVALID: " << std::endl;
		}
		if (secondIntValidity == InterceptValidity::INVALID)
		{
			//std::cout << "#### The second intercept is INVALID: " << std::endl;
		}


		if (in_debugFlag == 0)
		{
			interceptToUse = getInterceptToUseFromLine(intercept1, intercept2, firstIntValidity, secondIntValidity, in_moveVals);		//FIX HERE
		}
		else if (in_debugFlag == 1)
		{
			interceptToUse = getInterceptToUseFromLineDebug(intercept1, intercept2, firstIntValidity, secondIntValidity, in_moveVals);		//FIX HERE
		}

		if (in_debugFlag == 1)
		{
			std::cout << "Move vals are: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
			std::cout << ":::: Intercept 1 is: " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;
			std::cout << ":::: Intercept 2 is: " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;
			std::cout << "Intercept to use is: " << interceptToUse.x << "," << interceptToUse.y << "," << interceptToUse.z << "," << std::endl;
		}
	}

	// an intercept count of 3 means we need to get the appropriate slope from the corner (in theory, there should only be 1 valid slope ever)
	else if (tempStorage.numberOfIntercepts == 3)
	{

		if (in_debugFlag == 1)
		{
			std::cout << ">>>>>>>> Corner encountered! " << std::endl;
		}

		moveValsToPass = in_dataMapRef->cornerMap[in_beginOrientationResults.osubtype].borderLimits;
		ECBPolyPoint polyPointToPass;
		polyPointToPass.x = float(moveValsToPass.x);
		polyPointToPass.y = float(moveValsToPass.y);
		polyPointToPass.z = float(moveValsToPass.z);
		interceptToUse = getInterceptToUseFromCorner(in_xInt, in_yInt, in_zInt, in_perfectClampValue, in_moveVals);			//FIX HERE
	}

	if (in_debugFlag == 1)
	{
		//std::cout << "Intercept to use will be: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << ", " << std::endl;
	}

	/*
	if
	(
		(interceptToUse.x == 0)
		&&
		(interceptToUse.y == 0)
		&&
		(interceptToUse.z == 0)
	)
	{
		std::cout << "!!!!!!!!!!!!! WARNING, invalid slope detected!!! HALTING  " << std::endl;
		std::cout << "x-slope: " << in_xIntOrig.x << ", " << in_xIntOrig.y << ", " << in_xIntOrig.z << ", " << std::endl;
		std::cout << "y-slope: " << in_yIntOrig.x << ", " << in_yIntOrig.y << ", " << in_yIntOrig.z << ", " << std::endl;
		std::cout << "z-slope: " << in_zIntOrig.x << ", " << in_zIntOrig.y << ", " << in_zIntOrig.z << ", " << std::endl;

		int someVal = 5;
		std::cin >> someVal;

	}
	*/
	return interceptToUse;
}

void IndependentUtils::checkForSecondarySlopeInversion(ECBPolyPoint in_intendedFaces, EnclaveKeyDef::EnclaveKey in_moveVals, ECBPolyPoint* in_xIntRef, ECBPolyPoint* in_yIntRef, ECBPolyPoint* in_zIntRef)
{
	/*
if (in_intendedFaces.x != in_moveVals.x)
{
	*in_xIntRef = OrganicUtils::invertSlope(*in_xIntRef);	// invert x
																	//std::cout << "|||>>> x invert requirement detected" << current_x_slope.x << ", " << current_x_slope.y << ", " << current_x_slope.z << std::endl
}
if (in_intendedFaces.y != in_moveVals.y)
{
	*in_yIntRef = OrganicUtils::invertSlope(*in_yIntRef);
	//std::cout << "|||>>> y invert requirement detected" << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
}
if (in_intendedFaces.z != in_moveVals.z)
{

	*in_zIntRef = OrganicUtils::invertSlope(*in_zIntRef);
	//std::cout << "|||>>> z invert requirement detected" << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
}
*/



	if
		(
		(in_intendedFaces.x != in_moveVals.x)
			&&
			in_intendedFaces.x != 0
			&&
			in_moveVals.x != 0
			)
	{
		*in_xIntRef = invertSlope(*in_xIntRef);	// invert x
																		//std::cout << "|||>>> x invert requirement detected" << current_x_slope.x << ", " << current_x_slope.y << ", " << current_x_slope.z << std::endl
	}
	if
		(
		(in_intendedFaces.y != in_moveVals.y)
			&&
			in_intendedFaces.y != 0
			&&
			in_moveVals.y != 0
			)
	{
		*in_yIntRef = invertSlope(*in_yIntRef);
		//std::cout << "|||>>> y invert requirement detected" << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
	}

	if
		(
		(in_intendedFaces.z != in_moveVals.z)
			&&
			in_intendedFaces.z != 0
			&&
			in_moveVals.z != 0
			)
	{

		*in_zIntRef = invertSlope(*in_zIntRef);
		//std::cout << "|||>>> z invert requirement detected" << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
	}
}

ECBPolyPoint IndependentUtils::invertSlope(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint returnPoint = in_polyPoint;	// set default value
	if (in_polyPoint.x != 0.0f)	// only invert x if it isn't 0
	{
		returnPoint.x *= -1.0f;
	}

	if (in_polyPoint.y != 0.0f)
	{
		returnPoint.y *= -1.0f;
	}

	if (in_polyPoint.z != 0.0f)
	{
		returnPoint.z *= -1.0f;
	}
	return returnPoint;
}

ECBPolyPoint IndependentUtils::getSlopeToUse(ECBPPOrientations in_interceptType, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope)
{
	if (in_interceptType == ECBPPOrientations::X_INT)
	{
		//std::cout << "!!! Returning x-int..." << std::endl;
		return in_xSlope;

	}
	else if (in_interceptType == ECBPPOrientations::Y_INT)
	{
		//std::cout << "!!! Returning y-int..." << std::endl;
		return in_ySlope;
	}
	else if (in_interceptType == ECBPPOrientations::Z_INT)
	{
		//std::cout << "!!! Returning z-int..." << std::endl;
		return in_zSlope;
	}
	else
	{
		std::cout << "|||| WARNING: No match found... " << std::endl;
		ECBPolyPoint badPoint;
		return badPoint;	// ++should never return, makes compiler happy (won't produce a warning)
	}
}

ECBPolyPoint IndependentUtils::findSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	ECBPolyPoint returnPoint;
	returnPoint.x = in_pointB.x - in_pointA.x;
	returnPoint.y = in_pointB.y - in_pointA.y;
	returnPoint.z = in_pointB.z - in_pointA.z;
	return returnPoint;
}

ECBPolyType IndependentUtils::convertIntToPolyType(int in_polyTypeInt)
{
	ECBPolyType returnPolyType = ECBPolyType::FREE;		// default, will be overwritten
	if (in_polyTypeInt == 0)
	{
		returnPolyType = ECBPolyType::FREE;
	}
	else if (in_polyTypeInt == 1)
	{
		returnPolyType = ECBPolyType::ROGUE;
	}
	else if (in_polyTypeInt == 2)
	{
		returnPolyType = ECBPolyType::SHELL;
	}
	else if (in_polyTypeInt == 3)
	{
		returnPolyType = ECBPolyType::SHELL_MASSDRIVER;
	}
	return returnPolyType;
}


InterceptValidity IndependentUtils::determineInterceptValidity(ECBPolyPoint in_xInt, 
															ECBPolyPoint in_yInt, 
															ECBPolyPoint in_zInt, 
															ECBPolyPoint in_slopeToCheck, 
															PerfectClampEnum in_perfectClampValue)
{
	InterceptValidity validity = InterceptValidity::VALID;	// initially set as valid, but may change to invalid.
//std::cout << "#### Perfect clamp value test:  " << in_perfectClampValue << std::endl;
//std::cout << "#### In slope to check is: " << in_slopeToCheck.x << ", " << in_slopeToCheck.y << ", " << in_slopeToCheck.z << std::endl;
//std::cout << "# X-int: " << in_xInt.x << ", " << in_xInt.y << ", " << in_xInt.z << std::endl;
//std::cout << "# Y-int: " << in_yInt.x << ", " << in_yInt.y << ", " << in_yInt.z << std::endl;
//std::cout << "# Z-int: " << in_zInt.x << ", " << in_zInt.y << ", " << in_zInt.z << std::endl;
	if (checkIfPolyPointsMatch(in_xInt, in_slopeToCheck) == 1)
	{
		//std::cout << ">> Input slope matches X-int. " << std::endl;
	}

	if (checkIfPolyPointsMatch(in_yInt, in_slopeToCheck) == 1)
	{
		//std::cout << ">> Input slope matches Y-int. " << std::endl;
	}

	if (checkIfPolyPointsMatch(in_zInt, in_slopeToCheck) == 1)
	{
		//std::cout << ">> Input slope matches Z-int. " << std::endl;
	}
	// x check.
	if
	(
		(checkIfPolyPointsMatch(in_xInt, in_slopeToCheck) == 1)
		
		&&
			!(checkIfPolyPointsMatch(in_xInt, in_yInt) == 1)				// can't match another slope, if it's going to be invalid.
			&&																//
			!(checkIfPolyPointsMatch(in_xInt, in_zInt) == 1)				//


			&&
			(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_X)	// is it clamped to X?
	)
	{
		//std::cout << "!!! X-intercept slopes are invalid! " << std::endl;
		validity = InterceptValidity::INVALID;
	}

	else if
	(
		(checkIfPolyPointsMatch(in_yInt, in_slopeToCheck) == 1)
			&&


			!(checkIfPolyPointsMatch(in_yInt, in_xInt) == 1)				// can't match another slope, if it's going to be invalid.
			&&																//
			!(checkIfPolyPointsMatch(in_yInt, in_zInt) == 1)				//


			&&
			(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_Y)	// is it clamped to Y
	)
	{
		//std::cout << "!!! Y-intercept slopes are invalid! " << std::endl;
		validity = InterceptValidity::INVALID;
	}

	else if
		(
		(checkIfPolyPointsMatch(in_zInt, in_slopeToCheck) == 1)
			&&


			!(checkIfPolyPointsMatch(in_zInt, in_xInt) == 1)				// can't match another slope, if it's going to be invalid.
			&&																//
			!(checkIfPolyPointsMatch(in_zInt, in_yInt) == 1)				//


			&&
			(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_Z)	// is it clamped to Z?
			)
	{
		//std::cout << "!!! Z-intercept slopes are invalid! " << std::endl;
		validity = InterceptValidity::INVALID;
	}

	//std::cout << "!!!! end validity check. " << std::endl;
	return validity;
}

int IndependentUtils::checkIfPolyPointsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	int pointMatch = 0;	// default is 0; means no match.
	if
		(
		(in_pointA.x == in_pointB.x)
			&&
			(in_pointA.y == in_pointB.y)
			&&
			(in_pointA.z == in_pointB.z)
			)
	{
		pointMatch = 1;
	}
	return pointMatch;
}

ECBPolyPoint IndependentUtils::getInterceptToUseFromLine(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2, InterceptValidity in_firstInterceptValidity, InterceptValidity in_secondInterceptValidity, EnclaveKeyDef::EnclaveKey in_moveVals)
{
	int first_intercept_invalid = 0;		// for storing if the first intercept value is valid selection, start at 0 (meaning its valid)
	int second_intercept_invalid = 0;		// for storing if the second intercept value is valid selection, start at 0 ""
	ECBPolyPoint normalized_first_intercept = IndependentUtils::findNormalizedPoint(in_intercept1);
	ECBPolyPoint normalized_second_intercept = IndependentUtils::findNormalizedPoint(in_intercept2);
	ECBPolyPoint returnPoint;
	if (
		(
		(in_moveVals.x == normalized_first_intercept.x)
			&&
			(in_moveVals.x != 0.0f)
			)
		||
		(
		(in_moveVals.y == normalized_first_intercept.y)
			&&
			(in_moveVals.y != 0.0f)
			)
		||
		(
		(in_moveVals.z == normalized_first_intercept.z)
			&&
			(in_moveVals.z != 0.0f)
			)
		||
		(in_firstInterceptValidity == InterceptValidity::INVALID)
		)
	{
		//std::cout << "First intercept set to invalid..." << std::endl;
		first_intercept_invalid = 1;
	}

	// compare values for second intercept
	if (
		(
		(in_moveVals.x == normalized_second_intercept.x)
			&&
			(in_moveVals.x != 0.0f)
			)
		||
		(
		(in_moveVals.y == normalized_second_intercept.y)
			&&
			(in_moveVals.y != 0.0f)
			)
		||
		(
		(in_moveVals.z == normalized_second_intercept.z)
			&&
			(in_moveVals.z != 0.0f)
			)
		||
		(in_secondInterceptValidity == InterceptValidity::INVALID)
		)
	{
		//std::cout << "Second intercept set to invalid..." << std::endl;
		second_intercept_invalid = 1;
	}

	// use the second intercept
	if ((first_intercept_invalid == 1) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept2;
		//std::cout << "Second slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// use the first intercept
	else if ((second_intercept_invalid == 1) && (first_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "First slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// if both are valid, use the first one
	else if ((first_intercept_invalid == 0) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "++++++++++++++++++++++++++++Both slopes technically valid, returning first. " << std::endl;
	}
	return returnPoint;
}

ECBPolyPoint IndependentUtils::getInterceptToUseFromLineDebug(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2, InterceptValidity in_firstInterceptValidity, InterceptValidity in_secondInterceptValidity, EnclaveKeyDef::EnclaveKey in_moveVals)
{
	int first_intercept_invalid = 0;		// for storing if the first intercept value is valid selection, start at 0 (meaning its valid)
	int second_intercept_invalid = 0;		// for storing if the second intercept value is valid selection, start at 0 ""
	ECBPolyPoint normalized_first_intercept = IndependentUtils::findNormalizedPoint(in_intercept1);
	ECBPolyPoint normalized_second_intercept = IndependentUtils::findNormalizedPoint(in_intercept2);
	std::cout << "---Debug info begins here---" << std::endl;
	std::cout << "Move vals: " << in_moveVals.x << ", " << in_moveVals.y << ", " << in_moveVals.z << std::endl;
	std::cout << "Normalized_first: " << normalized_first_intercept.x << ", " << normalized_first_intercept.y << ", " << normalized_first_intercept.z << ", " << std::endl;
	std::cout << "Normalized_second: " << normalized_second_intercept.x << ", " << normalized_second_intercept.y << ", " << normalized_second_intercept.z << ", " << std::endl;
	ECBPolyPoint returnPoint;
	if (
		(
		(in_moveVals.x == normalized_first_intercept.x)
			&&
			(in_moveVals.x != 0.0f)
			)
		||
		(
		(in_moveVals.y == normalized_first_intercept.y)
			&&
			(in_moveVals.y != 0.0f)
			)
		||
		(
		(in_moveVals.z == normalized_first_intercept.z)
			&&
			(in_moveVals.z != 0.0f)
			)
		||
		(in_firstInterceptValidity == InterceptValidity::INVALID)
		)
	{
		std::cout << "First intercept set to invalid..." << std::endl;
		if (in_firstInterceptValidity == InterceptValidity::INVALID)
		{
			std::cout << ">>> InterceptValidity was invalid. " << std::endl;
		}
		first_intercept_invalid = 1;
	}

	// compare values for second intercept
	if (
		(
		(in_moveVals.x == normalized_second_intercept.x)
			&&
			(in_moveVals.x != 0.0f)
			)
		||
		(
		(in_moveVals.y == normalized_second_intercept.y)
			&&
			(in_moveVals.y != 0.0f)
			)
		||
		(
		(in_moveVals.z == normalized_second_intercept.z)
			&&
			(in_moveVals.z != 0.0f)
			)
		||
		(in_secondInterceptValidity == InterceptValidity::INVALID)
		)
	{
		std::cout << "Second intercept set to invalid..." << std::endl;
		if (in_secondInterceptValidity == InterceptValidity::INVALID)
		{
			std::cout << ">>> InterceptValidity was invalid. " << std::endl;
		}
		second_intercept_invalid = 1;
	}

	// use the second intercept
	if ((first_intercept_invalid == 1) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept2;
		//std::cout << "Second slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// use the first intercept
	else if ((second_intercept_invalid == 1) && (first_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "First slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// if both are valid, use the first one
	else if ((first_intercept_invalid == 0) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "Both slopes technically valid, returning first. " << std::endl;
	}
	else if ((first_intercept_invalid == 1) && (second_intercept_invalid == 1))
	{
		std::cout << "!!!! WARNING: invalid return point! " << std::endl;
	}
	std::cout << "Returned point value is: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	return returnPoint;
}

ECBPolyPoint IndependentUtils::getInterceptToUseFromCorner(ECBPolyPoint in_xSlope, 
														ECBPolyPoint in_ySlope, 
														ECBPolyPoint in_zSlope, 
														PerfectClampEnum in_perfectClampValue, 
														EnclaveKeyDef::EnclaveKey in_moveVals)
{
	ECBPolyPoint returnPoint;
	int x_intercept_invalid = 0;	// for storing if the x intercept value is valid selection, start at 0 (meaning its valid)
	int y_intercept_invalid = 0;	// ""  y intercept
	int z_intercept_invalid = 0;	// ""  z intercept
	ECBPolyPoint normalized_x_intercept = IndependentUtils::findNormalizedPoint(in_xSlope);
	ECBPolyPoint normalized_y_intercept = IndependentUtils::findNormalizedPoint(in_ySlope);
	ECBPolyPoint normalized_z_intercept = IndependentUtils::findNormalizedPoint(in_zSlope);

	// check for x intercept invalid
	if ((in_moveVals.y == normalized_x_intercept.y)
		||
		(in_moveVals.z == normalized_x_intercept.z)
		||
		(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_X)	// x clamp check
		)
	{
		x_intercept_invalid = 1;
		//std::cout << "Corner calc: x int is invalid!!" << std::endl;
	}

	// check for y intercept invalid
	if ((in_moveVals.x == normalized_y_intercept.x)
		||
		(in_moveVals.z == normalized_y_intercept.z)
		||
		(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_Y)	// y clamp check
		)
	{
		y_intercept_invalid = 1;
		//std::cout << "Corner calc: y int is invalid!!" << std::endl;
	}

	// check for z intercept invalid
	if ((in_moveVals.x == normalized_z_intercept.x)
		||
		(in_moveVals.y == normalized_z_intercept.y)
		||
		(in_perfectClampValue == PerfectClampEnum::CLAMPED_TO_Z)	// z clamp check
		)
	{
		z_intercept_invalid = 1;
		//std::cout << "Corner calc: z int is invalid!!" << std::endl;
	}

	// return appropriate point
	if (x_intercept_invalid == 0)
	{
		//std::cout << "Returning valid x-slope!" << std::endl;
		returnPoint = in_xSlope;
	}
	else if (y_intercept_invalid == 0)
	{
		//std::cout << "Returning valid y-slope!" << std::endl;
		//std::cout << "-------> perfect clamp value waS: " << in_perfectClampValue << std::endl;
		returnPoint = in_ySlope;
	}
	else if (z_intercept_invalid == 0)
	{
		//std::cout << "Returning valid z-slope!" << std::endl;
		returnPoint = in_zSlope;
	}
	return returnPoint;

}

bool IndependentUtils::checkIfInterceptIsValid(ECBPolyPoint in_interceptToCheck)
{
	bool result = true;
	if
		(
		(in_interceptToCheck.x == 0)
			&&
			(in_interceptToCheck.y == 0)
			&&
			(in_interceptToCheck.z == 0)
			)
	{
		result = false;
	}
	return result;
}

BorderMDFaceList IndependentUtils::getFaceList(ECBPPOrientationResults in_beginOrientation, BorderDataMap* in_borderDataMapRef)
{
	BorderMDFaceList returnFaceList;
	// get values from face
	if (in_beginOrientation.otype == ECBPPOrientations::FACE)
	{
		//std::cout << "Retrieving data from FACE " << std::endl;
		returnFaceList.faceList[0] = in_beginOrientation.osubtype;	// store the face's actual value in here
	}

	// get values from a line
	else if (in_beginOrientation.otype == ECBPPOrientations::LINE)
	{
		//std::cout << "Retrieving data from LINE " << std::endl;
		returnFaceList.faceList[0] = in_borderDataMapRef->bdLinesMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->bdLinesMap[in_beginOrientation.osubtype].linkedFaces[1];
	}

	// get values from a corner
	else if (in_beginOrientation.otype == ECBPPOrientations::CORNER)
	{
		//std::cout << "Point is at corner: " << std::endl;
		//std::cout << "Retrieving data from CORNER " << std::endl;
		returnFaceList.faceList[0] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[1];
		returnFaceList.faceList[2] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[2];

	}
	return returnFaceList;
}

BorderMDFaceList IndependentUtils::getFaceListDebug(ECBPPOrientationResults in_beginOrientation, BorderDataMap* in_borderDataMapRef)
{
	BorderMDFaceList returnFaceList;
	// get values from face
	if (in_beginOrientation.otype == ECBPPOrientations::FACE)
	{
		std::cout << "Retrieving data from FACE " << std::endl;
		returnFaceList.faceList[0] = in_beginOrientation.osubtype;	// store the face's actual value in here
	}

	// get values from a line
	else if (in_beginOrientation.otype == ECBPPOrientations::LINE)
	{
		std::cout << "Retrieving data from LINE " << std::endl;
		returnFaceList.faceList[0] = in_borderDataMapRef->bdLinesMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->bdLinesMap[in_beginOrientation.osubtype].linkedFaces[1];
	}

	// get values from a corner
	else if (in_beginOrientation.otype == ECBPPOrientations::CORNER)
	{
		std::cout << "Point is at corner: " << std::endl;
		//std::cout << "Retrieving data from CORNER " << std::endl;
		returnFaceList.faceList[0] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[1];
		returnFaceList.faceList[2] = in_borderDataMapRef->cornerMap[in_beginOrientation.osubtype].linkedFaces[2];

	}
	return returnFaceList;
}

void IndependentUtils::printOrientationEnum(ECBPPOrientations in_pointOrientation)
{
	switch (in_pointOrientation)
	{
		// corners
		case ECBPPOrientations::CORNER_LOWERNE: { std::cout << "CORNER_LOWERNE" << std::endl; break; }
		case ECBPPOrientations::CORNER_LOWERNW: { std::cout << "CORNER_LOWERNW" << std::endl; break; }
		case ECBPPOrientations::CORNER_LOWERSE: { std::cout << "CORNER_LOWERSE" << std::endl; break; }
		case ECBPPOrientations::CORNER_LOWERSW: { std::cout << "CORNER_LOWERSW" << std::endl; break; }
		case ECBPPOrientations::CORNER_UPPERNE: { std::cout << "CORNER_UPPERNE" << std::endl; break; }
		case ECBPPOrientations::CORNER_UPPERNW: { std::cout << "CORNER_UPPERNW" << std::endl; break; }
		case ECBPPOrientations::CORNER_UPPERSE: { std::cout << "CORNER_UPPERSE" << std::endl; break; }
		case ECBPPOrientations::CORNER_UPPERSW: { std::cout << "CORNER_UPPERSW" << std::endl; break; }

		// lines
		case ECBPPOrientations::ZAXIS_LOWERWEST: { std::cout << "ZAXIS_LOWERWEST" << std::endl; break; }
		case ECBPPOrientations::ZAXIS_LOWEREAST: { std::cout << "ZAXIS_LOWEREAST" << std::endl; break; }
		case ECBPPOrientations::ZAXIS_UPPERWEST: { std::cout << "ZAXIS_UPPERWEST" << std::endl; break; }
		case ECBPPOrientations::ZAXIS_UPPEREAST: { std::cout << "ZAXIS_UPPEREAST" << std::endl; break; }
		case ECBPPOrientations::XAXIS_LOWERNORTH: { std::cout << "XAXIS_LOWERNORTH" << std::endl; break; }
		case ECBPPOrientations::XAXIS_LOWERSOUTH: { std::cout << "XAXIS_LOWERSOUTH" << std::endl; break; }
		case ECBPPOrientations::XAXIS_UPPERNORTH: { std::cout << "XAXIS_UPPERNORTH" << std::endl; break; }
		case ECBPPOrientations::XAXIS_UPPERSOUTH: { std::cout << "XAXIS_UPPERSOUTH" << std::endl; break; }
		case ECBPPOrientations::YAXIS_NORTHWEST: { std::cout << "YAXIS_NORTHWEST" << std::endl; break; }
		case ECBPPOrientations::YAXIS_NORTHEAST: { std::cout << "YAXIS_NORTHEAST" << std::endl; break; }
		case ECBPPOrientations::YAXIS_SOUTHWEST: { std::cout << "YAXIS_SOUTHWEST" << std::endl; break; }
		case ECBPPOrientations::YAXIS_SOUTHEAST: { std::cout << "YAXIS_SOUTHEAST" << std::endl; break; }

		// faces
		case ECBPPOrientations::NORTHFACE: {std::cout << "NORTHFACE" << std::endl; break; }
		case ECBPPOrientations::EASTFACE: {std::cout << "EASTFACE" << std::endl; break; }
		case ECBPPOrientations::SOUTHFACE: {std::cout << "SOUTHFACE" << std::endl; break; }
		case ECBPPOrientations::WESTFACE: {std::cout << "WESTFACE" << std::endl; break; }
		case ECBPPOrientations::TOPFACE: {std::cout << "TOPFACE" << std::endl; break; }
		case ECBPPOrientations::BOTTOMFACE: {std::cout << "BOTTOMFACE" << std::endl; break; }

		// interior
		case ECBPPOrientations::FREE: {std::cout << "FREE" << std::endl; break; }
	}
}

ECBPolyPoint IndependentUtils::snapPointToOrganicGrid(ECBPolyPoint in_polyPoint, float in_gridLimit)
{
	std::cout << "::::: original point is: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << std::endl;
	ECBPolyPoint originalPoint = in_polyPoint;
	float up_scaled_x = originalPoint.x * 100.0f;
	float up_scaled_y = originalPoint.y * 100.0f;
	float up_scaled_z = originalPoint.z * 100.0f;
	float new_x = ((floor(up_scaled_x) ) + 0.5f) / 100;
	float new_y = ((floor(up_scaled_y) ) + 0.5f) / 100;
	float new_z = ((floor(up_scaled_z) ) + 0.5f) / 100;

	std::cout << "::::: new point is: " << new_x << ", " << new_y << ", " << new_z << std::endl;

	// calibrate x against limits
	if (new_x > in_gridLimit)
	{
		new_x = in_gridLimit;
	}
	else if (new_x < 0.0f)
	{
		new_x = 0.0f;
	}

	// calibrate y against limits
	if (new_y > in_gridLimit)
	{
		new_y = in_gridLimit;
	}
	else if (new_y < 0.0f)
	{
		new_y = 0.0f;
	}

	// calibrate z against limits
	if (new_z > in_gridLimit)
	{
		new_z = in_gridLimit;
	}
	else if (new_z < 0.0f)
	{
		new_z = 0.0f;
	}

	return ECBPolyPoint(new_x, new_y, new_z);
}

int IndependentUtils::checkIfFaceListsMatch(BorderMDFaceList in_faceListA, BorderMDFaceList in_faceListB, int in_numberOfMatches)
{
	int matchFound = 0;
	int criteriaMet = 0;

	/*
	std::cout << "Face List A values:" << std::endl;
	for (int x = 0; x < 3; x++)
	{
		if (in_faceListA.faceList[x] == ECBPPOrientations::WESTFACE)
		{
			std::cout << "WESTFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::EASTFACE)
		{
			std::cout << "EASTFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::NORTHFACE)
		{
			std::cout << "NORTHFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::SOUTHFACE)
		{
			std::cout << "SOUTHFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::TOPFACE)
		{
			std::cout << "TOPFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::BOTTOMFACE)
		{
			std::cout << "BOTTOMFACE " << std::endl;
		}
		else if (in_faceListA.faceList[x] == ECBPPOrientations::NOVAL)
		{
			std::cout << "NOVAL" << std::endl;
		}
	}
	std::cout << "++++++++++++++++++++++++++" << std::endl;
	std::cout << "Face List B values:" << std::endl;
	for (int x = 0; x < 3; x++)
	{
		if (in_faceListB.faceList[x] == ECBPPOrientations::WESTFACE)
		{
			std::cout << "WESTFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::EASTFACE)
		{
			std::cout << "EASTFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::NORTHFACE)
		{
			std::cout << "NORTHFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::SOUTHFACE)
		{
			std::cout << "SOUTHFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::TOPFACE)
		{
			std::cout << "TOPFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::BOTTOMFACE)
		{
			std::cout << "BOTTOMFACE " << std::endl;
		}
		else if (in_faceListB.faceList[x] == ECBPPOrientations::NOVAL)
		{
			std::cout << "NOVAL" << std::endl;
		}
	}
	*/

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if ((in_faceListA.faceList[x] == in_faceListB.faceList[y]) &&
				(in_faceListA.faceList[x] != ECBPPOrientations::NOVAL)		// NOVAL is the default initialized value, so don't bother checking for it
				&&
				(in_faceListB.faceList[y] != ECBPPOrientations::NOVAL)
				)
			{
				matchFound++;
				//std::cout << "match found!!!!!` " << std::endl;
			}
		}
	}
	if (matchFound >= in_numberOfMatches)
	{
		//std::cout << ">>>>> !!!Face match detected!!! <<<<< (" << matchFound << " face match) " << std::endl;
		criteriaMet = 1;
	}
	return criteriaMet;
}

EnclaveBlockVertex IndependentUtils::convertPolyPointToBlockVertex(ECBPolyPoint in_polyPoint)
{
	EnclaveBlockVertex returnVertex;

	// ten-thousndths rounding
	ECBPolyPoint roundedPointsTenThousandths = roundPolyPointToTenThousandths(in_polyPoint);
	ECBPolyPoint roundedPointsThousandths = roundPolyPointToTenThousandths(roundedPointsTenThousandths);
	ECBPolyPoint roundedPoints = roundPolyPointToHundredths(roundedPointsThousandths);

	// thousandths rounding
	//ECBPolyPoint roundedPointsThousandths = roundPolyPointToThousandths(in_polyPoint);
	//ECBPolyPoint roundedPoints = roundPolyPointToHundredths(roundedPointsThousandths);

	//ECBPolyPoint roundedPoints = roundPolyPointToHundredths(in_polyPoint);	// get a value of the points rounded to nearest one hundredth
	//std::cout << "Rounded points: " << std::endl;
	//std::cout << roundedPoints.x << std::endl;
	//std::cout << roundedPoints.y << std::endl;
	//std::cout << roundedPoints.z << std::endl;
	returnVertex.x = unsigned char(roundedPoints.x * 100.0f);
	returnVertex.y = unsigned char(roundedPoints.y * 100.0f);
	returnVertex.z = unsigned char(roundedPoints.z * 100.0f);
	//std::cout << "Test of blockVertex conversion: " << std::endl;
	//std::cout << char(returnVertex.x) << std::endl;
	//std::cout << char(returnVertex.y) << std::endl;
	//std::cout << char(returnVertex.z) << std::endl;
	//if (returnVertex.x == 49)
	//{
		//std::cout << "Hey! ho! let's go!" << std::endl;
	//}
	return returnVertex;
}

EnclaveBlockVertex IndependentUtils::convertFTriangleDoublePointToBlockVertex(DoublePoint in_fTriangleDoublePoint)
{
	// There should no be need for round for points from an FTriangle, as that is handled by the FTriangle itself.
	// This function also assumes that the values being passed in are from a FTriangleOutput that has a destingedGridType of FTriangleType::BLOCK.
	EnclaveBlockVertex returnVertex;
	returnVertex.x = unsigned char(in_fTriangleDoublePoint.x * 100.0f);
	returnVertex.y = unsigned char(in_fTriangleDoublePoint.y * 100.0f);
	returnVertex.z = unsigned char(in_fTriangleDoublePoint.z * 100.0f);
	return returnVertex;
}

ECBPolyPoint IndependentUtils::findNormalizedPoint(ECBPolyPoint in_pointA)
{
	ECBPolyPoint returnPoint;
	// make sure x is not 0
	if (!((in_pointA.x) == 0.0f))
	{
		returnPoint.x = (in_pointA.x) / abs(in_pointA.x);
	}
	else
	{
		returnPoint.x = 0.0f;
	}

	// make sure y is not 0
	if (!((in_pointA.y) == 0.0f))
	{
		returnPoint.y = (in_pointA.y) / abs(in_pointA.y);
	}
	else
	{
		returnPoint.y = 0.0f;
	}

	// make sure z is not 0
	if (!((in_pointA.z) == 0.0f))
	{
		returnPoint.z = (in_pointA.z) / abs(in_pointA.z);
	}
	else
	{
		returnPoint.z = 0.0f;
	}
	return returnPoint;
}

ECBPolyPoint IndependentUtils::findNormalizedSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	ECBPolyPoint returnPoint;
	//std::cout << "(pre-normalization) point A values: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;
	//std::cout << "(pre-normalization) point B values: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	//returnPoint.x = (in_pointB.x - in_pointA.x) / abs(in_pointB.x - in_pointA.x);	// get normalized x

	// make sure x slope is not 0
	if (!((in_pointB.x - in_pointA.x) == 0.0f))
	{
		returnPoint.x = (in_pointB.x - in_pointA.x) / abs(in_pointB.x - in_pointA.x);	// " y
	}
	else
	{
		returnPoint.x = 0.0f;
	}



	// make sure y slope is not 0 
	if (!((in_pointB.y - in_pointA.y) == 0.0f))
	{
		returnPoint.y = (in_pointB.y - in_pointA.y) / abs(in_pointB.y - in_pointA.y);	// " y
	}
	else
	{
		returnPoint.y = 0.0f;
	}

	// make sure z slope is not 0
	if (!((in_pointB.z - in_pointA.z) == 0.0f))
	{
		returnPoint.z = (in_pointB.z - in_pointA.z) / abs(in_pointB.z - in_pointA.z);	// " y
	}
	else
	{
		//std::cout << "WARNING>>>>>: ZERO Z slope DETECTED! " << std::endl;
		returnPoint.z = 0.0f;
	}

	// returnPoint.z = (in_pointB.z - in_pointA.z) / abs(in_pointB.z - in_pointA.z);	// " z
	//std::cout << "normalized slope return val is: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	return returnPoint;
}

ECBPolyPoint IndependentUtils::findNormalizedSlope(DoublePoint in_pointA, DoublePoint in_pointB)
{
	ECBPolyPoint returnPoint;
	//std::cout << "(pre-normalization) point A values: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;
	//std::cout << "(pre-normalization) point B values: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	//returnPoint.x = (in_pointB.x - in_pointA.x) / abs(in_pointB.x - in_pointA.x);	// get normalized x

	// make sure x slope is not 0
	if (!((in_pointB.x - in_pointA.x) == 0.0f))
	{
		returnPoint.x = (in_pointB.x - in_pointA.x) / abs(in_pointB.x - in_pointA.x);	// " y
	}
	else
	{
		returnPoint.x = 0.0f;
	}



	// make sure y slope is not 0 
	if (!((in_pointB.y - in_pointA.y) == 0.0f))
	{
		returnPoint.y = (in_pointB.y - in_pointA.y) / abs(in_pointB.y - in_pointA.y);	// " y
	}
	else
	{
		returnPoint.y = 0.0f;
	}

	// make sure z slope is not 0
	if (!((in_pointB.z - in_pointA.z) == 0.0f))
	{
		returnPoint.z = (in_pointB.z - in_pointA.z) / abs(in_pointB.z - in_pointA.z);	// " y
	}
	else
	{
		//std::cout << "WARNING>>>>>: ZERO Z slope DETECTED! " << std::endl;
		returnPoint.z = 0.0f;
	}

	// returnPoint.z = (in_pointB.z - in_pointA.z) / abs(in_pointB.z - in_pointA.z);	// " z
	//std::cout << "normalized slope return val is: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	return returnPoint;
}

float IndependentUtils::findNormalizedDirection(float in_dimValueA, float in_dimValueB)
{
	float returnPoint;
	if (!((in_dimValueB - in_dimValueA) == 0.0f))
	{
		returnPoint = (in_dimValueB - in_dimValueA) / abs(in_dimValueB - in_dimValueA);	// " y
	}
	else
	{
		returnPoint = 0.0f;
	}
	return returnPoint;
}

ECBPolyPoint IndependentUtils::getBlockTracingEndpoint(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef)
{
	ECBPolyPoint pointToReturn;
	//std::cout << "OrganicUtils::getBlockTracingEndpoint begin point is: " << in_beginPoint.x << ", " << in_beginPoint.y << ", " << in_beginPoint.z << std::endl;
	ECBPolyPoint slopeDirection = IndependentUtils::findNormalizedPoint(in_slope); // get normalized slope
	ECBPolyPoint lineSlope = in_slope;
	ECBPolyPoint currentIterationBeginPoint = in_beginPoint;

	// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
	int x_dir = int(slopeDirection.x);
	int y_dir = int(slopeDirection.y);
	int z_dir = int(slopeDirection.z);

	float x_interceptCoord = 0.0f;
	float y_interceptCoord = 0.0f;
	float z_interceptCoord = 0.0f;

	// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
	float powSlopeX = pow(lineSlope.x, 2.0f);
	float powSlopeY = pow(lineSlope.y, 2.0f);
	float powSlopeZ = pow(lineSlope.z, 2.0f);
	float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

	// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	ECBPolyPoint calculatedPoint_for_y;
	ECBPolyPoint calculatedPoint_for_z;
	//std::cout << ">>>>>>>>> Current iter begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>>> Current iteration point values are: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>> Slope directions: " << slopeDirection.x << ", " << slopeDirection.y << ", " << slopeDirection.z << std::endl;
	//std::cout << "Line slopes: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
	// calculate distance to intercept for x-plane
	if (slopeDirection.x > 0)
	{
		x_dir = 1;			// going towards positive x 
		x_interceptCoord = 1.0f;								// x_interceptCoord is equal to 1.0f (EAST block border)
		float origin_to_border_x_diff = (x_interceptCoord - currentIterationBeginPoint.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
																											//std::cout << "line slope vals: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
		time_to_complete_x_traversal = origin_to_border_x_diff / lineSlope.x;	// get the distance that the ray has to travel to get to this value of x
																				//ECBPolyPoint calculatedPoint_for_x;					// the actual point as it would exist on the EAST face
																				//std::cout << "current iter begin point x: " << currentIterationBeginPoint.x << std::endl;
																				//std::cout << "current iter begin point y: " << currentIterationBeginPoint.y << std::endl;
																				//std::cout << "current iter begin point z: " << currentIterationBeginPoint.z << std::endl;
																				//std::cout << "time for x traversal: " << time_to_complete_x_traversal << std::endl;
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "Calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
		//std::cout << "(pos) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}
	else if (slopeDirection.x < 0)
	{
		x_dir = -1;			// going towards negative x
		x_interceptCoord = 0.0f;								// x_interceptCoord is equal to 0.0f (WEST block border)
		float origin_to_border_x_diff = abs(x_interceptCoord - currentIterationBeginPoint.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / lineSlope.x);	// ""
																					//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the WEST face
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);	// "" 
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);	// ""

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(neg) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}

	// ... for y plane
	if (slopeDirection.y > 0)
	{
		y_dir = 1;		// going towards positive y
		y_interceptCoord = 1.0f;								// y_interceptCoord is the precise location of y at the TOP face border
		float origin_to_border_y_diff = y_interceptCoord - currentIterationBeginPoint.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
		time_to_complete_y_traversal = origin_to_border_y_diff / lineSlope.y;	// get the distance this ray has to travel to get to this value of y
																				//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the TOP face
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);


	}
	else if (slopeDirection.y < 0)
	{
		y_dir = -1;
		y_interceptCoord = 0.0f;
		float origin_to_border_y_diff = abs(y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / lineSlope.y);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);

	}

	// ... for z plane
	if (slopeDirection.z > 0)
	{
		z_dir = 1;
		z_interceptCoord = 1.0f;
		float origin_to_border_z_diff = z_interceptCoord - currentIterationBeginPoint.z;
		time_to_complete_z_traversal = origin_to_border_z_diff / lineSlope.z;
		//std::cout << "Z intercept coord: " << z_interceptCoord << std::endl;
		//std::cout << "Current iteration begin point: " << currentIterationBeginPoint.z << std::endl;
		//std::cout << "Line slope z: " << lineSlope.z << std::endl;
		//ECBPolyPoint calculatedPoint;

		//std::cout << "beginPoint.x: " << currentIterationBeginPoint.x << std::endl;
		//std::cout << "line slope.x: " << lineSlope.x << std::endl;
		//std::cout << "time to complete z traversal: " << time_to_complete_z_traversal << std::endl;

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(POS Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;

	}
	else if (slopeDirection.z < 0)
	{
		z_dir = -1;
		z_interceptCoord = 0.0f;
		float origin_to_border_z_diff = abs(z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / lineSlope.z);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(NEG Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;
	}



	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;
	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;

	// std::cout << "Calc'd point x: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;

	PolyLineEndpointMeta returnEndpointData = IndependentUtils::getCalculatedEndpointMetadata(currentIterationBeginPoint, in_blockBorderRef, distanceValues, slopeDirection, triPointParam);
	return returnEndpointData.pointLocation;
}

ECBPolyPoint IndependentUtils::getBlockTracingEndpointDebug(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef)
{
	ECBPolyPoint pointToReturn;
	//std::cout << "OrganicUtils::getBlockTracingEndpoint begin point is: " << in_beginPoint.x << ", " << in_beginPoint.y << ", " << in_beginPoint.z << std::endl;
	ECBPolyPoint slopeDirection = IndependentUtils::findNormalizedPoint(in_slope); // get normalized slope
	ECBPolyPoint lineSlope = in_slope;
	ECBPolyPoint currentIterationBeginPoint = in_beginPoint;

	// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
	int x_dir = int(slopeDirection.x);
	int y_dir = int(slopeDirection.y);
	int z_dir = int(slopeDirection.z);

	float x_interceptCoord = 0.0f;
	float y_interceptCoord = 0.0f;
	float z_interceptCoord = 0.0f;

	// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
	float powSlopeX = pow(lineSlope.x, 2.0f);
	float powSlopeY = pow(lineSlope.y, 2.0f);
	float powSlopeZ = pow(lineSlope.z, 2.0f);
	float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

	// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	ECBPolyPoint calculatedPoint_for_y;
	ECBPolyPoint calculatedPoint_for_z;
	//std::cout << ">>>>>>>>> Current iter begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>>> Current iteration point values are: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>> Slope directions: " << slopeDirection.x << ", " << slopeDirection.y << ", " << slopeDirection.z << std::endl;
	std::cout << "Line slopes: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
	// calculate distance to intercept for x-plane
	if (slopeDirection.x > 0)
	{
		x_dir = 1;			// going towards positive x 
		x_interceptCoord = 1.0f;								// x_interceptCoord is equal to 1.0f (EAST block border)
		float origin_to_border_x_diff = (x_interceptCoord - currentIterationBeginPoint.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
																											//std::cout << "line slope vals: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
		time_to_complete_x_traversal = origin_to_border_x_diff / lineSlope.x;	// get the distance that the ray has to travel to get to this value of x
																				//ECBPolyPoint calculatedPoint_for_x;					// the actual point as it would exist on the EAST face
																				//std::cout << "current iter begin point x: " << currentIterationBeginPoint.x << std::endl;
																				//std::cout << "current iter begin point y: " << currentIterationBeginPoint.y << std::endl;
																				//std::cout << "current iter begin point z: " << currentIterationBeginPoint.z << std::endl;
																				//std::cout << "time for x traversal: " << time_to_complete_x_traversal << std::endl;
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "Calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
		//std::cout << "(pos) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}
	else if (slopeDirection.x < 0)
	{
		x_dir = -1;			// going towards negative x
		x_interceptCoord = 0.0f;								// x_interceptCoord is equal to 0.0f (WEST block border)
		float origin_to_border_x_diff = abs(x_interceptCoord - currentIterationBeginPoint.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / lineSlope.x);	// ""
																					//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the WEST face
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);	// "" 
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);	// ""

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(neg) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}

	// ... for y plane
	if (slopeDirection.y > 0)
	{
		y_dir = 1;		// going towards positive y
		y_interceptCoord = 1.0f;								// y_interceptCoord is the precise location of y at the TOP face border
		float origin_to_border_y_diff = y_interceptCoord - currentIterationBeginPoint.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
		time_to_complete_y_traversal = origin_to_border_y_diff / lineSlope.y;	// get the distance this ray has to travel to get to this value of y
																				//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the TOP face
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);


	}
	else if (slopeDirection.y < 0)
	{
		y_dir = -1;
		y_interceptCoord = 0.0f;
		float origin_to_border_y_diff = abs(y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / lineSlope.y);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);

	}

	// ... for z plane
	if (slopeDirection.z > 0)
	{
		z_dir = 1;
		z_interceptCoord = 1.0f;
		float origin_to_border_z_diff = z_interceptCoord - currentIterationBeginPoint.z;
		time_to_complete_z_traversal = origin_to_border_z_diff / lineSlope.z;
		//std::cout << "Z intercept coord: " << z_interceptCoord << std::endl;
		//std::cout << "Current iteration begin point: " << currentIterationBeginPoint.z << std::endl;
		//std::cout << "Line slope z: " << lineSlope.z << std::endl;
		//ECBPolyPoint calculatedPoint;

		//std::cout << "beginPoint.x: " << currentIterationBeginPoint.x << std::endl;
		//std::cout << "line slope.x: " << lineSlope.x << std::endl;
		//std::cout << "time to complete z traversal: " << time_to_complete_z_traversal << std::endl;

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(POS Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;

	}
	else if (slopeDirection.z < 0)
	{
		z_dir = -1;
		z_interceptCoord = 0.0f;
		float origin_to_border_z_diff = abs(z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / lineSlope.z);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(NEG Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;
	}



	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;
	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;

	// std::cout << "Calc'd point x: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;

	PolyLineEndpointMeta returnEndpointData = IndependentUtils::getCalculatedEndpointMetadataDebug(currentIterationBeginPoint, in_blockBorderRef, distanceValues, slopeDirection, triPointParam);
	return returnEndpointData.pointLocation;
}

PolyLineEndpointMeta IndependentUtils::getCalculatedEndpointMetadata(ECBPolyPoint in_originPoint, BlockBorderLineList* in_blockBorderRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords)
{
	PolyLineEndpointMeta calculatedEndpointData;	// the returned endpoint meta
	EnclaveKeyDef::EnclaveKey newKey;				// the key that will be returned for adding/subtracting x/y/z
	int isPointOnALine = 1;						// 0 indicates a condition in which xyz distance are all equal, 1 indicates two other axis are equal (point would be on a line in that case)
													//float dist_to_X = in_distanceValues.x;			// get distance for x
													//float dist_to_Y = in_distanceValues.y;			// ... for y
													//float dist_to_Z = in_distanceValues.z;			// ... for z
	//float dist_to_X = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.x);			// get distance for x
	//float dist_to_Y = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.y);			// ... for y
	//float dist_to_Z = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.z);
	//float dist_to_X = OrganicUtils::roundToTenThousandths(in_distanceValues.x);			// get distance for x
	//float dist_to_Y = OrganicUtils::roundToTenThousandths(in_distanceValues.y);			// ... for y
	//float dist_to_Z = OrganicUtils::roundToTenThousandths(in_distanceValues.z);

	//std::cout << "Origin point is: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "*** Base distance values: " << in_distanceValues.x << "," << in_distanceValues.y << "," << in_distanceValues.z << std::endl;
	ECBPolyPoint testXYZenhanced = IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecision(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = testXYZenhanced.x;
	float dist_to_Y = testXYZenhanced.y;
	float dist_to_Z = testXYZenhanced.z;

	//ECBPolyPoint x_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[0]);	// get x point																										
	//ECBPolyPoint y_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[1]);	// ...y point																									
	//ECBPolyPoint z_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[2]);	// ...z point

	ECBPolyPoint x_intercept_coords = in_XYZinterceptCoords.triPoints[0];	// get x point																										
	ECBPolyPoint y_intercept_coords = in_XYZinterceptCoords.triPoints[1];	// ...y point																									
	ECBPolyPoint z_intercept_coords = in_XYZinterceptCoords.triPoints[2];	// ...z point


	// We must determine if any distances are equal: It is a corner when:
	// A: X/Y/Z distances are equal
	// B: Two distances are equal,  and the remaining distance's respective coordinate is either on 1.0f or 0.0f.

	// OLD LOGIC

	/*
	if
		(
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
			||																// remove this and folliwng 5 lines for debug
			((dist_to_X == dist_to_Y) && dist_to_Z == 0.0f)
			||
			((dist_to_Y == dist_to_Z) && dist_to_X == 0.0f)
			||
			((dist_to_X == dist_to_Z) && dist_to_Y == 0.0f)

		)
	*/


	if
		(
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
			||																// remove this and folliwng 5 lines for debug
			(
			(dist_to_X == dist_to_Y) &&
				(dist_to_Z == 0.0f) &&
				(
				(in_originPoint.z == 1.0f)
					||
					(in_originPoint.z == 0.0f)
					)
				)
			||
			(
			(dist_to_Y == dist_to_Z) &&
				(dist_to_X == 0.0f) &&
				(
				(in_originPoint.x == 1.0f)
					||
					(in_originPoint.x == 0.0f)
					)
				)
			||
			(
			(dist_to_X == dist_to_Z) &&
				(dist_to_Y == 0.0f) &&
				(
				(in_originPoint.y == 1.0f)
					||
					(in_originPoint.y == 0.0f)
					)
				)

			)

	{
		//std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}
	ECBPolyPoint pointToCheck;				// the actual intercept point that will be used when comparing to border lines, border corners, or border faces
											// Special condition: check to see if origin point is directly on one of the x/y/z planes
											//if (dist_to_Y == dist_to_Z)
											//{
											//std::cout << "Y and Z dist is equal!" << std::endl;
											//}

											// Step 1: check if the resulting point is a perfect corner intercept
											// Multiple conditions:
											//		Condition 1: X, Y, -AND- Z intercept distance are the same 
											//		Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
											//      Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
											//		Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
											//      Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)
											//      Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
											//      Condition 7: in_slope.z is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (z intercept coord's x and y are exactly on border)
	//std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "Dist values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << " |||| Slopes: " << in_slopeDirection.x << ", " << in_slopeDirection.y << ", " << in_slopeDirection.z << std::endl;
	//std::cout << "Hundred thousandth rounding vals are: " << new_x << ", " << new_y << ", " << new_z << std::endl;
	//std::cout << "---------Calculated point MetaData for this iteration-----------" << std::endl;
	//std::cout << "testyval: " << testyval << std::endl;
	//std::cout << "X-intercept coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << " | Distance to x-intercept: " << dist_to_X << std::endl;
	//std::cout << "Y-intercept coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << " | Distance to y-intercept: " << dist_to_Y << std::endl;
	//std::cout << "Z-intercept coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << " | Distance to z-intercept: " << dist_to_Z << std::endl;

	//std::cout << "Distance values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;


	if (

		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == 1.0f) || (in_originPoint.z == 0.0f))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == 1.0f) || (in_originPoint.y == 0.0f))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == 1.0f) || (in_originPoint.x == 0.0f))
			&&
			in_slopeDirection.x == 0.0f
			)

		||

		(														//  Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)		
		(dist_to_X != 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((x_intercept_coords.y == 1.0f) || (x_intercept_coords.y == 0.0f))
				&&
				((x_intercept_coords.z == 1.0f) || (x_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Y != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((y_intercept_coords.x == 1.0f) || (y_intercept_coords.x == 0.0f))
				&&
				((y_intercept_coords.z == 1.0f) || (y_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 7: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Z != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(
			((z_intercept_coords.x == 1.0f) || (z_intercept_coords.x == 0.0f))
				&&
				((z_intercept_coords.y == 1.0f) || (z_intercept_coords.y == 0.0f))
				)
			)


		)

	{
		//std::cout << ":::: corner point hit! " << std::endl;
		/**/
		if (in_slopeDirection.x != 0.0f)
		{

			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);		// x_intercept coords
																											//std::cout << "point to check is x: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);		// y_intercept_coords
																											//std::cout << "point to check is y: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);		// z_intercept coords
																											//std::cout << "point to check is z: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y	&&		pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)		// Lower NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNW.borderValues);
			//std::cout << "Point is at lower NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// Lower SW
		{
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSW.borderValues);
			//std::cout << "Point is at lower SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSW.pointXYZ.x   &&	pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y   &&  pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// Upper SW	 
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSW.borderValues);
			//std::cout << "Point is at upper SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)			// Upper NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNW.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x	&&  pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)			// Lower NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// Lower SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// Upper SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNE.pointXYZ.z)			// Upper NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
			//std::cout << "Point is at upper NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
	}

	// Step 2: check if the resulting point is a border line intercept
	else if ((dist_to_X == dist_to_Y) || (dist_to_Y == dist_to_Z) || (dist_to_X == dist_to_Z))
	{
		//std::cout << ">>>>> PHASE 2 entry: " << std::endl;
		// ||||||||||||||||||||||||||||||||||||||||| condition 1 (Z-lines): check for Z line intercepts, when X and Y are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		if ((dist_to_X == dist_to_Y) && (dist_to_X != 0.0f))							// condition 1: X equals Y, but neither X or Y are 0 (0 would indicate their distances are not used)	
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			if (
				(dist_to_Z > dist_to_X) && (in_slopeDirection.z != 0.0f)		//     condition 1: if Z's distance is greater  than X (could also be y here), AND it's slope is not 0, we go with x/y distance point
				||																//		-- OR --
				(in_slopeDirection.z == 0.0f)									//     condition 2: Z's slope is 0, check x and y
				)	// we can compare to either x or y, since they are both the same value; if Z is greater, it will not hit a Z-face before X and Y does
			{
				//std::cout << "X equals Y, Non-face entry" << std::endl;

				// use x_intercept coords
				// old: pointToCheck = x_intercept_coords (first parameter value of 0, for x)
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// make the pointToCheck equal to either x or y intercept coords
																											//std::cout << "entry check" << std::endl;
																											//perform comparisons to Zaxis borders -- lowerWest, lowerEast, upperWest, upperEast
																											// Lower West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerWest);
					//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
				}

				// Upper West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperWest);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper west line ...shift key is: " << std::endl;
				}

				// Upper East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
					//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper east line ...shift key is: " << std::endl;
				}
				// Lower East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at lower east line ...shift key is: " << std::endl;
				}

			}
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y, so it's a face hit and we don't need to edit the points.

			{
				//std::cout << "X equals Y, Face entry" << std::endl;
				pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				if (in_slopeDirection.z == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->SouthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->NorthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
				}

			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): check for Y line intercepts, , when X and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))							// condition 1: X equals Z, but neither X or Z are 0 (0 would indicate their distance is not used)	
		{

			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			// check if y distance is greater than x and z
			if (
				(dist_to_Y > dist_to_X) && (in_slopeDirection.y != 0.0f)			// condition 1:	if Y's distance is greater  than X (could also be z here), AND it's slope is not 0, we go with x/z distance point
				//||																	// --OR-- 
				//((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))	// condition 2: X distance is not 0, but Y and Z are both 0.0f	(a straight line along the x-axis, with no y/z slope, so NORTH or SOUTH face)
				//||																			// -- OR --
				//((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))	// condition 3:	Z distance is not 0, but X and Y are both 0.0f	(a straight line along the z-axis, with no x/y slope, so WEST or EAST face)
				||
				(in_slopeDirection.y == 0.0f)										// condition 2: Y's slope is 0, check x and y	
				)
			{
				// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northEast);
					//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// South East line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southEast);
					//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// South West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southWest);
					//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// North West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northWest);
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

				}


			}
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				pointToCheck = y_intercept_coords;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
				if (in_slopeDirection.y == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->TopFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->BottomFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
					//std::cout << "X/Z distances match, but Y is hit first (negative y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 3 (X-lines): Y and Z distances match, when Y and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y == dist_to_Z) && (dist_to_Y != 0.0f))
		{
			//std::cout << "BRANCH HIT: DIST_TO_Y equals DIST_TO_Z" << std::endl;
			//std::cout << "Slope direction x: " << in_slopeDirection.x << std::endl;
			//std::cout << "is point on a line>: " << isPointOnALine << std::endl;
			//std::cout << "Z-int coourds, before modification: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << " | Distance to z-intercept: " << dist_to_Z << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			if (
				(dist_to_X > dist_to_Y) && (in_slopeDirection.x != 0.0f)	// condition 1:	if X's distance is greater  than Y (could also be Z here), AND it's slope is not 0, we go with y/z distance point
				||															// --OR-- 
				(in_slopeDirection.x == 0.0f)								// condition 2: X's slope is 0, check y and z
				)
			{
				pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				//std::cout << " >>>> Y equals Z (branch 1) " << std::endl;
				//std::cout << " >>>> Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.x < in_blockBorderRef->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 1) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerNorth);
				}
				// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.x < in_blockBorderRef->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key

					//std::cout << " >>>> Y equals Z (branch 1, 2) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerSouth);
				}
				// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperSW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 3) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperSouth);
				}
				// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperNW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 4) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperNorth);
				}
			}
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << " >>>> Y equals Z (branch 2) " << std::endl;
				pointToCheck = x_intercept_coords;

				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				//std::cout << " |||| (branch 2, 2) Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if (in_slopeDirection.x == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->EastFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), East face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->WestFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), West face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
				}
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for X, but Y and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
																										// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
			else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}


		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
			else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			if ((pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
			else if ((pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}

		}





		else
		{
			//std::cout << "NO MATCH TO ANYTHING " << std::endl;
		}
	}

	// Step 3: else, it's just a normal intercept
	else {
		//std::cout << "::::NORMAL INTERCEPT DETECTED::::" << std::endl;
		isPointOnALine = 0;		// reset this (needs a fix, 1/21/2019)
		//std::cout << "Value of is point on a line: " << isPointOnALine << std::endl;
		float currentShortestTime = 0.0f;
		int currentShortestIndex = -1;		// -1 is default (no valid index selected)
		float distanceValues[3];			// set up a temp array
		distanceValues[0] = in_distanceValues.x;	// store x distance value
		distanceValues[1] = in_distanceValues.y;	// "" y
		distanceValues[2] = in_distanceValues.z;	// "" z

		for (int x = 0; x < 3; x++)
		{
			if (distanceValues[x] != 0.0f)	// is there an actual valid time for this?
			{
				if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
				{
					currentShortestTime = distanceValues[x];	// set the initial length
					currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
				}
				else if (currentShortestTime != 0.0f)
				{
					if (distanceValues[x] < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
					{
						currentShortestTime = distanceValues[x];	// reset the length
						currentShortestIndex = x;							// reset the index
					}
				}
			}
		}
		if (currentShortestIndex == 0)	// x was found
		{
			//pointToCheck = x_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = x_intercept_coords;
			if (in_slopeDirection.x == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				//::cout << "Type 3: line intersects at +X" << std::endl;
				//calculatedEndpointData.moveDirectionValues = in_blockBorderRef->EastFace.
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			//pointToCheck = y_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = y_intercept_coords;
			if (in_slopeDirection.y == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
		}
		else if (currentShortestIndex == 2)		// z was found
		{
			//pointToCheck = z_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			pointToCheck = z_intercept_coords;
			if (in_slopeDirection.z == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
		}

	}
	//std::cout << "whoa" << std::endl;

	calculatedEndpointData.pointLocation = pointToCheck;
	calculatedEndpointData.matchSlopesToDirections(in_slopeDirection);
	//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
	//std::cout << ">>>>>>> end point metadata values" << std::endl;
	//std::cout << "Distance to intercepts: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	//std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "OrganicUtils EndPoint location: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
	//std::cout << "::>>>> Dimension reset values: " << calculatedEndpointData.dimensionResetValues.x << ", " << calculatedEndpointData.dimensionResetValues.y << ", " << calculatedEndpointData.dimensionResetValues.z << ", " << std::endl;
	//std::cout << "((new function)) Move values::: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << ", " << std::endl;

	return calculatedEndpointData;
}

PolyLineEndpointMeta IndependentUtils::getCalculatedEndpointMetadataDebug(ECBPolyPoint in_originPoint, BlockBorderLineList* in_blockBorderRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords)
{
	PolyLineEndpointMeta calculatedEndpointData;	// the returned endpoint meta
	EnclaveKeyDef::EnclaveKey newKey;				// the key that will be returned for adding/subtracting x/y/z
	int isPointOnALine = 1;						// 0 indicates a condition in which xyz distance are all equal, 1 indicates two other axis are equal (point would be on a line in that case)
												//float dist_to_X = in_distanceValues.x;			// get distance for x
												//float dist_to_Y = in_distanceValues.y;			// ... for y
												//float dist_to_Z = in_distanceValues.z;			// ... for z
	//float rounded_to_hunThou_x = OrganicUtils::roundToOneMillionthDebug(in_distanceValues.x);
	//float rounded_to_hunThou_y = OrganicUtils::roundToOneMillionthDebug(in_distanceValues.y);
	//float rounded_to_hunThou_z = OrganicUtils::roundToOneMillionthDebug(in_distanceValues.z);
	//float dist_to_X = OrganicUtils::roundToOneHundredThousandth(rounded_to_hunThou_x);
	//float dist_to_Y = OrganicUtils::roundToOneHundredThousandth(rounded_to_hunThou_y);
	//float dist_to_Z = OrganicUtils::roundToOneHundredThousandth(rounded_to_hunThou_z);


	//float dist_to_X = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.x);			// get distance for x
	//float dist_to_Y = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.y);			// ... for y
	//float dist_to_Z = OrganicUtils::roundToOneHundredThousandth(in_distanceValues.z);
	ECBPolyPoint testXYZenhanced = IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecisionDebug(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = testXYZenhanced.x;
	float dist_to_Y = testXYZenhanced.y;
	float dist_to_Z = testXYZenhanced.z;

	std::cout << "Dist to X enhanced: " << dist_to_X << std::endl;
	std::cout << "Dist to Y enhanced: " << dist_to_Y << std::endl;
	std::cout << "Dist to Z enhanced: " << dist_to_Z << std::endl;

	//float dist_to_X = OrganicUtils::roundToTenThousandths(in_distanceValues.x);			// get distance for x
	//float dist_to_Y = OrganicUtils::roundToTenThousandths(in_distanceValues.y);			// ... for y
	//float dist_to_Z = OrganicUtils::roundToTenThousandths(in_distanceValues.z);


	//ECBPolyPoint x_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[0]);	// get x point																										
	//ECBPolyPoint y_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[1]);	// ...y point																									
	//ECBPolyPoint z_intercept_coords = OrganicUtils::roundPolyPointToOneMillionths(in_XYZinterceptCoords.triPoints[2]);	// ...z point

	ECBPolyPoint x_intercept_coords = in_XYZinterceptCoords.triPoints[0];	// get x point																										
	ECBPolyPoint y_intercept_coords = in_XYZinterceptCoords.triPoints[1];	// ...y point																									
	ECBPolyPoint z_intercept_coords = in_XYZinterceptCoords.triPoints[2];	// ...z point

	// We must determine if any distances are equal: It is a corner when:
	// A: X/Y/Z distances are equal
	// B: Two distances are equal,  and the remaining distance's respective coordinate is either on 1.0f or 0.0f.
	if
		(
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
			||																// remove this and folliwng 5 lines for debug
			(
			(dist_to_X == dist_to_Y) &&
				(dist_to_Z == 0.0f) &&
				(
				(in_originPoint.z == 1.0f)
					||
					(in_originPoint.z == 0.0f)
					)
				)
			||
			(
			(dist_to_Y == dist_to_Z) &&
				(dist_to_X == 0.0f) &&
				(
				(in_originPoint.x == 1.0f)
					||
					(in_originPoint.x == 0.0f)
					)
				)
			||
			(
			(dist_to_X == dist_to_Z) &&
				(dist_to_Y == 0.0f) &&
				(
				(in_originPoint.y == 1.0f)
					||
					(in_originPoint.y == 0.0f)
					)
				)

			)
	{
		std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}
	ECBPolyPoint pointToCheck;				// the actual intercept point that will be used when comparing to border lines, border corners, or border faces
											// Special condition: check to see if origin point is directly on one of the x/y/z planes
											//if (dist_to_Y == dist_to_Z)
											//{
											//std::cout << "Y and Z dist is equal!" << std::endl;
											//}

											// Step 1: check if the resulting point is a perfect corner intercept
											// Multiple conditions:
											//		Condition 1: X, Y, -AND- Z intercept distance are the same 
											//		Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
											//      Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
											//		Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
											//      Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)
											//      Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
											//      Condition 7: in_slope.z is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (z intercept coord's x and y are exactly on border)
											//std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
											//std::cout << "Dist values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << " |||| Slopes: " << in_slopeDirection.x << ", " << in_slopeDirection.y << ", " << in_slopeDirection.z << std::endl;
											//std::cout << "Hundred thousandth rounding vals are: " << new_x << ", " << new_y << ", " << new_z << std::endl;
											//std::cout << "---------Calculated point MetaData for this iteration-----------" << std::endl;
											//std::cout << "testyval: " << testyval << std::endl;
											//std::cout << "Original x-dist: " << in_distanceValues.x << std::endl;
											//std::cout << "Original y-dist: " << in_distanceValues.y << std::endl;
											//std::cout << "Original z-dist: " << in_distanceValues.z << std::endl;
											//std::cout << "New test x-dist: " << dist_to_X << std::endl;
											//std::cout << "New test y-dist: " << dist_to_Y << std::endl;
											//std::cout << "New test z-dist: " << dist_to_Z << std::endl;

											//std::cout << "X-intercept coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << " | Distance to x-intercept: " << dist_to_X << std::endl;
											//std::cout << "Y-intercept coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << " | Distance to y-intercept: " << dist_to_Y << std::endl;
											//std::cout << "Z-intercept coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << " | Distance to z-intercept: " << dist_to_Z << std::endl;

											// std::cout << "Distance values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	//ECBPolyPoint testXYZenhanced = OrganicUtils::roundXYZInterceptDistancesToAppropriatePrecision(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);

	if (

		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == 1.0f) || (in_originPoint.z == 0.0f))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == 1.0f) || (in_originPoint.y == 0.0f))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == 1.0f) || (in_originPoint.x == 0.0f))
			&&
			in_slopeDirection.x == 0.0f
			)

		||

		(														//  Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)		
		(dist_to_X != 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((x_intercept_coords.y == 1.0f) || (x_intercept_coords.y == 0.0f))
				&&
				((x_intercept_coords.z == 1.0f) || (x_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Y != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((y_intercept_coords.x == 1.0f) || (y_intercept_coords.x == 0.0f))
				&&
				((y_intercept_coords.z == 1.0f) || (y_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 7: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Z != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(
			((z_intercept_coords.x == 1.0f) || (z_intercept_coords.x == 0.0f))
				&&
				((z_intercept_coords.y == 1.0f) || (z_intercept_coords.y == 0.0f))
				)
			)


		)

	{
		std::cout << ":::: corner point hit! " << std::endl;
		if (in_slopeDirection.x != 0.0f)
		{

			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);		// x_intercept coords
			std::cout << "point to check is x: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);		// y_intercept_coords
			std::cout << "point to check is y: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);		// z_intercept coords
			std::cout << "point to check is z: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y	&&		pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)		// Lower NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNW.borderValues);
			//std::cout << "Point is at lower NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// Lower SW
		{
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSW.borderValues);
			//std::cout << "Point is at lower SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSW.pointXYZ.x   &&	pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y   &&  pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// Upper SW	 
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSW.borderValues);
			//std::cout << "Point is at upper SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)			// Upper NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNW.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x	&&  pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)			// Lower NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// Lower SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// Upper SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNE.pointXYZ.z)			// Upper NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
			//std::cout << "Point is at upper NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
	}

	// Step 2: check if the resulting point is a border line intercept
	else if ((dist_to_X == dist_to_Y) || (dist_to_Y == dist_to_Z) || (dist_to_X == dist_to_Z))
	{
		//std::cout << ">>>>> PHASE 2 entry: " << std::endl;
		// ||||||||||||||||||||||||||||||||||||||||| condition 1 (Z-lines): check for Z line intercepts, when X and Y are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		if ((dist_to_X == dist_to_Y) && (dist_to_X != 0.0f))							// condition 1: X equals Y, but neither X or Y are 0 (0 would indicate their distances are not used)	
		{
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			if (
				(dist_to_Z > dist_to_X) && (in_slopeDirection.z != 0.0f)		//     condition 1: if Z's distance is greater  than X (could also be y here), AND it's slope is not 0, we go with x/y distance point
				||																//		-- OR --
				(in_slopeDirection.z == 0.0f)									//     condition 2: Z's slope is 0, check x and y
				)	// we can compare to either x or y, since they are both the same value; if Z is greater, it will not hit a Z-face before X and Y does
			{
				//std::cout << "X equals Y, Non-face entry" << std::endl;

				// use x_intercept coords
				// old: pointToCheck = x_intercept_coords (first parameter value of 0, for x)
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// make the pointToCheck equal to either x or y intercept coords
				//std::cout << "entry check" << std::endl;
				//perform comparisons to Zaxis borders -- lowerWest, lowerEast, upperWest, upperEast
				// Lower West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerWest);
					//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
				}

				// Upper West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperWest);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper west line ...shift key is: " << std::endl;
				}

				// Upper East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
					//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper east line ...shift key is: " << std::endl;
				}
				// Lower East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at lower east line ...shift key is: " << std::endl;
				}

			}
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y

			{
				//std::cout << "X equals Y, Face entry" << std::endl;
				pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				if (in_slopeDirection.z == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->SouthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->NorthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
				}

			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): check for Y line intercepts, , when X and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))							// condition 1: X equals Z, but neither X or Z are 0 (0 would indicate their distance is not used)	
		{

			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
																													// check if y distance is greater than x and z
			if (
				(dist_to_Y > dist_to_X) && (in_slopeDirection.y != 0.0f)			// condition 1:	if Y's distance is greater  than X (could also be z here), AND it's slope is not 0, we go with x/z distance point
																					//||																	// --OR-- 
																					//((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))	// condition 2: X distance is not 0, but Y and Z are both 0.0f	(a straight line along the x-axis, with no y/z slope, so NORTH or SOUTH face)
																					//||																			// -- OR --
																					//((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))	// condition 3:	Z distance is not 0, but X and Y are both 0.0f	(a straight line along the z-axis, with no x/y slope, so WEST or EAST face)
				||
				(in_slopeDirection.y == 0.0f)										// condition 2: Y's slope is 0, check x and y	
				)
			{
				// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				std::cout << "X equals Z, Non-face entry" << std::endl;
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northEast);
					//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

				// South East line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southEast);
					//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// South West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southWest);
					//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				// North West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northWest);
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

				}


			}
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				pointToCheck = y_intercept_coords;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
				if (in_slopeDirection.y == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->TopFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->BottomFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
					//std::cout << "X/Z distances match, but Y is hit first (negative y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 3 (X-lines): Y and Z distances match, when Y and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y == dist_to_Z) && (dist_to_Y != 0.0f))
		{
			pointToCheck = z_intercept_coords;
			std::cout << " >>>> Point to check values, PRE-CHANGE:  " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			if (
				(dist_to_X > dist_to_Y) && (in_slopeDirection.x != 0.0f)	// condition 1:	if X's distance is greater  than Y (could also be Z here), AND it's slope is not 0, we go with y/z distance point
				||															// --OR-- 
				(in_slopeDirection.x == 0.0f)								// condition 2: X's slope is 0, check y and z
				)
			{
				std::cout << "5-face entry" << std::endl;
				pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				//std::cout << " >>>> Y equals Z (branch 1) " << std::endl;
				std::cout << " >>>> Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.x < in_blockBorderRef->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 1) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerNorth);
				}
				// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.x < in_blockBorderRef->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key

					//std::cout << " >>>> Y equals Z (branch 1, 2) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerSouth);
				}
				// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperSW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 3) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperSouth);
				}
				// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperNW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 4) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperNorth);
				}
			}
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << " >>>> Y equals Z (branch 2) " << std::endl;
				pointToCheck = x_intercept_coords;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				//std::cout << " |||| (branch 2, 2) Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if (in_slopeDirection.x == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->EastFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), East face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->WestFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), West face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
				}
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for X, but Y and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
																													// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
			else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}


		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
			else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			if ((pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
			else if ((pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}

		}





		else
		{
			//std::cout << "NO MATCH TO ANYTHING " << std::endl;
		}
	}

	// Step 3: else, it's just a normal intercept
	else {
		std::cout << "::::NORMAL INTERCEPT DETECTED::::" << std::endl;
		float currentShortestTime = 0.0f;
		int currentShortestIndex = -1;		// -1 is default (no valid index selected)
		float distanceValues[3];			// set up a temp array
		distanceValues[0] = in_distanceValues.x;	// store x distance value
		distanceValues[1] = in_distanceValues.y;	// "" y
		distanceValues[2] = in_distanceValues.z;	// "" z

		for (int x = 0; x < 3; x++)
		{
			if (distanceValues[x] != 0.0f)	// is there an actual valid time for this?
			{
				if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
				{
					currentShortestTime = distanceValues[x];	// set the initial length
					currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
				}
				else if (currentShortestTime != 0.0f)
				{
					if (distanceValues[x] < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
					{
						currentShortestTime = distanceValues[x];	// reset the length
						currentShortestIndex = x;							// reset the index
					}
				}
			}
		}
		if (currentShortestIndex == 0)	// x was found
		{
			std::cout << "STANDARD INTERCEPT: use X-int" << std::endl;
			std::cout << "Is point on a line> " << isPointOnALine << std::endl;
			//pointToCheck = x_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = x_intercept_coords;
			if (in_slopeDirection.x == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				//::cout << "Type 3: line intersects at +X" << std::endl;
				//calculatedEndpointData.moveDirectionValues = in_blockBorderRef->EastFace.
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			std::cout << "STANDARD INTERCEPT: use Y-int" << std::endl;
			std::cout << "Is point on a line> " << isPointOnALine << std::endl;
			//pointToCheck = y_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = y_intercept_coords;
			if (in_slopeDirection.y == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
		}
		else if (currentShortestIndex == 2)		// z was found
		{
			std::cout << "STANDARD INTERCEPT: use Z-int" << std::endl;
			std::cout << "Is point on a line> " << isPointOnALine << std::endl;
			//pointToCheck = z_intercept_coords;
			pointToCheck = IndependentUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			pointToCheck = z_intercept_coords;
			if (in_slopeDirection.z == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
		}

	}
	//std::cout << "whoa" << std::endl;

	calculatedEndpointData.pointLocation = pointToCheck;
	calculatedEndpointData.matchSlopesToDirections(in_slopeDirection);
	//std::cout << "getCalculatedEndPointMeta endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
	//std::cout << ">>>>>>> end point metadata values" << std::endl;
	//std::cout << "Distance to intercepts: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	std::cout << "OrganicUtils EndPoint location: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
	//std::cout << "::>>>> Dimension reset values: " << calculatedEndpointData.dimensionResetValues.x << ", " << calculatedEndpointData.dimensionResetValues.y << ", " << calculatedEndpointData.dimensionResetValues.z << ", " << std::endl;
	//std::cout << "((new function)) Move values::: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << ", " << std::endl;

	return calculatedEndpointData;
}

ECBPolyPoint IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ)
{
	ECBPolyPoint returnPoint;
	struct DistTracker
	{
		float distance;	// stores the passed in distance to x/y/z
		int dimension;	// represents dimension type, 0 = x, 1 = y, 2 = z
	};
	DistTracker tracker[3];			// create an array
	float edgeThreshold = 0.0001f;	// set the threshold value; used to prevent F-007

	// initialize array
	tracker[0].distance = in_distX;
	tracker[1].distance = in_distY;
	tracker[2].distance = in_distZ;

	// cycle through and determine the candidate count, while also setting dimension values
	int candidateCount = 3;
	for (int x = 0; x < 3; x++)
	{
		tracker[x].dimension = x;
		if (tracker[x].distance == 0.0f)
		{
			candidateCount--;	// subtract as a candidate if it is 0 
		}
	}

	// do stuff based on number of candidates
	if (candidateCount == 3)		// it will be 3, rarely
	{
		//std::cout << ">>>>> Candidate count  is 3" << std::endl;
		DistTracker tripleTracker[3];
		float leastValue = 0.0f;
		for (int x = 0; x < 3; x++)			// initialize all rows
		{
			tripleTracker[x] = tracker[x];
		}

		// order from least to greatest
		//leastValue = tripleTracker[0].distance;	// initiate the first least value
		//int leastIndex = 0;
		for (int a = 0; a < 2; a++)
		{
			for (int b = a; b < 2; b++)
			{
				if (tripleTracker[a].distance >= tripleTracker[b + 1].distance)		// is the current "least" greater than the selected value? if so, then we must swap
				{
					DistTracker tempTracker = tripleTracker[a];
					tripleTracker[a] = tripleTracker[b + 1];
					tripleTracker[b + 1] = tempTracker;
				}
			}
		}

		// check relationship between 0 and 1
		for (int j = 0; j < 2; j++)
		{
			for (int z = 0; z < 2; z++)
			{
				float numeratorFloat = tripleTracker[z].distance;
				float denominatorFloat = tripleTracker[z + 1].distance;

				//std::cout << "3 candidates, :: numerator float   -> " << numeratorFloat << std::endl;
				//std::cout << "3 candidates, :: denominator float -> " << denominatorFloat << std::endl;



				float ratioResult = numeratorFloat / denominatorFloat;
				if (ratioResult > 1.0f)
				{
					float resultantThreshold = ratioResult - 1.0f;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
						//std::cout << "3 candidates: ratio > 1.0f" << std::endl;
					}

				}
				else if (ratioResult < 1.0f)
				{
					float resultantThreshold = 1.0f - ratioResult;

					//std::cout << "branch 2, resultantThreshold: " << resultantThreshold << std::endl;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
						//std::cout << "3 candidates: ratio < 1.0f" << std::endl;
					}
				}

			}
		}

		// populate values from tripleTracker into tracker
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (tripleTracker[x].dimension == tracker[y].dimension)
				{
					tracker[y].distance = tripleTracker[x].distance;
					//std::cout << "ratio < 1.0f" << std::endl;
				}
			}
		}



	}

	else if (candidateCount == 2)	// it will be 2, most of the time
	{
		//std::cout << ">>>>> Candidate count  is 2" << std::endl;
		DistTracker duoTracker[2];	// will store the two candidates
		int trackerIndex = 0;		// index, incremented by 1 after first is found
		for (int x = 0; x < 3; x++)
		{
			if (tracker[x].distance != 0.0f)
			{
				duoTracker[trackerIndex] = tracker[x];
				trackerIndex++;
			}
		}

		// now, check the relationship between both
		float numeratorFloat = duoTracker[0].distance;
		float denominatorFloat = duoTracker[1].distance;

		//std::cout << ":: numerator float   -> " << numeratorFloat << std::endl;
		//std::cout << ":: denominator float -> " << denominatorFloat << std::endl;

		// calculate the ratio
		float ratioResult = numeratorFloat / denominatorFloat;
		//std::cout << ":: Ratio result is: ---> " << ratioResult;
		if (ratioResult > 1.0f)
		{
			//std::cout << "Ratio result entered > 1 branch... " << std::endl;
			float resultantThreshold = ratioResult - 1.0f;
			//std::cout << "Resultant threshold is: " << resultantThreshold << std::endl;

			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
				//std::cout << "ratio > 1.0f" << std::endl;
			}

		}
		else if (ratioResult < 1.0f)
		{
			float resultantThreshold = 1.0f - ratioResult;
			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
				//std::cout << "ratio < 1.0f" << std::endl;
			}
		}


		// populate values from duoTracker into tracker
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (duoTracker[x].dimension == tracker[y].dimension)	// do this dimensions match?
				{
					tracker[y].distance = duoTracker[x].distance;		// ok, so replace them
				}
			}
		}



	}

	returnPoint.x = tracker[0].distance;
	returnPoint.y = tracker[1].distance;
	returnPoint.z = tracker[2].distance;



	//std::cout << "Return distance values: " << std::endl;
	//std::cout << returnPoint.x << std::endl;
	//std::cout << returnPoint.y << std::endl;
	//std::cout << returnPoint.z << std::endl;

	//std::cout << "Distance values, post change: " << returnPoint.x << ", " << returnPoint.y << ",  " << returnPoint.z << std::endl;


	// DEBUG ONLY, remove when done
	/*
	if
		(
		(in_distX >= .00308642f)
			&&
			(in_distX <= .00308646f)
			)
	{
		std::cout << "Halting via in_distX; " << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}
	*/

	//std::cout << "Returning precisions..." << std::endl;
	return returnPoint;
}

ECBPolyPoint IndependentUtils::roundXYZInterceptDistancesToAppropriatePrecisionDebug(float in_distX, float in_distY, float in_distZ)
{
	ECBPolyPoint returnPoint;
	struct DistTracker
	{
		float distance;	// stores the passed in distance to x/y/z
		int dimension;	// represents dimension type, 0 = x, 1 = y, 2 = z
	};
	DistTracker tracker[3];			// create an array
	float edgeThreshold = 0.0001f;	// set the threshold value

									// initialize array
	tracker[0].distance = in_distX;
	tracker[1].distance = in_distY;
	tracker[2].distance = in_distZ;

	//std::cout << "~~~ Unsorted distances:" << std::endl;
	//std::cout << "x: " << tracker[0].distance << std::endl;
	//std::cout << "y: " << tracker[1].distance << std::endl;
	//std::cout << "z: " << tracker[2].distance << std::endl;

	// cycle through and determine the candidate count, while also setting dimension values
	int candidateCount = 3;
	for (int x = 0; x < 3; x++)
	{
		tracker[x].dimension = x;
		if (tracker[x].distance == 0.0f)
		{
			candidateCount--;	// subtract as a candidate if it is 0 
		}
	}

	// do stuff based on number of candidates
	if (candidateCount == 3)		// it will be 3, rarely
	{
		//std::cout << ">>>>> Candidate count  is 3" << std::endl;
		DistTracker tripleTracker[3];
		float leastValue = 0.0f;
		for (int x = 0; x < 3; x++)			// initialize all rows
		{
			tripleTracker[x] = tracker[x];
		}

		// order from least to greatest
		//leastValue = tripleTracker[0].distance;	// initiate the first least value
		//int leastIndex = 0;
		for (int a = 0; a < 2; a++)
		{
			for (int b = a; b < 2; b++)
			{
				if (tripleTracker[a].distance >= tripleTracker[b + 1].distance)		// is the current "least" greater than the selected value? if so, then we must swap
				{
					DistTracker tempTracker = tripleTracker[a];
					tripleTracker[a] = tripleTracker[b + 1];
					tripleTracker[b + 1] = tempTracker;
				}
			}
		}

		//std::cout << "Second calculation values: (pre 1st pass) " << std::endl;
		//std::cout << "0: " << tripleTracker[0].distance << std::endl;
		//std::cout << "1: " << tripleTracker[1].distance << std::endl;
		//std::cout << "2: " << tripleTracker[2].distance << std::endl;
		//std::cout << std::setprecision(7) << std::endl;

		// check relationship between 0 and 1
		for (int f = 0; f < 2; f++)
		{
			for (int z = 0; z < 2; z++)
			{
				float numeratorFloat = tripleTracker[z].distance;
				float denominatorFloat = tripleTracker[z + 1].distance;
				float ratioResult = numeratorFloat / denominatorFloat;
				if (ratioResult > 1.0f)
				{
					float resultantThreshold = ratioResult - 1.0f;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
																					//std::cout << "3 candidates: ratio > 1.0f" << std::endl;
					}

				}
				else if (ratioResult < 1.0f)
				{
					float resultantThreshold = 1.0f - ratioResult;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
																					//std::cout << "3 candidates: ratio < 1.0f" << std::endl;
					}
				}

			}
		}

		// populate values from tripleTracker into tracker
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (tripleTracker[x].dimension == tracker[y].dimension)
				{
					tracker[y].distance = tripleTracker[x].distance;
					//std::cout << "ratio < 1.0f" << std::endl;
				}
			}
		}
	}

	else if (candidateCount == 2)	// it will be 2, most of the time
	{
		//std::cout << ">>>>> Candidate count  is 2" << std::endl;
		DistTracker duoTracker[2];	// will store the two candidates
		int trackerIndex = 0;		// index, incremented by 1 after first is found
		for (int x = 0; x < 3; x++)
		{
			if (tracker[x].distance != 0.0f)
			{
				duoTracker[trackerIndex] = tracker[x];
				trackerIndex++;
			}
		}

		// now, check the relationship between both
		float numeratorFloat = duoTracker[0].distance;
		float denominatorFloat = duoTracker[1].distance;

		// calculate the ratio
		float ratioResult = numeratorFloat / denominatorFloat;
		//std::cout << "Ratio result is: " << ratioResult;
		if (ratioResult > 1.0f)
		{
			float resultantThreshold = ratioResult - 1.0f;
			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
																	//std::cout << "ratio > 1.0f" << std::endl;
			}

		}
		else if (ratioResult < 1.0f)
		{
			float resultantThreshold = 1.0f - ratioResult;
			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
																	//std::cout << "ratio < 1.0f" << std::endl;
			}
		}

		// populate values from duoTracker into tracker
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (duoTracker[x].dimension == tracker[y].dimension)	// do this dimensions match?
				{
					tracker[y].distance = duoTracker[x].distance;		// ok, so replace them
				}
			}
		}



	}

	returnPoint.x = tracker[0].distance;
	returnPoint.y = tracker[1].distance;
	returnPoint.z = tracker[2].distance;

	return returnPoint;
}

ECBPolyPoint IndependentUtils::roundToNearestBlockLineOrCorner(int in_xoryorz, ECBPolyPoint in_polyPoint, int in_lineOrCorner)
{
	ECBPolyPoint calibratedPoint = in_polyPoint;
	// correct for any over shot between 0.0f and 1.0f (i.e., if x y or z value is -0.00005f , make it 0.0f
	// corrections for X
	//std::cout << "original point: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << std::endl;
	if (calibratedPoint.x < 0.0f)
	{
		calibratedPoint.x = 0.0f;
	}
	else if (calibratedPoint.x > 1.0f)
	{
		calibratedPoint.x = 1.0f;
	}

	// corrections for Y
	if (calibratedPoint.y < 0.0f)
	{
		calibratedPoint.y = 0.0f;
	}
	else if (calibratedPoint.y > 1.0f)
	{
		calibratedPoint.y = 1.0f;
	}

	// corrections for Z
	if (calibratedPoint.z < 0.0f)
	{
		calibratedPoint.z = 0.0f;
	}
	else if (calibratedPoint.z > 1.0f)
	{
		calibratedPoint.z = 1.0f;
	}



	if (in_lineOrCorner == 0)		// type 0 = all distances are equal; it's a corner point
	{
		// Round other points to X
		//std::cout << "||||>>>>>>>>>> point is on a corner <<<<< " << std::endl;
		if (in_xoryorz == 0)
		{
			//if (in_polyPoint.x == 1.0f)		// is x == 1.0?
			//{
				// check for y 
			if (in_polyPoint.y > 0.5f)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = 1.0f;
			}
			else if (in_polyPoint.y < 0.5f)
			{
				calibratedPoint.y = 0.0f;
			}

			// check for z
			if (in_polyPoint.z > 0.5f) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.z = 1.0f;
			}
			else if (in_polyPoint.z < 0.5f)
			{
				calibratedPoint.z = 0.0f;
			}
			//}
		}

		// Round other points to Y
		else if (in_xoryorz == 1)	// otherwise, point is on a line
		{
			// check for x
			if (in_polyPoint.x > 0.5f)
			{
				calibratedPoint.x = 1.0f;
			}
			else if (in_polyPoint.x < 0.5f)
			{
				calibratedPoint.x = 0.0f;
			}

			// check for z
			if (in_polyPoint.z > 0.5f)
			{
				calibratedPoint.z = 1.0f;
			}
			else if (in_polyPoint.z < 0.5f)
			{
				calibratedPoint.z = 0.0f;
			}
		}

		// Round other points to Z
		else if (in_xoryorz == 2)
		{
			// check for x
			if (in_polyPoint.x > 0.5f)
			{
				calibratedPoint.x = 1.0f;
			}
			else if (in_polyPoint.x < 0.5f)
			{
				calibratedPoint.x = 0.0f;
			}

			// check for y
			if (in_polyPoint.y > 0.5f)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = 1.0f;
			}
			else if (in_polyPoint.y < 0.5f)
			{
				calibratedPoint.y = 0.0f;
			}


		}
	}
	else if (in_lineOrCorner == 1)	// point exists on a single plane, so two distances are equal and one is zero; point is on a line and not a corner
	{
		// Solve for Y and Z
		//std::cout << "++Line " << std::endl;
		//std::cout << "original point: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << std::endl;
		int numberOfCalibrations = 0;	// any valid calibration will give this value something greater than 0
		if (in_xoryorz == 0)
		{
			/*
			if (in_polyPoint.y > 0.9999f)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.y < 0.0001f)
			{
				calibratedPoint.y = 0.0f;
				numberOfCalibrations++;
			}

			// check for z
			if (in_polyPoint.z > 0.9999f) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.z = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.z < 0.0001f)
			{
				calibratedPoint.z = 0.0f;
				numberOfCalibrations++;
			}
			*/
			LinePointSynchronizer pointSync(calibratedPoint, 0);
			calibratedPoint = pointSync.sync();
		}

		// Solve for X and Z
		else if (in_xoryorz == 1)
		{
			// check for x
			/*
			if (in_polyPoint.x > 0.9999f)
			{
				calibratedPoint.x = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.x < 0.0001f)
			{
				calibratedPoint.x = 0.0f;
				numberOfCalibrations++;
			}

			// check for z
			if (in_polyPoint.z > 0.9999f)
			{
				calibratedPoint.z = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.z < 0.0001f)
			{
				calibratedPoint.z = 0.0f;
				numberOfCalibrations++;
			}
			*/
			LinePointSynchronizer pointSync(calibratedPoint, 1);
			calibratedPoint = pointSync.sync();
		}

		// check for x
		else if (in_xoryorz == 2)
		{
			/*
			if (in_polyPoint.x > 0.9999f)
			{
				calibratedPoint.x = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.x < 0.0001f)
			{
				calibratedPoint.x = 0.0f;
				numberOfCalibrations++;
			}

			// check for y
			if (in_polyPoint.y > 0.9999f)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				calibratedPoint.y = 1.0f;
				numberOfCalibrations++;
			}
			else if (in_polyPoint.y < 0.0001f)
			{
				calibratedPoint.y = 0.0f;
				numberOfCalibrations++;
			}
			*/
			LinePointSynchronizer pointSync(calibratedPoint, 2);
			calibratedPoint = pointSync.sync();
		}

		if (numberOfCalibrations == 0)
		{
			//std::cout << "!!!! WARNING: improper calibration detected. " << std::endl;
			//std::cout << "!!!! point is: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << ", " << std::endl;
			//std::cout << "!!!! calibrated point is: " << calibratedPoint.x << ", " << calibratedPoint.y << ", " << calibratedPoint.z << std::endl;
			//std::cout << "!!!! in_xoryorz: " << in_xoryorz << std::endl;
			//int stopVal;
			//std::cin >> stopVal;
		}
	}

	return calibratedPoint;
}

PLTracingResult IndependentUtils::getBlockTracingResult(ECBPolyPoint in_beginPoint, ECBPolyPoint in_interceptToUse, BlockBorderLineList* in_borderLineListRef, BorderDataMap* in_borderDataMapRef, int in_debugFlag)
{
	PLTracingResult resultToReturn;
	ECBPolyPoint resultEndpoint = IndependentUtils::getBlockTracingEndpoint(in_beginPoint, in_interceptToUse, in_borderLineListRef);
	if (in_debugFlag == 1)
	{
		std::cout << ":::::::::::: debug tracing call begins here ::::::::::::::" << std::endl;
		std::cout << "in_beginPoint: " << in_beginPoint.x << ", " << in_beginPoint.y << ", " << in_beginPoint.z << ", " << std::endl;
		std::cout << "in_slope: " << in_interceptToUse.x << ", " << in_interceptToUse.y << ", " << in_interceptToUse.z << ", " << std::endl;
		resultEndpoint = IndependentUtils::getBlockTracingEndpointDebug(in_beginPoint, in_interceptToUse, in_borderLineListRef);	// find the appropriate point that is found when the slope of in_interceptToUse is applied
		std::cout << "resultant end point after getBlockTracingResult call: " << resultEndpoint.x << ", " << resultEndpoint.y << ", " << resultEndpoint.z << ", " << std::endl;
	}
	ECBPPOrientationResults orientationResults = IndependentUtils::GetPointOrientation(resultEndpoint, in_borderLineListRef);		// get the orientation from resultEndpoint
	BorderMDFaceList resultFaceList = IndependentUtils::getFaceList(orientationResults, in_borderDataMapRef);
	resultToReturn.resultingEndPoint = resultEndpoint;
	resultToReturn.resultingFaceList = resultFaceList;
	resultToReturn.resultingOrientation = orientationResults;
	return resultToReturn;
}

PolyLineEndpointMeta IndependentUtils::getBlockTracingEndpointMeta(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef)
{
	ECBPolyPoint pointToReturn;
	//std::cout << "OrganicUtils::getBlockTracingEndpoint begin point is: " << in_beginPoint.x << ", " << in_beginPoint.y << ", " << in_beginPoint.z << std::endl;
	ECBPolyPoint slopeDirection = IndependentUtils::findNormalizedPoint(in_slope); // get normalized slope
	ECBPolyPoint lineSlope = in_slope;
	ECBPolyPoint currentIterationBeginPoint = in_beginPoint;

	// flags for determining what the direction of x/y/z is; if they remain 0, there is no slope (perfectly flat for the line on that axis)
	int x_dir = int(slopeDirection.x);
	int y_dir = int(slopeDirection.y);
	int z_dir = int(slopeDirection.z);

	float x_interceptCoord = 0.0f;
	float y_interceptCoord = 0.0f;
	float z_interceptCoord = 0.0f;

	// calculate total line length, which is the square root of the sum of x/y/z squared (pythagorean theorem)
	float powSlopeX = pow(lineSlope.x, 2.0f);
	float powSlopeY = pow(lineSlope.y, 2.0f);
	float powSlopeZ = pow(lineSlope.z, 2.0f);
	float fullLineLength = sqrt(powSlopeX + powSlopeY + powSlopeZ);

	// check slope direction, and calculate distance for x ********************************************************************************************************************************************************************
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, OrganicUtils::findBlueprintBorderMoveMeta
	ECBPolyPoint calculatedPoint_for_y;
	ECBPolyPoint calculatedPoint_for_z;
	//std::cout << ">>>>>>>>> Current iter begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>>> Current iteration point values are: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << ">>>>> Slope directions: " << slopeDirection.x << ", " << slopeDirection.y << ", " << slopeDirection.z << std::endl;
	//std::cout << "Line slopes: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
	// calculate distance to intercept for x-plane
	if (slopeDirection.x > 0)
	{
		x_dir = 1;			// going towards positive x 
		x_interceptCoord = 1.0f;								// x_interceptCoord is equal to 1.0f (EAST block border)
		float origin_to_border_x_diff = (x_interceptCoord - currentIterationBeginPoint.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
																											//std::cout << "line slope vals: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;
		time_to_complete_x_traversal = origin_to_border_x_diff / lineSlope.x;	// get the distance that the ray has to travel to get to this value of x
																				//ECBPolyPoint calculatedPoint_for_x;					// the actual point as it would exist on the EAST face
																				//std::cout << "current iter begin point x: " << currentIterationBeginPoint.x << std::endl;
																				//std::cout << "current iter begin point y: " << currentIterationBeginPoint.y << std::endl;
																				//std::cout << "current iter begin point z: " << currentIterationBeginPoint.z << std::endl;
																				//std::cout << "time for x traversal: " << time_to_complete_x_traversal << std::endl;
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "Calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
		//std::cout << "(pos) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}
	else if (slopeDirection.x < 0)
	{
		x_dir = -1;			// going towards negative x
		x_interceptCoord = 0.0f;								// x_interceptCoord is equal to 0.0f (WEST block border)
		float origin_to_border_x_diff = abs(x_interceptCoord - currentIterationBeginPoint.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / lineSlope.x);	// ""
																					//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the WEST face
		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_x_traversal);	// "" 
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_x_traversal);	// ""

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_x.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_x.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_x.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(neg) calc'd point for x is: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;
	}

	// ... for y plane
	if (slopeDirection.y > 0)
	{
		y_dir = 1;		// going towards positive y
		y_interceptCoord = 1.0f;								// y_interceptCoord is the precise location of y at the TOP face border
		float origin_to_border_y_diff = y_interceptCoord - currentIterationBeginPoint.y;						// this value represents what we need to multiply x and z by in order to get the distance to the border (pythagorean theorem again)
		time_to_complete_y_traversal = origin_to_border_y_diff / lineSlope.y;	// get the distance this ray has to travel to get to this value of y
																				//ECBPolyPoint calculatedPoint;					// the actual point as it would exist on the TOP face
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);


	}
	else if (slopeDirection.y < 0)
	{
		y_dir = -1;
		y_interceptCoord = 0.0f;
		float origin_to_border_y_diff = abs(y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / lineSlope.y);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (lineSlope.z * time_to_complete_y_traversal);

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_y.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_y.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_y.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);

	}

	// ... for z plane
	if (slopeDirection.z > 0)
	{
		z_dir = 1;
		z_interceptCoord = 1.0f;
		float origin_to_border_z_diff = z_interceptCoord - currentIterationBeginPoint.z;
		time_to_complete_z_traversal = origin_to_border_z_diff / lineSlope.z;
		//std::cout << "Z intercept coord: " << z_interceptCoord << std::endl;
		//std::cout << "Current iteration begin point: " << currentIterationBeginPoint.z << std::endl;
		//std::cout << "Line slope z: " << lineSlope.z << std::endl;
		//ECBPolyPoint calculatedPoint;

		//std::cout << "beginPoint.x: " << currentIterationBeginPoint.x << std::endl;
		//std::cout << "line slope.x: " << lineSlope.x << std::endl;
		//std::cout << "time to complete z traversal: " << time_to_complete_z_traversal << std::endl;

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(POS Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;

	}
	else if (slopeDirection.z < 0)
	{
		z_dir = -1;
		z_interceptCoord = 0.0f;
		float origin_to_border_z_diff = abs(z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / lineSlope.z);
		//ECBPolyPoint calculatedPoint;
		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (lineSlope.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (lineSlope.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		ECBPolyPoint distanceToCalculatedPoint;
		distanceToCalculatedPoint.x = calculatedPoint_for_z.x - currentIterationBeginPoint.x;
		distanceToCalculatedPoint.y = calculatedPoint_for_z.y - currentIterationBeginPoint.y;
		distanceToCalculatedPoint.z = calculatedPoint_for_z.z - currentIterationBeginPoint.z;
		float squared_distance_to_x = pow(distanceToCalculatedPoint.x, 2.0f);
		float squared_distance_to_y = pow(distanceToCalculatedPoint.y, 2.0f);
		float squared_distance_to_z = pow(distanceToCalculatedPoint.z, 2.0f);
		//std::cout << "(NEG Z) Calc'd point for z is: " << calculatedPoint_for_z.x << ", " << calculatedPoint_for_z.y << ", " << calculatedPoint_for_z.z << std::endl;
	}



	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;
	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;

	// std::cout << "Calc'd point x: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;

	PolyLineEndpointMeta returnEndpointData = IndependentUtils::getCalculatedEndpointMetadata(currentIterationBeginPoint, in_blockBorderRef, distanceValues, slopeDirection, triPointParam);
	return returnEndpointData;
}

float IndependentUtils::convertPreciseCoordToFloat(unsigned char in_unsignedCharToCheck)
{
	float floatToReturn = 0.0f;
	if (in_unsignedCharToCheck != 0)
	{
		float convertedUnsignedChar = float(in_unsignedCharToCheck);
		float dividedBy100 = convertedUnsignedChar / 100.0f;
		//std::cout << "Converted from: " << int(in_unsignedCharToCheck) << " to " << 
		floatToReturn = dividedBy100;
	}
	return floatToReturn;
}

int IndependentUtils::isUnsignedCharBitSet(unsigned char in_unsignedChar, int in_bitToCheck)
{
	int returnVal = 0;
	int exponent = in_bitToCheck - 1;
	unsigned char charToCheck = in_unsignedChar;
	//std::cout << "Value of char is: " << int(in_unsignedCharPtr) << std::endl;
	unsigned char checkValue = 1;
	int resultValue = (charToCheck >> exponent) & checkValue;
	if (resultValue == 1)
	{
		//std::cout << "Value match; bit to check was: " << in_bitToCheck << std::endl;
		return 1;
	}
	return returnVal;
}

void IndependentUtils::setUnsignedCharBit(unsigned char* in_unsignedCharPtr, int in_bitToSet, int in_bitValue)
{
	//std::cout << ">>>>>>before change: " << unsigned int(*in_unsignedCharPtr) << std::endl;
	int exponent = in_bitToSet - 1;

	int bitValue = int(pow(2, exponent));
	if (in_bitValue == 1)
	{
		//unsigned char otherBitValue = 1;		// 1 is default
		//otherBitValue |= (otherBitValue << exponent);
		int bitToSet = 1;
		*in_unsignedCharPtr |= (bitToSet << exponent);
		//std::cout << "new value of pointed to char is:::" << unsigned int(*in_unsignedCharPtr) << std::endl;

	}
	if (in_bitValue == 0)
	{
		int bitToSet = 0;
		*in_unsignedCharPtr &= ~(bitToSet << exponent);
		//std::cout << "new value of pointed to char is:::" << unsigned int(*in_unsignedCharPtr) << std::endl;
	}
	//std::cout << "bitValue is: " << bitValue << std::endl;
	int testbit = 1;
	int finalbit = testbit << 0;
	//std::cout << "test bit is: " << finalbit << std::endl;
}

CursorPathTraceContainer IndependentUtils::getPreciseCoordinate(float x)
{
	CursorPathTraceContainer tempPathTrace;
	float x_divide = (x / 32);							// here, 32 is the length of an entire collection (8 chunks = 32 blocks width)
	// std::cout << "test of x divide is: " << x_divide << std::endl;
	int collection_x = 0;								// the coordinate of the collection that will be returned (x or y or z)
	int chunk_x = 0;									// the coordinate of the chunk that will be returned (x or y or z)
	int block_x = 0;									// the coordinate of the block that will be returned (x or y or z)
	float dist_to_pos = 0;								// distance from the camera's point to the nearest positive axis block border
	float dist_to_neg = 0;								// distance from the camera's point to the nearest negative axis block border
	float exact_xyz = 0.0f;								// start exact_xyz at 1.0f
	float remainder = 0.0f;								// the modulo left after dividing by 4 (chunk width) and 1 (block size)
	int on_border = 0;


	//std::cout << "value of x is: " << x << std::endl;

	if (x < 0)											// operations to be performed when the input value is less than 0.
	{
		collection_x = 0;
		if ((x_divide < 0) && x >= -32)
		{
			//cout << "x_divide entry. " << endl;
			if (fmod(x, 32) != 0)
			{
				collection_x = -1;  // collection_x = (x_divide - 1);
			}
			else
			{
				collection_x = int(x_divide);
				on_border = 1;
			}


		}
		else if ((x_divide < 0) && x < -32)
		{
			if (fmod(x, 32) != 0)
			{
				collection_x = int(x_divide) - 1;
			}
			else
			{
				collection_x = int(x_divide);
				on_border = 1;
			}
		}

		// STEP 2: get remainder
		float negModResult = float(fmod(x, 32));
		//std::cout << "negModResult is: " << negModResult << std::endl;
		float baseWidth = 32.0f;
		float truePosition = baseWidth + negModResult;
		chunk_x = int((fmod(truePosition, 32) / 4));					// old:  chunk_x = ((x % 32) / 4);
																		//cout << "chunk_x: " << chunk_x << endl;
		block_x = int((fmod(fmod(truePosition, 32), 4)));				// old : block_x = ((x % 32) % 4);
																		//cout << "block_x: " << block_x << endl;
		float nearest_floor = floor(truePosition);
		exact_xyz = abs(nearest_floor - truePosition);
		//std::cout << "true position is: " << truePosition << std::endl;
		//std::cout << "chunk is: " << chunk_x << std::endl;
		//std::cout << "block is: " << block_x << std::endl;
		//std::cout << "exact_xyz position is: " << exact_xyz << std::endl;																	//cout << "exact_neg_x: " << dist_to_neg << endl;

	}
	else if (x > 0)										// operations t be performed when the input value is greater than/equal to 0
	{
		float nearest_floor = floor(x);
		exact_xyz = abs(nearest_floor - x);
		collection_x = 0;
		if ((x_divide > 0) && x > 32)
		{
			if (fmod(x, 32) != 0)
			{
				collection_x = int(x_divide);  // collection_x = (x_divide + 1);
			}
			else
			{
				collection_x = int(x_divide);
			}
		}
		//else if ((x_divide > 0) && x <= 32)
		//{
		//collection_x = 1;
		//}
		//cout << "collection_x: " << collection_x << endl;
		chunk_x = int((fmod(x, 32) / 4));					// old:  chunk_x = ((x % 32) / 4);
															//cout << "chunk_x: " << chunk_x << endl;
		block_x = int((fmod(fmod(x, 32), 4)));				// old : block_x = ((x % 32) % 4);
															//cout << "block_x: " << block_x << endl;

		dist_to_pos = float(ceil(fmod(fmod(x, 32), 4)) - (fmod(fmod(x, 32), 4)));
		dist_to_neg = float((fmod(fmod(x, 32), 4)) - block_x);
		//exact_xyz = 0.0f + float(fmod(fmod(fmod(x, 32), 4), 1));									// set the exact point in the block to be 1.0f - the distance to get to 1.0f. // OLD: exact_xyz = 1.0f - dist_to_pos;
																									//cout << "ceil of fmod x2: " << ceil(fmod(fmod(x, 32), 4)) << endl;
																									//cout << "without ceil: " << fmod(fmod(x, 32), 4) << endl;
																									//cout << "fmod test 1: " << fmod(x, 32) << endl;
																									//cout << "fmod test 2: " << fmod(fmod(x, 32), 4) << endl;
																									//cout << " exact_xyz (pos): " << exact_xyz << endl;

																									//cout << "NoOfCollections passed: " << NoOfCollections << endl;
	}
	tempPathTrace.CollectionCoord = collection_x;		// set the return value for the Collection coordinate
	tempPathTrace.EnclaveCoord = chunk_x;					// set the return value for the Chunk coordinate
	tempPathTrace.BlockCoord = block_x;					// set the return value for the block coordinate
	tempPathTrace.distance_to_pos = dist_to_pos;
	tempPathTrace.distance_to_neg = dist_to_neg;
	//std::cout << "Final value of exact_xyz: " << exact_xyz << std::endl;
	//std::cout << " | >>> chunk coord: " << tempPathTrace.EnclaveCoord << std::endl;
	//std::cout << " | >>> block coord: " << tempPathTrace.BlockCoord << std::endl;
	tempPathTrace.ExactBlockCoord = exact_xyz;				// the exact block coord


	return tempPathTrace;
}

int IndependentUtils::calibrateEnclaveBlockKeys(float in_remainder, float in_slope)
{
	if (in_remainder == 0.0f)
	{
		if (in_slope == -1.0f)
		{
			//std::cout << "Shift detected! " << std::endl;
			return -1;		// return -1
		}
	}
	return 0;	// if there is no match, return 0
}

ECBPolyPoint IndependentUtils::determineIntendedFaces(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB, ECBPolyPoint in_polyPointC)
{
	ECBPolyPoint returnPoint;
	//std::cout << "!!!~~~~~~~~~~~~~~~~~~~~~~Intended faces: calculatling slope: " << std::endl;
	//std::cout << ">> First point: " << in_polyPointA.x << ", " << in_polyPointA.y << ", " << in_polyPointA.z << std::endl;
	//std::cout << ">> Second point: " << in_polyPointB.x << ", " << in_polyPointB.y << ", " << in_polyPointB.z << std::endl;
	//std::cout << ">> Third point: " << in_polyPointC.x << ", " << in_polyPointC.y << ", " << in_polyPointC.z << std::endl;
	//std::cout << "X: " << in_polyPointB.x - in_polyPointA.x << std::endl;
	//std::cout << "Y: " << in_polyPointB.y - in_polyPointA.y << std::endl;
	//std::cout << "Z: " << in_polyPointB.z - in_polyPointA.z << std::endl;
	// set default initial values
	returnPoint.x = in_polyPointB.x - in_polyPointA.x;
	returnPoint.y = in_polyPointB.y - in_polyPointA.y;
	returnPoint.z = in_polyPointB.z - in_polyPointA.z;


	// check for X intention:
	if (in_polyPointB.x == in_polyPointA.x)		// x of A and B must equal each other
	{
		if (in_polyPointC.x != in_polyPointA.x)	// only do the following operation if C's x is not equal to either
		{
			returnPoint.x = ((in_polyPointC.x - in_polyPointA.x) / abs(in_polyPointC.x - in_polyPointA.x)) * -1.0f;	// subtract either A or B's x here (it doesnt matter), and invert the value
			//std::cout << "Intended x face is: " << returnPoint.x << std::endl;
		}
		else if (in_polyPointC.x == in_polyPointA.x)	// if C.x == A.x and  B.x == C.x, it is perfectly clamped, set to 0
		{
			returnPoint.x = 0.0f;
		}
	}
	else if (in_polyPointB.x != in_polyPointA.x)
	{
		returnPoint.x = ((in_polyPointB.x - in_polyPointA.x) / abs(in_polyPointB.x - in_polyPointA.x));
	}



	// check for Y intention:
	if (in_polyPointB.y == in_polyPointA.y)
	{
		if (in_polyPointC.y != in_polyPointA.y)
		{
			returnPoint.y = ((in_polyPointC.y - in_polyPointA.y) / abs(in_polyPointC.y - in_polyPointA.y)) * -1.0f;
			//std::cout << "Intended y face is: " << returnPoint.y << std::endl;
		}
		else if (in_polyPointC.y == in_polyPointA.y)
		{
			returnPoint.y = 0.0f;
		}
	}
	else if (in_polyPointB.y != in_polyPointA.y)
	{
		returnPoint.y = ((in_polyPointB.y - in_polyPointA.y) / abs(in_polyPointB.y - in_polyPointA.y));
	}

	// check for Z intention:
	if (in_polyPointB.z == in_polyPointA.z)
	{
		if (in_polyPointC.z != in_polyPointA.z)
		{
			returnPoint.z = ((in_polyPointC.z - in_polyPointA.z) / abs(in_polyPointC.z - in_polyPointA.z)) * -1.0f;
			//std::cout << "Intended z face is: " << returnPoint.z << std::endl;
		}
		else if (in_polyPointC.z == in_polyPointA.z)
		{
			returnPoint.z = 0.0f;
		}
	}
	else if (in_polyPointB.z != in_polyPointA.z)
	{
		returnPoint.z = ((in_polyPointB.z - in_polyPointA.z) / abs(in_polyPointB.z - in_polyPointA.z));
	}

	//std::cout << "Calculated intended faces are: " << std::endl;
	//std::cout << "X: " << returnPoint.x << std::endl;
	//std::cout << "Y: " << returnPoint.y << std::endl;
	//std::cout << "Z: " << returnPoint.z << std::endl;

	return returnPoint;

}

ECBPolyPoint IndependentUtils::determineIntendedFacesV2(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB, ECBPolyPoint in_polyPointC, ECBPolyPoint in_xintercept, ECBPolyPoint in_yintercept, ECBPolyPoint in_zintercept)
{
	ECBPolyPoint returnPoint;
	//std::cout << "!!!~~~~~~~~~~~~~~~~~~~~~~(V2) Intended faces: calculatling slope: " << std::endl;
	//std::cout << ">> ! First point: " << in_polyPointA.x << ", " << in_polyPointA.y << ", " << in_polyPointA.z << std::endl;
	//std::cout << ">> ! Second point: " << in_polyPointB.x << ", " << in_polyPointB.y << ", " << in_polyPointB.z << std::endl;
	//std::cout << ">> ! Third point: " << in_polyPointC.x << ", " << in_polyPointC.y << ", " << in_polyPointC.z << std::endl;
	//std::cout << "X: " << in_polyPointB.x - in_polyPointA.x << std::endl;
	//std::cout << "Y: " << in_polyPointB.y - in_polyPointA.y << std::endl;
	//std::cout << "Z: " << in_polyPointB.z - in_polyPointA.z << std::endl;
	// set default initial values


	returnPoint.x = in_polyPointB.x - in_polyPointA.x;
	returnPoint.y = in_polyPointB.y - in_polyPointA.y;
	returnPoint.z = in_polyPointB.z - in_polyPointA.z;

	ECBPolyPoint slopePoint = returnPoint;		// not sure about the 3 lines above this???
	// check for X intention:
	if (in_polyPointB.x == in_polyPointA.x)		// x of A and B must equal each other
	{
		if (in_polyPointC.x != in_polyPointA.x)	// only do the following operation if C's x is not equal to either
		{
			returnPoint.x = ((in_polyPointC.x - in_polyPointA.x) / abs(in_polyPointC.x - in_polyPointA.x)) * -1.0f;	// subtract either A or B's x here (it doesnt matter), and invert the value
			//std::cout << "Intended x face is: " << returnPoint.x << std::endl;

			// perform face validity
			float normalizedDirectionX = ((in_polyPointC.x - in_polyPointA.x) / abs(in_polyPointC.x - in_polyPointA.x));
			if (determineIntendedFaceValidity(0, returnPoint.x, normalizedDirectionX, in_xintercept, slopePoint) == 0)	// if this is 0, the intention is invalid; we must revert it back.
			{
				//std::cout << "Reverting!!! " << std::endl;
				returnPoint.x *= -1;
			}
		}
		else if (in_polyPointC.x == in_polyPointA.x)	// if C.x == A.x and  B.x == C.x, it is perfectly clamped, set to 0
		{
			returnPoint.x = 0.0f;
		}
	}
	else if (in_polyPointB.x != in_polyPointA.x)
	{
		returnPoint.x = ((in_polyPointB.x - in_polyPointA.x) / abs(in_polyPointB.x - in_polyPointA.x));
	}



	// check for Y intention:
	if (in_polyPointB.y == in_polyPointA.y)
	{
		if (in_polyPointC.y != in_polyPointA.y)
		{
			returnPoint.y = ((in_polyPointC.y - in_polyPointA.y) / abs(in_polyPointC.y - in_polyPointA.y)) * -1.0f;
			//std::cout << "Intended y face is: " << returnPoint.y << std::endl;

			// perform face validity
			float normalizedDirectionY = ((in_polyPointC.y - in_polyPointA.y) / abs(in_polyPointC.y - in_polyPointA.y));
			if (determineIntendedFaceValidity(0, returnPoint.y, normalizedDirectionY, in_yintercept, slopePoint) == 0)	// if this is 0, the intention is invalid; we must revert it back.
			{
				//std::cout << "Reverting!!! " << std::endl;
				returnPoint.y *= -1;
			}
		}
		else if (in_polyPointC.y == in_polyPointA.y)
		{
			returnPoint.y = 0.0f;
		}
	}
	else if (in_polyPointB.y != in_polyPointA.y)
	{
		returnPoint.y = ((in_polyPointB.y - in_polyPointA.y) / abs(in_polyPointB.y - in_polyPointA.y));
	}

	// check for Z intention:
	if (in_polyPointB.z == in_polyPointA.z)
	{
		if (in_polyPointC.z != in_polyPointA.z)
		{
			returnPoint.z = ((in_polyPointC.z - in_polyPointA.z) / abs(in_polyPointC.z - in_polyPointA.z)) * -1.0f;
			//std::cout << "Intended z face is: " << returnPoint.z << std::endl;

			float normalizedDirectionZ = ((in_polyPointC.z - in_polyPointA.z) / abs(in_polyPointC.z - in_polyPointA.z));
			if (determineIntendedFaceValidity(0, returnPoint.z, normalizedDirectionZ, in_zintercept, slopePoint) == 0)	// if this is 0, the intention is invalid; we must revert it back.
			{
				//std::cout << "Reverting!!! " << std::endl;
				returnPoint.z *= -1;
			}
		}
		else if (in_polyPointC.z == in_polyPointA.z)
		{
			returnPoint.z = 0.0f;
		}
	}
	else if (in_polyPointB.z != in_polyPointA.z)
	{
		returnPoint.z = ((in_polyPointB.z - in_polyPointA.z) / abs(in_polyPointB.z - in_polyPointA.z));
	}

	//std::cout << "Calculated intended faces are: " << std::endl;
	//std::cout << "X: " << returnPoint.x << std::endl;
	//std::cout << "Y: " << returnPoint.y << std::endl;
	//std::cout << "Z: " << returnPoint.z << std::endl;

	return returnPoint;
}

int IndependentUtils::determineIntendedFaceValidity(int in_xyorz, float in_suggestedIntendedFace, float in_normalizedDirectionFloat, ECBPolyPoint in_xyorzIntercept, ECBPolyPoint in_slopeOfAB)
{
	int isValid = 1;	// we will assume it will be valid, but this will be set to 0 if it is not.
	if (in_xyorz == 0) // x calcs
	{
		// Part A: first, determine the slope that will be used:
		// compare the suggested face to the normalized direction float
		ECBPolyPoint interceptSlope = in_xyorzIntercept;
		if (in_suggestedIntendedFace != in_normalizedDirectionFloat)
		{
			interceptSlope = invertSlope(in_xyorzIntercept);	// invert the slope
		}

		ECBPolyPoint normalizedInterceptSlope = findNormalizedPoint(interceptSlope);	// get the normalized directions of the intercept slope
		ECBPolyPoint normalizedAB = findNormalizedPoint(in_slopeOfAB);
		if
			(
			(normalizedInterceptSlope.x == normalizedAB.x)
				&&
				(normalizedInterceptSlope.y == normalizedAB.y)
				&&
				(normalizedInterceptSlope.z == normalizedAB.z)
				)
		{
			isValid = 0;
			//std::cout << "###### Face validity check: this face is invalid, will return 0..." << std::endl;
		}

	}

	else if (in_xyorz == 1) // y calcs
	{

	}

	else if (in_xyorz == 2) // z calcs
	{

	}

	return isValid;
}


ECBPoly IndependentUtils::buildECBPolyFromEnclaveTriangle(EnclaveTriangle in_enclaveTriangle, 
														  EnclaveKeyDef::EnclaveKey in_blueprintKeyForTranslation,
														  EnclaveKeyDef::EnclaveKey in_oreKeyForTranslation)
{
	ECBPoly returnPoly;

	ECBPolyPoint translatedPoint0, translatedPoint1, translatedPoint2;
	//translatedPoint0 = translateEnclavePointToWorldSpace(in_enclaveTriangle.lineArray[0].pointA, in_blueprintKeyForTranslation, in_oreKeyForTranslation);
	//translatedPoint1 = translateEnclavePointToWorldSpace(in_enclaveTriangle.lineArray[1].pointA, in_blueprintKeyForTranslation, in_oreKeyForTranslation);
	//translatedPoint2 = translateEnclavePointToWorldSpace(in_enclaveTriangle.lineArray[2].pointA, in_blueprintKeyForTranslation, in_oreKeyForTranslation);

	translatedPoint0 = translateEnclavePointToWorldSpace(in_enclaveTriangle.points[0], in_blueprintKeyForTranslation, in_oreKeyForTranslation);
	translatedPoint1 = translateEnclavePointToWorldSpace(in_enclaveTriangle.points[1], in_blueprintKeyForTranslation, in_oreKeyForTranslation);
	translatedPoint2 = translateEnclavePointToWorldSpace(in_enclaveTriangle.points[2], in_blueprintKeyForTranslation, in_oreKeyForTranslation);

	ECBPolyLine polyLine0, polyLine1, polyLine2;

	polyLine0.pointA = translatedPoint0;
	polyLine0.pointB = translatedPoint1;
	polyLine0.pointC = translatedPoint2;

	polyLine1.pointA = translatedPoint1;
	polyLine1.pointB = translatedPoint2;
	polyLine1.pointC = translatedPoint0;

	polyLine2.pointA = translatedPoint2;
	polyLine2.pointB = translatedPoint0;
	polyLine2.pointC = translatedPoint1;

	// determine the slopes for each line
	PolyUtils::determineLineInterceptSlopes(&polyLine0, translatedPoint2);
	PolyUtils::determineLineInterceptSlopes(&polyLine1, translatedPoint0);
	PolyUtils::determineLineInterceptSlopes(&polyLine2, translatedPoint1);

	returnPoly.lineMap[0] = polyLine0;
	returnPoly.lineMap[1] = polyLine1;
	returnPoly.lineMap[2] = polyLine2;
	returnPoly.materialID = in_enclaveTriangle.enclaveTriangleMaterialID;
	returnPoly.isPolyPerfectlyClamped = in_enclaveTriangle.isEnclaveTrianglePolyPerfectlyClamped;
	returnPoly.emptyNormal = in_enclaveTriangle.emptyNormal;
	return returnPoly;
}

ECBPolyPoint IndependentUtils::translateEnclavePointToWorldSpace(ECBPolyPoint in_pointToTranslate,
	EnclaveKeyDef::EnclaveKey in_blueprintKeyForTranslation,
	EnclaveKeyDef::EnclaveKey in_oreKeyForTranslation)
{
	ECBPolyPoint translatedPoint;
	translatedPoint.x = (in_blueprintKeyForTranslation.x*32.0f) + (in_oreKeyForTranslation.x*4.0f) + in_pointToTranslate.x;
	translatedPoint.y = (in_blueprintKeyForTranslation.y*32.0f) + (in_oreKeyForTranslation.y*4.0f) + in_pointToTranslate.y;
	translatedPoint.z = (in_blueprintKeyForTranslation.z*32.0f) + (in_oreKeyForTranslation.z*4.0f) + in_pointToTranslate.z;
	return translatedPoint;
}

ECBPolyPoint IndependentUtils::convertEnclaveBlockVertexToFloats(EnclaveBlockVertex in_vertex)
{
	ECBPolyPoint returnPoint;
	returnPoint.x = convertPreciseCoordToFloat(in_vertex.x);
	returnPoint.y = convertPreciseCoordToFloat(in_vertex.y);
	returnPoint.z = convertPreciseCoordToFloat(in_vertex.z);
	return returnPoint;
}

ECBPolyPointTri IndependentUtils::convertEnclaveBlockVertexesToFloats(EnclaveBlockVertexTri in_vertexTri)
{
	ECBPolyPointTri returnTri;
	returnTri.triPoints[0].x = convertPreciseCoordToFloat(in_vertexTri.pointA.x);
	returnTri.triPoints[0].y = convertPreciseCoordToFloat(in_vertexTri.pointA.y);
	returnTri.triPoints[0].z = convertPreciseCoordToFloat(in_vertexTri.pointA.z);

	returnTri.triPoints[1].x = convertPreciseCoordToFloat(in_vertexTri.pointB.x);
	returnTri.triPoints[1].y = convertPreciseCoordToFloat(in_vertexTri.pointB.y);
	returnTri.triPoints[1].z = convertPreciseCoordToFloat(in_vertexTri.pointB.z);

	returnTri.triPoints[2].x = convertPreciseCoordToFloat(in_vertexTri.pointC.x);
	returnTri.triPoints[2].y = convertPreciseCoordToFloat(in_vertexTri.pointC.y);
	returnTri.triPoints[2].z = convertPreciseCoordToFloat(in_vertexTri.pointC.z);
	return returnTri;

}

ECBPolyPointTri IndependentUtils::combineClampedCoordsWithPrecise(ECBPolyPointTri in_precisePolyPointTri, EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	ECBPolyPointTri actualCoords;
	ECBPolyPoint blockClampedVals;
	blockClampedVals.x = (in_blueprintKey.x*32.0f) + (in_enclaveKey.x*4.0f) + in_blockKey.x;
	blockClampedVals.y = (in_blueprintKey.y*32.0f) + (in_enclaveKey.y*4.0f) + in_blockKey.y;
	blockClampedVals.z = (in_blueprintKey.z*32.0f) + (in_enclaveKey.z*4.0f) + in_blockKey.z;
	actualCoords.triPoints[0].x = blockClampedVals.x + in_precisePolyPointTri.triPoints[0].x;
	actualCoords.triPoints[0].y = blockClampedVals.y + in_precisePolyPointTri.triPoints[0].y;
	actualCoords.triPoints[0].z = blockClampedVals.z + in_precisePolyPointTri.triPoints[0].z;

	actualCoords.triPoints[1].x = blockClampedVals.x + in_precisePolyPointTri.triPoints[1].x;
	actualCoords.triPoints[1].y = blockClampedVals.y + in_precisePolyPointTri.triPoints[1].y;
	actualCoords.triPoints[1].z = blockClampedVals.z + in_precisePolyPointTri.triPoints[1].z;

	actualCoords.triPoints[2].x = blockClampedVals.x + in_precisePolyPointTri.triPoints[2].x;
	actualCoords.triPoints[2].y = blockClampedVals.y + in_precisePolyPointTri.triPoints[2].y;
	actualCoords.triPoints[2].z = blockClampedVals.z + in_precisePolyPointTri.triPoints[2].z;
	return actualCoords;
}

ECBPolyPointTri IndependentUtils::adjustEnclaveTriangleCoordsToWorldSpace(ECBPolyPointTri in_precisePolyPointTri, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	ECBPolyPointTri actualCoords;
	ECBPolyPoint enclavePointAdjustmentValue;
	enclavePointAdjustmentValue.x = (in_blueprintKey.x*32.0f) + (in_enclaveKey.x*4.0f);
	enclavePointAdjustmentValue.y = (in_blueprintKey.y*32.0f) + (in_enclaveKey.y*4.0f);
	enclavePointAdjustmentValue.z = (in_blueprintKey.z*32.0f) + (in_enclaveKey.z*4.0f);
	actualCoords.triPoints[0].x = enclavePointAdjustmentValue.x + in_precisePolyPointTri.triPoints[0].x;
	actualCoords.triPoints[0].y = enclavePointAdjustmentValue.y + in_precisePolyPointTri.triPoints[0].y;
	actualCoords.triPoints[0].z = enclavePointAdjustmentValue.z + in_precisePolyPointTri.triPoints[0].z;

	actualCoords.triPoints[1].x = enclavePointAdjustmentValue.x + in_precisePolyPointTri.triPoints[1].x;
	actualCoords.triPoints[1].y = enclavePointAdjustmentValue.y + in_precisePolyPointTri.triPoints[1].y;
	actualCoords.triPoints[1].z = enclavePointAdjustmentValue.z + in_precisePolyPointTri.triPoints[1].z;

	actualCoords.triPoints[2].x = enclavePointAdjustmentValue.x + in_precisePolyPointTri.triPoints[2].x;
	actualCoords.triPoints[2].y = enclavePointAdjustmentValue.y + in_precisePolyPointTri.triPoints[2].y;
	actualCoords.triPoints[2].z = enclavePointAdjustmentValue.z + in_precisePolyPointTri.triPoints[2].z;
	return actualCoords;
}

BoundaryOrientation IndependentUtils::getOppositeOrientation(BoundaryOrientation in_boundaryOrientation)
{
	BoundaryOrientation returnOrientation = BoundaryOrientation::NONE;
	switch (in_boundaryOrientation)
	{
		case BoundaryOrientation::POS_X: {	returnOrientation = BoundaryOrientation::NEG_X; break; }
		case BoundaryOrientation::NEG_X: {	returnOrientation = BoundaryOrientation::POS_X; break; }
		case BoundaryOrientation::POS_Y: {	returnOrientation = BoundaryOrientation::NEG_Y; break; }
		case BoundaryOrientation::NEG_Y: {	returnOrientation = BoundaryOrientation::POS_Y; break; }
		case BoundaryOrientation::POS_Z: {	returnOrientation = BoundaryOrientation::NEG_Z; break; }
		case BoundaryOrientation::NEG_Z: {	returnOrientation = BoundaryOrientation::POS_Z; break; }
	}
	return returnOrientation;
}

void IndependentUtils::printBoundaryOrientation(BoundaryOrientation in_boundaryOrientation)
{
	switch (in_boundaryOrientation)
	{
		case BoundaryOrientation::POS_X: { std::cout << "POS_X"; break; }
		case BoundaryOrientation::NEG_X: { std::cout << "NEG_X"; break; }
		case BoundaryOrientation::POS_Y: { std::cout << "POS_Y"; break; }
		case BoundaryOrientation::NEG_Y: { std::cout << "NEG_Y"; break; }
		case BoundaryOrientation::POS_Z: { std::cout << "POS_Z"; break; }
		case BoundaryOrientation::NEG_Z: { std::cout << "NEG_Z"; break; }
	}
}

std::string IndependentUtils::getBoundaryOrientationString(BoundaryOrientation in_boundaryOrientation)
{
	std::string returnString = "NONE";
	switch (in_boundaryOrientation)
	{
		case BoundaryOrientation::POS_X: { returnString = "POS_X"; break; }
		case BoundaryOrientation::NEG_X: { returnString = "NEG_X"; break; }
		case BoundaryOrientation::POS_Y: { returnString = "POS_Y"; break; }
		case BoundaryOrientation::NEG_Y: { returnString = "NEG_Y"; break; }
		case BoundaryOrientation::POS_Z: { returnString = "POS_Z"; break; }
		case BoundaryOrientation::NEG_Z: { returnString = "NEG_Z"; break; }
	}
	return returnString;
}

int IndependentUtils::convertBoundaryOrientationToInt(BoundaryOrientation in_boundaryOrientation)
{
	int returnValue = 0;
	switch (in_boundaryOrientation)
	{
		case BoundaryOrientation::NONE:  { returnValue = 0; break; }
		case BoundaryOrientation::POS_X: { returnValue = 2; break; }
		case BoundaryOrientation::NEG_X: { returnValue = 4; break; }
		case BoundaryOrientation::POS_Y: { returnValue = 5; break; }
		case BoundaryOrientation::NEG_Y: { returnValue = 6; break; }
		case BoundaryOrientation::POS_Z: { returnValue = 3; break; }
		case BoundaryOrientation::NEG_Z: { returnValue = 1; break; }
	}
	return returnValue;
}

BoundaryOrientation IndependentUtils::convertIntToBoundaryOrientation(int in_indexValueToConvert)
{
	BoundaryOrientation returnOrientation = BoundaryOrientation::NONE;
	switch (in_indexValueToConvert)
	{
		case 0: { returnOrientation = BoundaryOrientation::NONE; break; }
		case 2: { returnOrientation = BoundaryOrientation::POS_X; break;}
		case 4: { returnOrientation = BoundaryOrientation::NEG_X; break;}
		case 5: { returnOrientation = BoundaryOrientation::POS_Y; break;}
		case 6: { returnOrientation = BoundaryOrientation::NEG_Y; break;}
		case 3: { returnOrientation = BoundaryOrientation::POS_Z; break;}
		case 1: { returnOrientation = BoundaryOrientation::NEG_Z; break;}
	}
	return returnOrientation;
}