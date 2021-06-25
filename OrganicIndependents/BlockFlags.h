#pragma once

#ifndef BLOCKFLAGS_H
#define BLOCKFLAGS_H

#include "BlockFlagStates.h"
#include "IndependentUtils.h"

class BlockFlags
{
public:
	unsigned char flags = 0;

	BlockFlagStates getTriangleDataLocationFlag();		// checks the first bit (least significant); 0 = LOCAL, 1 = OUTSOURCED
	void setTriangleDataLocationFlag(short in_bitValue);
};

#endif
