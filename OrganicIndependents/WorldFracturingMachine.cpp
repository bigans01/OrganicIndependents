#include "stdafx.h"
#include "WorldFracturingMachine.h"

void WorldFracturingMachine::setPointTranslationMode(FTriangleReverseTranslationMode in_translationModeToRunAs)
{
	translationMode = in_translationModeToRunAs;
}

void WorldFracturingMachine::runFracturing()
{
	/*
	std::cout << "(WorldFracturingMachine) !!! Start of runFracturing(). " << std::endl;
	std::cout << "Points are: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		originFTrianglePoints[x].printPointCoords();
		std::cout << std::endl;
	}
	*/

	// set the ray cast interval to 32.0f
	rayCastDimInterval = 32.0f;

	determineUncalibratedBlueprintKeys();	// Step 1: get the uncalibrated keys.
	calibrateFTriangleLineAndScannerBlueprintKeys();			// Step 2: calibrate the keys
	translateTriangleByBlueprintKeys();		// Step 3: do any required translations.
	loadLocalizedPoints();					// Step 4: Once we have translated, load the DoublePoints into ECBPolyPoints
											// (don't worry about double to float conversion, as the float values should be very small, and far less than 80000.00~, where the precision loss begins to occur)
	runWorldTracing();						// Step 5: create exterior FTriangleLines.
	
	buildAndRunFRayCasters();			// Step 6: third, determine the ray casters we have to use for the FTriangle, and run them.
	//buildAndRunFRayCasters(originFTriangleKeys);			// Step 6: third, determine the ray casters we have to use for the FTriangle, and run them.
	runRaycastCollisionResolver();				// Step 7: (NEW step): check for any situations where a External-InteriorRayCast occurs, and mitigate it.

	// NEW: with exterior lines done, and FRay casting complete, attempt to find UV coords in all triangle points (if the triangle is configured to do so)


	buildAndRunFLineScanners();				// Step 8: fourth, figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
											// and then run them.

	analyzeAndCleanupStagers(FTriangleType::WORLD);				// Step 9 (from base class): run analysis on the stagers.

	reverseTranslateWorldStagerLines();			// Step 10: reverse translate the stager line points; the choice made is dependent on the reverse
											// transation mode.

	buildWorldMachineTriangleContainers();	// Step 11: build the FTriangleContainers, according to what this class specifies.

	// when everything is done, we need to translate back.

	//std::cout << "(WorldFracturingMachine) !!! End of runFracturing(). " << std::endl;
}

void WorldFracturingMachine::determineUncalibratedBlueprintKeys()
{
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = IndependentUtils::getUncalibratedBlueprintKeyForPoint(originFTrianglePoints[x].point);
	}
}

void WorldFracturingMachine::calibrateFTriangleLineAndScannerBlueprintKeys()
{
	// Primer: this function determines the appropriate Blueprint key sets to use for FTriangleLine tracing and scanning.

	// we can't use auto for the below operation, because for some reason C++ thinks it should be an array of ref's and not a copy...
	EnclaveKeyDef::EnclaveKey triangleKeysCopy[3];
	for (int x = 0; x < 3; x++)
	{
		triangleKeysCopy[x] = originFTriangleKeys[x];
	}


	FTriangleKeySetCalibrator pairA(FTriangleType::WORLD);
	pairA.initialize(triangleKeysCopy[0], 
					 triangleKeysCopy[1], 
					 originFTrianglePoints[0].point, 
					 originFTrianglePoints[1].point, 
					 originFTrianglePoints[2].point);
	pairA.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairB(FTriangleType::WORLD);
	pairB.initialize(triangleKeysCopy[1], 
					triangleKeysCopy[2], 
					originFTrianglePoints[1].point, 
					originFTrianglePoints[2].point, 
					originFTrianglePoints[0].point);
	pairB.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairC(FTriangleType::WORLD);
	pairC.initialize(triangleKeysCopy[2], 
					triangleKeysCopy[0], 
					originFTrianglePoints[2].point, 
					originFTrianglePoints[0].point, 
					originFTrianglePoints[1].point);
	pairC.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[2] = pairC.getBeginAndEndKeys();

	// Store the calibrated Blueprint keys for the FTriangle lines.
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = originFTriangleLineKeypairs[x].keyA;
	}

	// Once key pairs for the FTriangleLines have been established, do the same for the scanningKeys.

	FTriangleKeySetCalibrator scanPairA(FTriangleType::WORLD);
	scanPairA.initialize(triangleKeysCopy[0], 
						triangleKeysCopy[1], 
						originFTrianglePoints[0].point, 
						originFTrianglePoints[1].point, 
						originFTrianglePoints[2].point);
	scanPairA.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[0] = scanPairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairB(FTriangleType::WORLD);
	scanPairB.initialize(triangleKeysCopy[1], 
						triangleKeysCopy[2], 
						originFTrianglePoints[1].point, 
						originFTrianglePoints[2].point, 
						originFTrianglePoints[0].point);
	scanPairB.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[1] = scanPairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairC(FTriangleType::WORLD);
	scanPairC.initialize(triangleKeysCopy[2], 
						triangleKeysCopy[0], 
						originFTrianglePoints[2].point, 
						originFTrianglePoints[0].point, 
						originFTrianglePoints[1].point);
	scanPairC.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[2] = scanPairC.getBeginAndEndKeys();

	for (int x = 0; x < 3; x++)
	{
		scanningKeys[x] = originFTriangleLineKeypairs[x].keyA;
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

void WorldFracturingMachine::translateTriangleByBlueprintKeys()
{
	// the sets for x/y/z that will contain the min.
	std::set<int> keyedXValues;
	std::set<int> keyedYValues;
	std::set<int> keyedZValues;

	// insert all the values.
	for (int x = 0; x < 3; x++)
	{
		keyedXValues.insert(originFTriangleLineKeypairs[x].keyA.x);
		keyedYValues.insert(originFTriangleLineKeypairs[x].keyA.y);
		keyedZValues.insert(originFTriangleLineKeypairs[x].keyA.z);
	}

	// create an EnclaveKey that has the min values (just use the begin of each set, since the minimum value is there.)
	EnclaveKeyDef::EnclaveKey minimumKey(*keyedXValues.begin(), *keyedYValues.begin(), *keyedZValues.begin());

	// Below: FTDEBUG (uncomment when needed.
	/*
	std::cout << "Minimum key value is: ";
	minimumKey.printKey();
	std::cout << std::endl;
	*/

	// fetch the inverted key; this will be the value of the translationKey.
	translationKey = minimumKey.getInvertedKey();

	// Below: FTDEBUG (uncomment when needed.
	/*
	std::cout << "Inverted minimum key (aka, translationKey) is: ";
	translationKey.printKey();
	std::cout << std::endl;
	*/

	adjustBlueprintKeysByValue(translationKey);

	DoublePoint pointAdjustmentValue(translationKey.x*32.0f, translationKey.y*32.0f, translationKey.z*32.0f);

	// Below: FTDEBUG (uncomment when needed.
	/*
	std::cout << "pointAdjustmentValue is: ";
	pointAdjustmentValue.printPointCoords();
	std::cout << std::endl;
	*/

	adjustPointsByValue(pointAdjustmentValue);

	// FTDEBUG, (uncomment when needed for debugging)

	//std::cout << "Done with translation by blueprint keys. " << std::endl;
	//int doneTranslate = 3;
	//std::cin >> doneTranslate;
}

void WorldFracturingMachine::adjustBlueprintKeysByValue(EnclaveKeyDef::EnclaveKey in_adjustingKey)
{
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] += in_adjustingKey;
		originFTriangleLineKeypairs[x].keyA += in_adjustingKey;
		originFTriangleLineKeypairs[x].keyB += in_adjustingKey;

		scanningKeys[x] += in_adjustingKey;
		scanningKeypairs[x].keyA += in_adjustingKey;
		scanningKeypairs[x].keyB += in_adjustingKey;
	}
}

void WorldFracturingMachine::adjustPointsByValue(DoublePoint in_adjustingPoint)
{
	for (int x = 0; x < 3; x++)
	{
		originFTrianglePoints[x].point += in_adjustingPoint;
		originFTrianglePoints[x].point.roundHundredth();
	}
}

void WorldFracturingMachine::loadLocalizedPoints()
{
	for (int x = 0; x < 3; x++)
	{
		localizedFTrianglePoints[x] = originFTrianglePoints[x];
	}
}

void WorldFracturingMachine::reverseTranslateWorldStagerLines()
{
	for (auto& currentStager : stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentTranslationKey;
		switch (translationMode)
		{
			case FTriangleReverseTranslationMode::ABSOLUTE_TRANSLATE:
			{
				// when in ABSOLUTE mode, all points will be translated by the 
				// reverse value of the translationKey;
				currentTranslationKey = translationKey.getInvertedKey();
				break;
			}
			case FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE:
			{
				// when in LOCALIZED mode, all points will be translated by the
				// reverse value of the key that the maps to the stager.
				EnclaveKeyDef::EnclaveKey currentMapKeyCopy = currentStager.first;
				currentTranslationKey = currentMapKeyCopy.getInvertedKey();
				break;
			}
		}

		// Below: FTDEBUG (uncomment when needed.
		/*
		std::cout << "Reverse translation key is: ";
		currentTranslationKey.printKey();
		std::cout << std::endl;
			
		// with the key determined, do the translation.
		EnclaveKeyDef::EnclaveKey debugKeyOutput = currentStager.first;
		std::cout << "Translating lines for key at: "; debugKeyOutput.printKey();
		std::cout << std::endl;
		*/

		currentStager.second.translateLines(currentTranslationKey, rayCastDimInterval);
	}

	// FTDEBUG, (uncomment when needed for debugging)
	//std::cout << "Done with reverseTranslateStagerLines()" << std::endl;
	//int translationWait = 3;
	//std::cin >> translationWait;
}

void WorldFracturingMachine::buildWorldMachineTriangleContainers()
{
	// get the current key of each mapped stager;
	// we will need to apply the inverse of the translationKey to this, in order to determine
	// the appropriate Blueprint key to use for each mapped FTriangleContainer.
	//
	// If a container needs to be deleted, put into the container containerRemovalSet.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> containerRemovalSet;
	for (auto& currentStager: stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentStagerKey = currentStager.first;

		// Check if the currentStagerKey is incalculable.
		bool incalculableFound = false;
		auto incalculableFinder = incalculableKeys.find(currentStagerKey);
		if (incalculableFinder != incalculableKeys.end())	// it was found
		{
			incalculableFound = true;
		}

		currentStagerKey += translationKey.getInvertedKey();	// apply the inverse of the translationKey.

		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "Creating World machine FTriangleContainer, at key: ";
		currentStagerKey.printKey();
		std::cout << std::endl;
		*/

		// Assuming that the stager is calculable (99% of cases), proceed.
		if (incalculableFound == false)
		{
			(*ftfOutputRef)[currentStagerKey].insertConstructionLines(currentStager.second.fetchStagerLines());


			// Remember: the WorldFracturingMachine must produce FTriangleOutput instances that have a type of FTriangleType::BLUEPRINT.
			(*ftfOutputRef)[currentStagerKey].produceFTriangles(FTriangleType::BLUEPRINT,
				originFTriangleEmptynormal,
				originBoundaryOrientation,
				currentStagerKey,
				originMaterial);

			// Do the boundary tests; remove any containers that have no FOutputTriangles in them,
			// which is determined by the call to runBoundaryTests below.
			bool shouldContainerBeDeleted = (*ftfOutputRef)[currentStagerKey].runBoundaryTests(translationMode, currentStagerKey, originFTriangleEmptynormal);
			if (shouldContainerBeDeleted)
			{
				containerRemovalSet.insert(currentStagerKey);
			}
		}

		// Remember: incalculable means that a FTriangleProductionStager 
		// was unable to produce any triangles, when that instance called the
		// analyzeAndReorganize() function, so we must record this. 
		else if (incalculableFound == true)
		{
			// Replace the incalculable key with it's offset value. But erase the old one first.
			EnclaveKeyDef::EnclaveKey originalKeyToErase = currentStager.first;
			incalculableKeys.erase(originalKeyToErase);
			incalculableKeys.insert(currentStagerKey);
		}


		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "Lines in this container are: " << std::endl;
		(*ftfOutputRef)[currentStagerKey].printConstructionLines();
		*/
	}

	// Erase any keyed, empty containers that were in the containerRemovalSet.
	for (auto& currentContainerToRemove : containerRemovalSet)
	{
		(*ftfOutputRef).erase(currentContainerToRemove);
	}

	// FTDEBUG, (uncomment when needed for debugging)
	//std::cout << "Done with buildWorldMachineTriangleContainers()." << std::endl;
	//int buildDone = 3;
	//std::cin >> buildDone;
}

void WorldFracturingMachine::runWorldTracing()
{
	FTriangleWorldTracer worldTracer;
	//worldTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, originFTrianglePoints);
	worldTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, localizedFTrianglePoints);
	worldTracer.runLineTracing();
}