#include "stdafx.h"
#include "WorldFracturingMachine.h"

void WorldFracturingMachine::runFracturing()
{
	determineUncalibratedBlueprintKeys();	// Step 1: get the uncalibrated keys.
	calibrateOriginBlueprintKeys();			// Step 2: calibrate the keys.
	translateTriangleByBlueprintKeys();		// first, do any required translations.
}

void WorldFracturingMachine::determineUncalibratedBlueprintKeys()
{
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = IndependentUtils::getUncalibratedBlueprintKeyForPoint(originFTrianglePoints[x]);
	}
}

void WorldFracturingMachine::calibrateOriginBlueprintKeys()
{

}

void WorldFracturingMachine::translateTriangleByBlueprintKeys()
{

}
