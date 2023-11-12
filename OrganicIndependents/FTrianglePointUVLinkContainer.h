#pragma once

#ifndef FTRIANGLEPOINTUVLINKCONTAINER_H
#define FTRIANGLEPOINTUVLINKCONTAINER_H

#include "FTrianglePoint.h"
#include "FTrianglePointUVLink.h"

class FTrianglePointUVLinkContainer
{
	public:
		void createNewLink(FTrianglePoint* in_pointToUseForLink);
	private:
		std::vector<FTrianglePointUVLink> uvLinkVector;
};

#endif
