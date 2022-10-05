#pragma once

#ifndef FTRIANGLETYPE_H
#define FTRIANGLETYPE_H

enum class FTriangleType
{
	NOVAL,		// should always be default value
	WORLD,		// the triangle has no border limits (it was spawned in world space)
	BLUEPRINT,	// the triangle was spawned within the confines of a blueprint.
	ORE,		// the triangle was spawned within the confines of an ORE.
	BLOCK,		// the triangle was spawned within the confines of a block.
	ATOMIC		// the triangle cannot be fratured any lower than this; the output of this FTriangle should be its own points,
				// all ranging between 0.0f and 1.0f (the same as a block)

};
#endif
