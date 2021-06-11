#pragma once

#ifndef DYNAMICLINEPOINTSYNCHRONIZER_H
#define DYNAMICLINEPOINTSYNCHRONIZER_H

#include "ECBPolyPoint.h"

class DynamicLinePointSynchronizer
{
	public:
		DynamicLinePointSynchronizer(ECBPolyPoint in_pointToSync, int in_coordToSyncTo, float in_dimLength);
		ECBPolyPoint sync();
		ECBPolyPoint basePoint;		// the base point to work on
		int coordToSyncTo;			// 0 = x, 1 = y, 2 = z;
		float dimLimit = 0.0f;
};

#endif
