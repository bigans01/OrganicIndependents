#include "stdafx.h"
#include "PolyUtils.h"

void PolyUtils::determineLineInterceptSlopes(OrganicLineBase* in_LinePtr, ECBPolyPoint in_thirdPoint)
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
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
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
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
	}
	else if // ##################################################### thirdPoint's x value is before point A
		(
		((thirdPoint_x < line_pointA_x) && currentSlopeNormalized.x == 1)		// slope is positive, but third point's x would have to be less than point A's x
			||
			((thirdPoint_x > line_pointA_x) && currentSlopeNormalized.x == -1)		// slope is negative, but third point's x would have to be greater than point A's x
			)
	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value

	}
	else if (thirdPoint_x == line_pointA_x)		// ################### x of third point is equal to first line point's x
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_x == line_pointB_x)		// ##################x of third point is equal to second line point's x
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_x == line_pointB_x)
	{
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
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
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
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
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;

	}
	else if // ##################################################### thirdPoint's y value is before point A
		(
		((thirdPoint_y < line_pointA_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y > line_pointA_y) && currentSlopeNormalized.y == -1)
			)
	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
	}
	else if (thirdPoint_y == line_pointA_y)		// ################### y of third point is equal to first line point's y
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_y == line_pointB_y)		// ################## y of third point is equal to second line point's y
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_y == line_pointB_y)
	{
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
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
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
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
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}
	else if // ##################################################### thirdPoint's z value is before point A
		(
		((thirdPoint_z < line_pointA_z) && currentSlopeNormalized.z == 1)
			||
			((thirdPoint_z > line_pointA_z) && currentSlopeNormalized.z == -1)
			)
	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.z - in_LinePtr->pointB.z);
		float original_line_length = abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}

	else if (thirdPoint_z == line_pointA_z)		// ################### z of third point is equal to first line point's z
	{
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (thirdPoint_z == line_pointB_z)		// ################## z of third point is equal to second line point's z
	{
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if (line_pointA_z == line_pointB_z)
	{
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
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

bool PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(TriangleLine* in_LinePtr, ECBPolyPoint in_thirdPoint, ECBPolyPoint in_point0Original, ECBPolyPoint in_point1Original, ECBPolyPoint in_point2Original)
{
	// X axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_x = in_LinePtr->pointA.x;		// get the x values of each point
	float line_pointB_x = in_LinePtr->pointB.x;
	float thirdPoint_x = in_thirdPoint.x;
	bool isValid = true;		// assumes true. can be false, however.
	//std::cout << "|||||||||||| THIRD POINT ||||||||||||||: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;


	ECBPolyPoint currentSlope;						// get the current slope (this may be temporarily switched later, until out of X logic)
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	ECBPolyPoint currentSlopeNormalized;
	currentSlopeNormalized.x = (in_LinePtr->pointB.x - in_LinePtr->pointA.x) / abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);
	currentSlopeNormalized.y = (in_LinePtr->pointB.y - in_LinePtr->pointA.y) / abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
	currentSlopeNormalized.z = (in_LinePtr->pointB.z - in_LinePtr->pointA.z) / abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);

	//std::cout << "-------------float test----------" << std::endl;
	//std::cout << "Value 1: " << -83.0f / -16.0645f << std::endl;
	//std::cout << "Value 2: " << 16.0833f / 3.1129f << std::endl;

	//std::cout << ">>>>>>>>>>>>>>>>> calling determineLineAxisIntercept" << std::endl;
	if
		(
		((thirdPoint_x > line_pointA_x) && (thirdPoint_x < line_pointB_x))
			||
			((thirdPoint_x < line_pointA_x) && (thirdPoint_x > line_pointB_x))
			)	// the third point's x lies between the line's point A and B x
	{
		//std::cout << "X is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_x - line_pointA_x);		// distance on the line that the new point will have to travel
		float newPointSlope = newPointTravelDistance / currentSlope.x;		// get the new slope calculation for this point's x (y, z?)
		ECBPolyPoint newPoint;												// value that stores the new point coordinates
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);

		//std::cout << "current normalized slope: " << currentSlopeNormalized.x << ", " << currentSlopeNormalized.y << ", " << currentSlopeNormalized.z << std::endl;
		//std::cout << "slope multiply test: " << currentSlope.y * newPointSlope << std::endl;
		//std::cout << "----First point coordinates----: " << in_LinePtr->pointA.x << ",  " << in_LinePtr->pointA.y << ", " << in_LinePtr->pointA.z << std::endl;
		//std::cout << "----Second point coordinates----: " << in_LinePtr->pointB.x << ",  " << in_LinePtr->pointB.y << ", " << in_LinePtr->pointB.z << std::endl;
		//std::cout << "----Third point coordinates----: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		//std::cout << "----New point coordinates----  : " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;

		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//temp_x_interceptSlope.x = in_thirdPoint.x - newPoint.x;
		//temp_x_interceptSlope.y = in_thirdPoint.y - newPoint.y;
		//temp_x_interceptSlope.z = in_thirdPoint.z - newPoint.z;
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;
	}
	else if
		(
		((thirdPoint_x > line_pointB_x) && currentSlopeNormalized.x == 1)		// slope is positive; third point's x would have to be greater than point B's x
			||
			((thirdPoint_x < line_pointB_x) && currentSlopeNormalized.x == -1)		// slope is negative; third point's x would have to be less than point B's x
			)	// thirdPoint's x value is further past point B 
	{
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		//std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
		if (currentSlopeNormalized.x == 1)
		{
			//std::cout << "X slope is positive, but third point's X is greater than point B's X" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
		else if (currentSlopeNormalized.x == -1)
		{
			//std::cout << "X slope is negative, but third point's X is less than point B's x" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);	// distance from point A to the new point (x)
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
	}
	else if
		(
		((thirdPoint_x < line_pointA_x) && currentSlopeNormalized.x == 1)		// slope is positive, but third point's x would have to be less than point A's x
			||
			((thirdPoint_x > line_pointA_x) && currentSlopeNormalized.x == -1)		// slope is negative, but third point's x would have to be greater than point A's x
			)

	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		//std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
		if (currentSlopeNormalized.x == 1)
		{
			//std::cout << "X slope is positive, but third point's X is less than than point A's X" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/

		}
		else if (currentSlopeNormalized.x == -1)
		{
			//std::cout << "X slope is negative, but third point's X is greater than point A's x" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
	}
	else if
		(
		(thirdPoint_x == line_pointA_x)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "x of third point is equal to the x of the line's point A!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_x == line_pointB_x)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "x of third point is equal to the x of the line's point B!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_x == line_pointB_x)
			)
	{
		//std::cout << "point A and point B x are equal!" << std::endl;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}




	// Y axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_y = in_LinePtr->pointA.y;
	float line_pointB_y = in_LinePtr->pointB.y;
	float thirdPoint_y = in_thirdPoint.y;

	// reset currentSlope for y
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if (
		((thirdPoint_y > line_pointA_y) && (thirdPoint_y < line_pointB_y))
		||
		((thirdPoint_y < line_pointA_y) && (thirdPoint_y > line_pointB_y))
		)	// the third point's y lies between the line's point A and B y
	{
		//std::cout << "Y is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_y - line_pointA_y);
		float newPointSlope = newPointTravelDistance / currentSlope.y;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
	}
	else if
		(
		((thirdPoint_y > line_pointB_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y < line_pointB_y) && currentSlopeNormalized.y == -1)
			)
	{
		//std::cout << ">>>>>>>>>>>>>> point A z is: (post y axis): (ELSE 1)" << in_LinePtr->pointA.z << std::endl;
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
		if (currentSlopeNormalized.y == 1)
		{
			//std::cout << "Y slope is positive, but third point's Y is greater than point B's Y" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/

		}
		else if (currentSlopeNormalized.y == -1)
		{
			//std::cout << "Y slope is negative, but third point's Y is less than point B's Y" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}
	}
	else if
		(
		((thirdPoint_y < line_pointA_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y > line_pointA_y) && currentSlopeNormalized.y == -1)
			)
	{
		//std::cout << ">>>>>>>>>>>>>> point A z is: (post y axis): (ELSE 2)" << in_LinePtr->pointA.z << std::endl;
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
		if (currentSlopeNormalized.y == 1)
		{
			//std::cout << "Y slope is positive, but third point's Y is less than than point A's Y" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}
		else if (currentSlopeNormalized.y == -1)
		{
			//std::cout << "Y slope is negative, but third point's Y is greater than point A's Y" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}


	}

	else if
		(
		(thirdPoint_y == line_pointA_y)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "y of third point is equal to the y of the line's point A!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_y == line_pointB_y)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "y of third point is equal to the y of the line's point B!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_y == line_pointB_y)
			)
	{
		//std::cout << "point A and point B y are equal!" << std::endl;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}


	// Z axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_z = in_LinePtr->pointA.z;
	float line_pointB_z = in_LinePtr->pointB.z;
	float thirdPoint_z = in_thirdPoint.z;

	// reset currentSlope for z
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if
		(
		((thirdPoint_z > line_pointA_z) && (thirdPoint_z < line_pointB_z))
			||
			((thirdPoint_z < line_pointA_z) && (thirdPoint_z > line_pointB_z))
			)
	{
		//std::cout << "Z is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_z - line_pointA_z);
		float newPointSlope = newPointTravelDistance / currentSlope.z;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}
	else if
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
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
		if (currentSlopeNormalized.z == 1)
		{
			//std::cout << "Z slope is positive, but third point's Z is greater than point B's Z" << std::endl;
		}
		else if (currentSlopeNormalized.z == -1)
		{
			//std::cout << "Z slope is negative, but third point's Z is less than point B's Z" << std::endl;
		}
	}
	else if
		(
		((thirdPoint_z < line_pointA_z) && currentSlopeNormalized.z == 1)
			||
			((thirdPoint_z > line_pointA_z) && currentSlopeNormalized.z == -1)
			)
	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.z - in_LinePtr->pointB.z);
		float original_line_length = abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);
		newPoint.z = thirdPoint_z;
		//std::cout << "Z CATCHER: " << newPoint.z << std::endl;
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
		if (currentSlopeNormalized.z == 1)
		{
			//std::cout << "Z slope is positive, but third point's Z is less than than point A's Z" << std::endl;
		}
		else if (currentSlopeNormalized.z == -1)
		{
			//std::cout << "Z slope is negative, but third point's Z is greater than point A's Z" << std::endl;
		}
	}

	else if
		(
		(thirdPoint_z == line_pointA_z)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "z of third point is equal to the z of the line's point A!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_z == line_pointB_z)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "z of third point is equal to the z of the line's point B!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_z == line_pointB_z)
			)
	{
		//std::cout << "point A and point B z are equal!" << std::endl;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}


	//std::cout << "intercept Slopes for this line: " << std::endl;
	//std::cout << "x-slope: " << in_LinePtr->x_interceptSlope.x << ", " << in_LinePtr->x_interceptSlope.y << ", " << in_LinePtr->x_interceptSlope.z << ", " << std::endl;
	//std::cout << "y-slope: " << in_LinePtr->y_interceptSlope.x << ", " << in_LinePtr->y_interceptSlope.y << ", " << in_LinePtr->y_interceptSlope.z << ", " << std::endl;
	//std::cout << "z-slope: " << in_LinePtr->z_interceptSlope.x << ", " << in_LinePtr->z_interceptSlope.y << ", " << in_LinePtr->z_interceptSlope.z << ", " << std::endl;

	if
		(
		((in_LinePtr->x_interceptSlope.x == 0) && (in_LinePtr->x_interceptSlope.y == 0) && (in_LinePtr->x_interceptSlope.z == 0))
			||
			((in_LinePtr->y_interceptSlope.x == 0) && (in_LinePtr->y_interceptSlope.y == 0) && (in_LinePtr->y_interceptSlope.z == 0))
			||
			((in_LinePtr->z_interceptSlope.x == 0) && (in_LinePtr->z_interceptSlope.y == 0) && (in_LinePtr->z_interceptSlope.z == 0))
			)
	{
		//std::cout << "!!!! WARNING: Missing Slopes...halting. " << std::endl;
		//std::cout << "|||||||||||| THIRD POINT ||||||||||||||: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		//std::cout << ">>> Original points were: " << std::endl;
		//std::cout << "0: " << in_point0Original.x << ", " << in_point0Original.y << ", " << in_point0Original.z << std::endl;
		//std::cout << "1: " << in_point1Original.x << ", " << in_point1Original.y << ", " << in_point1Original.z << std::endl;
		//std::cout << "1: " << in_point2Original.x << ", " << in_point2Original.y << ", " << in_point2Original.z << std::endl;
		//std::cout << ">>> SERVER INTERCEPT DEBUG -> Points being compared are: " << std::endl;
		//std::cout << "0: " << in_LinePtr->pointA.x << ", " << in_LinePtr->pointA.y << ", " << in_LinePtr->pointA.z << std::endl;
		//std::cout << "1: " << in_LinePtr->pointB.x << ", " << in_LinePtr->pointB.y << ", " << in_LinePtr->pointB.z << std::endl;
		//std::cout << "2: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		//isValid = false;

		//std::cout << "Point check: " << std::endl;
		if
			(
			(
				(in_LinePtr->pointA.x == in_LinePtr->pointB.x)
				&&
				(in_LinePtr->pointA.y == in_LinePtr->pointB.y)
				&&
				(in_LinePtr->pointA.z == in_LinePtr->pointB.z)
				)

				||

				(
				(in_LinePtr->pointA.x == in_thirdPoint.x)
					&&
					(in_LinePtr->pointA.y == in_thirdPoint.y)
					&&
					(in_LinePtr->pointA.z == in_thirdPoint.z)
					)

				||

				(
				(in_LinePtr->pointB.x == in_thirdPoint.x)
					&&
					(in_LinePtr->pointB.y == in_thirdPoint.y)
					&&
					(in_LinePtr->pointB.z == in_thirdPoint.z)
					)
				)
		{
			int someVal = 3;
			isValid = false;
			//std::cout << "Flag triggered!! " << std::endl;
		}


		//int someVal;
		//std::cin >> someVal;
	}

	return isValid;
}

void PolyUtils::determineLineInterceptSlopesDebug(TriangleLine* in_LinePtr, ECBPolyPoint in_thirdPoint, int in_lineID)
{
	// X axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_x = in_LinePtr->pointA.x;		// get the x values of each point
	float line_pointB_x = in_LinePtr->pointB.x;
	float thirdPoint_x = in_thirdPoint.x;

	//std::cout << "|||||||||||| THIRD POINT ||||||||||||||: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
	std::cout << ">>> SERVER INTERCEPT DEBUG -> Points being compared are: " << std::endl;
	std::cout << "0: " << in_LinePtr->pointA.x << ", " << in_LinePtr->pointA.y << ", " << in_LinePtr->pointA.z << std::endl;
	std::cout << "1: " << in_LinePtr->pointB.x << ", " << in_LinePtr->pointB.y << ", " << in_LinePtr->pointB.z << std::endl;
	std::cout << "2: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;

	ECBPolyPoint currentSlope;						// get the current slope (this may be temporarily switched later, until out of X logic)
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	ECBPolyPoint currentSlopeNormalized;
	currentSlopeNormalized.x = (in_LinePtr->pointB.x - in_LinePtr->pointA.x) / abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);
	currentSlopeNormalized.y = (in_LinePtr->pointB.y - in_LinePtr->pointA.y) / abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
	currentSlopeNormalized.z = (in_LinePtr->pointB.z - in_LinePtr->pointA.z) / abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
	//std::cout << "Current slope normalized is: " << currentSlopeNormalized.x << ",  " << currentSlopeNormalized.y << ",  " << currentSlopeNormalized.z << ",  " << std::endl;
	if ((in_LinePtr->pointB.y - in_LinePtr->pointA.y) == 0)
	{
		std::cout << "Y was 0! " << std::endl;
	}
	//std::cout << "-------------float test----------" << std::endl;
	//std::cout << "Value 1: " << -83.0f / -16.0645f << std::endl;
	//std::cout << "Value 2: " << 16.0833f / 3.1129f << std::endl;

	//std::cout << ">>>>>>>>>>>>>>>>> calling determineLineAxisIntercept" << std::endl;
	if
		(
		((thirdPoint_x > line_pointA_x) && (thirdPoint_x < line_pointB_x))
			||
			((thirdPoint_x < line_pointA_x) && (thirdPoint_x > line_pointB_x))
			)	// the third point's x lies between the line's point A and B x
	{
		//std::cout << "X is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_x - line_pointA_x);		// distance on the line that the new point will have to travel
		float newPointSlope = newPointTravelDistance / currentSlope.x;		// get the new slope calculation for this point's x (y, z?)
		ECBPolyPoint newPoint;												// value that stores the new point coordinates
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);

		//std::cout << "current normalized slope: " << currentSlopeNormalized.x << ", " << currentSlopeNormalized.y << ", " << currentSlopeNormalized.z << std::endl;
		//std::cout << "slope multiply test: " << currentSlope.y * newPointSlope << std::endl;
		//std::cout << "----First point coordinates----: " << in_LinePtr->pointA.x << ",  " << in_LinePtr->pointA.y << ", " << in_LinePtr->pointA.z << std::endl;
		//std::cout << "----Second point coordinates----: " << in_LinePtr->pointB.x << ",  " << in_LinePtr->pointB.y << ", " << in_LinePtr->pointB.z << std::endl;
		//std::cout << "----Third point coordinates----: " << in_thirdPoint.x << ", " << in_thirdPoint.y << ", " << in_thirdPoint.z << std::endl;
		//std::cout << "----New point coordinates----  : " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;

		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//temp_x_interceptSlope.x = in_thirdPoint.x - newPoint.x;
		//temp_x_interceptSlope.y = in_thirdPoint.y - newPoint.y;
		//temp_x_interceptSlope.z = in_thirdPoint.z - newPoint.z;
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;
	}
	else if
		(
		((thirdPoint_x > line_pointB_x) && currentSlopeNormalized.x == 1)		// slope is positive; third point's x would have to be greater than point B's x
			||
			((thirdPoint_x < line_pointB_x) && currentSlopeNormalized.x == -1)		// slope is negative; third point's x would have to be less than point B's x
			)	// thirdPoint's x value is further past point B 
	{
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		//std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
		if (currentSlopeNormalized.x == 1)
		{
			//std::cout << "X slope is positive, but third point's X is greater than point B's X" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
		else if (currentSlopeNormalized.x == -1)
		{
			//std::cout << "X slope is negative, but third point's X is less than point B's x" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointA.x);	// distance from point A to the new point (x)
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// distance from point A to point B (x)
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * slope_multiplier);
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
	}
	else if
		(
		((thirdPoint_x < line_pointA_x) && currentSlopeNormalized.x == 1)		// slope is positive, but third point's x would have to be less than point A's x
			||
			((thirdPoint_x > line_pointA_x) && currentSlopeNormalized.x == -1)		// slope is negative, but third point's x would have to be greater than point A's x
			)

	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
		float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = thirdPoint_x;
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		//std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
		ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		//std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
		in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
		if (currentSlopeNormalized.x == 1)
		{
			//std::cout << "X slope is positive, but third point's X is less than than point A's X" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/

		}
		else if (currentSlopeNormalized.x == -1)
		{
			//std::cout << "X slope is negative, but third point's X is greater than point A's x" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);	// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.x - in_LinePtr->pointB.x);	// needs to be reversed; distance is to new point now comes as the distance between point B and the new point's x
			float original_line_length = abs(in_LinePtr->pointB.x - in_LinePtr->pointA.x);	// this doesn't need to be reversed since it's always an absolute value
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = thirdPoint_x;
			newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);		// point A must be swapped with point B
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			std::cout << "New point is: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
			ECBPolyPoint temp_x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			std::cout << "X-intercept slope is: " << temp_x_interceptSlope.x << ", " << temp_x_interceptSlope.y << ", " << temp_x_interceptSlope.z << std::endl;
			in_LinePtr->x_interceptSlope = temp_x_interceptSlope;		// store the result in the line's x-intercept slope value
			*/
		}
	}
	else if
		(
		(thirdPoint_x == line_pointA_x)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "x of third point is equal to the x of the line's point A!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_x == line_pointB_x)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "x of third point is equal to the x of the line's point B!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_x == line_pointB_x)
			)
	{
		//std::cout << "point A and point B x are equal!" << std::endl;
		in_LinePtr->x_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}




	// Y axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_y = in_LinePtr->pointA.y;
	float line_pointB_y = in_LinePtr->pointB.y;
	float thirdPoint_y = in_thirdPoint.y;

	// reset currentSlope for y
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if (
		((thirdPoint_y > line_pointA_y) && (thirdPoint_y < line_pointB_y))
		||
		((thirdPoint_y < line_pointA_y) && (thirdPoint_y > line_pointB_y))
		)	// the third point's y lies between the line's point A and B y
	{
		//std::cout << "Y is between A and B points..." << std::endl;
		std::cout << "Y--> Case 1 hit. " << std::endl;
		float newPointTravelDistance = (thirdPoint_y - line_pointA_y);
		float newPointSlope = newPointTravelDistance / currentSlope.y;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * newPointSlope);
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
	}
	else if
		(
		((thirdPoint_y > line_pointB_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y < line_pointB_y) && currentSlopeNormalized.y == -1)
			)
	{
		//std::cout << ">>>>>>>>>>>>>> point A z is: (post y axis): (ELSE 1)" << in_LinePtr->pointA.z << std::endl;
		std::cout << "Y--> Case 2 hit. " << std::endl;
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
		if (currentSlopeNormalized.y == 1)
		{
			//std::cout << "Y slope is positive, but third point's Y is greater than point B's Y" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/

		}
		else if (currentSlopeNormalized.y == -1)
		{
			//std::cout << "Y slope is negative, but third point's Y is less than point B's Y" << std::endl;
			/*
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointA.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * slope_multiplier);
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointA.z + (currentSlope.z * slope_multiplier);
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}
	}
	else if
		(
		((thirdPoint_y < line_pointA_y) && currentSlopeNormalized.y == 1)
			||
			((thirdPoint_y > line_pointA_y) && currentSlopeNormalized.y == -1)
			)
	{
		//std::cout << ">>>>>>>>>>>>>> point A z is: (post y axis): (ELSE 2)" << in_LinePtr->pointA.z << std::endl;
		std::cout << "Y--> Case 3 hit. " << std::endl;
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
		float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
		newPoint.y = thirdPoint_y;
		newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
		ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
		if (currentSlopeNormalized.y == 1)
		{
			//std::cout << "Y slope is positive, but third point's Y is less than than point A's Y" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}
		else if (currentSlopeNormalized.y == -1)
		{
			//std::cout << "Y slope is negative, but third point's Y is greater than point A's Y" << std::endl;
			/*
			currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
			float line_length_to_new_point = abs(in_thirdPoint.y - in_LinePtr->pointB.y);
			float original_line_length = abs(in_LinePtr->pointB.y - in_LinePtr->pointA.y);
			float slope_multiplier = line_length_to_new_point / original_line_length;
			ECBPolyPoint newPoint;
			newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);		// point A must be swapped with point B
			newPoint.y = thirdPoint_y;
			newPoint.z = in_LinePtr->pointB.z + (currentSlope.z * slope_multiplier);		// ""
			ECBPolyPoint temp_y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
			in_LinePtr->y_interceptSlope = temp_y_interceptSlope;
			*/
		}


	}

	else if
		(
		(thirdPoint_y == line_pointA_y)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "y of third point is equal to the y of the line's point A!" << std::endl;
		std::cout << "Y--> Case 4 hit. " << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_y == line_pointB_y)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "y of third point is equal to the y of the line's point B!" << std::endl;
		std::cout << "Y--> Case 5 hit. " << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_y == line_pointB_y)
			)
	{
		//std::cout << "point A and point B y are equal!" << std::endl;
		std::cout << "Y--> Case 6 hit. " << std::endl;
		in_LinePtr->y_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}


	// Z axis	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	float line_pointA_z = in_LinePtr->pointA.z;
	float line_pointB_z = in_LinePtr->pointB.z;
	float thirdPoint_z = in_thirdPoint.z;

	// reset currentSlope for z
	currentSlope.x = in_LinePtr->pointB.x - in_LinePtr->pointA.x;
	currentSlope.y = in_LinePtr->pointB.y - in_LinePtr->pointA.y;
	currentSlope.z = in_LinePtr->pointB.z - in_LinePtr->pointA.z;

	if
		(
		((thirdPoint_z > line_pointA_z) && (thirdPoint_z < line_pointB_z))
			||
			((thirdPoint_z < line_pointA_z) && (thirdPoint_z > line_pointB_z))
			)
	{
		//std::cout << "Z is between A and B points..." << std::endl;
		float newPointTravelDistance = (thirdPoint_z - line_pointA_z);
		float newPointSlope = newPointTravelDistance / currentSlope.z;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointA.x + (currentSlope.x * newPointSlope);
		newPoint.y = in_LinePtr->pointA.y + (currentSlope.y * newPointSlope);
		newPoint.z = thirdPoint_z;
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
	}
	else if
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
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
		if (currentSlopeNormalized.z == 1)
		{
			//std::cout << "Z slope is positive, but third point's Z is greater than point B's Z" << std::endl;
		}
		else if (currentSlopeNormalized.z == -1)
		{
			//std::cout << "Z slope is negative, but third point's Z is less than point B's Z" << std::endl;
		}
	}
	else if
		(
		((thirdPoint_z < line_pointA_z) && currentSlopeNormalized.z == 1)
			||
			((thirdPoint_z > line_pointA_z) && currentSlopeNormalized.z == -1)
			)
	{
		currentSlope = IndependentUtils::findSlope(in_LinePtr->pointB, in_LinePtr->pointA);		// reverse the slope
		float line_length_to_new_point = abs(in_thirdPoint.z - in_LinePtr->pointB.z);
		float original_line_length = abs(in_LinePtr->pointB.z - in_LinePtr->pointA.z);
		float slope_multiplier = line_length_to_new_point / original_line_length;
		ECBPolyPoint newPoint;
		newPoint.x = in_LinePtr->pointB.x + (currentSlope.x * slope_multiplier);
		newPoint.y = in_LinePtr->pointB.y + (currentSlope.y * slope_multiplier);
		newPoint.z = thirdPoint_z;
		//std::cout << "Z CATCHER: " << newPoint.z << std::endl;
		ECBPolyPoint temp_z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
		in_LinePtr->z_interceptSlope = temp_z_interceptSlope;
		if (currentSlopeNormalized.z == 1)
		{
			//std::cout << "Z slope is positive, but third point's Z is less than than point A's Z" << std::endl;
		}
		else if (currentSlopeNormalized.z == -1)
		{
			//std::cout << "Z slope is negative, but third point's Z is greater than point A's Z" << std::endl;
		}
	}

	else if
		(
		(thirdPoint_z == line_pointA_z)		// x of third point is equal to first line point's x
			)

	{
		//std::cout << "z of third point is equal to the z of the line's point A!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointA;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(thirdPoint_z == line_pointB_z)		// x of third point is equal to second line point's x
			)
	{
		//std::cout << "z of third point is equal to the z of the line's point B!" << std::endl;
		ECBPolyPoint newPoint = in_LinePtr->pointB;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(newPoint, in_thirdPoint);
	}
	else if
		(
		(line_pointA_z == line_pointB_z)
			)
	{
		//std::cout << "point A and point B z are equal!" << std::endl;
		in_LinePtr->z_interceptSlope = IndependentUtils::findSlope(in_LinePtr->pointA, in_LinePtr->pointB);
	}


	std::cout << "DEBUG: intercept Slopes for line " << in_lineID << std::endl;
	std::cout << "x-slope: " << in_LinePtr->x_interceptSlope.x << ", " << in_LinePtr->x_interceptSlope.y << ", " << in_LinePtr->x_interceptSlope.z << ", " << std::endl;
	std::cout << "y-slope: " << in_LinePtr->y_interceptSlope.x << ", " << in_LinePtr->y_interceptSlope.y << ", " << in_LinePtr->y_interceptSlope.z << ", " << std::endl;
	std::cout << "z-slope: " << in_LinePtr->z_interceptSlope.x << ", " << in_LinePtr->z_interceptSlope.y << ", " << in_LinePtr->z_interceptSlope.z << ", " << std::endl;
}

TriangleLine PolyUtils::convertT1toSimpleTriangleLine(PrimaryLineT1 in_T1Line)
{
	// although we can store more data in TriangleLine, let's just get points A and B.
	TriangleLine returnLine;
	returnLine.pointA = in_T1Line.beginPointRealXYZ;
	returnLine.pointB = in_T1Line.endPointRealXYZ;
	return returnLine;
}

TriangleLine PolyUtils::convertT2toSimpleTriangleLine(PrimaryLineT2 in_T2Line)
{
	TriangleLine returnLine;
	returnLine.pointA = in_T2Line.beginPointRealXYZ;
	returnLine.pointB = in_T2Line.endPointRealXYZ;
	return returnLine;
}

ECBPolyLine PolyUtils::convertToECBPolyLine(TriangleLine in_line, ECBPolyPoint in_thirdPoint)
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

int PolyUtils::convertBlockCoordsToSingle(int in_x, int in_y, int in_z)
{
	int x = in_x * 16;
	int y = in_y * 4;
	return x + y + in_z;
}

int PolyUtils::convertBlockCoordsToSingle(EnclaveKeyDef::EnclaveKey in_key)
{
	int x = in_key.x * 16;
	int y = in_key.y * 4;
	return x + y + in_key.z;
}

EnclaveKeyDef::EnclaveKey PolyUtils::convertSingleToBlockKey(int in_value)
{
	EnclaveKeyDef::EnclaveKey tempkey;

	int x = in_value / 16;
	int remainder_x = in_value % 16;

	int y = remainder_x / 4;
	int remainder_y = remainder_x % 4;

	int z = remainder_y;

	//cout << x << " " << y << " " << z << " " << endl;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;

	return tempkey;
}

ECBPoly PolyUtils::produceECBPolyFromECBPolySkeleton(ECBPolySkeleton in_skeleton)
{
	ECBPoly returnPoly(IndependentUtils::convertIntToPolyType(in_skeleton.polyType));	// set the poly type
	TriangleLine line1, line2, line3;
	line1.pointA = in_skeleton.points[0]; //in_point1;
	line1.pointB = in_skeleton.points[1]; //in_point2;

	line2.pointA = in_skeleton.points[1]; //in_point2;
	line2.pointB = in_skeleton.points[2]; //in_point3;

	line3.pointA = in_skeleton.points[2]; //in_point3;
	line3.pointB = in_skeleton.points[0]; //in_point1;

	// determine the slopes for each line
	PolyUtils::determineLineInterceptSlopes(&line1, in_skeleton.points[2]);	// in_point3
	PolyUtils::determineLineInterceptSlopes(&line2, in_skeleton.points[0]);	// in_point1
	PolyUtils::determineLineInterceptSlopes(&line3, in_skeleton.points[1]);	// in_point2

	// add each line to the poly
	returnPoly.lineMap[0] = PolyUtils::convertToECBPolyLine(line1, in_skeleton.points[2]);		// line consists of points 1 and 2, so third = point 3
	returnPoly.lineMap[1] = PolyUtils::convertToECBPolyLine(line2, in_skeleton.points[0]);		// line consists of points 2 and 3, so third = point 1
	returnPoly.lineMap[2] = PolyUtils::convertToECBPolyLine(line3, in_skeleton.points[1]);		// line consists of points 3 and 1, so third = point 2

	// construct the poly's lines

	returnPoly.materialID = in_skeleton.materialID;
	returnPoly.isPolyPerfectlyClamped = in_skeleton.isPolyPerfectlyClamped;
	returnPoly.mrp = in_skeleton.mrp;
	returnPoly.emptyNormal = in_skeleton.emptyNormal;
	return returnPoly;
}

ECBPoly PolyUtils::produceTranslatedECBPoly(ECBPoly in_originalPoly, EnclaveKeyDef::EnclaveKey in_originalBlueprintKey)
{
	ECBPoly returnPoly;

	// acquire the old poly's points
	ECBPolyPoint original0, original1, original2;
	original0 = in_originalPoly.lineMap[0].pointA;
	original1 = in_originalPoly.lineMap[1].pointA;
	original2 = in_originalPoly.lineMap[2].pointA;

	// created translated points
	ECBPolyPoint translated0, translated1, translated2, translatedMRP;

	// translate point 0
	translated0 = original0;
	translated0.x = original0.x - (in_originalBlueprintKey.x * 32);
	translated0.y = original0.y - (in_originalBlueprintKey.y * 32);
	translated0.z = original0.z - (in_originalBlueprintKey.z * 32);

	// translated point 1
	translated1 = original1;
	translated1.x = original1.x - (in_originalBlueprintKey.x * 32);
	translated1.y = original1.y - (in_originalBlueprintKey.y * 32);
	translated1.z = original1.z - (in_originalBlueprintKey.z * 32);

	// translated point 2
	translated2 = original2;
	translated2.x = original2.x - (in_originalBlueprintKey.x * 32);
	translated2.y = original2.y - (in_originalBlueprintKey.y * 32);
	translated2.z = original2.z - (in_originalBlueprintKey.z * 32);

	// translatedMRP
	translatedMRP = in_originalPoly.mrp;
	translatedMRP.x = translatedMRP.x - (in_originalBlueprintKey.x * 32);
	translatedMRP.y = translatedMRP.y - (in_originalBlueprintKey.y * 32);
	translatedMRP.z = translatedMRP.z - (in_originalBlueprintKey.z * 32);

	// new lines
	TriangleLine line1, line2, line3;
	line1.pointA = translated0;
	line1.pointB = translated1;

	line2.pointA = translated1;
	line2.pointB = translated2;

	line3.pointA = translated2;
	line3.pointB = translated0;

	// determine the slopes for each line
	PolyUtils::determineLineInterceptSlopes(&line1, translated2);	// in_point3
	PolyUtils::determineLineInterceptSlopes(&line2, translated0);	// in_point1
	PolyUtils::determineLineInterceptSlopes(&line3, translated1);	// in_point2

	// add each line to the poly
	returnPoly.lineMap[0] = PolyUtils::convertToECBPolyLine(line1, translated2);
	returnPoly.lineMap[1] = PolyUtils::convertToECBPolyLine(line2, translated0);
	returnPoly.lineMap[2] = PolyUtils::convertToECBPolyLine(line3, translated1);

	// construct the poly's metadata
	returnPoly.materialID = in_originalPoly.materialID;
	returnPoly.isPolyPerfectlyClamped = in_originalPoly.isPolyPerfectlyClamped;
	returnPoly.mrp = translatedMRP;
	returnPoly.emptyNormal = in_originalPoly.emptyNormal;
	return returnPoly;
}