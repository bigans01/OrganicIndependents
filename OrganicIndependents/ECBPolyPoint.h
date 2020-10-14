#pragma once

#ifndef ECBPOLYPOINT_NEW_H
#define ECBPOLYPOINT_NEW_H

class ECBPolyPoint
{
public:

	ECBPolyPoint() {};
	ECBPolyPoint(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {};

	ECBPolyPoint& operator+=(const ECBPolyPoint& ecbPolyPoint_b)
	{
		x += ecbPolyPoint_b.x;
		y += ecbPolyPoint_b.y;
		z += ecbPolyPoint_b.z;
		return *this;
	}
	ECBPolyPoint& operator-=(const ECBPolyPoint& ecbPolyPoint_b)
	{
		x -= ecbPolyPoint_b.x;
		y -= ecbPolyPoint_b.y;
		z -= ecbPolyPoint_b.z;
		return *this;
	}


	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	bool isAllZero()  		// returns true if the point has a value of 0 for x,y and z.
	{
		return 
		(
			x == 0.0f
			&&
			y == 0.0f
			&&
			z == 0.0f
		);
	}
};

#endif