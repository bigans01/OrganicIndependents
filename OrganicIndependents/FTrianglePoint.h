#pragma once

#ifndef FTRIANGLEPOINT_H
#define FTRIANGLEPOINT_H

#include "FTrianglePointType.h"

class FTrianglePoint
{
	public:
		FTrianglePoint() {};
		FTrianglePoint(glm::vec3 in_point,
					FTrianglePointType in_pointType) :
			point(in_point),
			pointType(in_pointType)
		{};

		FTrianglePoint(ECBPolyPoint in_point,
			FTrianglePointType in_pointType) :
			pointType(in_pointType)
		{
			point.x = in_point.x;
			point.y = in_point.y;
			point.z = in_point.z;
		};



		bool operator==(const ECBPolyPoint& in_otherPoint)
		{
			return (point.x == in_otherPoint.x
				&&
				point.y == in_otherPoint.y
				&&
				point.z == in_otherPoint.z);
		}

		bool operator==(const FTrianglePoint& in_otherPoint)
		{
			return (point == in_otherPoint.point);
		}

		FTrianglePointType pointType = FTrianglePointType::NOVAL;	// must be set by constructor #2 above
		glm::vec3 point;

		void printPointData()
		{
			std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ") | Type: ";
			switch (pointType)
			{
				case FTrianglePointType::EXTERIOR: { std::cout << "EXTERIOR"; break; }
				case FTrianglePointType::INTERIOR: { std::cout << "INTERIOR"; break; }
				case FTrianglePointType::NOVAL: { std::cout << "NOVAL"; break; }
			}
		}
};

#endif