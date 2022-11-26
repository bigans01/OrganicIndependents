#pragma once

#ifndef ECBPOLYPOINT_NEW_H
#define ECBPOLYPOINT_NEW_H

class ECBPolyPoint
{
public:

	ECBPolyPoint() {};
	ECBPolyPoint(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {};
	ECBPolyPoint(glm::vec3 in_vec3) : x(in_vec3.x), y(in_vec3.y), z(in_vec3.z) {};

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

	ECBPolyPoint operator-(const ECBPolyPoint& in_pointB)
	{
		ECBPolyPoint returnPoint;
		returnPoint.x = (x - in_pointB.x);
		returnPoint.y = (y - in_pointB.y);
		returnPoint.z = (z - in_pointB.z);
		return returnPoint;
	}

	bool operator==(const ECBPolyPoint& in_otherPoint)
	{
		return (x == in_otherPoint.x
			&&
			y == in_otherPoint.y
			&&
			z == in_otherPoint.z);
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

	void printPointCoords()
	{
		std::cout << " (" << x << ", " << y << ", " << z << ") ";
	}
};

#endif