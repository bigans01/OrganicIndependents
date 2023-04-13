#pragma once

#ifndef ORGANICFANGROUP_H
#define ORGANICFANGROUP_H

#include "OrganicWrappedBBFan.h"
#include "BlockSubType.h"

class OrganicFanGroup
{
	public:
		OrganicFanGroup();
		OrganicFanGroup(OrganicWrappedBBFan in_initialFan);	// can be used for most cases, where there is only one fan to produce.

		BlockSubType groupSubType = BlockSubType::BLOCK_NORMAL;
		std::vector<OrganicWrappedBBFan> fans;
	private:

};

#endif
