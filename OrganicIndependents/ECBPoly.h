#pragma once

#ifndef ECBPOLYLINE_NEW_H
#define ECBPOLYLINE_NEW_H

#include "ECBPolyLine.h"
#include "ECBPolyType.h"


class ECBPoly
{
public:
	ECBPoly() {};
	ECBPoly(ECBPolyType in_polyType) : polyType(in_polyType) {};
	ECBPolyType polyType = ECBPolyType::FREE;
	std::unordered_map<int, ECBPolyLine> lineMap;
	short materialID = 0;			// is the triangle dirt, stone, snow, wood, etc
	short isPolyPerfectlyClamped = 0;					// determines if the entire polygon is perfectly clamped to x, y, or z azis
	ECBPolyPoint mrp;
	ECBPolyPoint emptyNormal;

	void printLineData()
	{
		std::cout << "(ECBPoly): printing ECBPolyLine data..." << std::endl;
		for (int x = 0; x < 3; x++)
		{
			std::cout << "Line " << x << ": " << std::endl;
			std::cout << "pointA: " << lineMap[x].pointA.x << ", " << lineMap[x].pointA.y << ", " << lineMap[x].pointA.z << std::endl;
			std::cout << "pointB: " << lineMap[x].pointB.x << ", " << lineMap[x].pointB.y << ", " << lineMap[x].pointB.z << std::endl;
			std::cout << "pointC: " << lineMap[x].pointC.x << ", " << lineMap[x].pointC.y << ", " << lineMap[x].pointC.z << std::endl;
		}
	};
};

#endif

