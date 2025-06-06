#pragma once

#ifndef UNIQUEPOINTCONTAINER_H
#define UNIQUEPOINTCONTAINER_H

#include "FTrianglePoint.h"

/*

Description:

A simple container class used to store unique values for FTrianglePoints. Its intention is to be used with derivatives 
of the FTriangleFracturerBaseClass, but may have uses in other places (and the code may be expanded upon as well).

*/

class UniquePointContainer
{
	public:
		void insertFTrianglePoint(FTrianglePoint in_pointToInsert)	// attempt to insert a point; will not insert if the point already exists.
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


		void insertFTrianglePointAndFlagAsUVOrigin(FTrianglePoint in_pointToInsert,
			float in_textureUValue,
			float in_textureVValue)
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
				in_pointToInsert.fTextureU = in_textureUValue;
				in_pointToInsert.fTextureV = in_textureVValue;
				in_pointToInsert.originalPointFlag = true;
				points.push_back(in_pointToInsert);
			}
		}

		std::vector<FTrianglePoint> getAllPointsWithX(float in_xLookupValue)	// fetch all points that match a particular X value.
		{
			std::vector<FTrianglePoint> xPoints;
			for (auto& currentFTrianglePoint : points)
			{
				if (currentFTrianglePoint.point.x == in_xLookupValue)
				{
					xPoints.push_back(currentFTrianglePoint);
				}
			}
			return xPoints;
		}

		std::vector<FTrianglePoint> getAllPointsWithY(float in_yLookupValue)	// fetch all points that match a particular Y value.
		{
			std::vector<FTrianglePoint> yPoints;
			for (auto& currentFTrianglePoint : points)
			{
				if (currentFTrianglePoint.point.y == in_yLookupValue)
				{
					yPoints.push_back(currentFTrianglePoint);
				}
			}
			return yPoints;
		}

		std::vector<FTrianglePoint> getAllPointsWithZ(float in_zLookupValue) // fetch all points that match a particular Z value.
		{
			std::vector<FTrianglePoint> zPoints;
			for (auto& currentFTrianglePoint : points)
			{
				if (currentFTrianglePoint.point.z == in_zLookupValue)
				{
					zPoints.push_back(currentFTrianglePoint);
				}
			}
			return zPoints;
		}

		void printAllPoints()
		{
			std::cout << "Printing all points: " << std::endl;
			for (auto& currentPoint : points)
			{
				currentPoint.printPointData();
				std::cout << std::endl;
			}
		}

		void printOriginPoints()
		{
			std::cout << "Printing origin points (if detected): " << std::endl;
			for (auto& currentPoint : points)
			{
				if (currentPoint.originalPointFlag)
				{
					std::cout << "! Found original point: ";
					currentPoint.printPointData();
					std::cout << " fTextureU: " << currentPoint.fTextureU << " | fTextureV: " << currentPoint.fTextureV << std::endl;
				}
			}
		}

	private:
		friend class FExteriorRaycastCollisionResolver;
		friend class FTriangleUtils;
		friend class FTrianglePointUVProducer;
		friend class FTriangleUVCalculator;
		std::vector<FTrianglePoint> points;
};

#endif
