#pragma once

#ifndef BLOCKFLAGS_H
#define BLOCKFLAGS_H

#include "BlockFlagStates.h"

class IndependentUtils;
class BlockFlags
{
public:
	unsigned char flags = 0;

	// Boost serialization function, for reading/writing (primarily for usage by OrganicRawEnclave)
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& flags;
	}

	BlockFlagStates getTriangleDataLocationFlag();		// checks the first bit (least significant); 0 = LOCAL, 1 = OUTSOURCED
	void setTriangleDataLocationFlag(short in_bitValue);
	void resetFlags(); // sets the flags member back to 0.
};

#endif
