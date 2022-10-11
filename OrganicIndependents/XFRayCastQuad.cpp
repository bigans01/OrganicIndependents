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
			std::cout << "XFRayCast built: Y -> " << currentYLocation << " | Z -> " << currentZLocation << std::endl;
		}
	}
}