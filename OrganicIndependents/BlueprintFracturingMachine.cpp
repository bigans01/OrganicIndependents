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

	
	buildAndRunBlueprintFRayCasters(); // Step 5: buildAndRunBlueprintFRayCasters() -- determine the ray casters we have to use for the FTriangle, and run them.
	// Step 6: buildAndRunFLineScanners figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
	//		   and then run them.

	// Step 7: analyzeAndCleanupStagers(): analyze the lines we will be using in each FTriangleProductionStager of our map; remove invalid lines, etc

	// Step 8: Reverse translate the stager lines, in a FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE fashion.

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

void BlueprintFracturingMachine::buildAndRunBlueprintFRayCasters()
{
	// We must first determine which ray casters are eligible to run, and get their init data.
	auto acceptedRayCasterTypes = getUsableRayCasters();

	// Unless perfectly clamped to a dimension, all ray casters XFRayCastQuad, YFRayCastQuad, ZFRayCastQuad will 
	// be used. So we can just use a simple switch statement here.
	switch (originPerfectClampValue)
	{
		// the typical case, when we aren't clamped to anything: check all 3 ray casters.
	case PerfectClampEnum::NONE:
	{
		//std::cout << "No perfect clamp value detected; checking for all 3 ray casters. " << std::endl;

		// check for X ray caster.
		auto checkForXRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::X_RAY);
		if (checkForXRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found X-ray caster, data is: " << std::endl;
			std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMin << std::endl;
			std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMax << std::endl;
			std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMin << std::endl;
			std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (X min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (X max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> xRayCaster(new (XFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::X_RAY] = xRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::X_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			//selectedRayCasters[FRayCasterTypeEnum::X_RAY]->insertIntoTestVec(5);
			//std::cout << "!!!!!! DONE with insert vector for X ray. " << std::endl;

			selectedRayCasters[FRayCasterTypeEnum::X_RAY]->buildAndCastRays();


		}

		// check for Y ray caster.
		auto checkForYRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Y_RAY);
		if (checkForYRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found Y-ray caster, data is: " << std::endl;
			std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMin << std::endl;
			std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMax << std::endl;
			std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMin << std::endl;
			std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (Y min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (Y max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> yRayCaster(new (YFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY] = yRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->buildAndCastRays();
		}

		// check for Z ray caster.
		auto checkForZRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Z_RAY);
		if (checkForZRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found Z-ray caster, data is: " << std::endl;
			std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMin << std::endl;
			std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMax << std::endl;
			std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMin << std::endl;
			std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (Z min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (Z max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> zRayCaster(new (ZFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY] = zRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->buildAndCastRays();
		}

		break;
	}

	// check for X ray only.
	case PerfectClampEnum::CLAMPED_TO_X:
	{
		//std::cout << "X clamp value detected, checking for X ray caster. " << std::endl;
		// check for X ray caster.
		auto checkForXRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::X_RAY);
		if (checkForXRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found X-ray caster, data is: " << std::endl;
			std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMin << std::endl;
			std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMax << std::endl;
			std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMin << std::endl;
			std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (X min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (X max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> xRayCaster(new (XFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::X_RAY] = xRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::X_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			selectedRayCasters[FRayCasterTypeEnum::X_RAY]->buildAndCastRays();
		}

		break;
	}

	case PerfectClampEnum::CLAMPED_TO_Y:
	{
		//std::cout << "Y clamp value detected, checking for Y ray caster. " << std::endl;
		// check for Y ray caster.
		auto checkForYRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Y_RAY);
		if (checkForYRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found Y-ray caster, data is: " << std::endl;
			std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMin << std::endl;
			std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMax << std::endl;
			std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMin << std::endl;
			std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (Y min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (Y max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> yRayCaster(new (YFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY] = yRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->buildAndCastRays();
		}

		break;
	}

	case PerfectClampEnum::CLAMPED_TO_Z:
	{
		//std::cout << "Z clamp value detected, checking for Z ray caster. " << std::endl;
		// check for Z ray caster.
		auto checkForZRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Z_RAY);
		if (checkForZRayCaster != acceptedRayCasterTypes.end())
		{
			// Below: FTDEBUG (uncomment when needed.
			/*
			std::cout << "Found Z-ray caster, data is: " << std::endl;
			std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMin << std::endl;
			std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMax << std::endl;
			std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMin << std::endl;
			std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMax << std::endl;
			std::cout << "Target dim (Z min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMin << std::endl;
			std::cout << "Target dim (Z max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMax << std::endl;
			std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastInterval << std::endl;
			*/

			std::shared_ptr<FRayCasterQuadBase> zRayCaster(new (ZFRayCastQuad));
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY] = zRayCaster;
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY],
				//originFTrianglePoints,
				localizedFTrianglePoints,
				&fracturerPoints);
			selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->buildAndCastRays();
		}

		break;
	}
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