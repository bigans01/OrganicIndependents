#include "stdafx.h"
#include "OREFracturingMachine.h"

void OREFracturingMachine::runFracturing()
{
	// set the ray cast interval to 1.0f, as the output of this machine will be within block boundaries.
	rayCastDimInterval = 1.0f;

	determineUncalibratedBlockKeys();
	calibrateFTriangleLineAndScannerBlockKeys();

	// For the next steps, we don't do the same as in WorldFracturingMachine::runFracturing();
	// This class assumes that the ECBPoly it is going to operate on has already been localized (i.e, all points are > 0.0f and < 4.0f).
	// Because of this, we can just use the values that are already in originFTrianglePoints and originFTriangleKeys.
	loadLocalizedOREPoints();

	std::cout << "----------> start of runORETRacing. " << std::endl;
	runORETracing();
	std::cout << "----------> end of runORETRacing. " << std::endl;

	buildAndRunFRayCasters();

	runRaycastCollisionResolver(); // Step 6: (NEW step) check for any situations where a External-InteriorRayCast collision occurs, and mitigate it.

	buildAndRunFLineScanners();	// Step 7: buildAndRunFLineScanners figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
								//		   and then run them.

	//std::cout << "!!! Finished running FLineScanners for BlueprintFracturingMachine." << std::endl;


	//std::cout << "---------------------------------------OREFracturingMachine, starting cleanup...." << std::endl;
	analyzeAndCleanupStagers(); // Step 8: analyze the lines we will be using in each FTriangleProductionStager of our map; remove invalid lines, etc
	//std::cout << "---------------------------------------OREFracturingMachine, ending cleanup...." << std::endl;

	reverseTranslateOREStagerLines();

	buildOREMachineTriangleContainers();

	//std::cout << "(OREFracturingMachine): done with runORETracing(), points are: " << std::endl;
	//fracturerPoints.printAllPoints();
}

void OREFracturingMachine::determineUncalibratedBlockKeys()
{
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = getUncalibratedBlockKeyForPoint(originFTrianglePoints[x]);
	}
}

void OREFracturingMachine::calibrateFTriangleLineAndScannerBlockKeys()
{
	// Primer: this function determines the appropriate block key sets to use for FTriangleLine tracing and scanning.

	// we can't use auto for the below operation, because for some reason C++ thinks it should be an array of ref's and not a copy...
	EnclaveKeyDef::EnclaveKey triangleKeysCopy[3];
	for (int x = 0; x < 3; x++)
	{
		triangleKeysCopy[x] = originFTriangleKeys[x];
	}

	//std::cout << "!!! Calibrating keys for FTriangleLine tracing..." << std::endl;
	FTriangleKeySetCalibrator pairA(FTriangleType::ORE);
	pairA.initialize(triangleKeysCopy[0], triangleKeysCopy[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	pairA.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairB(FTriangleType::ORE);
	pairB.initialize(triangleKeysCopy[1], triangleKeysCopy[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	pairB.calibrate(FKeyCalibrationMode::FTRIANGLE_LINE);
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator pairC(FTriangleType::ORE);
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
	FTriangleKeySetCalibrator scanPairA(FTriangleType::ORE);
	scanPairA.initialize(triangleKeysCopy[0], triangleKeysCopy[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	scanPairA.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[0] = scanPairA.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairB(FTriangleType::ORE);
	scanPairB.initialize(triangleKeysCopy[1], triangleKeysCopy[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	scanPairB.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[1] = scanPairB.getBeginAndEndKeys();

	FTriangleKeySetCalibrator scanPairC(FTriangleType::ORE);
	scanPairC.initialize(triangleKeysCopy[2], triangleKeysCopy[0], originFTrianglePoints[2], originFTrianglePoints[0], originFTrianglePoints[1]);
	scanPairC.calibrate(FKeyCalibrationMode::FTRIANGLE_SCANNER);
	scanningKeypairs[2] = scanPairC.getBeginAndEndKeys();

	for (int x = 0; x < 3; x++)
	{
		scanningKeys[x] = scanningKeypairs[x].keyA;
	}
}

void OREFracturingMachine::loadLocalizedOREPoints()
{
	for (int x = 0; x < 3; x++)
	{
		localizedFTrianglePoints[x].x = float(originFTrianglePoints[x].x);
		localizedFTrianglePoints[x].y = float(originFTrianglePoints[x].y);
		localizedFTrianglePoints[x].z = float(originFTrianglePoints[x].z);
	}
}

void OREFracturingMachine::runORETracing()
{
	FTriangleORETracer oreTracer;
	oreTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, localizedFTrianglePoints);
	oreTracer.runLineTracing();
}

void OREFracturingMachine::reverseTranslateOREStagerLines()
{
	// Remember: unlike a WorldFracturingMachine (but LIKE a BlueprintFracturingMachine), a OREFracturingMachine's reverse translate mode always
	// operates as FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE, so there is no need to use a switch statement.
	for (auto& currentStager : stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentMapKeyCopy = currentStager.first;
		EnclaveKeyDef::EnclaveKey currentTranslationKey = currentMapKeyCopy.getInvertedKey();
		currentStager.second.translateLines(currentTranslationKey, rayCastDimInterval);
	}
}

void OREFracturingMachine::buildOREMachineTriangleContainers()
{
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> containerRemovalSet;
	for (auto& currentStager : stagerMap)
	{
		EnclaveKeyDef::EnclaveKey currentStagerKey = currentStager.first;

		(*ftfOutputRef)[currentStagerKey].insertConstructionLines(currentStager.second.fetchStagerLines());

		// Remember: the BlueprintFracturingMachine must produce FTriangleOutput instances that have a type of FTriangleType::ORE.
		(*ftfOutputRef)[currentStagerKey].produceFTriangles(FTriangleType::BLOCK,
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
}

EnclaveKeyDef::EnclaveKey OREFracturingMachine::getUncalibratedBlockKeyForPoint(DoublePoint in_point)
{
	EnclaveKeyDef::EnclaveKey rawKey(0, 0, 0);

	// check x
	if (in_point.x != 0.0f)
	{
		rawKey.x = floor(in_point.x / 1);
	}

	// check y
	if (in_point.y != 0.0f)
	{
		rawKey.y = floor(in_point.y / 1);
	}

	// check z
	if (in_point.z != 0.0f)
	{
		rawKey.z = floor(in_point.z / 1);
	}

	return rawKey;
}