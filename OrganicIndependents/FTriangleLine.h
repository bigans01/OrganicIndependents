#pragma once

#ifndef FTRIANGLELINE_H
#define FTRIANGLELINE_H

#include "FTriangleLineType.h"

class FTriangleLine
{
	public:
		FTriangleLine() {};
		FTriangleLine(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, FTriangleLineType in_lineType) :
			lineType(in_lineType)
		{
			pointA = IndependentUtils::convertECBPolyPointToVec3(in_pointA);
			pointB = IndependentUtils::convertECBPolyPointToVec3(in_pointB);
		};

		FTriangleLine(glm::vec3 in_pointA, glm::vec3 in_pointB, FTriangleLineType in_lineType) :
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
		void printLine()
		{
			std::cout << " pointA -> " << pointA.x << ", " << pointA.y << ", " << pointA.z
					<< "| pointB -> " << pointB.x << ", " << pointB.y << ", " << pointB.z;

			std::string lineTypeString = "";
			switch (lineType)
			{
				case FTriangleLineType::EXTERIOR: { lineTypeString = "EXTERIOR"; break; };
				case FTriangleLineType::XSLICE_SCANNED: { lineTypeString = "XSLICE_SCANNED"; break; };
				case FTriangleLineType::YSLICE_SCANNED: { lineTypeString = "YSLICE_SCANNED"; break; };
				case FTriangleLineType::ZSLICE_SCANNED: { lineTypeString = "ZSLICE_SCANNED"; break; };
			}
			std::cout << " | linetType -> " << lineTypeString << std::endl;
		}


		glm::vec3 pointA;
		glm::vec3 pointB;
	private:
		FTriangleLineType lineType = FTriangleLineType::NONE;
};

#endif
