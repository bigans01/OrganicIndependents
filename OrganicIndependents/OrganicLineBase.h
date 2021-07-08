#pragma once

#ifndef ORGANICLINEBASE_H
#define ORGANICLINEBASE_H

#include "ECBPolyPoint.h"

class OrganicLineBase
{
	public:
		ECBPolyPoint pointA;
		ECBPolyPoint pointB;
		ECBPolyPoint pointC;
		ECBPolyPoint x_interceptSlope;
		ECBPolyPoint y_interceptSlope;
		ECBPolyPoint z_interceptSlope;
};

#endif
