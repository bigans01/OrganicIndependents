#include "stdafx.h"
#include "FTriangleUtils.h"

FIntersectMeta FTriangleUtils::findIntersectionData(ECBPolyPoint in_pointA,
													ECBPolyPoint in_pointB,
													EnclaveKeyDef::EnclaveKey in_pointAKey,
													EnclaveKeyDef::EnclaveKey in_pointBKey,
													TracingLineBoundingBox in_boundingBox,
													FTraceType in_fTraceType)
{
	FIntersectMeta intersectMetaReturn;

	// determine the distance between cells, based on the FTraceType.
	float cellLength = 0.0f;
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE: { cellLength = 32.0f; break; }
		case FTraceType::ORE_TRACE: { cellLength = 4.0f; break; }
		case FTraceType::BLOCK_TRACE: { cellLength = 1.0f; break; }
	}

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
			x_interceptCoord = float((in_pointAKey.x * cellLength) + cellLength);								// x_interceptCoord is the precise location of x at the EAST face border
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
			x_interceptCoord = float(in_pointAKey.x * cellLength);
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
			y_interceptCoord = float((in_pointAKey.y * cellLength) + cellLength);								// y_interceptCoord is the precise location of y at the TOP face border
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

			y_interceptCoord = float(in_pointAKey.y * cellLength);
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
			z_interceptCoord = float((in_pointAKey.z * cellLength) + cellLength);
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
			z_interceptCoord = float(in_pointAKey.z * cellLength);
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


		// Do the following if the FTracetype is BLUEPRINT_TRACE

		ECBCalibratedPointPair newTri = compareAndCalibrateDistances(&triPointParam, distanceValues, resultantSlope, in_pointAKey);
		//std::cout << "Comparing and calibrate complete..." << std::endl;
		ECBPolyPoint slopeDirection;
		slopeDirection.x = float(x_dir);
		slopeDirection.y = float(y_dir);
		slopeDirection.z = float(z_dir);

		triPointParam = newTri.calibratedPointTri;
		distanceValues = newTri.calibratedDistance;

		//std::cout << "Calling BP movemeta..." << std::endl;

		
		intersectMetaReturn = calculateIntersection(in_pointAKey,
			in_pointA,
			distanceValues,
			slopeDirection,
			triPointParam,
			in_boundingBox,
			in_fTraceType);
		

		//std::cout << "||||| Intersect return meta point: " << intersectMetaReturn.intersectedPoint.x << ", " << intersectMetaReturn.intersectedPoint.y << ", " << intersectMetaReturn.intersectedPoint.z << std::endl;
		//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| End of movemeta finding. " << std::endl;
	}

	else if (in_pointAKey == in_pointBKey)
	{
		intersectMetaReturn.originPoint = in_pointA;
		intersectMetaReturn.intersectedPoint = in_pointB;
		//std::cout << "finish entry" << std::endl;
	}

	return intersectMetaReturn;

}

EnclaveKeyDef::EnclaveKey FTriangleUtils::getBorderShiftResult(ECBBorder in_Border, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
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

FIntersectMeta FTriangleUtils::calculateIntersection(EnclaveKeyDef::EnclaveKey in_Key1,
	ECBPolyPoint in_originPoint,
	ECBPolyPoint in_distanceValues,
	ECBPolyPoint in_slopeDirection,
	ECBPolyPointTri in_XYZinterceptCoords,
	TracingLineBoundingBox in_boundingBox,
	FTraceType in_fTraceType)
{
	FIntersectMeta intersectMeta;
	// determine the distance between cells, based on the FTraceType.
	float cellLength = 0.0f;
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE: { cellLength = 32.0f; break; }
		case FTraceType::ORE_TRACE: { cellLength = 4.0f; break; }
		case FTraceType::BLOCK_TRACE: { cellLength = 1.0f; break; }
	}

	EnclaveKeyDef::EnclaveKey newKey;
	FTraceBorderLineList pointABorderLineList;
	FTraceBorderValues borderLimits = getCurrentTracingLimits(in_Key1, in_fTraceType);
	pointABorderLineList = getCurrentBorderLines(in_Key1, in_fTraceType);
	int isPointOnALine = 1;

	ECBPolyPoint testXYZenhanced = roundXYZInterceptDistancesToAppropriatePrecision(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = testXYZenhanced.x;
	float dist_to_Y = testXYZenhanced.y;
	float dist_to_Z = testXYZenhanced.z;

	ECBPolyPoint x_intercept_coords;
	ECBPolyPoint y_intercept_coords;
	ECBPolyPoint z_intercept_coords;

	// do trace specific mode logic here
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE:
		{
			x_intercept_coords = roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[0], in_Key1);
			y_intercept_coords = roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[1], in_Key1);
			z_intercept_coords = roundToAppropriatePrecisionForHundredths(in_XYZinterceptCoords.triPoints[2], in_Key1);
			break;
		}

		case FTraceType::ORE_TRACE:
		{
			x_intercept_coords = roundPointToHundredth(in_XYZinterceptCoords.triPoints[0]);
			y_intercept_coords = roundPointToHundredth(in_XYZinterceptCoords.triPoints[1]);
			z_intercept_coords = roundPointToHundredth(in_XYZinterceptCoords.triPoints[2]);
			break;
		}

		case FTraceType::BLOCK_TRACE:
		{
			x_intercept_coords = in_XYZinterceptCoords.triPoints[0];
			y_intercept_coords = in_XYZinterceptCoords.triPoints[1];
			z_intercept_coords = in_XYZinterceptCoords.triPoints[2];
			break;
		}
	}

	// Below: This is the value that will contain the final point to use.
	ECBPolyPoint pointToCheck;

	// Next, perform corner check.
	if ((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
	{
		//std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}

	if (
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == ((in_Key1.z*cellLength) + cellLength)) || (in_originPoint.z == in_Key1.z*cellLength))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == ((in_Key1.y*cellLength) + cellLength)) || (in_originPoint.y == in_Key1.y*cellLength))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == ((in_Key1.x*cellLength) + cellLength)) || (in_originPoint.x == in_Key1.x*cellLength))		// THIS WAS AN ERROR ON: 5/27/2018! These values were z instead of x!!!!
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
			((x_intercept_coords.y == ((in_Key1.y*cellLength) + cellLength)) || (x_intercept_coords.y == in_Key1.y*cellLength))
				&&
				((x_intercept_coords.z == ((in_Key1.z*cellLength) + cellLength)) || (x_intercept_coords.z == in_Key1.z*cellLength))
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
			((y_intercept_coords.x == ((in_Key1.x*cellLength) + cellLength)) || (y_intercept_coords.x == in_Key1.x*cellLength))
				&&
				((y_intercept_coords.z == ((in_Key1.z*cellLength) + cellLength)) || (y_intercept_coords.z == in_Key1.z*cellLength))
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
			((z_intercept_coords.x == ((in_Key1.x*cellLength) + cellLength)) || (z_intercept_coords.x == in_Key1.x*cellLength))
				&&
				((z_intercept_coords.y == ((in_Key1.y*cellLength) + cellLength)) || (z_intercept_coords.y == in_Key1.y*cellLength))
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
			pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			//std::cout << "Y point chosen... " << std::endl;
			pointToCheck = roundToNearestLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			//std::cout << "Z point chosen... " << std::endl;
			pointToCheck = roundToNearestLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x	&&		pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y	&&		pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z)		// Lower NW
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_LowerNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key
			//std::cout << "Point is at lower NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z)			// Lower SW
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			//std::cout << "Point is at lower SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSW.cornerPoint.x   &&	pointToCheck.y == pointABorderLineList.corner_UpperSW.cornerPoint.y   &&  pointToCheck.z == pointABorderLineList.corner_UpperSW.cornerPoint.z)			// Upper SW	 
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_UpperSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SW
			//std::cout << "Point is at upper SW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNW.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNW.cornerPoint.z)			// Upper NW
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x	&&  pointToCheck.y == pointABorderLineList.corner_LowerNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerNE.cornerPoint.z)			// Lower NE
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_LowerSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_LowerSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_LowerSE.cornerPoint.z)			// Lower SE
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperSE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperSE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperSE.cornerPoint.z)			// Upper SE
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
			//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		}
		else if (pointToCheck.x == pointABorderLineList.corner_UpperNE.cornerPoint.x	&&	pointToCheck.y == pointABorderLineList.corner_UpperNE.cornerPoint.y	&&	pointToCheck.z == pointABorderLineList.corner_UpperNE.cornerPoint.z)			// Upper NE
		{
			newKey = getBorderShiftResult(pointABorderLineList.corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
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
			pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
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
					newKey = getBorderShiftResult(pointABorderLineList.Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
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
					newKey = getBorderShiftResult(pointABorderLineList.Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}

			}
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y
			{
				std::cout << "(Z-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
				pointToCheck = roundToNearestLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
				if (in_slopeDirection.z == 1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
					std::cout << "X/Y distances match, but Z is hit first (positive z): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
					std::cout << "X/Y distances match, but Z is hit first (negative z)" << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				//if 
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): X and Z distances match, check for Y ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))
		{
			//std::cout << "STEP 2:2 ENTRY: Y-lines " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);

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
					newKey = getBorderShiftResult(pointABorderLineList.Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;

				}

			}
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				//std::cout << "(Y-line) OPTION 2: entry check" << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
				pointToCheck = roundToNearestLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
				if (in_slopeDirection.y == 1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
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
			pointToCheck = roundToNearestLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
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
					newKey = getBorderShiftResult(pointABorderLineList.Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
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
					newKey = getBorderShiftResult(pointABorderLineList.Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
				}
			}
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << "STEP 2:3:2 entry " << std::endl;
				//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
				pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
				if (in_slopeDirection.x == 1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					newKey = getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				}
			}
		}

		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type
			pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			// pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(0, x_intercept_coords, isPointOnALine);
			if ((pointToCheck.x == pointABorderLineList.corner_LowerNW.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.WestFace);
				newKey = getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.x == pointABorderLineList.corner_LowerNE.cornerPoint.x))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.EastFace);
				newKey = getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
			}

		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = roundToNearestLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			if ((pointToCheck.y == pointABorderLineList.corner_LowerNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.BottomFace);
				newKey = getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.y == pointABorderLineList.corner_UpperNW.cornerPoint.y))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.TopFace);
				newKey = getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(2, z_intercept_coords, isPointOnALine);
			pointToCheck = roundToNearestLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			if ((pointToCheck.z == pointABorderLineList.corner_LowerNW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.NorthFace);
				newKey = getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
			}
			else if ((pointToCheck.z == pointABorderLineList.corner_LowerSW.cornerPoint.z))
			{
				//calculatedEndpointData.applyBlockBorder(pointABorderLineList.SouthFace);
				newKey = getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
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
			pointToCheck = roundToNearestLineOrCorner(0, x_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			if (in_slopeDirection.x == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +X" << std::endl;
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			//pointToCheck = y_intercept_coords;
			//pointToCheck = OrganicUtils::roundToNearestBlockLineOrCorner(1, y_intercept_coords, isPointOnALine);
			pointToCheck = roundToNearestLineOrCorner(1, y_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			if (in_slopeDirection.y == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
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
			//ECBPPOrientationResults testResults = IndependentUtils::GetBlueprintPointOrientation(pointToCheck, &pointABorderLineList);
			pointToCheck = roundToNearestLineOrCorner(2, z_intercept_coords, isPointOnALine, &borderLimits, in_fTraceType);
			//std::cout << "Z intercept-Point to check, post conversion" << pointToCheck.x << "," << pointToCheck.y << "," << pointToCheck.z << std::endl;
			if (in_slopeDirection.z == 1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;

			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = IndependentUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				newKey = getFTraceCalibratedKey(in_originPoint, pointToCheck, &pointABorderLineList);
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

EnclaveKeyDef::EnclaveKey FTriangleUtils::getFTraceCalibratedKey(ECBPolyPoint in_originPoint,
	ECBPolyPoint in_interceptPoint,
	FTraceBorderLineList* in_borderLineListRef)
{
	EnclaveKeyDef::EnclaveKey valueToReturn;	// for returning the appropriate move key
	FTraceOrientationResults results = getFTracePointOrientation(in_interceptPoint, in_borderLineListRef);
	valueToReturn = lookupBlueprintBorderKey(results, in_borderLineListRef, in_originPoint, in_interceptPoint);									// get the move key
	return valueToReturn;
}

EnclaveKeyDef::EnclaveKey FTriangleUtils::lookupBlueprintBorderKey(FTraceOrientationResults in_results,
	FTraceBorderLineList* in_borderLineListRef,
	ECBPolyPoint in_originPoint,
	ECBPolyPoint in_interceptPoint)
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

FTraceOrientationResults FTriangleUtils::getFTracePointOrientation(ECBPolyPoint in_pointToCheck, FTraceBorderLineList* in_borderLineList)
{
	FTraceOrientationResults returnResults;

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

ECBBorderValues FTriangleUtils::getBlueprintLimits(EnclaveKeyDef::EnclaveKey in_Key)
{
	ECBBorderValues  borderValues;
	borderValues.negXlimit = in_Key.x * 32.0f;			// west border 
	borderValues.posXlimit = (in_Key.x * 32.0f) + 32.0f;	// east border
	borderValues.negYlimit = in_Key.y * 32.0f;			// bottom border 
	borderValues.posYlimit = (in_Key.y * 32.0f) + 32.0f;	// top border
	borderValues.negZlimit = in_Key.z * 32.0f;			// north border
	borderValues.posZlimit = (in_Key.z * 32.0f) + 32.0f;	// south border
	return borderValues;
}

ECBPolyPoint FTriangleUtils::findNormalizedPoint(ECBPolyPoint in_pointA)
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

ECBPolyPoint FTriangleUtils::roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ)
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

ECBCalibratedPointPair FTriangleUtils::compareAndCalibrateDistances(ECBPolyPointTri* in_polyPointTriRef,
	ECBPolyPoint in_distanceValues,
	ECBPolyPoint in_currentLineSlope,
	EnclaveKeyDef::EnclaveKey in_currentBlueprintKey)
{
	ECBCalibratedPointPair returnPair;
	ECBBorderValues current_border_values = getBlueprintLimits(in_currentBlueprintKey);
	//std::cout << "Pre-normalize: " << in_currentLineSlope.x << ", " << in_currentLineSlope.y << ", " << in_currentLineSlope.z << std::endl;
	ECBPolyPoint normalized_slope = findNormalizedPoint(in_currentLineSlope);
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

FTraceBorderValues FTriangleUtils::getCurrentTracingLimits(EnclaveKeyDef::EnclaveKey in_currentTracingKey, FTraceType in_fTraceType)
{
	FTraceBorderValues borderValues;

	// determine the distance between cells, based on the FTraceType.
	float cellLength = 0.0f;
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE: { cellLength = 32.0f; break; }
		case FTraceType::ORE_TRACE: { cellLength = 4.0f; break; }
		case FTraceType::BLOCK_TRACE: { cellLength = 1.0f; break; }
	}

	borderValues.negXlimit = in_currentTracingKey.x * cellLength;			// west border 
	borderValues.posXlimit = (in_currentTracingKey.x * cellLength) + cellLength;	// east border
	borderValues.negYlimit = in_currentTracingKey.y * cellLength;			// bottom border 
	borderValues.posYlimit = (in_currentTracingKey.y * cellLength) + cellLength;	// top border
	borderValues.negZlimit = in_currentTracingKey.z * cellLength;			// north border
	borderValues.posZlimit = (in_currentTracingKey.z * cellLength) + cellLength;	// south border

	return borderValues;
}

FTraceBorderLineList FTriangleUtils::getCurrentBorderLines(EnclaveKeyDef::EnclaveKey in_currentTracingKey, FTraceType in_fTraceType)
{
	FTraceBorderLineList newLineList;
	// determine the distance between cells, based on the FTraceType.
	float cellLength = 0.0f;
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE: { cellLength = 32.0f; break; }
		case FTraceType::ORE_TRACE: { cellLength = 4.0f; break; }
		case FTraceType::BLOCK_TRACE: { cellLength = 1.0f; break; }
	}

	// Lower NW coords
	newLineList.corner_LowerNW.cornerPoint.x = float(in_currentTracingKey.x * cellLength);
	newLineList.corner_LowerNW.cornerPoint.y = float(in_currentTracingKey.y * cellLength);
	newLineList.corner_LowerNW.cornerPoint.z = float(in_currentTracingKey.z * cellLength);

	newLineList.corner_LowerNW.cornerAmpValues.move_x = -1.0f;	// "" amp values (see diagram)
	newLineList.corner_LowerNW.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerNW.cornerAmpValues.move_z = -1.0f;


	// Lower SW coords
	newLineList.corner_LowerSW.cornerPoint.x = float(in_currentTracingKey.x * cellLength);
	newLineList.corner_LowerSW.cornerPoint.y = float(in_currentTracingKey.y * cellLength);
	newLineList.corner_LowerSW.cornerPoint.z = float((in_currentTracingKey.z * cellLength) + cellLength);

	newLineList.corner_LowerSW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_LowerSW.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerSW.cornerAmpValues.move_z = 1.0f;


	// Lower SE coords
	newLineList.corner_LowerSE.cornerPoint.x = float((in_currentTracingKey.x * cellLength) + cellLength);
	newLineList.corner_LowerSE.cornerPoint.y = float(in_currentTracingKey.y * cellLength);
	newLineList.corner_LowerSE.cornerPoint.z = float((in_currentTracingKey.z * cellLength) + cellLength);

	newLineList.corner_LowerSE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_LowerSE.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerSE.cornerAmpValues.move_z = 1.0f;


	// Lower NE coords
	newLineList.corner_LowerNE.cornerPoint.x = float((in_currentTracingKey.x * cellLength) + cellLength);
	newLineList.corner_LowerNE.cornerPoint.y = float(in_currentTracingKey.y * cellLength);
	newLineList.corner_LowerNE.cornerPoint.z = float(in_currentTracingKey.z * cellLength);

	newLineList.corner_LowerNE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_LowerNE.cornerAmpValues.move_y = -1.0f;
	newLineList.corner_LowerNE.cornerAmpValues.move_z = -1.0f;

	// Upper NW coords
	newLineList.corner_UpperNW.cornerPoint.x = float(in_currentTracingKey.x * cellLength);
	newLineList.corner_UpperNW.cornerPoint.y = float((in_currentTracingKey.y * cellLength) + cellLength);
	newLineList.corner_UpperNW.cornerPoint.z = float(in_currentTracingKey.z * cellLength);

	newLineList.corner_UpperNW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_UpperNW.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperNW.cornerAmpValues.move_z = -1.0f;

	// Upper SW coords
	newLineList.corner_UpperSW.cornerPoint.x = float(in_currentTracingKey.x * cellLength);
	newLineList.corner_UpperSW.cornerPoint.y = float((in_currentTracingKey.y * cellLength) + cellLength);
	newLineList.corner_UpperSW.cornerPoint.z = float((in_currentTracingKey.z * cellLength) + cellLength);

	newLineList.corner_UpperSW.cornerAmpValues.move_x = -1.0f;
	newLineList.corner_UpperSW.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperSW.cornerAmpValues.move_z = 1.0f;

	// Upper SE coords
	newLineList.corner_UpperSE.cornerPoint.x = float((in_currentTracingKey.x * cellLength) + cellLength);
	newLineList.corner_UpperSE.cornerPoint.y = float((in_currentTracingKey.y * cellLength) + cellLength);
	newLineList.corner_UpperSE.cornerPoint.z = float((in_currentTracingKey.z * cellLength) + cellLength);

	newLineList.corner_UpperSE.cornerAmpValues.move_x = 1.0f;
	newLineList.corner_UpperSE.cornerAmpValues.move_y = 1.0f;
	newLineList.corner_UpperSE.cornerAmpValues.move_z = 1.0f;

	// Upper NEW coords
	newLineList.corner_UpperNE.cornerPoint.x = float((in_currentTracingKey.x * cellLength) + cellLength);
	newLineList.corner_UpperNE.cornerPoint.y = float((in_currentTracingKey.y * cellLength) + cellLength);
	newLineList.corner_UpperNE.cornerPoint.z = float(in_currentTracingKey.z * cellLength);

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

float FTriangleUtils::roundToHundredth(float in_float)
{
	float calcedFloat = float(floor(in_float * 100 + 0.5) / 100);
	//std::cout << "Original float: " << in_float << std::endl;
	//std::cout << "Calc'ed float is: " << calcedFloat << std::endl;
	return calcedFloat;
}

ECBPolyPoint FTriangleUtils::roundPointToHundredth(ECBPolyPoint in_polyPoint)
{
	float roundedX = float(floor(in_polyPoint.x * 100 + 0.5) / 100);
	float roundedY = float(floor(in_polyPoint.y * 100 + 0.5) / 100);
	float roundedZ = float(floor(in_polyPoint.z * 100 + 0.5) / 100);
	return ECBPolyPoint(roundedX, roundedY, roundedZ);
}

float FTriangleUtils::roundToHundredthSpecial(float in_float, float in_lowerLimit, float in_upperLimit)
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

ECBPolyPoint FTriangleUtils::roundToAppropriatePrecisionForHundredths(ECBPolyPoint in_polyPoint, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	ECBBorderValues borderValues = getBlueprintLimits(in_blueprintKey);
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

ECBPolyPoint FTriangleUtils::determinePolyPointPrecisionLimits(ECBPolyPoint in_polyPoint)
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

ECBPolyPoint FTriangleUtils::roundToNearestLineOrCorner(int in_xoryorz,
	ECBPolyPoint in_polyPoint,
	int in_lineOrCorner,
	FTraceBorderValues* in_blueprintBorderValuesRef,
	FTraceType in_fTraceType)
{
	ECBPolyPoint calibratedPoint = in_polyPoint;

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

	// Rounding logic will be based on FTraceType mode.
	switch (in_fTraceType)
	{
		case FTraceType::BLUEPRINT_TRACE:
		{
			ECBPolyPoint precisionLimit = determinePolyPointPrecisionLimits(calibratedPoint);
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

				// Round a second point to be "like" z
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
		}
		break;
	}

	return calibratedPoint;
}

ECBPolyPoint FTriangleUtils::findNormalizedSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
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

bool FTriangleUtils::isLinePositivelyOriented(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	// assume it's not oriented, towards positives.
	bool isLineOriented = false;

	enum class SlopeDim
	{
		NOVAL,
		X_SLOPE,
		Y_SLOPE,
		Z_SLOPE
	};

	struct PointSlopeData
	{
		PointSlopeData() {};
		PointSlopeData(ECBPolyPoint in_dataForSlope)
		{
			dimMap[SlopeDim::X_SLOPE] = in_dataForSlope.x;
			dimMap[SlopeDim::Y_SLOPE] = in_dataForSlope.y;
			dimMap[SlopeDim::Z_SLOPE] = in_dataForSlope.z;
		}

		int getPositiveDimCount()
		{
			int positiveCount = 0;
			for (auto& currentDim : dimMap)
			{
				if (currentDim.second == 1.0f)
				{
					positiveCount++;
				}
			}
			return positiveCount;
		}

		SlopeDim getOnlyPositiveDim()
		{
			SlopeDim posDim = SlopeDim::NOVAL;
			for (auto& currentDim : dimMap)
			{
				if (currentDim.second == 1.0f)
				{
					posDim = currentDim.first;
				}
			}
			return posDim;
		}

		std::map<SlopeDim, float> dimMap;
	};

	// Find two sets of PointSlopeData: one for A>B, and one for B<A.
	auto normalizedAB = findNormalizedSlope(in_pointA, in_pointB);
	auto normalizedBA = findNormalizedSlope(in_pointB, in_pointA);

	// Form two instances of PointSlopeData.
	PointSlopeData normalizedABData(normalizedAB);
	PointSlopeData normalizedBAData(normalizedBA);

	// Get the number of positives from each one.
	int posCountAB = normalizedABData.getPositiveDimCount();
	int posCountBA = normalizedBAData.getPositiveDimCount();

	//std::cout << "Pos count AB:" << posCountAB << std::endl;
	//std::cout << "Pos count BA:" << posCountBA << std::endl;

	// There should be two conditions to check:
	//	-if posCountAB >= 2, no swap is needed.
	//	-if posCountAB == posCountBA, we'll need to do a compare.
	if (posCountAB >= 2)
	{
		//std::cout << "AB had >= 2 positives; already positive oriented. " << std::endl;
		isLineOriented = true;
	}

	// The only way posCountAB could equal posCountBA is if there's exactly 1 positive dim in each.
	else if (posCountAB == posCountBA)
	{
		// in this condition, we need to check whether its AB or BA that contains the preferred
		// case. The order is: X_SLOPE, Y_SLOPE, Z_SLOPE. I.e, positive X_SLOPE takes precedence always.
		SlopeDim dimPrefArray[3];
		dimPrefArray[0] = SlopeDim::X_SLOPE;
		dimPrefArray[1] = SlopeDim::Y_SLOPE;
		dimPrefArray[2] = SlopeDim::Z_SLOPE;

		SlopeDim abMatchedPositive = normalizedABData.getOnlyPositiveDim();
		int abMatchRun = 0;
		for (int x = 0; x < 3; x++)
		{
			if (abMatchedPositive == dimPrefArray[x])
			{
				abMatchRun = x;
				break;
			}
		}

		SlopeDim baMatchedPositive = normalizedBAData.getOnlyPositiveDim();
		int baMatchRun = 0;
		for (int x = 0; x < 3; x++)
		{
			if (baMatchedPositive == dimPrefArray[x])
			{
				baMatchRun = x;
				break;
			}
		}

		// if abMatchRun is < baMatchRun, we don't have to orient.
		// That is because, abMatchRun would match the priority before baMatchRun does, which means that A>B is already oriented.
		if (abMatchRun < baMatchRun)
		{
			isLineOriented = true;
		}
	}

	else if 
	(
		(posCountAB == 1)
		&&
		(posCountBA < 1)
	)
	{
		isLineOriented = true;
	}

	return isLineOriented;
}