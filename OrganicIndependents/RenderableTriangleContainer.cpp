#include "stdafx.h"
#include "RenderableTriangleContainer.h"

void RenderableTriangleContainer::insertTriangle(std::unique_ptr<RenderableTriangleBase>* in_rtEntryPtr)
{
	std::cout << "(RenderableTriangleContainer::insertTriangle): attempting to print stats prior to insert. " << std::endl;
	in_rtEntryPtr->get()->printStats();
	rtVector.push_back(std::move(*in_rtEntryPtr));
}

