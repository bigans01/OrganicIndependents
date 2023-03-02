#pragma once

#ifndef BLUEPRINTFRACTURINGMACHINE_H
#define BLUEPRINTFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
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
		void calibrateFTriangleLineAndScannerOREKeys();			// Step 2:  calibrate the ORE origin keys.
		void loadLocalizedBlueprintPoints();	// Step 3:  directly load the DoublePoint > ECBPolyPoint type conversions into the localizedFTrianglePoints array.
												//			The localizedFTrianglePoints is used by other functions to load point data.
		void runBlueprintTracing();				// Step 4:  create exterior FTriangleLines.

		void reverseTranslateBlueprintStagerLines();	// Step 9: Reverse translate stager lines, to localized style, so that they are compatible with the 0.0f to 4.0f boundaries of an ORE.

		void buildBlueprintMachineTriangleContainers();	// Step 10: Generate the FTriangleOutputs (the value of destinedGridType in all of these should be FTriangletype::ORE)

		EnclaveKeyDef::EnclaveKey getUncalibratedOREKeyForPoint(DoublePoint in_point);	// A utility function for this class, that determines uncalibrate ORE keys of a point.

};

#endif