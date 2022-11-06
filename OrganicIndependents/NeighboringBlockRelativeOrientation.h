#pragma once

#ifndef NEIGHBORINGBLOCKRELATIVEORIENTATION_H
#define NEIGHBORINGBLOCKRELATIVEORIENTATION_H

#include "NeighboringBlockLocation.h"

class NeighboringBlockRelativeOrientation
{
	public:
		NeighboringBlockRelativeOrientation() {};
		NeighboringBlockRelativeOrientation(NeighboringBlockLocation in_blockLocation,
											BoundaryOrientation in_relativeOrientation) :
			blockLocation(in_blockLocation),
			relativeOrientation(in_relativeOrientation)
		{};

		NeighboringBlockLocation blockLocation;	// set by constructor
		BoundaryOrientation relativeOrientation = BoundaryOrientation::NONE; // NONE by default
};

#endif
