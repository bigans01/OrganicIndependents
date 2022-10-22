#include "stdafx.h"
#include "YFRayCastQuad.h"

void YFRayCastQuad::buildAndCastRays()
{
	int xLength = quadDimOneMax - quadDimOneMin;
	int zLength = quadDimTwoMax - quadDimTwoMin;

	// current dims always start at the minimum.
	float currentXLocation = IndependentUtils::roundToHundredth(quadDimOneMin * quadRayCastInterval);
	float currentZLocation = IndependentUtils::roundToHundredth(quadDimTwoMin * quadRayCastInterval);
	float currentZLocationBase = currentZLocation;

	// determine the max float value for Y
	float minYDimValue = IndependentUtils::roundToHundredth(quadTargetRaycastDimMin * quadRayCastInterval);
	float maxYDimValue = IndependentUtils::roundToHundredth((quadTargetRaycastDimMax * quadRayCastInterval) + quadRayCastInterval);

	// do a nested loop; for each X, go through all Z.
	// We must also make sure to reset the currentZLocation to its min value, every time we iterate in the x loop.
	for (int x = 0; x < xLength; x++)
	{
		currentXLocation += quadRayCastInterval;	// always up the X, then round.
		currentXLocation = IndependentUtils::roundToHundredth(currentXLocation);

		currentZLocation = currentZLocationBase;	// ...and reset the Z back to it's base value.  
													// The Z will always get incremented in the inner loop below.

		for (int z = 0; z < zLength; z++)
		{
			currentZLocation += quadRayCastInterval;
			currentZLocation = IndependentUtils::roundToHundredth(currentZLocation);
			std::cout << "YFRayCast built: X -> " << currentXLocation << " | Z -> " << currentZLocation << " | Min Y: -> " << minYDimValue << " | Max Y: -> " << maxYDimValue << std::endl;

			YFRay currentYFRay(currentXLocation, currentZLocation, minYDimValue, maxYDimValue);
			yFRayVector.push_back(currentYFRay);
		}
	}

	// finally, cycle through each element in the vector and run the ray through a FRayInterceptionMachine, with
	// the appropriate interception type (ROUND_FOR_Y)
	for (auto& currentYFRay : yFRayVector)
	{
		auto currentRayPair = currentYFRay.getRayPoints();
		FRayIntersectionMachine currentMachine(currentRayPair, targetTrianglePoints, quadPointContainerRef, FRayIntersectionRoundingMode::ROUND_FOR_Y);
	}
}