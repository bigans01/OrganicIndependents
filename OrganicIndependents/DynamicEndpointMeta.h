#pragma once

#ifndef DYNAMICENDPOINTMETA_H
#define DYNAMICENDPOINTMETA_H

#include <iostream>
#include "ECBPolyPoint.h"
#include "DynamicBorder.h"

class DynamicEndpointMeta
{
	public:
		ECBPolyPoint pointLocation; // location of the point. Range per dimension is between 0.0f and the value of the dim length. (i.e, 0.02f)
		ECBPolyPoint moveDirectionValues;	// move values (0, 1 for positive, -1 for negative)
		ECBPolyPoint dimensionResetValues;	// reset values for when x/y/z hits a border
		void applyBlockBorder(DynamicBorder in_dynamicBorder)
		{
			moveDirectionValues.x = in_dynamicBorder.move_x;
			moveDirectionValues.y = in_dynamicBorder.move_y;
			moveDirectionValues.z = in_dynamicBorder.move_z;
			dimensionResetValues.x = in_dynamicBorder.new_point_x;
			dimensionResetValues.y = in_dynamicBorder.new_point_y;
			dimensionResetValues.z = in_dynamicBorder.new_point_z;
		}

		void matchSlopesToDirections(ECBPolyPoint in_slope)
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
};

#endif