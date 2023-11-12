#include "stdafx.h"
#include "FTrianglePointUVProducer.h"

void FTrianglePointUVProducer::initializeProducer(UniquePointContainer* in_uniquePointContainerRef)
{
	for (auto& currentUniquePoint : in_uniquePointContainerRef->points)
	{
		links.createNewLink(&currentUniquePoint);
	}
}