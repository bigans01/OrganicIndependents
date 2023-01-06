#pragma once

#ifndef WORLDFRACTURINGMACHINE_H
#define WORLDFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
#include "FTriangleWorldTracer.h"

/*

Description:

Instantiated by an FTriangle instance that has an FTriangleType::WORLD after
the FTriangle's fracture() function is called. Once instantiated and set up, the runFracturing() function
of this class needs to be called to produce the output. Reference the function FTriangle::setupAndRunFracturerMachine()
for more details.

This class is the only class that should have to translate an OSContouredTriangle into
near-0 coordinates (but it will still have to reverse translate like all the others)

All FTriangleOutput instances produced by this must have an FTriangleType::BLUEPRINT.

*/

class WorldFracturingMachine : public FTriangleFracturerBase
{
	public:
		void runFracturing();
	private:
		void determineUncalibratedBlueprintKeys();	// Step 1: find the uncalibrated blueprint keys.
		void calibrateOriginBlueprintKeys();		// Step 2: calibrate the blueprint origin keys.

		// Below: because we are working in world-space to produce triangles bound in a blueprint, and because there is no X/Y/Z limit, 
		// we should translate the triangle to a more manageable range (somewhere around 0,0,0). To do this, we will get the min X/Y/Z 
		// blueprint coords, and translate by that. The translateTriangle function will do this. This will also make debugging easier.
		void translateTriangleByBlueprintKeys();	// Step 3: do any required translations, to where precision loss doesn't occur.
		void loadLocalizedPoints();					// Step 4: after the points have been adjusted via translation, these new values will be the localized values;
													//		   we will have to put those localized values into the localizedFTrianglePoints array (see base class)

		void runWorldTracing();						// Step 5: create exterior FTriangleLines.
		void buildAndRunWorldFRayCasters();			// Step 6: determine the FRayCasters that this machine will have to use
		void buildAndRunFLineScanners();			// Step 7: build the line scanners, and run them.

													// Step 8: See base class function, analyzeAndCleanupStagers()											

		void reverseTranslateStagerLines();			// Step 9: apply the appropriate translation to stager lines; uses the inverse of the translationKey

		void buildWorldMachineTriangleContainers();	// Step 10: build the FTriangleContainers, according to what this class specifies.

		// translation assisting functions.
		void adjustBlueprintKeysByValue(EnclaveKeyDef::EnclaveKey in_adjustingKey);
		void adjustPointsByValue(DoublePoint in_adjustingPoint);	// DoublePoint is needed for dealing 
																	// with potentially large values in an OSContouredTriangle (see OrganicServerLib)

		EnclaveKeyDef::EnclaveKey translationKey;	// the key that is used to translate the calibrated keys and points to near 0,0,0;
													// the value of this key is equivalent to the inverse of an EnclaveKey which contains the 
													// minimum values for all dimensions. For example, if the min values are 1, 2, 3 for x/y/z
													// respectively, this key will be -1, -2, -3. Its value is determined by the function WorldFracturingMachine::translateTriangleByBlueprintKeys().


		FTriangleReverseTranslationMode translationMode = FTriangleReverseTranslationMode::ABSOLUTE_TRANSLATE;	// the value used by the function reverseTranslateStagerLines()
																										// to determine the method of reverse translation.
};

#endif
