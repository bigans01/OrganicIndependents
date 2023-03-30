#include "stdafx.h"
#include "BlueprintFracturingMachine.h"

void BlueprintFracturingMachine::runFracturing()
{
	/*
	std::cout << "(BlueprintFracturingMachine) !!! Start of runFracturing(). " << std::endl;
	std::cout << "Points are: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		originFTrianglePoints[x].printPointCoords();
		std::cout << std::endl;
	}
	*/
	
	// set the ray cast interval to 4.0f
	rayCastDimInterval = 4.0f;

	determineUncalibratedOREKeys();					// Step 1: get the uncalibrated keys.
	calibrateFTriangleLineAndScannerOREKeys();		// Step 2: calibrate the keys

	// For the next steps, we don't do the same as in WorldFracturingMachine::runFracturing();
	// This class assumes that the ECBPoly it is going to operate on has already been localized (i.e, all points are > 0.0f and < 32.0f).
	// Because of this, we can just use the values that are already in originFTrianglePoints and originFTriangleKeys.

	loadLocalizedBlueprintPoints();		// Step 3: loadLocalizedPoints():  Once we have translated, load the DoublePoints into ECBPolyPoints
										// (don't worry about double to float conversion, as the float values should be very small, and far less than 80000.00~, where the precision loss begins to occur)

	runBlueprintTracing();				// Step 4: runBlueprintTracing(), to create exterior FTriangleLines.

	//std::cout << "(BlueprintFracturingMachine): completed runBlueprintTracing." << std::endl;

	
	buildAndRunFRayCasters(); // Step 5: buildAndRunBlueprintFRayCasters() -- determine the ray casters we have to use for the FTriangle, and run them.

	runRaycastCollisionResolver(); // Step 6: (NEW step) check for any situations where a External-InteriorRayCast collision occurs, and mitigate it.

	buildAndRunFLineScanners();	// Step 7: buildAndRunFLineScanners figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
								//		   and then run them.

	//std::cout << "!!! Finished running FLineScanners for BlueprintFracturingMachine." << std::endl;


	analyzeAndCleanupStagers(FTriangleType::BLUEPRINT); // Step 8: analyze the lines we will be using in each FTriangleProductionStager of our map; remove invalid lines, etc;

	reverseTranslateBlueprintStagerLines(); // Step 9: Reverse translate the stager lines, in a FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE fashion.

	buildBlueprintMachineTriangleContainers();	// Step 10: Produce the outputs (the type of the 

	// Step 9: build the FTriangleOutputs, and remove the ones that are not correctly boundary oriented.

	//std::cout << "(BlueprintFracturingMachine) !!! End of runFracturing(). " << std::endl;
}

void BlueprintFracturingMachine::determineUncalibratedOREKeys()
{
	for (int x = 0; x < 3; x++)
	{
		// NEED: a function to determine the uncalibrated keys for ORE coordinates (i.e, the point 29, 0, 0  would be at ORE 7, 0, 0)
		originFTriangleKeys[x] = getUncalibratedOREKeyForPoint(originFTrianglePoints[x]);
	}
}

void BlueprintFracturingMachine::calibrateFTriangleLineAndScannerOREKeys()
{
	// Primer: this function determines the appropriate ORE key sets to use for FTriangleLine tracing and scanning.

	// we can't use auto for the below operation, because for some reason C++ thinks it should be an array of ref's and not a copy...
	EnclaveKeyDef::EnclaveKey triangleKeysCopy[3];
	for (int x = 0; x < 3; x++)
	{
		triangleKeysCopy[x] = originFTriangleKeys[x];
	}

	//std::cout << "!!! Calibrating keys for FTriangleLine tracing..." << std::endl;
	FTriangleKeySetCalibrator pairA(FTriangleType::BLUEPRINT);
	pairA.initialize(triangleKeysCopy[0], triangleKeysCopy[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	pairA.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairB(FTriangleType::BLUEPRINT);
	pairB.initialize(triangleKeysCopy[1], triangleKeysCopy[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	pairB.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairC(FTriangleType::BLUEPRINT);
	pairC.initialize(triangleKeysCopy[2], triangleKeysCopy[0], originFTrianglePoints[2], originFTrianglePoints[0], originFTrianglePoints[1]);
	pairC.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[2] = pairC.getBeginAndEndKeys();

	// Store the calibrated ORE keys for the FTriangle lines.
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = originFTriangleLineKeypairs[x].keyA;
	}

	// Once key pairs for the FTriangleLines have been established, do the same for the scanningKeys.
	//std::cout << "!!! Calibrating keys for scanning..." << std::endl;
	FTriangleKeySetCalibrator scanPairA(FTriangleType::BLUEPRINT);
	scanPairA.initialize(triangleKeysCopy[0], triangleKeysCopy[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	scanPairA.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[0] = scanPairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairB(FTriangleType::BLUEPRINT);
	scanPairB.initialize(triangleKeysCopy[1], triangleKeysCopy[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	scanPairB.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[1] = scanPairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairC(FTriangleType::BLUEPRINT);
	scanPairC.initialize(triangleKeysCopy[2], triangleKeysCopy[0], originFTrianglePoints[2], originFTrianglePoints[0], originFTrianglePoints[1]);
	scanPairC.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[2] = scanPairC.getBeginAndEndKeys();

	for (int x = 0; x < 3; x++)
	{
		scanningKeys[x] = scanningKeypairs[x].keyA;
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

void BlueprintFracturingMachine::buildBlueprintMachineTriangleContainers()
{
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> containerRemovalSet;
	for (auto& currentStager : stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentStagerKey = currentStager.first;

		(*ftfOutputRef)[currentStagerKey].insertConstructionLines(currentStager.second.fetchStagerLines());

		// Remember: the BlueprintFracturingMachine must produce FTriangleOutput instances that have a type of FTriangleType::ORE.
		(*ftfOutputRef)[currentStagerKey].produceFTriangles(FTriangleType::ORE,
			originFTriangleEmptynormal,
			originBoundaryOrientation,
			currentStagerKey,
			originMaterial);

		// Do the boundary tests; remove any containers that have no FOutputTriangles in them,
		// which is determined by the call to runBoundaryTests below.
		bool shouldContainerBeDeleted = (*ftfOutputRef)[currentStagerKey].runBoundaryTests(FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE, currentStagerKey, originFTriangleEmptynormal);
		if (shouldContainerBeDeleted)
		{
			containerRemovalSet.insert(currentStagerKey);
		}

	}

	// Erase any keyed, empty containers that were in the containerRemovalSet.
	for (auto& currentContainerToRemove : containerRemovalSet)
	{
		(*ftfOutputRef).erase(currentContainerToRemove);
	}

	/*
	// Lastly, for debug only: print out the triangles in each container:
	std::cout << "-------Printing out output container triangles. " << std::endl;
	for (auto& currentContainer : *ftfOutputRef)
	{
		std::cout << "Output triangles for ORE key ";
		EnclaveKeyDef::EnclaveKey curentKey = currentContainer.first;
		curentKey.printKey();
		std::cout << ": " << std::endl;

		currentContainer.second.printProducedFTriangles();

	}

	std::cout << "Done printing out container triangles. " << std::endl;
	int junkInTrunk = 0;
	std::cin >> junkInTrunk;
	*/
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