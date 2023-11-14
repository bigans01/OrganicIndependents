#pragma once

#ifndef WORLDTEXTUREFRACTURINGMACHINE_H
#define WORLDTEXTUREFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
#include "FTriangleWorldTracer.h"

/*

Description:

Instantiated by an FTriangle instance that has an FTriangleType::TEXTURE_WORLD after
the FTriangle's fracture() function is called. Once instantiated and set up, the runFracturing() function
of this class needs to be called to produce the output. The core difference between this class and
the WorldFracturingMachine class is that this fracturing class is designed to take the original FTrianglePoint objects used
in the fracturing process (assuming those points have UV values), and have the outputs of this FTriangle contain
the appropriately scaled UV values.

Reference the function FTriangle::setupAndRunFracturerMachine()
for more details.

Whichever world-contour class (such as OSContouredTriangle) utilizes this one, that class
will have to take into account that this class will translate those values to near-0 coordinates 
(but it will still have to reverse translate like all the others)

All FTriangleOutput instances produced by this must have an FTriangleType::BLUEPRINT.

*/

class WorldTextureFracturingMachine : public FTriangleFracturerBase
{
	public:
		void setPointTranslationMode(FTriangleReverseTranslationMode in_translationModeToRunAs);
		void runFracturing();
	private:
		void determineUncalibratedBlueprintKeys();	// Step 1: find the uncalibrated blueprint keys.
		void calibrateFTriangleLineAndScannerBlueprintKeys();		// Step 2: calibrate the blueprint origin keys.

		// Below: because we are working in world-space to produce triangles bound in a blueprint, and because there is no X/Y/Z limit, 
		// we should translate the triangle to a more manageable range (somewhere around 0,0,0). To do this, we will get the min X/Y/Z 
		// blueprint coords, and translate by that. The translateTriangle function will do this. This will also make debugging easier.
		void translateTriangleByBlueprintKeys();	// Step 3: do any required translations, to where precision loss doesn't occur.
		void loadLocalizedPoints();					// Step 4: after the points have been adjusted via translation, these new values will be the localized values;
													//		   we will have to put those localized values into the localizedFTrianglePoints array (see base class)

		void runWorldTracing();						// Step 5: create exterior FTriangleLines.

													// Step 9: See base class function, analyzeAndCleanupStagers()											

		void reverseTranslateWorldStagerLines();			// Step 10: apply the appropriate translation to stager lines; uses the inverse of the translationKey

		void buildWorldMachineTriangleContainers();	// Step 11: build the FTriangleContainers, according to what this class specifies.

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
