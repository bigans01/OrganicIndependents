#include "stdafx.h"
#include "FTrianglePointUVLinkContainer.h"

void FTrianglePointUVLinkContainer::createNewLink(FTrianglePoint* in_pointToUseForLink)
{
	FTrianglePointUVLink newLink(in_pointToUseForLink);
	uvLinkVector.push_back(in_pointToUseForLink);
}