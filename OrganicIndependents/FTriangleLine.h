#pragma once

#ifndef FTRIANGLELINE_H
#define FTRIANGLELINE_H

#include "ECBPolyPoint.h"
#include "FTriangleLineType.h"

class FTriangleLine
{
	public:
		FTriangleLine() {};
		FTriangleLine(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, FTriangleLineType in_lineType) :
			pointA(in_pointA),
			pointB(in_pointB),
			lineType(in_lineType)
		{};
	private:
		ECBPolyPoint pointA;
		ECBPolyPoint pointB;
		FTriangleLineType lineType = FTriangleLineType::NONE;
};

#endif
