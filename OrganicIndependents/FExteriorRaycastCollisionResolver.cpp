#include "stdafx.h"
#include "FExteriorRaycastCollisionResolver.h"

FExteriorRaycastCollisionResolver::FExteriorRaycastCollisionResolver(UniquePointContainer* in_uniquePointContainerRef, float in_fixedDimInterval)
{
	resolverPointContainerRef = in_uniquePointContainerRef;
	resolverFixedDimInterval = in_fixedDimInterval;
	checkForPotentialCollisions();
}

void FExteriorRaycastCollisionResolver::checkForPotentialCollisions()
{
	// Figure out which points we need to check. We only care about EXTERIOR points which have potential collision,
	// as these are the ones that have precedence over any INTER* type points, such as INTER_FROM_X_RAYCAST.
	// Additionally, an external point which also has 3 potential rays is also considered (this might be the case when an EXTERNAL point
	// hits a corner in the grid)
	for (auto& currentPoint : resolverPointContainerRef->points)
	{
		if (currentPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			auto currentCollisionValue = currentPoint.checkIfPointHasPotentialCollision(resolverFixedDimInterval);

			if (currentCollisionValue == FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_X_RAY)
			{
				PotentialCollisionRecord xRecord(FTriangleUtils::convertDoublePointToVec3(currentPoint.point), currentCollisionValue);
				collisionRecords.push_back(xRecord);
			}
			else if (currentCollisionValue == FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Y_RAY)
			{
				PotentialCollisionRecord yRecord(FTriangleUtils::convertDoublePointToVec3(currentPoint.point), currentCollisionValue);
				collisionRecords.push_back(yRecord);
			}
			else if (currentCollisionValue == FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Z_RAY)
			{
				PotentialCollisionRecord zRecord(FTriangleUtils::convertDoublePointToVec3(currentPoint.point), currentCollisionValue);
				collisionRecords.push_back(zRecord);
			}

			else if (currentCollisionValue == FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_ALL_RAYS)
			{
				PotentialCollisionRecord allRecord(FTriangleUtils::convertDoublePointToVec3(currentPoint.point), currentCollisionValue);
				collisionRecords.push_back(allRecord);
			}
		}
	}

	// Iterate through all collision records.
	for (auto& currentRecord : collisionRecords)
	{
		// Remember, for each record, we must run against all other points that are not exterior.
		// For the time being, the currentIteratorOffset is only for debugging purposes.
		int currentIteratorOffset = 0;

		// Find the target point type(s) to compare against, when looking at other points;
		// In most cases that a potential collision is detected, it will be one of the individual ones (i.e, POTENTIAL_COLLISION_BY_X_RAY)
		// but it is still possible for POTENTIAL_COLLISION_BY_ALL_RAYS to be hit under very specific circumstances, such as when the coordinates
		// of an EXTERIOR point hit 3 rays simultaneously.
		// 
		// It is important to remember, that an EXTERIOR point can only have two types of potential collision scenarios:
		// A.) It has a potential collision with either an X ray, Y ray, or Z ray
		// OR
		// B.) It has a potentiall collison with ALL of them (X ray, Y ray, AND the Z ray)
		//
		// In the event that scenario B above is met, all 3 FTrianglePointTypes must be inserted into the set.
		std::set<FTrianglePointType> targetTypes;
		switch (currentRecord.collisionType)
		{
			case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_ALL_RAYS:
			{
				targetTypes.insert(FTrianglePointType::INTER_FROM_XRAYCAST);
				targetTypes.insert(FTrianglePointType::INTER_FROM_YRAYCAST);
				targetTypes.insert(FTrianglePointType::INTER_FROM_ZRAYCAST);
				break;
			}

			case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_X_RAY:
			{
				targetTypes.insert(FTrianglePointType::INTER_FROM_XRAYCAST);
				break;
			}

			case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Y_RAY:
			{
				targetTypes.insert(FTrianglePointType::INTER_FROM_YRAYCAST);
				break;
			}

			case FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Z_RAY:
			{
				targetTypes.insert(FTrianglePointType::INTER_FROM_XRAYCAST);
				break;
			}
		}
			

		// With the targetTypes set in play, find other points to match against, for each element in the set.

		std::vector<glm::vec3> pointsToRemove; // this vector of points will contain the ones we need to remove, if we find any.

		for (auto& currentOtherPoints : resolverPointContainerRef->points)
		{
			// Get the pointType value from the currentOtherPoint, such as INTER_FROM_XRAYCAST, and see if 
			// that value is found in the targetTypes set. If it is, we'll have to do a checkFor2dMatch on the current other point.
			auto currentPointTypeValue = currentOtherPoints.pointType;
			auto setValueFinder = targetTypes.find(currentPointTypeValue);
			if (setValueFinder != targetTypes.end())	// it was found, if this is the case.
			{
				switch (currentPointTypeValue)
				{
					case FTrianglePointType::INTER_FROM_XRAYCAST:
					{
						bool collisionDetected = currentOtherPoints.checkFor2DMatch(currentRecord.pcPoint.y, currentRecord.pcPoint.z, FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_X_RAY);
						if (collisionDetected)
						{
							//std::cout << "The exterior point of (" << currentRecord.pcPoint.x << ", " << currentRecord.pcPoint.y << ", " << currentRecord.pcPoint.z << ") was detected as having a COLLISION_BY_X_RAY, against the point "
							//	<< "(" << currentOtherPoints.point.x << ", " << currentOtherPoints.point.y << ", " << currentOtherPoints.point.z << ")" << std::endl;

							//std::cout << "The index of the found point was: " << currentIteratorOffset << std::endl;

							pointsToRemove.push_back(FTriangleUtils::convertDoublePointToVec3(currentOtherPoints.point));
						}
						break;
					}

					case FTrianglePointType::INTER_FROM_YRAYCAST:
					{
						bool collisionDetected = currentOtherPoints.checkFor2DMatch(currentRecord.pcPoint.x, currentRecord.pcPoint.z, FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Y_RAY);
						if (collisionDetected)
						{
							//std::cout << "The exterior point of (" << currentRecord.pcPoint.x << ", " << currentRecord.pcPoint.y << ", " << currentRecord.pcPoint.z << ") was detected as having a COLLISION_BY_Y_RAY, against the point "
							//	<< "(" << currentOtherPoints.point.x << ", " << currentOtherPoints.point.y << ", " << currentOtherPoints.point.z << ")" << std::endl;

							//std::cout << "The index of the found point was: " << currentIteratorOffset << std::endl;

							pointsToRemove.push_back(FTriangleUtils::convertDoublePointToVec3(currentOtherPoints.point));
						}
						break;
					}

					case FTrianglePointType::INTER_FROM_ZRAYCAST:
					{
						bool collisionDetected = currentOtherPoints.checkFor2DMatch(currentRecord.pcPoint.x, currentRecord.pcPoint.y, FPotentialCollisionEnum::POTENTIAL_COLLISION_BY_Z_RAY);
						if (collisionDetected)
						{
							//std::cout << "The exterior point of (" << currentRecord.pcPoint.x << ", " << currentRecord.pcPoint.y << ", " << currentRecord.pcPoint.z << ") was detected as having a COLLISION_BY_Z_RAY, against the point "
							//	<< "(" << currentOtherPoints.point.x << ", " << currentOtherPoints.point.y << ", " << currentOtherPoints.point.z << ")" << std::endl;

							//std::cout << "The index of the found point was: " << currentIteratorOffset << std::endl;

							pointsToRemove.push_back(FTriangleUtils::convertDoublePointToVec3(currentOtherPoints.point));
						}
						break;
					}
				}
			}

			currentIteratorOffset++;
		}

		// Now, for each point to remove, we must iterate through the point container until we find that one to remove. The number of tiems to do this, 
		// is equal to the total number of ponits in pointsToRemove.
		for (auto& currentPointToRemove : pointsToRemove)
		{
			int removalOffset = 0;
			for (auto& currentOtherPoints : resolverPointContainerRef->points)
			{
				if (currentPointToRemove == FTriangleUtils::convertDoublePointToVec3(currentOtherPoints.point))
				{
					resolverPointContainerRef->points.erase(resolverPointContainerRef->points.begin() + removalOffset);
					break;
				}
				removalOffset++;
			}
		}
	}
}