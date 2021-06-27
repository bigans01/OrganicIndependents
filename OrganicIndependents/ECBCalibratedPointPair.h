#pragma once

#ifndef ECBCALIBRATEDPOINTPAIR_H
#define ECBCALIBRATEDPOINTPAIR_H

#include "ECBPolyPointTri.h"
#include "ECBPolyPoint.h"

class ECBCalibratedPointPair
{
public:
	ECBPolyPointTri calibratedPointTri;
	ECBPolyPoint calibratedDistance;
};

#endif

