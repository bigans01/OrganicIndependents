#pragma once

#ifndef PMASSSTATE_H
#define PMASSSTATE_H

enum class PMassState
{
	UNDETERMINED,				// when it hasn't been decided yet
	POINT_AT_CORE,				// should be flagged when two or more PAtomBase instanvces have the exact same center point; the resulting point will be
								// that core point.
	POINT_IN_FUSABLE_AREA		// should be flagged when two or more atoms meet in a fusable area around the atoms; the resulting point can be any
								// of these fusable areas.
};

#endif
