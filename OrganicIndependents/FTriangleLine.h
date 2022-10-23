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

		// self-comparison operator; will return true if the points of both lines match.
		bool operator==(const FTriangleLine& in_otherLine)
		{
			bool doLinesMatch = false;
			return 
			(
				((pointA == in_otherLine.pointA) && (pointB == in_otherLine.pointB))
				||
				((pointA == in_otherLine.pointB) && (pointB == in_otherLine.pointA))
			);
		}

		FTriangleLineType getType() { return lineType; }

	private:
		ECBPolyPoint pointA;
		ECBPolyPoint pointB;
		FTriangleLineType lineType = FTriangleLineType::NONE;
};

#endif
