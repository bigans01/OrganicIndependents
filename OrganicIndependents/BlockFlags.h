#pragma once

#ifndef BLOCKFLAGS_H
#define BLOCKFLAGS_H

#include "BlockFlagStates.h"

class IndependentUtils;
class BlockFlags
{
public:
	unsigned char flags = 0;

	BlockFlagStates getTriangleDataLocationFlag();		// checks the first bit (least significant); 0 = LOCAL, 1 = OUTSOURCED
	void setTriangleDataLocationFlag(short in_bitValue);
	void resetFlags(); // sets the flags member back to 0.
};

#endif
