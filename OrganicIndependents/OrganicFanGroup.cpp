#include "stdafx.h"
#include "OrganicFanGroup.h"

OrganicFanGroup::OrganicFanGroup()
{

}

OrganicFanGroup::OrganicFanGroup(OrganicWrappedBBFan in_initialFan)
{
	fans.push_back(in_initialFan);
}