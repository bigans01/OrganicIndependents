#pragma once

#ifndef FTRIANGLEPOINTUVPRODUCER_H
#define FTRIANGLEPOINTUVPRODUCER_H

#include "FTrianglePointUVLinkContainer.h"
#include "UniquePointContainer.h"

class FTrianglePointUVProducer
{
	public:
		FTrianglePointUVProducer() {};
		void initializeProducer(UniquePointContainer* in_uniquePointContainerRef);
	private:
		FTrianglePointUVLinkContainer links;

};

#endif
