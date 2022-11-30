#pragma once

#ifndef ENCLAVETRIANGLESKELETON_H
#define ENCLAVETRIANGLESKELETON_H

#include "EnclaveTriangle.h"
#include "TriangleLine.h"

class IndependentUtils;
class EnclaveTriangleSkeleton
{
public:
	ECBPolyPoint points[3];
	TriangleMaterial materialID = TriangleMaterial::NOVAL;			// is the triangle dirt, stone, snow, wood, etc
	short isPolyPerfectlyClamped = 0;					// determines if the entire polygon is perfectly clamped to x, y, or z azis
	//ECBPolyPoint mrp;				// probably not needed, but comment out just in case.
	ECBPolyPoint emptyNormal;
	BoundaryPolyIndicator skeletonBoundaryIndicatorValue;
	EnclaveTriangle inflateToEnclaveTriangle()
	{
		EnclaveTriangle returnTriangle;
		TriangleLine line1, line2, line3;

		ECBPolyPoint roundedPoint1 = IndependentUtils::roundPolyPointToHundredths(points[0]);
		ECBPolyPoint roundedPoint2 = IndependentUtils::roundPolyPointToHundredths(points[1]);
		ECBPolyPoint roundedPoint3 = IndependentUtils::roundPolyPointToHundredths(points[2]);

		line1.pointA = roundedPoint1;
		line1.pointB = roundedPoint2;

		line2.pointA = roundedPoint2;
		line2.pointB = roundedPoint3;

		line3.pointA = roundedPoint3;
		line3.pointB = roundedPoint1;

		returnTriangle.lineArray[0] = PolyUtils::convertToECBPolyLine(line1, roundedPoint3);
		returnTriangle.lineArray[1] = PolyUtils::convertToECBPolyLine(line2, roundedPoint1);
		returnTriangle.lineArray[2] = PolyUtils::convertToECBPolyLine(line3, roundedPoint2);

		returnTriangle.enclaveTriangleMaterialID = materialID;
		returnTriangle.isEnclaveTrianglePolyPerfectlyClamped = isPolyPerfectlyClamped;
		returnTriangle.emptyNormal = emptyNormal;
		returnTriangle.enclaveTriangleBoundaryPolyIndicator = skeletonBoundaryIndicatorValue;

		return returnTriangle;
	};

	void printSkeletonTriangle()
	{
		std::cout << "Material: " << int(materialID) << " | Empty normal: "; emptyNormal.printPointCoords();
		for (int x = 0; x < 3; x++)
		{
			std::cout << " | point " << x << ": ";
			points[x].printPointCoords();
		}
		std::cout << std::endl;
	}
};

#endif
