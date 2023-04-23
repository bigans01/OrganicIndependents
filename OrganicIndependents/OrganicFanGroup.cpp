#include "stdafx.h"
#include "OrganicFanGroup.h"

OrganicFanGroup::OrganicFanGroup()
{

}

OrganicFanGroup::OrganicFanGroup(OrganicWrappedBBFan in_initialFan)
{
	fans.push_back(in_initialFan);
}

void OrganicFanGroup::setAsFilledBecauseIncalculable()
{
	filledBecauseIncalculable = true;
}

bool OrganicFanGroup::getFilledBecauseIncalculableValue()
{
	return filledBecauseIncalculable;
}

void OrganicFanGroup::insertFan(OrganicWrappedBBFan in_fanToInsert)
{
	fans.push_back(in_fanToInsert);
}