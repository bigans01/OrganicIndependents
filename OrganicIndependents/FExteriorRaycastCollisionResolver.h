#pragma once

#ifndef FEXTERIORRAYCASTCOLLISIONRESOLVER_H
#define FEXTERIORRAYCASTCOLLISIONRESOLVER_H

#include "UniquePointContainer.h"
#include "FPotentialCollisionEnum.h"

/*

Description:

This class analyzes the FTrianglePoints having an FTrianglePointType::EXTERIOR in the given referenced UniquePointContainer,
to see if they might have a potential collision when compared to other non-EXTERNAL points (ie., an FTrianglePoint having an FTriangleType::INTER_FROM_XRAYCAST).
A collision is detected if any two dimensions (or all three) of an EXTERNAL point match exactly against a non-EXTERNAL point that has those same dimensions.

For example, if an FTrianglePoint having a FTrianglePointType::EXTERIOR has the coordinates (4.0f, 2.72f, 8.0f), and we are 
operating on an FTriangle that is using a BlueprintFracturingMachine (which would have a fixed dim interval of 4.0f), then this
would collide with any ray-casted FTrianglePoint that is of a type FTrianglePointType::INTER_FROM_YRAYCAST that has X = 4.0f and Z = 8.0f).

The event of a collision happening is known as a External-InteriorRayCast collision.

In this case, if the non-EXTERIOR, ray-casted point had a value of (4.0f, 2.71f, 8.0f), it would be ignored in favor of the EXTERIOR point (4.0f, 2.72f, 8.0f),
as the EXTERIOR point always overrides the non-EXTERIOR point, causing the non-EXTERIOR point to be flagged and deleted later on.

There have been numerous cases detected where the EXTERIOR and non-EXTERIOR point would produce two differen values, in the case of a collision being detected;
it is for this reason that the EXTERIOR point takes precedence, as it was discovered first before the interior point.

*/

class FExteriorRaycastCollisionResolver
{
	public:
		FExteriorRaycastCollisionResolver(UniquePointContainer* in_uniquePointContainerRef, float in_fixedDimInterval);
	private:
		float resolverFixedDimInterval = 0.0f;						// must be set by constructor, and be one of: 32.0f, 4.0f, 1.0f
		UniquePointContainer* resolverPointContainerRef = nullptr;	// must be set by constructor.

		struct PotentialCollisionRecord
		{
			PotentialCollisionRecord() {};
			PotentialCollisionRecord(glm::vec3 in_pcPoint, FPotentialCollisionEnum in_collisionType) :
				pcPoint(in_pcPoint),
				collisionType(in_collisionType)
			{};

			glm::vec3 pcPoint;
			FPotentialCollisionEnum collisionType = FPotentialCollisionEnum::NO_POTENTIAL_COLLISION; // set by constructor
		};

		std::vector<PotentialCollisionRecord> collisionRecords;

		void checkForPotentialCollisions();
};

#endif
