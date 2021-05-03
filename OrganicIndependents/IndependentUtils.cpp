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

float IndependentUtils::roundToHundredth(float in_float)
{
	float calcedFloat = float(floor(in_float * 100 + 0.5) / 100);
	//std::cout << "Original float: " << in_float << std::endl;
	//std::cout << "Calc'ed float is: " << calcedFloat << std::endl;
	return calcedFloat;
}