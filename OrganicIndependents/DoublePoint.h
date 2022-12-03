#pragma once

#ifndef DOUBLEPOINT_H
#define DOUBLEPOINT_H

/*

Description:

This class essentially copies an ECBPolyPoint, but allows for higher values (doubles). It will eventually
be used in OSContouredTriangles (OrganicServerLib), to allow for very far out OSContouredTriangles to be processed.
It is currently used in multiple other classes, however.

*/

class DoublePoint
{
	public:
		DoublePoint() {};
		DoublePoint(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {};
		DoublePoint(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {};
		DoublePoint(glm::vec3 in_vec3) : x(in_vec3.x), y(in_vec3.y), z(in_vec3.z) {};
		DoublePoint(ECBPolyPoint in_polyPoint) : x(in_polyPoint.x), y(in_polyPoint.y), z(in_polyPoint.z) {};

		// copy operators
		DoublePoint& operator=(const ECBPolyPoint& in_polyPoint)
		{
			x = in_polyPoint.x;
			y = in_polyPoint.y;
			z = in_polyPoint.z;
			return *this;
		}

		DoublePoint& operator=(const glm::vec3& in_vec3)
		{
			x = in_vec3.x;
			y = in_vec3.y;
			z = in_vec3.z;
			return *this;
		}

		// equivalent operators
		bool operator==(const DoublePoint in_vec3)
		{
			return
			(
				(x == in_vec3.x)
				&&
				(y == in_vec3.y)
				&&
				(z == in_vec3.z)
			);
		}

		bool operator !=(const DoublePoint in_vec3)
		{
			return
			!(
				(x == in_vec3.x)
				&&
				(y == in_vec3.y)
				&&
				(z == in_vec3.z)
			);
		}

		bool operator==(const glm::vec3 in_vec3)
		{
			return
			(
				(x == in_vec3.x)
				&&
				(y == in_vec3.y)
				&&
				(z == in_vec3.z)
			);
		}

		DoublePoint& operator+=(const DoublePoint& in_doublePointB)
		{
			x += in_doublePointB.x;
			y += in_doublePointB.y;
			z += in_doublePointB.z;
			return *this;
		}

		DoublePoint& operator-=(const DoublePoint& in_doublePointB)
		{
			x -= in_doublePointB.x;
			y -= in_doublePointB.y;
			z -= in_doublePointB.z;
			return *this;
		}

		DoublePoint operator-(const DoublePoint& in_doublePointB)
		{
			DoublePoint returnPoint;
			returnPoint.x = (x - in_doublePointB.x);
			returnPoint.y = (y - in_doublePointB.y);
			returnPoint.z = (z - in_doublePointB.z);
			return returnPoint;
		}

		DoublePoint operator+(const DoublePoint& in_doublePointB)
		{
			DoublePoint returnPoint;
			returnPoint.x = (x + in_doublePointB.x);
			returnPoint.y = (y + in_doublePointB.y);
			returnPoint.z = (z + in_doublePointB.z);
			return returnPoint;
		}
		
		double x = 0.0f;
		double y = 0.0f;
		double z = 0.0f;

		void printPointCoords()
		{
			std::cout << " (" << x << ", " << y << ", " << z << ") ";
		}

		void roundHundredth()
		{
			x = double(floor(x * 100 + 0.5) / 100);
			y = double(floor(y * 100 + 0.5) / 100);
			z = double(floor(z * 100 + 0.5) / 100);
		}
};

#endif
