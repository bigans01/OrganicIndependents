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

float IndependentUtils::roundToHundredth(float in_float)
{
	float calcedFloat = float(floor(in_float * 100 + 0.5) / 100);
	//std::cout << "Original float: " << in_float << std::endl;
	//std::cout << "Calc'ed float is: " << calcedFloat << std::endl;
	return calcedFloat;
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