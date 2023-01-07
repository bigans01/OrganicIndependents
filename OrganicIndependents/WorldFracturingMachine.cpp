#include "stdafx.h"
#include "WorldFracturingMachine.h"

void WorldFracturingMachine::runFracturing()
{
	// set the ray cast interval to 32.0f
	rayCastDimInterval = 32.0f;

	determineUncalibratedBlueprintKeys();	// Step 1: get the uncalibrated keys.
	calibrateOriginBlueprintKeys();			// Step 2: calibrate the keys
	translateTriangleByBlueprintKeys();		// Step 3: do any required translations.
	loadLocalizedPoints();					// Step 4: Once we have translated, load the DoublePoints into ECBPolyPoints
											// (don't worry about double to float conversion, as the float values should be very small, and far less than 80000.00~, where the precision loss begins to occur)
	runWorldTracing();						// Step 5: create exterior FTriangleLines.
	buildAndRunFRayCasters();			// Step 6: third, determine the ray casters we have to use for the FTriangle, and run them.
	buildAndRunFLineScanners();				// Step 7: fourth, figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
											// and then run them.

	analyzeAndCleanupStagers();				// Step 8 (from base class): run analysis on the stagers.

	reverseTranslateWorldStagerLines();			// Step 9: reverse translate the stager line points; the choice made is dependent on the reverse
											// transation mode.

	buildWorldMachineTriangleContainers();	// Step 10: build the FTriangleContainers, according to what this class specifies.

	// when everything is done, we need to translate back.
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
	CalibratableBlueprintKeyPair pairA;
	pairA.initialize(originFTriangleKeys[0], originFTriangleKeys[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	pairA.calibrate();
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	CalibratableBlueprintKeyPair pairB;
	pairB.initialize(originFTriangleKeys[1], originFTriangleKeys[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	pairB.calibrate();
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	CalibratableBlueprintKeyPair pairC;
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
	}
}

void WorldFracturingMachine::adjustPointsByValue(DoublePoint in_adjustingPoint)
{
	for (int x = 0; x < 3; x++)
	{
		originFTrianglePoints[x] += in_adjustingPoint;
		originFTrianglePoints[x].roundHundredth();
	}
}

void WorldFracturingMachine::loadLocalizedPoints()
{
	for (int x = 0; x < 3; x++)
	{
		localizedFTrianglePoints[x].x = float(originFTrianglePoints[x].x);
		localizedFTrianglePoints[x].y = float(originFTrianglePoints[x].y);
		localizedFTrianglePoints[x].z = float(originFTrianglePoints[x].z);
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
		currentStagerKey += translationKey.getInvertedKey();	// apply the inverse of the translationKey.

		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "Creating World machine FTriangleContainer, at key: ";
		currentStagerKey.printKey();
		std::cout << std::endl;
		*/

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