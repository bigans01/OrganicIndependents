#include "stdafx.h"
#include "EnclaveTriangleSkeleton.h"

EnclaveTriangle EnclaveTriangleSkeleton::inflateToEnclaveTriangle()
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

	//returnTriangle.lineArray[0] = PolyUtils::convertToECBPolyLine(line1, roundedPoint3);
	//returnTriangle.lineArray[1] = PolyUtils::convertToECBPolyLine(line2, roundedPoint1);
	//returnTriangle.lineArray[2] = PolyUtils::convertToECBPolyLine(line3, roundedPoint2);

	returnTriangle.points[0] = roundedPoint1;
	returnTriangle.points[1] = roundedPoint2;
	returnTriangle.points[2] = roundedPoint3;

	returnTriangle.enclaveTriangleMaterialID = materialID;
	returnTriangle.isEnclaveTrianglePolyPerfectlyClamped = isPolyPerfectlyClamped;
	returnTriangle.emptyNormal = emptyNormal;
	returnTriangle.enclaveTriangleBoundaryPolyIndicator = skeletonBoundaryIndicatorValue;

	return returnTriangle;
};

void EnclaveTriangleSkeleton::printSkeletonTriangle()
{
	std::cout << "Material: " << int(materialID) << " | Empty normal: "; emptyNormal.printPointCoords();
	for (int x = 0; x < 3; x++)
	{
		std::cout << " | point " << x << ": ";
		points[x].printPointCoords();
	}
	std::cout << std::endl;
}

Message EnclaveTriangleSkeleton::convertETSkeletonToMessage()
{
	Message convertedMessage;

	// first: add the 3 points.
	for (int x = 0; x < 3; x++)
	{
		convertedMessage.insertPoint(points[x]);
	}

	// next: add the materialID.
	convertedMessage.insertInt(int(materialID));

	// next: add the perfect clamp enum value.
	convertedMessage.insertInt(int(isPolyPerfectlyClamped));

	// next: add the empty normal.
	convertedMessage.insertPoint(emptyNormal);

	// next: convert te unsigned char of the BoundaryPolyIndicator value, to int, and put that in.
	convertedMessage.insertInt(int(skeletonBoundaryIndicatorValue.getIndicatorData()));



	return convertedMessage;
}