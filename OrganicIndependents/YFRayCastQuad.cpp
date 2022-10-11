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
			std::cout << "YFRayCast built: X -> " << currentXLocation << " | Z -> " << currentZLocation << std::endl;
		}
	}
}