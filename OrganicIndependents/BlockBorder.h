#pragma once

#ifndef BLOCKBORDER_H
#define BLOCKBORDER_H

class BlockBorder
{
public:
	char move_x = 0;		// the direction to move the block in
	char move_y = 0;
	short move_z = 0;

	char new_point_x = 0;	// indicates what the x/y/z values of the point will be when it resets (will always be either 1.0f or 0.0f)
	char new_point_y = 0;
	short new_point_z = 0;
};

#endif