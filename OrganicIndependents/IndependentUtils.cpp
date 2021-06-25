#include "stdafx.h"
#include "IndependentUtils.h"

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

float IndependentUtils::roundToHundredth(float in_float)
{
	float calcedFloat = float(floor(in_float * 100 + 0.5) / 100);
	//std::cout << "Original float: " << in_float << std::endl;
	//std::cout << "Calc'ed float is: " << calcedFloat << std::endl;
	return calcedFloat;
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
	if (in_results.otype == ECBPPOrientations::FACE)
	{
		returnPoint = in_dataMapRef->faceMap[in_results.osubtype].borderLimits;
		//std::cout << "(face) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}
	else if (in_results.otype == ECBPPOrientations::LINE)
	{
		returnPoint = in_dataMapRef->lineMap[in_results.osubtype].borderLimits;
		//std::cout << "(line) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}
	else if (in_results.otype == ECBPPOrientations::CORNER)
	{
		returnPoint = in_dataMapRef->cornerMap[in_results.osubtype].borderLimits;
		//std::cout << "(corner) Limits are: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << " " << std::endl;
	}

	return returnPoint;
}

ECBPolyPoint IndependentUtils::getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(BorderDataMap* in_dataMapRef, ECBPPOrientationResults in_beginOrientationResults, ECBPolyPoint in_xIntOrig, ECBPolyPoint in_yIntOrig, ECBPolyPoint in_zIntOrig, EnclaveKeyDef::EnclaveKey in_moveVals, int in_perfectClampValue, int in_debugFlag, ECBPolyPoint in_intendedFaces)
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
		tempStorage.faceList.faceList[0] = in_dataMapRef->lineMap[in_beginOrientationResults.osubtype].linkedFaces[0];
		tempStorage.faceList.faceList[1] = in_dataMapRef->lineMap[in_beginOrientationResults.osubtype].linkedFaces[1];
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

InterceptValidity IndependentUtils::determineInterceptValidity(ECBPolyPoint in_xInt, ECBPolyPoint in_yInt, ECBPolyPoint in_zInt, ECBPolyPoint in_slopeToCheck, int in_perfectClampValue)
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
			(in_perfectClampValue == 1)
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
			(in_perfectClampValue == 2)
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
			(in_perfectClampValue == 3)
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

ECBPolyPoint IndependentUtils::getInterceptToUseFromCorner(ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope, int in_perfectClampValue, EnclaveKeyDef::EnclaveKey in_moveVals)
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
		(in_perfectClampValue == 1)	// x clamp check
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
		(in_perfectClampValue == 2)	// y clamp check
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
		(in_perfectClampValue == 3)	// z clamp check
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
		returnFaceList.faceList[0] = in_borderDataMapRef->lineMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->lineMap[in_beginOrientation.osubtype].linkedFaces[1];
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
		returnFaceList.faceList[0] = in_borderDataMapRef->lineMap[in_beginOrientation.osubtype].linkedFaces[0];
		returnFaceList.faceList[1] = in_borderDataMapRef->lineMap[in_beginOrientation.osubtype].linkedFaces[1];
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