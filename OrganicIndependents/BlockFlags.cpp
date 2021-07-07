#include "stdafx.h"
#include "BlockFlags.h"
#include "IndependentUtils.h"

BlockFlagStates BlockFlags::getTriangleDataLocationFlag()
{
	BlockFlagStates returnStates = BlockFlagStates::LOCAL;	// default is local
	if (IndependentUtils::isUnsignedCharBitSet(flags, 1) == 1)
	{
		returnStates = BlockFlagStates::OUTSOURCED;
	};		// check first bit
	return returnStates;
}

void BlockFlags::setTriangleDataLocationFlag(short in_bitValue)
{
	IndependentUtils::setUnsignedCharBit(&flags, 1, in_bitValue);
}