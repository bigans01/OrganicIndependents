#include "stdafx.h"
#include "FTriangleLine.h"

FTriangleLineType FTriangleLine::getType()
{ 
	return lineType; 
}

void FTriangleLine::printLine()
{
	std::cout << " pointA -> " << pointA.point.x << ", " << pointA.point.y << ", " << pointA.point.z
		<< "| pointB -> " << pointB.point.x << ", " << pointB.point.y << ", " << pointB.point.z;

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

std::string FTriangleLine::printLineToString()
{
	// Format the floats of the points, to make them more readable.
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2)
		<< " pointA -> " << pointA.point.x << ", " << pointA.point.y << ", " << pointA.point.z
		<< " | pointB -> " << pointB.point.x << ", " << pointB.point.y << ", " << pointB.point.z;

	std::string lineTypeString = "";
	switch (lineType)
	{
	case FTriangleLineType::EXTERIOR: { lineTypeString = "EXTERIOR"; break; };
	case FTriangleLineType::XSLICE_SCANNED: { lineTypeString = "XSLICE_SCANNED"; break; };
	case FTriangleLineType::YSLICE_SCANNED: { lineTypeString = "YSLICE_SCANNED"; break; };
	case FTriangleLineType::ZSLICE_SCANNED: { lineTypeString = "ZSLICE_SCANNED"; break; };
	}

	stream << " | linetType -> " << lineTypeString;

	return stream.str();
}

void FTriangleLine::swapPoints()
{
	//DoublePoint pointATemp = pointA;
	FTrianglePoint pointATemp = pointA;
	pointA = pointB;
	pointB = pointATemp;
}

bool FTriangleLine::doesPointExist(glm::vec3 in_point)
{
	bool exists = false;

	if
	(
		(pointA == in_point)
		||
		(pointB == in_point)
	)
	{
		exists = true;
	}
	return exists;
}

void FTriangleLine::translateLine(DoublePoint in_translationValue)
{
	pointA.point += in_translationValue;
	pointB.point += in_translationValue;
	pointA.point.roundHundredth();
	pointB.point.roundHundredth();
}