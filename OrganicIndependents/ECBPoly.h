#pragma once

#ifndef ECBPOLYLINE_NEW_H
#define ECBPOLYLINE_NEW_H

#include <unordered_map>
#include "ECBPolyPoint.h"
#include "ECBPolyLine.h"
#include "ECBPolyType.h"


class ECBPoly
{
public:
	ECBPoly() {};
	ECBPoly(ECBPolyType in_polyType) : polyType(in_polyType) {};
	ECBPolyType polyType = ECBPolyType::FREE;
	int sillyVal = 3;
	std::unordered_map<int, ECBPolyLine> lineMap;
	short materialID = 0;			// is the triangle dirt, stone, snow, wood, etc
	short isPolyPerfectlyClamped = 0;					// determines if the entire polygon is perfectly clamped to x, y, or z azis
	ECBPolyPoint mrp;
	ECBPolyPoint emptyNormal;
};

#endif

