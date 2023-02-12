#pragma once

#ifndef FPOTENTIALCOLLISIONENUM_H
#define FPOTENTIALCOLLISIONENUM_H

/*

Description: 

This enum class is used as part of a way to determine if an EXTERNAL FTrianglePoint might potentially collide with a separate
ray casted interior point -- i.e., an FTrianglePoint having a FTriangleType value of INTER_FROM_XRAYCAST.
The enum value does not guarantee in any way shape or form that there will be a collision; it simply guarantees that
thare is a possibility one might happen.

This value is calculated by calling the function FTrianglePoint::checkIfPointHasPotentialCollision,
and is used by the class FExteriorRaycastCollisionResolver when determining which EXTERNAL points to check for potential collisions on.

*/

enum class FPotentialCollisionEnum
{
	NO_POTENTIAL_COLLISION,
	POTENTIAL_COLLISION_BY_X_RAY,
	POTENTIAL_COLLISION_BY_Y_RAY,
	POTENTIAL_COLLISION_BY_Z_RAY,
	POTENTIAL_COLLISION_BY_ALL_RAYS
};

#endif
