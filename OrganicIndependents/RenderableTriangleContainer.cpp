#include "stdafx.h"
#include "RenderableTriangleContainer.h"

void RenderableTriangleContainer::insertTriangle(std::unique_ptr<RenderableTriangleBase>* in_rtEntryPtr)
{
	rtVector.push_back(std::move(*in_rtEntryPtr));
}

