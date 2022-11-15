#include "stdafx.h"
#include "WorldFracturingMachine.h"

void WorldFracturingMachine::runFracturing()
{
	// set the ray cast interval to 32.0f
	rayCastDimInterval = 32.0f;

	determineUncalibratedBlueprintKeys();	// Step 1: get the uncalibrated keys.
	calibrateOriginBlueprintKeys();			// Step 2: calibrate the keys.
	translateTriangleByBlueprintKeys();		// first, do any required translations.
	runWorldTracing();						// second, create exterior FTriangleLines.
	buildAndRunWorldFRayCasters();			// third, determine the ray casters we have to use for the FTriangle, and run them.
	determineAndBuildFLineScanners();		// fourth, figure out which FLineScanner-derived classes to use by analyzing the triangle points, 
											// and then run them.

	// as a test, get all points with X = 64.0f.
	float xValue = 64.0f;
	std::cout << "(WorldFracturingMachine::runFracturing): printing X points that have a value of " << xValue << ": " << std::endl;
	for (auto& x64points : fracturerPoints.getAllPointsWithX(xValue))
	{
		x64points.printPointData();
		std::cout << std::endl;
	}
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

	// once everyting has been calibrated, we can store the keys for each point.
	std::cout << "Calibrated keys for each point are: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = originFTriangleLineKeypairs[x].keyA;
		std::cout << "Point " << x << ": ";
		originFTrianglePoints[x].printPointCoords();
		std::cout << " Key: ";
		originFTriangleKeys[x].printKey();
		std::cout << std::endl;
	}

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
}

void WorldFracturingMachine::translateTriangleByBlueprintKeys()
{

}

void WorldFracturingMachine::runWorldTracing()
{
	FTriangleWorldTracer worldTracer;
	worldTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, originFTrianglePoints);
	worldTracer.runLineTracing();
}

void WorldFracturingMachine::buildAndRunWorldFRayCasters()
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
			std::cout << "No perfect clamp value detected; checking for all 3 ray casters. " << std::endl;

			// check for X ray caster.
			auto checkForXRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::X_RAY);
			if (checkForXRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found X-ray caster, data is: " << std::endl;
				std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMin << std::endl;
				std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMax << std::endl;
				std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMin << std::endl;
				std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (X min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (X max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> xRayCaster(new (XFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::X_RAY] = xRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::X_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY],
																		originFTrianglePoints,
																		&fracturerPoints);
				//selectedRayCasters[FRayCasterTypeEnum::X_RAY]->insertIntoTestVec(5);
				//std::cout << "!!!!!! DONE with insert vector for X ray. " << std::endl;

				selectedRayCasters[FRayCasterTypeEnum::X_RAY]->buildAndCastRays();

				
			}

			// check for Y ray caster.
			auto checkForYRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Y_RAY);
			if (checkForYRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found Y-ray caster, data is: " << std::endl;
				std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMin << std::endl;
				std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMax << std::endl;
				std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMin << std::endl;
				std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (Y min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (Y max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> yRayCaster(new (YFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY] = yRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY],
																		originFTrianglePoints,
																		&fracturerPoints);
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->buildAndCastRays();
			}

			// check for Z ray caster.
			auto checkForZRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Z_RAY);
			if (checkForZRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found Z-ray caster, data is: " << std::endl;
				std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMin << std::endl;
				std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMax << std::endl;
				std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMin << std::endl;
				std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (Z min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (Z max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> zRayCaster(new (ZFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY] = zRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY],
																		originFTrianglePoints,
																		&fracturerPoints);
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->buildAndCastRays();
			}

			break;
		}

		// check for X ray only.
		case PerfectClampEnum::CLAMPED_TO_X:
		{
			std::cout << "X clamp value detected, checking for X ray caster. " << std::endl;
			// check for X ray caster.
			auto checkForXRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::X_RAY);
			if (checkForXRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found X-ray caster, data is: " << std::endl;
				std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMin << std::endl;
				std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimOneMax << std::endl;
				std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMin << std::endl;
				std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (X min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (X max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> xRayCaster(new (XFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::X_RAY] = xRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::X_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::X_RAY],
					originFTrianglePoints,
					&fracturerPoints);
				selectedRayCasters[FRayCasterTypeEnum::X_RAY]->buildAndCastRays();
			}

			break;
		}

		case PerfectClampEnum::CLAMPED_TO_Y:
		{
			std::cout << "Y clamp value detected, checking for Y ray caster. " << std::endl;
			// check for Y ray caster.
			auto checkForYRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Y_RAY);
			if (checkForYRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found Y-ray caster, data is: " << std::endl;
				std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMin << std::endl;
				std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimOneMax << std::endl;
				std::cout << "Z min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMin << std::endl;
				std::cout << "Z max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (Y min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (Y max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> yRayCaster(new (YFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY] = yRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Y_RAY],
					originFTrianglePoints,
					&fracturerPoints);
				selectedRayCasters[FRayCasterTypeEnum::Y_RAY]->buildAndCastRays();
			}

			break;
		}

		case PerfectClampEnum::CLAMPED_TO_Z:
		{
			std::cout << "Z clamp value detected, checking for Z ray caster. " << std::endl;
			// check for Z ray caster.
			auto checkForZRayCaster = acceptedRayCasterTypes.find(FRayCasterTypeEnum::Z_RAY);
			if (checkForZRayCaster != acceptedRayCasterTypes.end())
			{
				std::cout << "Found Z-ray caster, data is: " << std::endl;
				std::cout << "X min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMin << std::endl;
				std::cout << "X max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimOneMax << std::endl;
				std::cout << "Y min: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMin << std::endl;
				std::cout << "Y max: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].dimTwoMax << std::endl;
				std::cout << "Target dim (Z min): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMin << std::endl;
				std::cout << "Target dim (Z max): " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastDimMax << std::endl;
				std::cout << "Interval: " << acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY].rayCastInterval << std::endl;

				std::shared_ptr<FRayCasterQuadBase> zRayCaster(new (ZFRayCastQuad));
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY] = zRayCaster;
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->initialize(acceptedRayCasterTypes[FRayCasterTypeEnum::Z_RAY],
					originFTrianglePoints,
					&fracturerPoints);
				selectedRayCasters[FRayCasterTypeEnum::Z_RAY]->buildAndCastRays();
			}

			break;
		}
	}

}

void WorldFracturingMachine::determineAndBuildFLineScanners()
{
	auto acceptablePermits = getValidPermits();
	auto XYZLists = getScanningIntervals(32.0f);

	// Below: if the permit is accepted, AND the corresponding list it will use is available, we'll need to create a LineScanner instance.

	// For YZLineScanner (Moves along fixed X interval, makes lines with Y/Z)
	auto xPermitFinder = acceptablePermits.find(LineScanPermit::SCAN_X);
	if
	(
		(xPermitFinder != acceptablePermits.end())
		&&
		!(XYZLists.xList.empty())
	)
	{
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): criteria met for XDimLineScanner. (goes forward toward positive X)" << std::endl;
		int startBackwardXValue = getScanDimensionalStartKey(LineScanPermit::SCAN_X);
		int startForwardXValue = startBackwardXValue + 1;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): backward X start key is: " << startBackwardXValue << std::endl;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): forward X start key is: " << startForwardXValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> xScanner(new XDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_X] = xScanner;
		lineScannerMap[LineScanPermit::SCAN_X]->initializeLineScanner(XYZLists.xList,
																	&fracturerPoints,
																	&stagerMap,
																	startBackwardXValue,
																	startForwardXValue,
																	rayCastDimInterval);
		lineScannerMap[LineScanPermit::SCAN_X]->runScan();
																				
	}

	// For XZLineScanner (Moves along fixed Y interval, makes lines with X/Z)
	auto yPermitFinder = acceptablePermits.find(LineScanPermit::SCAN_Y);
	if
	(
		(yPermitFinder != acceptablePermits.end())
		&&
		!(XYZLists.yList.empty())
	)
	{
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): criteria met for YDimLineScanner. (goes forward toward positive Y)" << std::endl;
		int startBackwardYValue = getScanDimensionalStartKey(LineScanPermit::SCAN_Y);
		int startForwardYValue = startBackwardYValue + 1;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): backward Y start key is: " << startBackwardYValue << std::endl;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): forward Y start key is: " << startForwardYValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> yScanner(new YDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_Y] = yScanner;
		lineScannerMap[LineScanPermit::SCAN_Y]->initializeLineScanner(XYZLists.yList,
																	&fracturerPoints,
																	&stagerMap,
																	startBackwardYValue,
																	startForwardYValue,
																	rayCastDimInterval);
		lineScannerMap[LineScanPermit::SCAN_Y]->runScan();
	}

	// For XYLineScanner (Moves along fixed Z interval, makes lines with X/Y)
	auto zPermitFinder = acceptablePermits.find(LineScanPermit::SCAN_Z);
	if
	(
		(zPermitFinder != acceptablePermits.end())
		&&
		!(XYZLists.zList.empty())
	)
	{
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): criteria met for ZDimLineScanner. (goes forward toward positive Z)" << std::endl;
		int startBackwardZValue = getScanDimensionalStartKey(LineScanPermit::SCAN_Z);
		int startForwardZValue = startBackwardZValue + 1;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): backward Z start key is: " << startBackwardZValue << std::endl;
		std::cout << "(WorldFracturingMachine::determineAndBuildFLineScanners()): forward Z start key is: " << startForwardZValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> zScanner(new ZDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_Z] = zScanner;
		lineScannerMap[LineScanPermit::SCAN_Z]->initializeLineScanner(XYZLists.zList,
																	&fracturerPoints,
																	&stagerMap,
																	startBackwardZValue,
																	startForwardZValue,
																	rayCastDimInterval);
		lineScannerMap[LineScanPermit::SCAN_Z]->runScan();
	}
}