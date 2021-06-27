#pragma once

#ifndef ORGANICTRIANGLESECONDARYSUPERGROUP_H
#define ORGANICTRIANGLESECONDARYSUPERGROUP_H

#include <map>
#include "OrganicTriangleSecondary.h"

class OrganicTriangleSecondarySupergroup
{
public:
	std::map<int, OrganicTriangleSecondary> secondarySubgroups;
	void insertOrganicTriangleSecondary(int in_secondaryID, OrganicTriangleSecondary in_organicTriangleSecondary)
	{
		secondarySubgroups[in_secondaryID] = in_organicTriangleSecondary;
	}
};

#endif
