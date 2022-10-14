#include "stdafx.h"
#include "ZFRayCastQuad.h"

void ZFRayCastQuad::buildAndCastRays()
{
	int xLength = quadDimOneMax - quadDimOneMin;
	int yLength = quadDimTwoMax - quadDimTwoMin;

	// current dims always start at the minimum
	float currentXLocation = IndependentUtils::roundToHundredth(quadDimOneMin * quadRayCastInterval);
	float currentYLocation = IndependentUtils::roundToHundredth(quadDimTwoMin * quadRayCastInterval);
	float currentYLocationBase = currentYLocation;

	// determine the max float value for Z
	float minZDimValue = IndependentUtils::roundToHundredth(quadTargetRaycastDimMin * quadRayCastInterval);
	float maxZDimValue = IndependentUtils::roundToHundredth((quadTargetRaycastDimMax * quadRayCastInterval) + quadRayCastInterval);

	// do a nested loop; for each X, go through all Y.
	// We must also make sure to reset the currentYLocation to its min value, every time we iterate in the x loop.
	for (int x = 0; x < xLength; x++)
	{
		currentXLocation += quadRayCastInterval;	// always up the X, then round.
		currentXLocation = IndependentUtils::roundToHundredth(currentXLocation);

		currentYLocation = currentYLocationBase;	// ...and reset the Y back to it's base value.
													// The Y will always get incremented in the inner loop below.

		for (int y = 0; y < yLength; y++)
		{
			currentYLocation += quadRayCastInterval;
			currentYLocation = IndependentUtils::roundToHundredth(currentYLocation);
			std::cout << "ZFRayCast built: X -> " << currentXLocation << " | Y -> " << currentYLocation << " | Min Z: -> " << minZDimValue <<  " | Max Z: -> " << maxZDimValue << std::endl;

			ZFRay currentZFRay(currentXLocation, currentYLocation, minZDimValue, maxZDimValue);
			zFRayVector.push_back(currentZFRay);
		}
	}

	// finally, cycle through each element in the vector and run the ray through a FRayInterceptionMachine, with
	// the appropriate interception type (ROUND_FOR_Z)
	for (auto& currentZFRay : zFRayVector)
	{
		auto currentRayPair = currentZFRay.getRayPoints();
		FRayInterceptionMachine currentMachine(currentRayPair, targetTrianglePoints, quadPointContainerRef, FRayInterceptionType::ROUND_FOR_Z);
	}
}