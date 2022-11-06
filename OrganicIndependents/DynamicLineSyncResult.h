#pragma once

#ifndef DYNAMICLINESYNCRESULT_H
#define DYNAMICLINESYNCRESULT_H

#include "DynamicLineSyncType.h"

class DynamicLineSyncResult
{
public:
	DynamicLineSyncResult() {};
	DynamicLineSyncResult(DynamicLineSyncType in_syncType, ECBPolyPoint in_syncPoint) :
		syncType(in_syncType),
		syncPoint(in_syncPoint)
	{};
		DynamicLineSyncType syncType = DynamicLineSyncType::NOVAL;
		ECBPolyPoint syncPoint;
};

#endif
