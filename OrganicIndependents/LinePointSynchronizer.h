#pragma once

#ifndef LINEPOINTSYNCHRONIZER_H
#define LINEPOINTSYNCHRONIZER_H

#include "ECBPolyPoint.h"

class LinePointSynchronizer
{
public:
	LinePointSynchronizer(ECBPolyPoint in_pointToSync, int in_coordToSyncTo);
	ECBPolyPoint sync();
	ECBPolyPoint basePoint;		// the base point to work on
	int coordToSyncTo;			// 0 = x, 1 = y, 2 = z;
};

#endif