#pragma once

#ifndef FTRIANGLEPOINT_H
#define FTRIANGLEPOINT_H

#include "FTrianglePointType.h"
#include "FPotentialCollisionEnum.h"
#include "DoublePoint.h"

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

		FTrianglePoint(DoublePoint in_point,
			FTrianglePointType in_pointType) :
			pointType(in_pointType)
		{
			point = in_point;
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
		DoublePoint point;	// the actual point contained within this class
		float fTextureU = 0.0f;	// optional: used when using an FTriangle for fracturing a texture; coincides with the usage of 
								// of FTrianglePointType::ORIGINAL.
		float fTextureV = 0.0f;	// ""

		// Below: this function, when given an appropriate fixed dim interval value,
		// will determine if the FTrianglePoint might potentially collide with another point that has been X, Y or Z ray-casted.
		// This function should only be called on FTrianglePoint instances that have an FTriangleType of EXTERIOR.
		// See it's usage in FExteriorRaycastCollisionResolver::checkForPotentialCollisions().
		FPotentialCollisionEnum checkIfPointHasPotentialCollision(float in_fixedDimInterval)
		{
			FPotentialCollisionEnum returnEnum = FPotentialCollisionEnum::NO_POTENTIAL_COLLISION;

			int hitCount = 0;
			bool hitsXPlane = false;
			bool hitsYPlane = false;
			bool hitsZPlane = false;

			float moduloX = fmod(point.x, in_fixedDimInterval);
			float moduloY = fmod(point.y, in_fixedDimInterval);
			float moduloZ = fmod(point.z, in_fixedDimInterval);

			if (moduloX == 0.0f)
			{
				hitsXPlane = true;
				hitCount++;
			}

			if (moduloY == 0.0f)
			{
				hitsYPlane = true;
				hitCount++;
			}

			if (moduloZ == 0.0f)
			{
				hitsZPlane = true;
				hitCount++;
			}

			// check to see if there's any potential collision; this condition can only be met if hitCount >=2.
			if (hitCount >= 2)
			{
				// CASE 1: all planes are hit. 
				if (hitsXPlane && hitsYPlane && hitsZPlane)
				{
					returnEnum = FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_ALL_RAYS;
					//std::cout << "(FTrianglePoint) point found as having POTENTIAL_COLLISION_BY_ALL_RAYS. Values are: "; 
					//std::cout << point.x << ", " << point.y << ", " << point.z << std::endl;
				}

				// CASE 2: only the Y and Z planes are hit, so there is potential for an X-ray cast to hit the EXTERIOR point.
				else if (hitsYPlane && hitsZPlane)
				{
					returnEnum = FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_X_RAY;
					//std::cout << "(FTrianglePoint) point found as having POTENTIAL_COLLISION_BY_X_RAY. Values are: ";
					//std::cout << point.x << ", " << point.y << ", " << point.z << std::endl;
				}

				// CASE 2: only the X and Z planes are hit, so there is potential for an Y-ray cast to hit the EXTERIOR point.
				else if (hitsXPlane && hitsZPlane)
				{
					returnEnum = FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Y_RAY;
					//std::cout << "(FTrianglePoint) point found as having POTENTIAL_COLLISION_BY_Y_RAY. Values are: ";
					//std::cout << point.x << ", " << point.y << ", " << point.z << std::endl;
				}

				// CASE 3: only the X and Y planes are hit, so there is potential for an Z-ray cast to hit the EXTERIOR point.
				else if (hitsXPlane && hitsYPlane)
				{
					returnEnum = FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Z_RAY;
					//std::cout << "(FTrianglePoint) point found as having POTENTIAL_COLLISION_BY_Z_RAY. Values are: ";
					//std::cout << point.x << ", " << point.y << ", " << point.z << std::endl;
				}
			}

			return returnEnum;
		};

		// Below: This function, when given two float dimensional values and a FPotentialCollisionEnum value,
		// will check to see if the current FTrianglePoint has an External-InteriorRayCast collision.
		//
		// The float values are compared against the x,y, and/or z values of the "point" member of this class,
		// the logic of which is dependent on the input value of in_currentEnum.
		//
		// If both of the logic checks for the float match against their compared-to dimension, then a External-InteriorRayCast collision is
		// guaranteed to occur, and we should use the value of the EXTERNAL point that gave us the two float input values.
		// See this function's usage in FExteriorRaycastCollisionResolver::checkForPotentialCollisions().
		bool checkFor2DMatch(float in_valueA, float in_valueB, FPotentialCollisionEnum in_currentEnum)
		{
			bool collisionDetected = false;
			switch (in_currentEnum)
			{
				case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_X_RAY:
				{
					if
					(
						(in_valueA == point.y)
						&&
						(in_valueB == point.z)
					)
					{
						collisionDetected = true;
					}
					break;
				}

				case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Y_RAY:
				{
					if
					(
						(in_valueA == point.x)
						&&
						(in_valueB == point.z)
					)
					{
						collisionDetected = true;
					}
					break;
				}

				case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Z_RAY:
				{
					if
					(
						(in_valueA == point.x)
						&&
						(in_valueB == point.y)
					)
					{
						collisionDetected = true;
					}
					break;
				}
			}
			return collisionDetected;
		}


		void printPointData()
		{
			std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ") | Type: ";
			switch (pointType)
			{
				case FTrianglePointType::NOVAL: { std::cout << "NOVAL"; break; }
				case FTrianglePointType::ORIGINAL: { std::cout << "ORIGINAL"; break; }
				case FTrianglePointType::EXTERIOR: { std::cout << "EXTERIOR"; break; }
				case FTrianglePointType::INTER_FROM_XRAYCAST: { std::cout << "INTER_FROM_XRAYCAST"; break; }
				case FTrianglePointType::INTER_FROM_YRAYCAST: { std::cout << "INTER_FROM_YRAYCAST"; break; }
				case FTrianglePointType::INTER_FROM_ZRAYCAST: { std::cout << "INTER_FROM_ZRAYCAST"; break; }
			}
		}
};

#endif