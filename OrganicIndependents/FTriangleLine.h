#pragma once

#ifndef FTRIANGLELINE_H
#define FTRIANGLELINE_H

#include "FTriangleLineType.h"
#include <iomanip>
#include <sstream>
#include "FTrianglePoint.h"

/*

Description:

An FTriangleLine is produced by the production functions of a class
that is derived from the DimensionalLineScannerBase class.

For example, the function ZDimLineScanner::produceZSliceLines (from a class derived from DimensionalLineScannerBase)
will gather all the metadata needed for a ZSliceLine. The ZSliceLine's retrieveProducedTargets() function
will then produce all the necessary keys that the FTriangleLine will then go into.

*/

class FTriangleLine
{
	public:
		FTriangleLine() {};


		FTriangleLine(FTrianglePoint in_fPointA, FTrianglePoint in_fPointB, FTriangleLineType in_lineType) :
			lineType(in_lineType)
		{
			pointA = in_fPointA;
			pointB = in_fPointB;
		}


		// self-comparison operator; will return true if the points of both lines match.
		bool operator==(const FTriangleLine& in_otherLine) const	// const is required at end here for C++_20
		{
			bool doLinesMatch = false;
			return 
			(
				((pointA == in_otherLine.pointA) && (pointB == in_otherLine.pointB))
				||
				((pointA == in_otherLine.pointB) && (pointB == in_otherLine.pointA))
			);
		}

		FTriangleLineType getType();
		void printLine();
		std::string printLineToString();
		void swapPoints();
		bool doesPointExist(glm::vec3 in_point);
		void translateLine(DoublePoint in_translationValue);

		FTrianglePoint pointA;
		FTrianglePoint pointB;
	private:
		FTriangleLineType lineType = FTriangleLineType::NONE;
};

#endif
