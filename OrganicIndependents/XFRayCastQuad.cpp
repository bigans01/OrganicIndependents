#include "stdafx.h"
#include "XFRayCastQuad.h"

void XFRayCastQuad::buildAndCastRays()
{
	int yLength = quadDimOneMax - quadDimOneMin;
	int zLength = quadDimTwoMax - quadDimTwoMin;

	// current dims always start at the minimum.
	float currentYLocation = IndependentUtils::roundToHundredth(quadDimOneMin * quadRayCastInterval);
	float currentZLocation = IndependentUtils::roundToHundredth(quadDimTwoMin * quadRayCastInterval);
	float currentZLocationBase = currentZLocation;

	// determine the max float value for X
	float minXDimValue = IndependentUtils::roundToHundredth(quadTargetRaycastDimMin * quadRayCastInterval);
	float maxXDimValue = IndependentUtils::roundToHundredth((quadTargetRaycastDimMax * quadRayCastInterval) + quadRayCastInterval);

	// do a nested loop; for each Y, go through all Z. 
	// We must also make sure to reset the currentZLocation to its min value, every time we iterate in the y loop.
	for (int y = 0; y < yLength; y++)
	{
		currentYLocation += quadRayCastInterval;	// always up the Y, and round.
		currentYLocation = IndependentUtils::roundToHundredth(currentYLocation);

		currentZLocation = currentZLocationBase;	// ...and reset the Z back to it's base value. 
													// The Z will always get incremented in the inner loop below.

		for (int z = 0; z < zLength; z++)
		{
			currentZLocation += quadRayCastInterval;
			currentZLocation = IndependentUtils::roundToHundredth(currentZLocation);
			std::cout << "XFRayCast built: Y -> " << currentYLocation << " | Z -> " << currentZLocation << " | Min X: -> " << minXDimValue <<  " | Max X: -> " << maxXDimValue << std::endl;

			XFRay currentXFRay(currentYLocation, currentZLocation, minXDimValue, maxXDimValue);
			xFRayVector.push_back(currentXFRay);
		}
	}

	// finally, cycle through each element in the vector and run the ray through a FRayInterceptionMachine, with
	// the appropriate interception type (ROUND_FOR_X)
	for (auto& currentXFRay : xFRayVector)
	{
		auto currentRayPair = currentXFRay.getRayPoints();
		FRayIntersectionMachine currentMachine(currentRayPair, targetTrianglePoints, quadPointContainerRef, FRayIntersectionRoundingMode::ROUND_FOR_X);
	}
}

ECBPolyPointPair XFRayCastQuad::XFRay::getRayPoints()
{
	return ECBPolyPointPair(ECBPolyPoint(minX, yCoord, zCoord),
							ECBPolyPoint(maxX, yCoord, zCoord));
}