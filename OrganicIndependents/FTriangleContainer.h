#pragma once

#ifndef FTRIANGLECONTAINER_H
#define FTRIANGLECONTAINER_H

#include "FTriangleLine.h"
#include "FTriangleType.h"
#include "PerfectClampEnum.h"
#include "FTriangleOutput.h"

/*

Description:

This class is meant to contain one or more FTriangleOutput instances.

In the produceFTriangles function, the parentEmptyNormal is used to organize the points of an FTriangleOutput in a way, 
such that the signage of the empty normal in the FTriangleOutput matches the signage
of the passed in empty normal (in_parentEmptyNormal).

The in_parentOrientation parameter of that same function, the passed in EnclaveKey, and the passed in
FTriangleType are used to determine if the points of the FTriangle are aligned in such a way
that they need a value for the BoundaryOrientation. The FTriangleOutput's orientation can then
be compared to the FTriangle's BoundaryOrientation, to see if they are different, allowing us
to check for anomalies.

*/

class FTriangleContainer
{
	public:
		std::map<int, FTriangleOutput> fracturedTriangles;
		void insertConstructionLines(std::vector<FTriangleLine> in_constructionLines);
		void printConstructionLines();
		void produceFTriangles(FTriangleType in_productionType,
								ECBPolyPoint in_parentEmptyNormal,
								BoundaryOrientation in_parentOrientation,
								EnclaveKeyDef::EnclaveKey in_containerBounds);
	private:
		std::vector<FTriangleLine> constructionLines;

		struct OutputTriangleFrame
		{
			OutputTriangleFrame() {};
			OutputTriangleFrame(DoublePoint in_pointA,
								DoublePoint in_pointB, 
								DoublePoint in_pointC)
			{
				framePoints[0] = in_pointA;
				framePoints[1] = in_pointB;
				framePoints[2] = in_pointC;
			}

			DoublePoint framePoints[3];
		};
};

#endif
