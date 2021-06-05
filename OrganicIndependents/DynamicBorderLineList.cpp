#include "stdafx.h"
#include "DynamicBorderLineList.h"

void DynamicBorderLineList::setBorders()
{
	// set up corner values ---------------------------------------------------------------------------------------

	// Lower NW coords
	corner_LowerNW.pointXYZ.x = 0;
	corner_LowerNW.pointXYZ.y = 0;
	corner_LowerNW.pointXYZ.z = 0;

	corner_LowerNW.borderValues.move_x = -1;
	corner_LowerNW.borderValues.move_y = -1;
	corner_LowerNW.borderValues.move_z = -1;

	corner_LowerNW.borderValues.new_point_x = dimLength;
	corner_LowerNW.borderValues.new_point_y = dimLength;
	corner_LowerNW.borderValues.new_point_z = dimLength;


	// Lower SW coords
	corner_LowerSW.pointXYZ.x = 0;
	corner_LowerSW.pointXYZ.y = 0;
	corner_LowerSW.pointXYZ.z = dimLength;

	corner_LowerSW.borderValues.move_x = -1;
	corner_LowerSW.borderValues.move_y = -1;
	corner_LowerSW.borderValues.move_z = 1;

	corner_LowerSW.borderValues.new_point_x = dimLength;
	corner_LowerSW.borderValues.new_point_y = dimLength;
	corner_LowerSW.borderValues.new_point_z = 0;


	// Lower SE coords
	corner_LowerSE.pointXYZ.x = dimLength;
	corner_LowerSE.pointXYZ.y = 0;
	corner_LowerSE.pointXYZ.z = dimLength;

	corner_LowerSE.borderValues.move_x = 1;
	corner_LowerSE.borderValues.move_y = -1;
	corner_LowerSE.borderValues.move_z = 1;

	corner_LowerSE.borderValues.new_point_x = 0;
	corner_LowerSE.borderValues.new_point_y = dimLength;
	corner_LowerSE.borderValues.new_point_z = 0;


	// Lower NE coords
	corner_LowerNE.pointXYZ.x = dimLength;
	corner_LowerNE.pointXYZ.y = 0;
	corner_LowerNE.pointXYZ.z = 0;

	corner_LowerNE.borderValues.move_x = 1;
	corner_LowerNE.borderValues.move_y = -1;
	corner_LowerNE.borderValues.move_z = -1;

	corner_LowerNE.borderValues.new_point_x = 0;
	corner_LowerNE.borderValues.new_point_y = dimLength;
	corner_LowerNE.borderValues.new_point_z = dimLength;

	// Upper NW coords
	corner_UpperNW.pointXYZ.x = 0;
	corner_UpperNW.pointXYZ.y = dimLength;
	corner_UpperNW.pointXYZ.z = 0;

	corner_UpperNW.borderValues.move_x = -1;
	corner_UpperNW.borderValues.move_y = 1;
	corner_UpperNW.borderValues.move_z = -1;

	corner_UpperNW.borderValues.new_point_x = dimLength;
	corner_UpperNW.borderValues.new_point_y = 0;
	corner_UpperNW.borderValues.new_point_z = dimLength;

	// Upper SW coords
	corner_UpperSW.pointXYZ.x = 0;
	corner_UpperSW.pointXYZ.y = dimLength;
	corner_UpperSW.pointXYZ.z = dimLength;

	corner_UpperSW.borderValues.move_x = -1;
	corner_UpperSW.borderValues.move_y = 1;
	corner_UpperSW.borderValues.move_z = 1;

	corner_UpperSW.borderValues.new_point_x = dimLength;
	corner_UpperSW.borderValues.new_point_y = 0;
	corner_UpperSW.borderValues.new_point_z = 0;

	// Upper SE coords
	corner_UpperSE.pointXYZ.x = dimLength;
	corner_UpperSE.pointXYZ.y = dimLength;
	corner_UpperSE.pointXYZ.z = dimLength;

	corner_UpperSE.borderValues.move_x = 1;
	corner_UpperSE.borderValues.move_y = 1;
	corner_UpperSE.borderValues.move_z = 1;

	corner_UpperSE.borderValues.new_point_x = 0;
	corner_UpperSE.borderValues.new_point_y = 0;
	corner_UpperSE.borderValues.new_point_z = 0;

	// Upper NE coords
	corner_UpperNE.pointXYZ.x = dimLength;
	corner_UpperNE.pointXYZ.y = dimLength;
	corner_UpperNE.pointXYZ.z = 0;

	corner_UpperNE.borderValues.move_x = 1;
	corner_UpperNE.borderValues.move_y = 1;
	corner_UpperNE.borderValues.move_z = -1;

	corner_UpperNE.borderValues.new_point_x = 0;
	corner_UpperNE.borderValues.new_point_y = 0;
	corner_UpperNE.borderValues.new_point_z = dimLength;

	// set up line values

	//start with x lines ------------------------------------------------------------------------------------


	// lower North
	Xaxis_lowerNorth.move_x = 0;		// moving on x isn't applicable
	Xaxis_lowerNorth.move_y = -1;
	Xaxis_lowerNorth.move_z = -1;

	Xaxis_lowerNorth.new_point_x = -1;	// new point for x isn't applicable, set to -1
	Xaxis_lowerNorth.new_point_y = dimLength;
	Xaxis_lowerNorth.new_point_z = dimLength;

	// lower South
	Xaxis_lowerSouth.move_x = 0;		// " " isn't applicable
	Xaxis_lowerSouth.move_y = -1;
	Xaxis_lowerSouth.move_z = 1;

	Xaxis_lowerSouth.new_point_x = -1;	// " " set to -1
	Xaxis_lowerSouth.new_point_y = dimLength;
	Xaxis_lowerSouth.new_point_z = 0;

	// upper South
	Xaxis_upperSouth.move_x = 0;		// " " isn't applicable
	Xaxis_upperSouth.move_y = 1;
	Xaxis_upperSouth.move_z = 1;

	Xaxis_upperSouth.new_point_x = -1;	// " " set to -1
	Xaxis_upperSouth.new_point_y = 0;
	Xaxis_upperSouth.new_point_z = 0;

	// upper North
	Xaxis_upperNorth.move_x = 0;		// " " isn't applicable
	Xaxis_upperNorth.move_y = 1;
	Xaxis_upperNorth.move_z = -1;

	Xaxis_upperNorth.new_point_x = -1;	// " " set to -1
	Xaxis_upperNorth.new_point_y = 0;
	Xaxis_upperNorth.new_point_z = dimLength;

	// z lines ------------------------------------------------------------------------------------------------

	// lower West
	Zaxis_lowerWest.move_x = -1;
	Zaxis_lowerWest.move_y = -1;
	Zaxis_lowerWest.move_z = 0;			// moving on z isn't applicable

	Zaxis_lowerWest.new_point_x = dimLength;
	Zaxis_lowerWest.new_point_y = dimLength;
	Zaxis_lowerWest.new_point_z = -1;	// new point for z isn't applicable, set to -1

	// upper West
	Zaxis_upperWest.move_x = -1;
	Zaxis_upperWest.move_y = 1;
	Zaxis_upperWest.move_z = 0;			// " " isn't applicable

	Zaxis_upperWest.new_point_x = dimLength;
	Zaxis_upperWest.new_point_y = 0;
	Zaxis_upperWest.new_point_z = -1;	// " " set to -1

	// upper East
	Zaxis_upperEast.move_x = 1;
	Zaxis_upperEast.move_y = 1;
	Zaxis_upperEast.move_z = 0;			// " " isn't applicable

	Zaxis_upperEast.new_point_x = 0;
	Zaxis_upperEast.new_point_y = 0;
	Zaxis_upperEast.new_point_z = -1;	// " " set to -1

	// lower East
	Zaxis_lowerEast.move_x = 1;
	Zaxis_lowerEast.move_y = -1;
	Zaxis_lowerEast.move_z = 0;		// " " isn't applicable

	Zaxis_lowerEast.new_point_x = 0;
	Zaxis_lowerEast.new_point_y = dimLength;
	Zaxis_lowerEast.new_point_z = -1;

	// y lines ------------------------------------------------------------------------------------------------

	// north East
	Yaxis_northEast.move_x = 1;
	Yaxis_northEast.move_y = 0;		// moving on y isn't applicable
	Yaxis_northEast.move_z = -1;

	Yaxis_northEast.new_point_x = 0;
	Yaxis_northEast.new_point_y = -1; // new point for y isn't applicable, set to -1
	Yaxis_northEast.new_point_z = dimLength;

	// south East
	Yaxis_southEast.move_x = 1;
	Yaxis_southEast.move_y = 0;		// " " isn't applicable
	Yaxis_southEast.move_z = 1;

	Yaxis_southEast.new_point_x = 0;
	Yaxis_southEast.new_point_y = -1;	// " " set to -1
	Yaxis_southEast.new_point_z = 0;

	// south West
	Yaxis_southWest.move_x = -1;
	Yaxis_southWest.move_y = 0;		// " " isn't applicable
	Yaxis_southWest.move_z = 1;

	Yaxis_southWest.new_point_x = dimLength;
	Yaxis_southWest.new_point_y = -1;	//  " " set to -1
	Yaxis_southWest.new_point_z = 0;

	// north West
	Yaxis_northWest.move_x = -1;
	Yaxis_northWest.move_y = 0;		// " " isn't applicable
	Yaxis_northWest.move_z = -1;

	Yaxis_northWest.new_point_x = dimLength;
	Yaxis_northWest.new_point_y = -1;
	Yaxis_northWest.new_point_z = dimLength;




	// set up face values -----------------------------------------------------------------------------------------

	// west face
	WestFace.move_x = -1;
	WestFace.move_y = 0;		// moving on y isn't applicable
	WestFace.move_z = 0;		// moving on z isn't applicable

	WestFace.new_point_x = dimLength;
	WestFace.new_point_y = -1;	// new point for y isn't applicable, set to -1
	WestFace.new_point_z = -1;	// new point for z isn't applicable, set to -1

	// north face
	NorthFace.move_x = 0;		// moving on x isn't applicable
	NorthFace.move_y = 0;		// moving on y isn't applicable
	NorthFace.move_z = -1;

	NorthFace.new_point_x = -1;	// new point for x isn't applicable, set to -1
	NorthFace.new_point_y = -1; // new point for y isn't applicable, set to -1
	NorthFace.new_point_z = dimLength;

	// east face 
	EastFace.move_x = 1;
	EastFace.move_y = 0;		// moving on y isn't applicable
	EastFace.move_z = 0;		// moving on z isn't applicable

	EastFace.new_point_x = 0;
	EastFace.new_point_y = -1;	// new point for y isn't applicable, set to -1
	EastFace.new_point_z = -1;  // new point for z isn't applciable, set to -1

	// south face
	SouthFace.move_x = 0;
	SouthFace.move_y = 0;
	SouthFace.move_z = 1;

	SouthFace.new_point_x = -1;
	SouthFace.new_point_y = -1;
	SouthFace.new_point_z = 0;

	// top face
	TopFace.move_x = 0;
	TopFace.move_y = 1;
	TopFace.move_z = 0;

	TopFace.new_point_x = -1;
	TopFace.new_point_y = 0;
	TopFace.new_point_z = -1;

	// bottom face
	BottomFace.move_x = 0;
	BottomFace.move_y = -1;
	BottomFace.move_z = 0;

	BottomFace.new_point_x = -1;
	BottomFace.new_point_y = dimLength;
	BottomFace.new_point_z = -1;
}