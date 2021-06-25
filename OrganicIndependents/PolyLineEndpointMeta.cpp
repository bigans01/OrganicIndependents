#include "stdafx.h"
#include "PolyLineEndpointMeta.h"

void PolyLineEndpointMeta::applyBlockBorder(BlockBorder in_blockBorder)
{
	moveDirectionValues.x = in_blockBorder.move_x;
	moveDirectionValues.y = in_blockBorder.move_y;
	moveDirectionValues.z = in_blockBorder.move_z;
	dimensionResetValues.x = in_blockBorder.new_point_x;
	dimensionResetValues.y = in_blockBorder.new_point_y;
	dimensionResetValues.z = in_blockBorder.new_point_z;
	//std::cout << "Border move direction is now: " << moveDirectionValues.x << ", " << moveDirectionValues.y << ", " << moveDirectionValues.z << ", " << std::endl;
}

void PolyLineEndpointMeta::matchSlopesToDirections(ECBPolyPoint in_slope)
{
	//std::cout << "matching for slopes: " << in_slope.x << ", " << in_slope.y << ", " << in_slope.z << std::endl;
	if (in_slope.x == 0.0f)
	{
		//std::cout << "X slope reset to 0." << std::endl;
		moveDirectionValues.x = 0.0f;
	}

	if (in_slope.y == 0.0f)
	{
		//std::cout << "Y slope reset to 0." << std::endl;
		moveDirectionValues.y = 0.0f;
	}

	if (in_slope.z == 0.0f)
	{
		//std::cout << "Z slope reset to 0." << std::endl;
		moveDirectionValues.z = 0.0f;
	}
	//std::cout << "New move direction values: " << moveDirectionValues.x << ", " << moveDirectionValues.y << ", " << moveDirectionValues.z << std::endl;
}