#include "stdafx.h"
#include "EnclaveTriangle.h"
#include "PrimaryLineT1Array.h"

EnclaveTriangle::EnclaveTriangle()
{

}

EnclaveTriangle::EnclaveTriangle(FTriangleOutput in_fTriangleOutput)
{
	TriangleLine line1, line2, line3;

	// rounding points isn't necessary, as it should already be done with an FTriangle.
	ECBPolyPoint roundedPoint1 = convertDoublePointToECBPolyPointForEnclaveTriangle(in_fTriangleOutput.fracturePoints[0]);
	ECBPolyPoint roundedPoint2 = convertDoublePointToECBPolyPointForEnclaveTriangle(in_fTriangleOutput.fracturePoints[1]);
	ECBPolyPoint roundedPoint3 = convertDoublePointToECBPolyPointForEnclaveTriangle(in_fTriangleOutput.fracturePoints[2]);

	line1.pointA = roundedPoint1;
	line1.pointB = roundedPoint2;

	line2.pointA = roundedPoint2;
	line2.pointB = roundedPoint3;

	line3.pointA = roundedPoint3;
	line3.pointB = roundedPoint1;

	bool isValid1 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line1, roundedPoint3, roundedPoint1, roundedPoint2, roundedPoint3);
	bool isValid2 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line2, roundedPoint1, roundedPoint1, roundedPoint2, roundedPoint3);
	bool isValid3 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line3, roundedPoint2, roundedPoint1, roundedPoint2, roundedPoint3);

	// add each line to the triangle
	//lineArray[0] = PolyUtils::convertToECBPolyLine(line1, roundedPoint3);		// line consists of points 1 and 2, so third = point 3
	//lineArray[1] = PolyUtils::convertToECBPolyLine(line2, roundedPoint1);		// line consists of points 2 and 3, so third = point 1
	//lineArray[2] = PolyUtils::convertToECBPolyLine(line3, roundedPoint2);		// line consists of points 3 and 1, so third = point 2

	points[0] = roundedPoint1;
	points[1] = roundedPoint2;
	points[2] = roundedPoint3;

	enclaveTriangleMaterialID = in_fTriangleOutput.outputTriangleMaterial;
	isEnclaveTrianglePolyPerfectlyClamped = in_fTriangleOutput.fractureRequiredClampValue;

	enclaveTriangleBoundaryPolyIndicator.setBoundaryIndicator(in_fTriangleOutput.fractureRequiredOrientation);

	emptyNormal = in_fTriangleOutput.fractureEmptyNormal;
}

Operable3DEnclaveKeySet EnclaveTriangle::executeRun(BlockBorderLineList* in_blockBorderLineList, 
								EnclaveKeyDef::EnclaveKey in_key, 
								bool in_badRunFlag)
{
	Operable3DEnclaveKeySet badKeySet;
	// NEW method for producing fan data starts here (from 3/29/2023)
	/*
	OrganicTriangleTertiary tertiaryProducer(lineArray[0].pointA,
		lineArray[1].pointB,
		lineArray[2].pointC,
		emptyNormal,
		enclaveTriangleBoundaryPolyIndicator.getBoundaryIndicatorValue(),
		isEnclaveTrianglePolyPerfectlyClamped,
		enclaveTriangleMaterialID);
	*/

	/*
	OrganicTriangleTertiary tertiaryProducer(lineArray[0].pointA,
		lineArray[1].pointA,
		lineArray[2].pointA,
		emptyNormal,
		enclaveTriangleBoundaryPolyIndicator.getBoundaryIndicatorValue(),
		isEnclaveTrianglePolyPerfectlyClamped,
		enclaveTriangleMaterialID);
	*/

	/*
	std::cout << "::: Point values, prior to tertiaryProducer:" << std::endl;
	std::cout << "lineArray[0], pointA: "; lineArray[0].pointA.printPointCoords(); std::cout << std::endl;
	std::cout << "points[0]: "; points[0].printPointCoords(); std::cout << std::endl;
	std::cout << "lineArray[1], pointB: "; lineArray[1].pointB.printPointCoords(); std::cout << std::endl;
	std::cout << "points[1]: "; points[1].printPointCoords(); std::cout << std::endl;
	std::cout << "lineArray[2], pointC: "; lineArray[2].pointC.printPointCoords(); std::cout << std::endl;
	std::cout << "points[2]: "; points[2].printPointCoords(); std::cout << std::endl;
	int waitVal = 3;
	std::cin >> waitVal;
	*/
	
	/*
	for (int x = 0; x < 3; x++)
	{
		if (!(lineArray[x].pointA == points[x]))
		{
			std::cout << "!!! NOTICE: points prior to executeRun were not equal; values are: " << std::endl;
			std::cout << "lineArray[" << x << "]: "; lineArray[x].pointA.printPointCoords(); std::cout << std::endl;
			std::cout << "points[" << x << "]: "; points[x].printPointCoords(); std::cout << std::endl;
			int nonMatchWait = 3;
			std::cin >> nonMatchWait;
		}
	}
	*/

	
	OrganicTriangleTertiary tertiaryProducer(points[0],
		points[1],
		points[2],
		emptyNormal,
		enclaveTriangleBoundaryPolyIndicator.getBoundaryIndicatorValue(),
		isEnclaveTrianglePolyPerfectlyClamped,
		enclaveTriangleMaterialID);
	

	enclaveTriangleTertiary = tertiaryProducer;
	badKeySet = tertiaryProducer.incalculableAttempts;
	return badKeySet;
}

/*
void EnclaveTriangle::executeRunDebug(BlockBorderLineList* in_blockBorderLineList, 
									BorderDataMap* in_borderDataMap, 
									EnclaveKeyDef::EnclaveKey in_key)
{
	
}


/*
void EnclaveTriangle::reform(ECBPolyPoint in_polyPoint0, ECBPolyPoint in_polyPoint1, ECBPolyPoint in_polyPoint2)
{
	TriangleLine line1, line2, line3;

	line1.pointA = in_polyPoint0; // roundedPoint1;
	line1.pointB = in_polyPoint1; // roundedPoint2;

	line2.pointA = in_polyPoint1; // roundedPoint2;
	line2.pointB = in_polyPoint2; // roundedPoint3;

	line3.pointA = in_polyPoint2; // roundedPoint3;
	line3.pointB = in_polyPoint0; // roundedPoint1;

	lineArray[0] = PolyUtils::convertToECBPolyLine(line1, in_polyPoint2);	// roundedPoint3
	lineArray[1] = PolyUtils::convertToECBPolyLine(line2, in_polyPoint0);	// roundedPoint1
	lineArray[2] = PolyUtils::convertToECBPolyLine(line3, in_polyPoint1);	// roundedPoint2

	// everything else -- enclaveTriangleMaterialID, clamping, etc...stays the same; we're only reforming the points.
}
*/

void EnclaveTriangle::printPoints()
{
	for (int x = 0; x < 3; x++)
	{
		//std::cout << x << ": " << lineArray[x].pointA.x << ", " << lineArray[x].pointA.y << ", " << lineArray[x].pointA.z << std::endl;
		//std::cout << x << ": " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
	}
}


void EnclaveTriangle::printBlockKeys()
{
	auto blocksBegin = enclaveTriangleTertiary.triangleMap.begin();
	auto blocksEnd = enclaveTriangleTertiary.triangleMap.end();
	for (; blocksBegin != blocksEnd; blocksBegin++)
	{
		auto currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
		std::cout << "Found block key: "; currentKey.printKey(); std::cout << std::endl;
	}
}

bool EnclaveTriangle::doBlocksExistAtY(int in_y)
{
	bool blocksExistAtY = false;
	auto existingTertiaryBlocksBegin = enclaveTriangleTertiary.triangleMap.begin();
	auto existingTertiaryBlocksEnd = enclaveTriangleTertiary.triangleMap.end();
	for (; existingTertiaryBlocksBegin != existingTertiaryBlocksEnd; existingTertiaryBlocksBegin++)
	{
		EnclaveKeyDef::EnclaveKey convertedKey = PolyUtils::convertSingleToBlockKey(existingTertiaryBlocksBegin->first);
		if (convertedKey.y == in_y)
		{
			std::cout << "! Found key at y " << in_y; convertedKey.printKey(); std::cout << std::endl;
			blocksExistAtY = true;
		}
	}
	return blocksExistAtY;
}

ECBPolyPoint EnclaveTriangle::convertDoublePointToECBPolyPointForEnclaveTriangle(DoublePoint in_doublePointToConvert)
{
	return ECBPolyPoint(float(in_doublePointToConvert.x),
		float(in_doublePointToConvert.y),
		float(in_doublePointToConvert.z));
}

