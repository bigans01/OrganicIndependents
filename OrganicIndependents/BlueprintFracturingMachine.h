#pragma once

#ifndef BLUEPRINTFRACTURINGMACHINE_H
#define BLUEPRINTFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
#include "CalibrateableOREKeyPair.h"
#include "FTriangleBlueprintTracer.h"

/*

Description: 

Instantiated by an FTriangle instance that has an FTriangleType::BLUEPRINT after
the FTriangle's fracture() function is called. Once instantiated and set up, the runFracturing() function
of this class needs to be called to produce the output. Reference the function FTriangle::setupAndRunFracturerMachine()
for more details.

All FTriangleOutput instances produced by this must have an FTriangleType::ORE.

*/

class BlueprintFracturingMachine : public FTriangleFracturerBase
{
	public:
		void runFracturing();
	private:
		void determineUncalibratedOREKeys();	// Step 1:  find the uncalibrated ORE keys.
		void calibrateOriginOREKeys();			// Step 2:  calibrate the ORE origin keys.
		void loadLocalizedBlueprintPoints();	// Step 3:  directly load the DoublePoint > ECBPolyPoint type conversions into the localizedFTrianglePoints array.
												//			The localizedFTrianglePoints is used by other functions to load point data.
		void runBlueprintTracing();				// Step 4:  create exterior FTriangleLines.
		void buildAndRunBlueprintFRayCasters();	// Step 5:  determine the FRayCasters that this machine will have to use

		EnclaveKeyDef::EnclaveKey getUncalibratedOREKeyForPoint(DoublePoint in_point);	// A utility function for this class, that determines uncalibrate ORE keys of a point.

};

#endif