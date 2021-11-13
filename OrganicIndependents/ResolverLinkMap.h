#pragma once

#ifndef RESOLVERLINKMAP_H
#define RESOLVERLINKMAP_H

#include <map>
#include "ResolverLink.h"

class ResolverLinkMap
{
	public:
		std::map<int, ResolverLink> links;
};

#endif
