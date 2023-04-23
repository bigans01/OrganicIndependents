#pragma once

#ifndef ORGANICFANGROUP_H
#define ORGANICFANGROUP_H

#include "OrganicWrappedBBFan.h"
#include "BlockSubType.h"

/*

Description: this class has the ability to store one or more OrganicWrappedBBFans,
and contains metadata flags for how to consider what the data inside is, such as what the
BlockSubType should be of the EnclaveBlock that this data is going into. 

*/

class OrganicFanGroup
{
	public:
		OrganicFanGroup();
		OrganicFanGroup(OrganicWrappedBBFan in_initialFan);	// can be used for most cases, where there is only one fan to produce.

		BlockSubType groupSubType = BlockSubType::BLOCK_NORMAL;
		std::vector<OrganicWrappedBBFan> fans;

		void setAsFilledBecauseIncalculable();		// signifies that all triangles of this group are meant to be considered
													// as part of a "fill" operation, such as what occurs in the function OrganicTriangleTertiary::buildFilledBlockFanGroup()
		bool getFilledBecauseIncalculableValue();
		void insertFan(OrganicWrappedBBFan in_fanToInsert);
	private:
		bool filledBecauseIncalculable = false;	// if true, it mean that this block is "filled" on all 6 sides.

};

#endif
