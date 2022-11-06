#include "stdafx.h"
#include "EnclaveLineRunner.h"

EnclaveLineRunner::EnclaveLineRunner(PrimaryLineT1 in_link, BlockBorderLineList* in_lineListRef, int in_lineID)
{
	lineMeta = in_link;		// set the line meta
	lineID = in_lineID;		// set the line ID
	lineSlope.x = lineMeta.endPointRealXYZ.x - lineMeta.beginPointRealXYZ.x;	// setup the slope
	lineSlope.y = lineMeta.endPointRealXYZ.y - lineMeta.beginPointRealXYZ.y;
	lineSlope.z = lineMeta.endPointRealXYZ.z - lineMeta.beginPointRealXYZ.z;
	blockBordersRef = in_lineListRef;
	//std::cout << std::endl;

	//std::cout << "Starting new enclave line runner...(Line ID:" << in_lineID << ")" << std::endl;
	//std::cout << "Point A values: " << lineMeta.beginPointRealXYZ.x << ", " << lineMeta.beginPointRealXYZ.y << ", " << lineMeta.beginPointRealXYZ.z << std::endl;
	//std::cout << "Begin Point block and chunk keys: " << lineMeta.beginPointMeta.blockKey.x << ", " << lineMeta.beginPointMeta.blockKey.y << ", " << lineMeta.beginPointMeta.blockKey.z << " || " << lineMeta.beginPointMeta.enclaveKey.x << ", " << lineMeta.beginPointMeta.enclaveKey.y << ", " << lineMeta.beginPointMeta.enclaveKey.z << std::endl;
	//std::cout << "End Point block and chunk keys: " << lineMeta.endPointMeta.blockKey.x << ", " << lineMeta.endPointMeta.blockKey.y << ", " << lineMeta.endPointMeta.blockKey.z << " || " << lineMeta.endPointMeta.enclaveKey.x << ", " << lineMeta.endPointMeta.enclaveKey.y << ", " << lineMeta.endPointMeta.enclaveKey.z << std::endl;
	//std::cout << "Point B values: " << lineMeta.endPointRealXYZ.x << ", " << lineMeta.endPointRealXYZ.y << ", " << lineMeta.endPointRealXYZ.z << std::endl;
	//std::cout << "EnclaveLineRunner Slope: " << lineSlope.x << ", " << lineSlope.y << ", " << lineSlope.z << std::endl;

	currentIterationMeta = lineMeta.beginPointMeta;
	slopeDirection = IndependentUtils::findNormalizedSlope(lineMeta.beginPointRealXYZ, lineMeta.endPointRealXYZ);	// get the normalized slope
	//std::cout << "Normalized slope: " << slopeDirection.x << ", " << slopeDirection.y << ", " << slopeDirection.z << std::endl;

	// set debug flag here
	/*
	if
		(
		(currentIterationMeta.enclaveKey.x == 7)
			&&
			(currentIterationMeta.enclaveKey.y == 2)
			&&
			(currentIterationMeta.enclaveKey.z == 3)
			)
	{
		debugFlag = 1;
	}
	*/

	// test if points are within same block
	/*
	if (
		(lineMeta.beginPointMeta.enclaveKey.x == lineMeta.endPointMeta.enclaveKey.x)
		&&
		(lineMeta.beginPointMeta.enclaveKey.y == lineMeta.endPointMeta.enclaveKey.y)
		&&
		(lineMeta.beginPointMeta.enclaveKey.z == lineMeta.endPointMeta.enclaveKey.z)
		&&
		(lineMeta.beginPointMeta.blockKey.x == lineMeta.endPointMeta.blockKey.x)
		&&
		(lineMeta.beginPointMeta.blockKey.y == lineMeta.endPointMeta.blockKey.y)
		&&
		(lineMeta.beginPointMeta.blockKey.z == lineMeta.endPointMeta.blockKey.z)
		)
	*/
	if
		(
		(lineMeta.beginPointMeta.enclaveKey == lineMeta.endPointMeta.enclaveKey)
			&&
			(lineMeta.beginPointMeta.blockKey == lineMeta.endPointMeta.blockKey)
			)

	{
		isRunSingleIteration = 1;
		currentIterationBeginPoint = lineMeta.beginPointMeta.preciseCoord;	// first point will be the the beginning of the lineMeta
		currentIterationEndPoint = lineMeta.endPointMeta.preciseCoord;		// second point will be the point thats on the border
	}

	// if they are not in same block, do normal operations
	else
	{
		currentIterationBeginPoint = lineMeta.beginPointMeta.preciseCoord;	// first point will be the the beginning of the lineMeta
		//std::cout << "|||||||||||| For line: " << in_lineID;
		//std::cout << "|||||||||||| prior to getEndPointData in constructor " << std::endl;
		//std::cout << ">>>>> (Before altered) Initial begin point location: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
		//std::cout << ">>>>> (Before altered) Initial end point location: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
		currentEndpointMeta = getEndpointData();					// get the endpoint data
		currentIterationEndPoint = currentEndpointMeta.pointLocation;	// get the endpoint data from currentEndpointMeta
		//std::cout << ">>>>> Initial point location: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
		//std::cout << ">>>>> Post-alter end point location: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
		//std::cout << ">>>>> Initial move vals: " << currentEndpointMeta.moveDirectionValues.x << ", " << currentEndpointMeta.moveDirectionValues.y << ", " << currentEndpointMeta.moveDirectionValues.z << std::endl;
		//std::cout << ">>>>> Post-alter end point location: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
	}
}

void EnclaveLineRunner::iterateToNextBlock()
{
	//std::cout << "ITERATING..." << std::endl;
	if (isRunComplete == 0)	// do this ONLY if the run isn't complete
	{
		// first, get move values and use them to find appropriate new block
		moveIntoNextBlock();
		/*
		if
			(
			(currentIterationMeta.enclaveKey.x == 0)
				&&
				(currentIterationMeta.enclaveKey.y == 7)
				&&
				(currentIterationMeta.enclaveKey.z == 2)
				)
		{
			debugFlag = 1;
		}
		if (debugFlag == 1)
		{
			//std::cout << "!!EnclaveLineRunner:: Chunk key (" << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z << ") | Block key: ( " << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ") " << std::endl;
			//std::cout << ">> begin point chunk/block is: Chunk ( " << lineMeta.beginPointMeta.enclaveKey.x << ", " << lineMeta.beginPointMeta.enclaveKey.y << ", " << lineMeta.beginPointMeta.enclaveKey.z << ") block (" << lineMeta.beginPointMeta.blockKey.x << ", " << lineMeta.beginPointMeta.blockKey.y << ", " << lineMeta.beginPointMeta.blockKey.z << ") " << std::endl;
			//std::cout << ">> end point chunk/block is: Chunk ( " << lineMeta.endPointMeta.enclaveKey.x << ", " << lineMeta.endPointMeta.enclaveKey.y << ", " << lineMeta.endPointMeta.enclaveKey.z << ") | block (" << lineMeta.endPointMeta.blockKey.x << ", " << lineMeta.endPointMeta.blockKey.y << ", " << lineMeta.endPointMeta.blockKey.z << ") " << std::endl;
		}
		*/
		//std::cout << "Block key" << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ", " << std::endl;

			//std::cout << "-->current interation begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << "-->current interation end point: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
		//std::cout << "!!EnclaveLineRunner:: Chunk key (" << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z 
								   //  << ") | Block key: ( " << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ") " << 
									//    ") | Precise coord: ( " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
		if (currentIterationMeta.enclaveKey.z < 0)
		{
			//std::cout << "!!!! WARNING, activating safety loop for debug..." << std::endl;
			//std::cout << ">> begin point chunk/block is: Chunk ( " << lineMeta.beginPointMeta.enclaveKey.x << ", " << lineMeta.beginPointMeta.enclaveKey.y << ", " << lineMeta.beginPointMeta.enclaveKey.z
			//	<< ") | block (" << lineMeta.beginPointMeta.blockKey.x << ", " << lineMeta.beginPointMeta.blockKey.y << ", " << lineMeta.beginPointMeta.blockKey.z
			//	<< ") | point (" << lineMeta.beginPointMeta.preciseCoord.x << ", " << lineMeta.beginPointMeta.preciseCoord.y << "," << lineMeta.beginPointMeta.preciseCoord.z << std::endl;
		//	std::cout << ">> end point chunk/block is: Chunk ( " << lineMeta.endPointMeta.enclaveKey.x << ", " << lineMeta.endPointMeta.enclaveKey.y << ", " << lineMeta.endPointMeta.enclaveKey.z
			//	<< ") | block (" << lineMeta.endPointMeta.blockKey.x << ", " << lineMeta.endPointMeta.blockKey.y << ", " << lineMeta.endPointMeta.blockKey.z
			//	<< ") | point (" << lineMeta.endPointMeta.preciseCoord.x << ", " << lineMeta.endPointMeta.preciseCoord.y << "," << lineMeta.endPointMeta.preciseCoord.z << std::endl;
			// << ") " << std::endl;

			//std::cout << ">> Current iteration values: " << std::endl;
			//std::cout << "#### >>: Chunk key (" << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z << ") | Block key: ( " << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ") " << std::endl;
			int stupidval = 0;
			while (stupidval == 0)
			{

			}
		}
	}
	else if (isRunComplete == 1)
	{
		//std::cout << "Run complete, no further activity...." << std::endl;
	}

	//std::cout << "!!! >> begin point chunk/block is: Chunk ( " << lineMeta.beginPointMeta.enclaveKey.x << ", " << lineMeta.beginPointMeta.enclaveKey.y << ", " << lineMeta.beginPointMeta.enclaveKey.z << ") block (" << lineMeta.beginPointMeta.blockKey.x << ", " << lineMeta.beginPointMeta.blockKey.y << ", " << lineMeta.beginPointMeta.blockKey.z << ") " << std::endl;
	//std::cout << "!!! >> end point chunk/block is: Chunk ( " << lineMeta.endPointMeta.enclaveKey.x << ", " << lineMeta.endPointMeta.enclaveKey.y << ", " << lineMeta.endPointMeta.enclaveKey.z << ") | block (" << lineMeta.endPointMeta.blockKey.x << ", " << lineMeta.endPointMeta.blockKey.y << ", " << lineMeta.endPointMeta.blockKey.z << ") " << std::endl;
}

void EnclaveLineRunner::moveIntoNextBlock()
{
	currentBlock++;
	//std::cout << std::endl;
	//std::cout << "||||||||||| call of move into next block...." << std::endl;
	//std::cout << "++++++Before movement++++++" << std::endl;
	//std::cout << "-->current interation begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << "-->current interation end point: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
		//std::cout << "current iteration reset values: " << currentEndpointMeta.dimensionResetValues.x << ", " << currentEndpointMeta.dimensionResetValues.y << ", " << currentEndpointMeta.dimensionResetValues.z << ", " << std::endl;
		//std::cout << "Pre- iteration data: " << std::endl;
		//std::cout << std::endl;
		//std::cout << "Pre- iteration data: ||| Block key " << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ", " << "||||| Chunk Key " << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z << ", " << std::endl;
	//std::cout << "-->Block key" << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ", " << std::endl;
	//std::cout << "-->Chunk key" << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z << ", " << std::endl; 

	// first, get the new beginning point, by applying any "reset" values to the endpoint
	currentIterationBeginPoint = currentIterationEndPoint;		// copy the endpoint value into the begin point, before applying checks below

	//std::cout << "current iteration slope direction: " << slopeDirection.x << ", " << slopeDirection.y << ", " << slopeDirection.z << std::endl;
	if (currentEndpointMeta.dimensionResetValues.x != -1 && (slopeDirection.x != 0.0f))
	{
		currentIterationBeginPoint.x = currentEndpointMeta.dimensionResetValues.x;
		//std::cout << "x has a new reset!" << std::endl;
	}
	if (currentEndpointMeta.dimensionResetValues.y != -1 && (slopeDirection.y != 0.0f))
	{
		currentIterationBeginPoint.y = currentEndpointMeta.dimensionResetValues.y;
		//std::cout << "---------debug: " << std::endl;
		//std::cout << "y reset value: " << currentEndpointMeta.dimensionResetValues.y << std::endl;
		//std::cout << "move direction value: " << currentEndpointMeta.moveDirectionValues.y << std::endl;
		//std::cout << "y has a new reset!" << std::endl;
	}
	if (currentEndpointMeta.dimensionResetValues.z != -1 && (slopeDirection.z != 0.0f))
	{
		currentIterationBeginPoint.z = currentEndpointMeta.dimensionResetValues.z;
		//std::cout << "z has a new reset!" << std::endl;
	}
	//std::cout << "New value, post reset: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << ", " << std::endl;

	// second, "move" check for any dimension to be moved into the new block 

	//std::cout << std::endl;
	//std::cout << "move direction values: " << currentEndpointMeta.moveDirectionValues.x << ", " << currentEndpointMeta.moveDirectionValues.y << ", " << currentEndpointMeta.moveDirectionValues.z << std::endl;
	checkDimensionForMove(&currentEndpointMeta.moveDirectionValues.x, &currentIterationMeta.blockKey.x, &currentIterationMeta.enclaveKey.x);	// check for x
	checkDimensionForMove(&currentEndpointMeta.moveDirectionValues.y, &currentIterationMeta.blockKey.y, &currentIterationMeta.enclaveKey.y);	// ...for y
	checkDimensionForMove(&currentEndpointMeta.moveDirectionValues.z, &currentIterationMeta.blockKey.z, &currentIterationMeta.enclaveKey.z);	// ...for z


	currentEndpointMeta = getEndpointData();

	currentIterationEndPoint = currentEndpointMeta.pointLocation;	// get the endpoint data from currentEndpointMeta
	//std::cout << "current interation begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << "current interation end point: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
	//std::cout << "------------~~~~~~~~" << std::endl;
	//std::cout << "++++++++++++++move, begin point: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
	//std::cout << "++++++++++++++move, end point: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;
	//std::cout << "----->Move vals are: " << currentEndpointMeta.moveDirectionValues.x << ",  " << currentEndpointMeta.moveDirectionValues.y << ",  " << currentEndpointMeta.moveDirectionValues.z << ",  " << std::endl;
	//std::cout << std::endl;
	//std::cout << "+++ end of move vals out. " << std::endl;

	//std::cout << "Block key" << currentIterationMeta.blockKey.x << ", " << currentIterationMeta.blockKey.y << ", " << currentIterationMeta.blockKey.z << ", " << std::endl;
	//std::cout << "Chunk key" << currentIterationMeta.enclaveKey.x << ", " << currentIterationMeta.enclaveKey.y << ", " << currentIterationMeta.enclaveKey.z << ", " << std::endl;

	//ECBPolyPoint sillyPoint(5.0f,5.0f,5.0f);
	//ECBPolyPoint sillyPoint;
	//debugPoint.doTheThing();

	/*
	if
	(
		(currentEndpointMeta.moveDirectionValues.x == 0)
		&&
		(currentEndpointMeta.moveDirectionValues.y == 0)
		&&
		(currentEndpointMeta.moveDirectionValues.z == 0)
	)
	*/
	if (currentEndpointMeta.moveDirectionValues.isAllZero())
	{
		std::cout << "!!!! WARNING, invalid Move val detected in EnclaveLineRunner::moveIntoNextBlock()...HALTING!!!; flagging as INVALID" << std::endl;
		//int haltval;
		//std::cin >> haltval;


		isRunComplete = 1;
		runState = LineRunnerState::INVALID;	// set it as INVALID
	}
	//std::cout << "current interation end point: " << currentIterationEndPoint.x << ", " << currentIterationEndPoint.y << ", " << currentIterationEndPoint.z << std::endl;

	checkEnclaveKeyValidity(currentIterationMeta.enclaveKey);
	if (currentIterationMeta.enclaveKey.x > 7)
	{
		//std::cout << "!!!! Enclave key out of range!!! HALTING..." << std::endl;
		//int someval;
		//std::cin >> someval;
	}

	//std::cout << "+++ end of move vals out. (2)" << std::endl;
}

void EnclaveLineRunner::checkDimensionForMove(float* in_moveDirection, int* in_blockKey, int* in_chunkKey)
{
	if (*in_moveDirection != 0)
	{
		//std::cout << "Movement initialized." << std::endl;
		//std::cout << "Pre-block key: " << *in_blockKey << std::endl;
		//std::cout << "Pre-chunk key: " << *in_chunkKey << std::endl;
		if (*in_moveDirection == 1) // going positive
		{
			if (*in_blockKey == 3) // is block on positive border, before the actual move?
			{
				*in_chunkKey += 1;	// increase the chunk key
				*in_blockKey = 0;	// set block key to 0
			}
			else if (*in_blockKey >= 0)
			{
				*in_blockKey += 1;  // increase the block key
			}
		}
		else // going negative
		{
			//std::cout << "Debug: going negative..." << std::endl;
			if (*in_blockKey == 0) // is block on negative border , before the actual move?
			{
				*in_chunkKey -= 1;	// decrease the chunk key
				*in_blockKey = 3;	// set block key to 3
			}
			else if (*in_blockKey <= 3)
			{
				*in_blockKey -= 1;
			}
		}
	}
}

int EnclaveLineRunner::checkIfBlockHasTriangleCorner(unsigned char* in_charRef)
{
	int hasCorner = 0;	// default value
	for (int x = 0; x < 3; x++)
	{
		hasCorner += IndependentUtils::isUnsignedCharBitSet(*in_charRef, 3 - x);	// check the bits for all three lines (bits 6 through 4)
	}
	return hasCorner;
}

PolyLineEndpointMeta EnclaveLineRunner::getEndpointData()
{

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
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, IndependentUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, IndependentUtils::findBlueprintBorderMoveMeta
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

	//std::cout << "Calc'd point x: " << calculatedPoint_for_x.x << ", " << calculatedPoint_for_x.y << ", " << calculatedPoint_for_x.z << std::endl;

	//PolyLineEndpointMeta returnEndpointData = calculatedEndpointData(currentIterationBeginPoint, distanceValues, slopeDirection, triPointParam);

	//std::cout << "!!! Calling getCalculatedEndpointMetaData from EnclaveLineRunner " << std::endl;

	PolyLineEndpointMeta returnEndpointData = IndependentUtils::getCalculatedEndpointMetadata(currentIterationBeginPoint, blockBordersRef, distanceValues, slopeDirection, triPointParam);
	// std::cout << "call before returnEndpointData " << std::endl;
	return returnEndpointData;
}

PolyLineEndpointMeta EnclaveLineRunner::getEndpointDataDebug()
{

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
	float time_to_complete_x_traversal = 0.0f; // initialize these values for use in new function call, IndependentUtils::findBlueprintBorderMoveMeta
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;		// initialize these values for use in new function call, IndependentUtils::findBlueprintBorderMoveMeta
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

	//PolyLineEndpointMeta returnEndpointData = calculatedEndpointData(currentIterationBeginPoint, distanceValues, slopeDirection, triPointParam);
	PolyLineEndpointMeta returnEndpointData = IndependentUtils::getCalculatedEndpointMetadataDebug(currentIterationBeginPoint, blockBordersRef, distanceValues, slopeDirection, triPointParam);
	// std::cout << "call before returnEndpointData " << std::endl;
	return returnEndpointData;
}



void EnclaveLineRunner::checkForCompletion()
{
	// if the keys match, set the completion flag to true
	//std::cout << ">> Checking for completion, move values are: " << currentEndpointMeta.moveDirectionValues.x << ", " << currentEndpointMeta.moveDirectionValues.y << ", " << currentEndpointMeta.moveDirectionValues.z << std::endl;
	/*
	if (
		(currentIterationMeta.enclaveKey.x == lineMeta.endPointMeta.enclaveKey.x)
		&&
		(currentIterationMeta.enclaveKey.y == lineMeta.endPointMeta.enclaveKey.y)
		&&
		(currentIterationMeta.enclaveKey.z == lineMeta.endPointMeta.enclaveKey.z)
		&&
		(currentIterationMeta.blockKey.x == lineMeta.endPointMeta.blockKey.x)
		&&
		(currentIterationMeta.blockKey.y == lineMeta.endPointMeta.blockKey.y)
		&&
		(currentIterationMeta.blockKey.z == lineMeta.endPointMeta.blockKey.z)
		)
	*/
	if
		(
		(currentIterationMeta.enclaveKey == lineMeta.endPointMeta.enclaveKey)
			&&
			(currentIterationMeta.blockKey == lineMeta.endPointMeta.blockKey)
			)
	{
		isRunComplete = 1;
		currentIterationEndPoint = lineMeta.endPointMeta.preciseCoord;		// second point will be the point thats on the border
	}

	/*
	if (isRunSingleIteration == 1)	// for when two points exist in same block.
	{
		isRunComplete = 1;
	}
	*/

}

void EnclaveLineRunner::checkEnclaveKeyValidity(EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	int invalidCount = 0;
	// x
	if (in_enclaveKey.x < 0 || in_enclaveKey.x > 7)
	{
		invalidCount++;
	}

	// y 
	if (in_enclaveKey.y < 0 || in_enclaveKey.y > 7)
	{
		invalidCount++;
	}

	// z 
	if (in_enclaveKey.z < 0 || in_enclaveKey.z > 7)
	{
		invalidCount++;
	}

	if (invalidCount > 0)
	{
		isRunComplete = 1;	// terminate run if the enclave is invalid.
	}
}