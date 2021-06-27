#pragma once

#ifndef SOMEFLAG_H
#define SOMEFLAG_H

#include "ECBPolyPoint.h"

class ECBPolyLine
{
public:
	ECBPolyPoint pointA;
	ECBPolyPoint pointB;
	ECBPolyPoint pointC;	// used when any x/y/z in point A and B are the same
	ECBPolyPoint x_interceptSlope;
	ECBPolyPoint y_interceptSlope;
	ECBPolyPoint z_interceptSlope;
	int lineID = 0;
};

#endif
