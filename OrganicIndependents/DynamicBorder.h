#pragma once

#ifndef DYNAMICBORDER_H
#define DYNAMICBORDER_H

class DynamicBorder
{
	public:
		char move_x = 0;		// the direction to move the block in
		char move_y = 0;
		short move_z = 0;

		float new_point_x = 0.0f;
		float new_point_y = 0.0f;
		float new_point_z = 0.0f;
};

#endif
