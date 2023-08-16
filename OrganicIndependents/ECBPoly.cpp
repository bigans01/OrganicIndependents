#include "stdafx.h"
#include "ECBPoly.h"

ECBPoly::ECBPoly(FTriangleOutput in_completeTriangleOutput)
{
	TriangleLine line1, line2, line3;

	ECBPolyPoint convertPoint0 = convertDoublePointToEcBPolyPoint(in_completeTriangleOutput.fracturePoints[0]);
	ECBPolyPoint convertPoint1 = convertDoublePointToEcBPolyPoint(in_completeTriangleOutput.fracturePoints[1]);
	ECBPolyPoint convertPoint2 = convertDoublePointToEcBPolyPoint(in_completeTriangleOutput.fracturePoints[2]);

	line1.pointA = convertPoint0;
	line1.pointB = convertPoint1;

	line2.pointA = convertPoint1;
	line2.pointB = convertPoint2;

	line3.pointA = convertPoint2;
	line3.pointB = convertPoint0;

	// determine the slopes for each line
	determineLineInterceptSlopes(&line1, convertPoint2);
	determineLineInterceptSlopes(&line2, convertPoint0);
	determineLineInterceptSlopes(&line3, convertPoint1);

	// UPDATE 4
	ecbPolyPoints[0] = convertPoint0;		
	ecbPolyPoints[1] = convertPoint1;		
	ecbPolyPoints[2] = convertPoint2;		

	materialID = in_completeTriangleOutput.outputTriangleMaterial;

	isPolyPerfectlyClamped = in_completeTriangleOutput.fractureRequiredClampValue;

	emptyNormal = in_completeTriangleOutput.fractureEmptyNormal;

	polyBoundaryOrientation = in_completeTriangleOutput.fractureRequiredOrientation;
}

void ECBPoly::printLineData()
{
	std::cout << "(ECBPoly): printing point data..." << std::endl;
	for (int x = 0; x < 3; x++)
	{
		// UPDATE 5
		std::cout << "point " << x << ": ";
		ecbPolyPoints[x].printPointCoords();
		std::cout << std::endl;
	}
}

Message ECBPoly::produceBDMFormatECBPolyMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey, int in_mappedPolyKey)
{
	Message ecbPolyData(MessageType::BDM_BLUEPRINT_ECBPOLY);

	ecbPolyData.insertEnclaveKey(in_blueprintKey);
	ecbPolyData.insertInt(in_mappedPolyKey);
	for (int x = 0; x < 3; x++)
	{
		ecbPolyData.insertPoint(ecbPolyPoints[x]);
	}
	ecbPolyData.insertInt(int(materialID));
	ecbPolyData.insertInt(int(isPolyPerfectlyClamped));
	ecbPolyData.insertPoint(emptyNormal);
	ecbPolyData.insertInt(int(polyBoundaryOrientation));

	return ecbPolyData;
}

ECBPolyLine ECBPoly::convertToECBPolyLine(TriangleLine in_line, ECBPolyPoint in_thirdPoint)
{
	//std::cout << "!!!! Calling convert " << std::endl;
	ECBPolyLine returnPolyLine;
	//returnPolyLine.pointA = in_line.pointA
	returnPolyLine.pointA = in_line.pointA;
	returnPolyLine.pointB = in_line.pointB;
	returnPolyLine.pointC = in_thirdPoint;
	returnPolyLine.x_interceptSlope = in_line.x_interceptSlope;
	returnPolyLine.y_interceptSlope = in_line.y_interceptSlope;
	returnPolyLine.z_interceptSlope = in_line.z_interceptSlope;
	return returnPolyLine;
}

ECBPolyPoint ECBPoly::convertDoublePointToEcBPolyPoint(DoublePoint in_doublePointToConvert)
{
	return ECBPolyPoint(float(in_doublePointToConvert.x),
		float(in_doublePointToConvert.y),
		float(in_doublePointToConvert.z));
}

ECBPolyPoint ECBPoly::findSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	ECBPolyPoint returnPoint;
	returnPoint.x = in_pointB.x - in_pointA.x;
	returnPoint.y = in_pointB.y - in_pointA.y;
	returnPoint.z = in_pointB.z - in_pointA.z;
	return returnPoint;
}

void ECBPoly::determineLineInterceptSlopes(OrganicLineBase* in_LinePtr, ECBPolyPoint in_thirdPoint)
{
	// Get the normalized slope values
	ECBPolyPoint currentSlopeNormalized;
	currentSlopeNormalized.x = (in_LinePtr->pointB.x - in_LinePtr->pointA.x) / abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);
	currentSlopeNormalized.y = (in_LinePtr->pointB.y - in_LinePtr->pointA.y) / abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
	currentSlopeNormalized.z = (in_LinePtr->pointB.z - in_LinePtr->pointA.z) / abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);



	// X axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_x = in_LinePtr->pointA.x;		// get the x values of each point
	float line_pointB_x = in_LinePtr->pointB.x;
	float thirdPoint_x = in_thirdPoint.x;

	ECBPolyPoint currentSlope;						// get the current slope (this may be temporarily switched later, until out of X logic)
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if  // ######################################################### the third point's x lies between the line's point A and B x
		(
		((thirdPoint_x > line_pointA_x) && (thirdPoint_x < line_pointB_x))
			||
			((thirdPoint_x < line_pointA_x) && (thirdPoint_x > line_pointB_x))
			)
	{
		//std::cout << "X is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_x - line_pointA_x);		// distance on the line that the new point will have to travel
		float newPointSlope = newPointTravelDistance / currentSlope.x;		// get the new slope calculation for this point's x (y, z?)
		ECBPolyPoint newPoint;												// value that stores the new point coordinates
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);
		ECBPolyPoint temp_x_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;
	}
	else if // ##################################################### thirdPoint's x value is further past point B 
		(
		((thirdPoint_x > line_pointB_x) && currentSlopeNormalized.x == 1)		// slope is positive; third point's x would have to be greater than point B's x
			||
			((thirdPoint_x < line_pointB_x) && currentSlopeNormalized.x == -1)		// slope is negative; third point's x would have to be less than point B's x
			)
	{
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		ECBPolyPoint temp_x_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
	}
	else if // ##################################################### thirdPoint's x value is before point A
		(
		((thirdPoint_x < line_pointA_x) && currentSlopeNormalized.x == 1)		// slope is positive, but third point's x would have to be less than point A's x
			||
			((thirdPoint_x > line_pointA_x) && currentSlopeNormalized.x == -1)		// slope is negative, but third point's x would have to be greater than point A's x
			)
	{
		currentSlope = findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_x_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value

	}
	else if (thirdPoint_x == line_pointA_x)		// ################### x of third point is equal to first line point's x
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->x_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_x == line_pointB_x)		// ##################x of third point is equal to second line point's x
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->x_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_x == line_pointB_x)
	{
		in_LinePtr->x_interceptSlope = findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}




	// Y axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_y = in_LinePtr->pointA.y;
	float line_pointB_y = in_LinePtr->pointB.y;
	float thirdPoint_y = in_thirdPoint.y;

	// reset currentSlope for y
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if   // ######################################################### the third point's y lies between the line's point A and B x
		(
		((thirdPoint_y > line_pointA_y) && (thirdPoint_y < line_pointB_y))
			||
			((thirdPoint_y < line_pointA_y) && (thirdPoint_y > line_pointB_y))
			)
	{
		float newPointTravelDistance = (thirdPoint_y - line_pointA_y);
		float newPointSlope = newPointTravelDistance / currentSlope.y;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);
		ECBPolyPoint temp_y_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
	}
	else if // ##################################################### thirdPoint's y value is further past point B 
		(
		((thirdPoint_y > line_pointB_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y < line_pointB_y) && currentSlopeNormalized.y == -1)
			)
	{
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		ECBPolyPoint temp_y_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;

	}
	else if // ##################################################### thirdPoint's y value is before point A
		(
		((thirdPoint_y < line_pointA_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y > line_pointA_y) && currentSlopeNormalized.y == -1)
			)
	{
		currentSlope = findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_y_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
	}
	else if (thirdPoint_y == line_pointA_y)		// ################### y of third point is equal to first line point's y
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->y_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_y == line_pointB_y)		// ################## y of third point is equal to second line point's y
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->y_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_y == line_pointB_y)
	{
		in_LinePtr->y_interceptSlope = findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}


	// Z axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_z = in_LinePtr->pointA.z;
	float line_pointB_z = in_LinePtr->pointB.z;
	float thirdPoint_z = in_thirdPoint.z;

	// reset currentSlope for z
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if    // ######################################################### the third point's z lies between the line's point A and B z
		(
		((thirdPoint_z > line_pointA_z) && (thirdPoint_z < line_pointB_z))
			||
			((thirdPoint_z < line_pointA_z) && (thirdPoint_z > line_pointB_z))
			)
	{
		float newPointTravelDistance = (thirdPoint_z - line_pointA_z);
		float newPointSlope = newPointTravelDistance / currentSlope.z;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}
	else if // ##################################################### thirdPoint's z value is further past point B
		(
		((thirdPoint_z > line_pointB_z) && currentSlopeNormalized.z == 1)
			||
			((thirdPoint_z < line_pointB_z) && currentSlopeNormalized.z == -1)
			)
	{
		float line_length_to_new_point = abs(in_thirdPoint.z - in_LinePtr->pointA.z);
		float original_line_length = abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}
	else if // ##################################################### thirdPoint's z value is before point A
		(
		((thirdPoint_z < line_pointA_z) && currentSlopeNormalized.z == 1)
			||
			((thirdPoint_z > line_pointA_z) && currentSlopeNormalized.z == -1)
			)
	{
		currentSlope = findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.z - in_LinePtr->pointB.z);
		float original_line_length = abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}

	else if (thirdPoint_z == line_pointA_z)		// ################### z of third point is equal to first line point's z
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->z_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_z == line_pointB_z)		// ################## z of third point is equal to second line point's z
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->z_interceptSlope = findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_z == line_pointB_z)
	{
		in_LinePtr->z_interceptSlope = findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}



	if
		(
		((in_LinePtr->x_interceptSlope.x == 0) && (in_LinePtr->x_interceptSlope.y == 0) && (in_LinePtr->x_interceptSlope.z == 0))
			||
			((in_LinePtr->y_interceptSlope.x == 0) && (in_LinePtr->y_interceptSlope.y == 0) && (in_LinePtr->y_interceptSlope.z == 0))
			||
			((in_LinePtr->z_interceptSlope.x == 0) && (in_LinePtr->z_interceptSlope.y == 0) && (in_LinePtr->z_interceptSlope.z == 0))
			)
	{
		std::cout << "!!!! WARNING: Missing Slopes...halting. " << std::endl;
		//std::cout << "|||||||||||| THIRD POINT ||||||||||||||: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		std::cout << ">>> SERVER INTERCEPT DEBUG -> Points being compared are: " << std::endl;
		std::cout << "0: " << in_LinePtr->pointA.x << ", " << in_LinePtr->pointA.y << ", " << in_LinePtr->pointA.z << std::endl;
		std::cout << "1: " << in_LinePtr->pointB.x << ", " << in_LinePtr->pointB.y << ", " << in_LinePtr->pointB.z << std::endl;
		std::cout << "2: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		int someVal;
		std::cin >> someVal;
	}
}