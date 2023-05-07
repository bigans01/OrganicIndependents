#pragma once

#ifndef BLOCKUTILS_H
#define BLOCKUTILS_H

#include  "OrganicFanGroup.h"

/*

Description: This class should contain useful/commonly used functions pertaining to EnclaveBlock instances, or its contained data
such as OrganicFanGroup members/meta; it should be able to be used by other functions/classes outside of EnclaveBlock, such as 
OrganicTriangleSecondarySupergroupManager.

*/

class BlockUtils
{
	public:
		static OrganicFanGroup buildFilledBlockFanGroup();
};

#endif