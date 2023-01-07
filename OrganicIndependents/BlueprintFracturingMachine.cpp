#include "stdafx.h"
#include "BlueprintFracturingMachine.h"

void BlueprintFracturingMachine::runFracturing()
{
	// set the ray cast interval to 4.0f
	rayCastDimInterval = 4.0f;

	determineUncalibratedOREKeys();		// Step 1: get the uncalibrated keys.
	calibrateOriginOREKeys();			// Step 2: calibrate the keys

	// For the next steps, we don't do the same as in WorldFracturingMachine::runFracturing();
	// This class assumes that the ECBPoly it is going to operate on has already been localized (i.e, all points are > 0.0f and < 32.0f).
	// Because of this, we can just use the values that are already in originFTrianglePoints and originFTriangleKeys.

	loadLocalizedBlueprintPoints();		// Step 3: loadLocalizedPoints():  Once we have translated, load the DoublePoints into ECBPolyPoints
										// (don't worry about double to float conversion, as the float values should be very small, and far less than 80000.00~, where the precision loss begins to occur)

	runBlueprintTracing();				// Step 4: runBlueprintTracing(), to create exterior FTriangleLines.

	std::cout << "!!! BlueprintFracturingMachine: completed runBlueprintTracing." << std::endl;

	
	buildAndRunFRayCasters(); // Step 5: buildAndRunBlueprintFRayCasters() -- determine the ray casters we have to use for the FTriangle, and run them.
	buildAndRunFLineScanners();	// Step 6: buildAndRunFLineScanners figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
								//		   and then run them.

	std::cout << "!!! Finished running FLineScanners for BlueprintFracturingMachine." << std::endl;



	analyzeAndCleanupStagers(); // Step 7: analyze the lines we will be using in each FTriangleProductionStager of our map; remove invalid lines, etc

	reverseTranslateBlueprintStagerLines(); // Step 8: Reverse translate the stager lines, in a FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE fashion.

	// Step 9: build the FTriangleOutputs, and remove the ones that are not correctly boundary oriented.

}

void BlueprintFracturingMachine::determineUncalibratedOREKeys()
{
	for (int x = 0; x < 3; x++)
	{
		// NEED: a function to determine the uncalibrated keys for ORE coordinates (i.e, the point 29, 0, 0  would be at ORE 7, 0, 0)
		originFTriangleKeys[x] = getUncalibratedOREKeyForPoint(originFTrianglePoints[x]);
	}
}

void BlueprintFracturingMachine::calibrateOriginOREKeys()
{
	// Calibrate the ORE keys with a new class, such as CalibrateableOREKeyPair; see WorldFracturingMachine::calibrateOriginBlueprintKeys() for a model.
	CalibrateableOREKeyPair pairA;
	pairA.initialize(originFTriangleKeys[0], originFTriangleKeys[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	pairA.calibrate();
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	CalibrateableOREKeyPair pairB;
	pairB.initialize(originFTriangleKeys[1], originFTriangleKeys[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	pairB.calibrate();
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	CalibrateableOREKeyPair pairC;
	pairC.initialize(originFTriangleKeys[2], originFTriangleKeys[0], originFTrianglePoints[2], originFTrianglePoints[0], originFTrianglePoints[1]);
	pairC.calibrate();
	originFTriangleLineKeypairs[2] = pairC.getBeginAndEndKeys();

	// once everything has been calibrated, we can store the keys for each point.
	// Below: FTDEBUG (uncomment when needed)
	//std::cout << "Calibrated keys for each point are: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = originFTriangleLineKeypairs[x].keyA;

		// Below: FTDEBUG (uncomment when needed.
		/*
		std::cout << "Point " << x << ": ";
		originFTrianglePoints[x].printPointCoords();
		std::cout << " Key: ";
		originFTriangleKeys[x].printKey();
		std::cout << std::endl;
		*/
	}

	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "(WorldFracturingMachine::calibrateOriginBlueprintKeys) -> finished calibrating blueprint keys; keys are: " << std::endl;

	std::cout << "FTriangle line 0: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[0].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[1].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[0].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[0].keyB.printKey(); std::cout << std::endl;

	std::cout << "FTriangle line 1: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[1].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[2].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[1].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[1].keyB.printKey(); std::cout << std::endl;

	std::cout << "FTriangle line 2: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[2].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[0].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[2].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[2].keyB.printKey(); std::cout << std::endl;
	*/
}

void BlueprintFracturingMachine::loadLocalizedBlueprintPoints()
{
	for (int x = 0; x < 3; x++)
	{
		localizedFTrianglePoints[x].x = float(originFTrianglePoints[x].x);
		localizedFTrianglePoints[x].y = float(originFTrianglePoints[x].y);
		localizedFTrianglePoints[x].z = float(originFTrianglePoints[x].z);
	}
}

void BlueprintFracturingMachine::runBlueprintTracing()
{
	FTriangleBlueprintTracer blueprintTracer;
	blueprintTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, localizedFTrianglePoints);
	blueprintTracer.runLineTracing();
}

void BlueprintFracturingMachine::reverseTranslateBlueprintStagerLines()
{
	// Remember: unlike a WorldFracturingMachine, a BlueprintFracturingMachine's reverse translate mode always
	// operates as FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE, so there is no need to use a switch statement.
	for (auto& currentStager : stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentMapKeyCopy = currentStager.first;
		EnclaveKeyDef::EnclaveKey currentTranslationKey = currentMapKeyCopy.getInvertedKey();
		currentStager.second.translateLines(currentTranslationKey, rayCastDimInterval);
	}
}

EnclaveKeyDef::EnclaveKey BlueprintFracturingMachine::getUncalibratedOREKeyForPoint(DoublePoint in_point)
{
	EnclaveKeyDef::EnclaveKey rawKey(0, 0, 0);

	// check x
	if (in_point.x != 0.0f)
	{
		rawKey.x = floor(in_point.x / 4);
	}

	// check y
	if (in_point.y != 0.0f)
	{
		rawKey.y = floor(in_point.y / 4);
	}

	// check z
	if (in_point.z != 0.0f)
	{
		rawKey.z = floor(in_point.z / 4);
	}

	return rawKey;
}