#pragma once

#ifndef UNIQUEPOINTCONTAINER_H
#define UNIQUEPOINTCONTAINER_H

#include <vector>
#include "ECBPolyPoint.h"

/*

Description:

A simple container class used to store unique values for ECBPolyPoints. Its intention is to be used with derivatives 
of the FTriangleFracturerBaseClass, but may have uses in other places (and the code may be expanded upon as well).

*/

class UniquePointContainer
{
	public:
		void insertPoint(ECBPolyPoint in_pointToInsert)	// attempt to insert a point; will not insert if the point already exists.
		{
			bool doesPointExist = false;

			// check if the point exists already. If it does, set existence to true and
			// break out of the loop (we won't insert the point)
			for (auto& currentPoint : points)
			{
				if (in_pointToInsert == currentPoint)
				{
					doesPointExist = true;
					break;
				}
			}

			if (doesPointExist == false)
			{
				points.push_back(in_pointToInsert);
			}
		}

		std::vector<ECBPolyPoint> getAllPointsWithX(float in_xLookupValue)	// fetch all points that match a particular X value.
		{
			std::vector<ECBPolyPoint> xPoints;
			for (auto& currentPoint : points)
			{
				if (currentPoint.x == in_xLookupValue)
				{
					xPoints.push_back(currentPoint);
				}
			}
			return xPoints;
		}

		std::vector<ECBPolyPoint> getAllPointsWithY(float in_yLookupValue)	// fetch all points that match a particular Y value.
		{
			std::vector<ECBPolyPoint> yPoints;
			for (auto& currentPoint : points)
			{
				if (currentPoint.y == in_yLookupValue)
				{
					yPoints.push_back(currentPoint);
				}
			}
			return yPoints;
		}

		std::vector<ECBPolyPoint> getAllPointsWithZ(float in_zLookupValue) // fetch all points that match a particular Z value.
		{
			std::vector<ECBPolyPoint> zPoints;
			for (auto& currentPoint : points)
			{
				if (currentPoint.z == in_zLookupValue)
				{
					zPoints.push_back(currentPoint);
				}
			}
			return zPoints;
		}

		void printAllPoints()
		{
			std::cout << "Printing all points: " << std::endl;
			for (auto& currentPoint : points)
			{
				currentPoint.printPointCoords();
				std::cout << std::endl;
			}
		}

	private:
		std::vector<ECBPolyPoint> points;
};

#endif
