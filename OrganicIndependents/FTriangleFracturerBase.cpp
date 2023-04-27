#include "stdafx.h"
#include "FTriangleFracturerBase.h"

void FTriangleFracturerBase::transferFTriangleMetadata(DoublePoint in_fracturePoint0,
	DoublePoint in_fracturePoint1,
	DoublePoint in_fracturePoint2,
	ECBPolyPoint in_fractureEmptyNormal,
	BoundaryOrientation in_originBoundaryOrientation,
	PerfectClampEnum in_originPerfectClampValue,
	TriangleMaterial in_originMaterial,
	OutputDirector* in_fracturerBaseWriterRef)
{
	originFTrianglePoints[0] = in_fracturePoint0;
	originFTrianglePoints[1] = in_fracturePoint1;
	originFTrianglePoints[2] = in_fracturePoint2;
	originFTriangleEmptynormal = in_fractureEmptyNormal;
	originBoundaryOrientation = in_originBoundaryOrientation;
	originPerfectClampValue = in_originPerfectClampValue;
	originMaterial = in_originMaterial;
	fracturerBaseWriterRef = in_fracturerBaseWriterRef;
}

void FTriangleFracturerBase::setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef)
{
	ftfOutputRef = in_outputRef;
};

void FTriangleFracturerBase::printFracturerPoints()
{
	fracturerPoints.printAllPoints();
}

void FTriangleFracturerBase::analyzeAndCleanupStagers(FTriangleType in_fTriangleTypeForDebug)
{
	// the set that will contain invalid stagers to erase.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> invalidStagerKeys;

	int preModifyStagerMapSize = stagerMap.size();
	//std::cout << "Size of stager map, pre-cleanup:" << preModifyStagerMapSize << std::endl;

	// analyze everything in the stagerMap.
	for (auto& currentStager : stagerMap)
	{
		// Below: FTDEBUG (uncomment when done)
		/*				
		std::cout << "Analyzing stager at key: ";
		EnclaveKeyDef::EnclaveKey currentStagerKey = currentStager.first;
		currentStagerKey.printKey();
		std::cout << std::endl;
		*/

		// check if the stager has met the conditions for being valid (i.e., at least 3 lines)

		/*
		bool isStagerValid = currentStager.second.analyzeAndReorganize();
		if (!isStagerValid)
		{
			
			//std::cout << "NOTICE: Detected stager as invalid, at key "; 
			//EnclaveKeyDef::EnclaveKey currentKey = currentStager.first;
			//currentKey.printKey();
			//std::cout << " will remove. " << std::endl;

			//int invalidRemoval = 3;
			//std::cin >> invalidRemoval;
			
			invalidStagerKeys.insert(currentStager.first);
		}
		*/

		FTLResolutionStatus resolutionStatus = currentStager.second.analyzeAndReorganize(currentStager.first,
																						in_fTriangleTypeForDebug,
																						localizedFTrianglePoints,
																						fracturerBaseWriterRef);
		// If it's ok to purge it, just do that here (we won't record it)...
		if
		(
			resolutionStatus == FTLResolutionStatus::FTLR_PURGABLE
		)
		{
			invalidStagerKeys.insert(currentStager.first);
		}

		// ...but if we could not find a solution at all (incalculable), we must record that.
		else if
		(
				resolutionStatus == FTLResolutionStatus::FTLR_INCALCULABLE
		)
		{
			incalculableKeys.insert(currentStager.first);
		}
	}

	// erase the invalid stagers.
	for (auto& currentInvalid : invalidStagerKeys)
	{	
		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "Erasing element at: ";
		EnclaveKeyDef::EnclaveKey keyCopy = currentInvalid;
		keyCopy.printKey();
		std::cout << std::endl;
		*/

		stagerMap.erase(currentInvalid);
	}

	//int postModifyStagerMapSize = stagerMap.size();
	//std::cout << "stagerMap size, pre-modify: " << preModifyStagerMapSize << " | post-modify: "<< postModifyStagerMapSize << std::endl;

	// FTDEBUG, (uncomment when needed for debugging)
	//std::cout << "(FTriangleFracturerBase::analyzeAndCleanupStagers) done with cleanup.";
	//int doneCleanup = 0;
	//std::cin >> doneCleanup;
}

std::map<FRayCasterTypeEnum, FRayCasterInitData> FTriangleFracturerBase::getUsableRayCasters()
{
	std::map<FRayCasterTypeEnum, FRayCasterInitData> mappedInitData;
	
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	int minZ = 0;
	int maxZ = 0;

	// cycle through each key to determine the min max for X/Y/Z
	std::set<int> xKeyValues;
	std::set<int> yKeyValues;
	std::set<int> zKeyValues;

	// throw each x/y/z into it's appropriate set; remember, sets are automatically ordered from least to greatest.
	for (int x = 0; x < 3; x++)
	{
		xKeyValues.insert(scanningKeys[x].x);
		yKeyValues.insert(scanningKeys[x].y);
		zKeyValues.insert(scanningKeys[x].z);
	}

	// the begin of a set of ints is the least value, the rbegin is the greatest value.
	minX = *xKeyValues.begin();
	maxX = *xKeyValues.rbegin();

	minY = *yKeyValues.begin();
	maxY = *yKeyValues.rbegin();

	minZ = *zKeyValues.begin();
	maxZ = *zKeyValues.rbegin();

	int xGridWidth = maxX - minX;
	int yGridWidth = maxY - minY;
	int zGridWidth = maxZ - minZ;

	// Below: FTDEBUG (uncomment when needed
	/*
	std::cout << "xGridWidth: " << xGridWidth << std::endl;
	std::cout << "yGridWidth: " << yGridWidth << std::endl;
	std::cout << "zGridWidth: " << zGridWidth << std::endl;
	*/

	// check for valid X grid (requires Y and Z grid width >= 1)
	if
	(
		(yGridWidth >= 1)
		&&
		(zGridWidth >= 1)
	)
	{
		//std::cout << "Found valid X grid. " << std::endl;

		// requires: minY/maxY, minZ/maxZ, and the max of our target ray cast dim (so maxX)
		FRayCasterInitData xData(minY, maxY, minZ, maxZ, rayCastDimInterval, minX, maxX);
		mappedInitData[FRayCasterTypeEnum::X_RAY] = xData;
	}

	// check for valid Y grid (requires X and Z grid width > = 1)
	if
	(
		(xGridWidth >= 1)
		&&
		(zGridWidth >= 1)
	)
	{
		//std::cout << "Found valid Y grid. " << std::endl;

		// requires: minX/maxX, minZ/maxZ, and the max of our target ray cast dim (so maxY)
		FRayCasterInitData yData(minX, maxX, minZ, maxZ, rayCastDimInterval, minY, maxY);
		mappedInitData[FRayCasterTypeEnum::Y_RAY] = yData;
	}

	// check for valid Z grid (requires X and Y grid width > = 1)
	if
	(
		(xGridWidth >= 1)
		&&
		(yGridWidth >= 1)
	)
	{
		//std::cout << "Found valid Z grid. " << std::endl;

		// requires: minX/maxX, minY/maxY, and the max of our target ray cast dim (so maxZ)
		FRayCasterInitData zData(minX, maxX, minY, maxY, rayCastDimInterval, minZ, maxZ);
		mappedInitData[FRayCasterTypeEnum::Z_RAY] = zData;
	}

	return mappedInitData;
}

void FTriangleFracturerBase::erasePermitFromSet(DifferingDimValues in_dimValueForErase, std::set<LineScanPermit>* in_permitMapRef)
{
	switch (in_dimValueForErase)
	{
		case DifferingDimValues::DIFFERENT_X: { in_permitMapRef->erase(LineScanPermit::SCAN_X); break;}
		case DifferingDimValues::DIFFERENT_Y: { in_permitMapRef->erase(LineScanPermit::SCAN_Y); break;}
		case DifferingDimValues::DIFFERENT_Z: { in_permitMapRef->erase(LineScanPermit::SCAN_Z); break;}
	}
}

std::set<FTriangleFracturerBase::LineScanPermit> FTriangleFracturerBase::getValidPermits()
{
	// populate the permit with all 3 valid permits; we will subtract from this as necessary.
	std::set<LineScanPermit> validPermits;
	validPermits.insert(LineScanPermit::SCAN_X);
	validPermits.insert(LineScanPermit::SCAN_Y);
	validPermits.insert(LineScanPermit::SCAN_Z);

	// First set of tests: perfect clamping.
	switch (originPerfectClampValue)
	{

		// For any dimension that is perfectly clamped, the scan that is aligned on that dimension should not be used.
		case PerfectClampEnum::CLAMPED_TO_X: { validPermits.erase(LineScanPermit::SCAN_X); break; }
		case PerfectClampEnum::CLAMPED_TO_Y: { validPermits.erase(LineScanPermit::SCAN_Y); break; }
		case PerfectClampEnum::CLAMPED_TO_Z: { validPermits.erase(LineScanPermit::SCAN_Z); break; }

		

		// Below: this case was meant to exclude unnecessary scans on certain dimensions,
		// in order to avoid duplicate lines from being inserted into FTriangleProductionStager instances. 
		// However, with the enhanced checks to FTriangleProductionStager::insertLine -- which prevent duplicate line insertion --
		// the below logic may not be needed. It will have to be looked at, at a later point in time.
		case PerfectClampEnum::NONE:
		{
			
			/*
			std::set<DifferingDimValues> baseValues;
			baseValues.insert(DifferingDimValues::DIFFERENT_X);
			baseValues.insert(DifferingDimValues::DIFFERENT_Y);
			baseValues.insert(DifferingDimValues::DIFFERENT_Z);

			// Check line 0 (point0 -> point1, subtract point0 from point1)
			ECBPolyPoint line0Diff = (originFTrianglePoints[1] - originFTrianglePoints[0]);
			std::set<DifferingDimValues> line0DimValues = baseValues;
			int line0NonDiffCount = 0;
			if (line0Diff.x == 0.0f)
			{
				line0DimValues.erase(DifferingDimValues::DIFFERENT_X);
				line0NonDiffCount++;
			}

			if (line0Diff.y == 0.0f)
			{
				line0DimValues.erase(DifferingDimValues::DIFFERENT_Y);
				line0NonDiffCount++;
			}

			if (line0Diff.z == 0.0f)
			{
				line0DimValues.erase(DifferingDimValues::DIFFERENT_Z);
				line0NonDiffCount++;
			}

			std::cout << "(Line 0): -> number of diffs: " << line0DimValues.size() << std::endl;
			if (line0DimValues.size() == 1)
			{
				//erasePermitFromSet(*line0DimValues.begin(), &validPermits);
			}


			// Check line 1 (point1 -> point2, subtract point1 from point2)
			ECBPolyPoint line1Diff = (originFTrianglePoints[2] - originFTrianglePoints[1]);
			std::set<DifferingDimValues> line1DimValues = baseValues;
			int line1NonDiffCount = 0;
			if (line1Diff.x == 0.0f)
			{
				line1DimValues.erase(DifferingDimValues::DIFFERENT_X);
				line1NonDiffCount++;
			}
			if (line1Diff.y == 0.0f)
			{
				line1DimValues.erase(DifferingDimValues::DIFFERENT_Y);
				line1NonDiffCount++;
			}
			if (line1Diff.z == 0.0f)
			{
				line1DimValues.erase(DifferingDimValues::DIFFERENT_Z);
				line1NonDiffCount++;
			}
			std::cout << "(Line 1): -> number of diffs: " << line1DimValues.size() << std::endl;
			if (line1DimValues.size() == 1)
			{
				//erasePermitFromSet(*line1DimValues.begin(), &validPermits);
			}



			// Check line 2 (point2 -> point0, subtract point2 from point0)
			ECBPolyPoint line2Diff = (originFTrianglePoints[0] - originFTrianglePoints[2]);
			std::set<DifferingDimValues> line2DimValues = baseValues;
			int line2NonDiffCount = 0;
			if (line2Diff.x == 0.0f)
			{
				line2DimValues.erase(DifferingDimValues::DIFFERENT_X);
				line2NonDiffCount++;
			}
			if (line2Diff.y == 0.0f)
			{
				line2DimValues.erase(DifferingDimValues::DIFFERENT_Y);
				line2NonDiffCount++;
			}
			if (line2Diff.z == 0.0f)
			{
				line2DimValues.erase(DifferingDimValues::DIFFERENT_Z);
				line2NonDiffCount++;
			}
			std::cout << "(Line 2): -> number of diffs: " << line2DimValues.size() << std::endl;
			if (line2DimValues.size() == 1)
			{
				//erasePermitFromSet(*line2DimValues.begin(), &validPermits);
			}
			*/
			break;
		}
	}

	/*
	std::cout << "Size of validPermits is: " << validPermits.size() << std::endl;

	int permitSizeWait = 7;
	std::map<int, int> crapCompileInts;
	std::cin >> permitSizeWait;
	*/

	return validPermits;
}

int FTriangleFracturerBase::getScanDimensionalStartKey(FTriangleFracturerBase::LineScanPermit in_dimensionToFetch)
{
	// Below: remember, in a set, the values are arranged least to greatest.
	int startingDimensionValue = 0;

	switch (in_dimensionToFetch)
	{
		case LineScanPermit::SCAN_X:
		{
			std::set<int> allXValues;
			for (int a = 0; a < 3; a++)
			{
				allXValues.insert(originFTriangleKeys[a].x);
			}

			startingDimensionValue = *allXValues.begin();
			break;
		}

		case LineScanPermit::SCAN_Y:
		{
			std::set<int> allYValues;
			for (int b = 0; b < 3; b++)
			{
				allYValues.insert(originFTriangleKeys[b].y);
			}

			startingDimensionValue = *allYValues.begin();
			break;
		}

		case LineScanPermit::SCAN_Z:
		{
			std::set<int> allZValues;
			for (int c = 0; c < 3; c++)
			{
				allZValues.insert(originFTriangleKeys[c].z);
			}

			startingDimensionValue = *allZValues.begin();
			break;
		}
	}

	return startingDimensionValue;
}

FTriangleFracturerBase::LineScanLists FTriangleFracturerBase::getScanningIntervals(float in_fixedIntervalValue)
{
	LineScanLists returnLists;

	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	int minZ = 0;
	int maxZ = 0;

	// cycle through each key to determine the min max for X/Y/Z
	std::set<int> xKeyValues;
	std::set<int> yKeyValues;
	std::set<int> zKeyValues;

	// throw each x/y/z into it's appropriate set; remember, sets are automatically ordered from least to greatest.
	for (int x = 0; x < 3; x++)
	{
		xKeyValues.insert(originFTriangleKeys[x].x);
		yKeyValues.insert(originFTriangleKeys[x].y);
		zKeyValues.insert(originFTriangleKeys[x].z);
	}

	// the begin of a set of ints is the least value, the rbegin is the greatest value.
	minX = *xKeyValues.begin();
	maxX = *xKeyValues.rbegin();

	minY = *yKeyValues.begin();
	maxY = *yKeyValues.rbegin();

	minZ = *zKeyValues.begin();
	maxZ = *zKeyValues.rbegin();

	
	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "Min/Max interval values are: " << std::endl;
	std::cout << "minX: " << minX << std::endl;
	std::cout << "maxX: " << maxX << std::endl;
	std::cout << "minY: " << minY << std::endl;
	std::cout << "maxY: " << maxY << std::endl;
	std::cout << "minZ: " << minZ << std::endl;
	std::cout << "maxZ: " << maxZ << std::endl;
	*/

	int xGridWidth = maxX - minX;
	int yGridWidth = maxY - minY;
	int zGridWidth = maxZ - minZ;

	// Determine the fixed X values; start with the minimum X value, and increment by the in_fixedIntervalValue, for
	// an amount of times equal to xGridWidth.
	if (xGridWidth >= 1)
	{
		float currentX = IndependentUtils::roundToHundredth(((*xKeyValues.begin()) * in_fixedIntervalValue) + in_fixedIntervalValue);
		for (int x = 0; x < xGridWidth; x++)
		{
			returnLists.xList.insert(currentX);
			//std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted X list value: " << currentX << std::endl;
			currentX = IndependentUtils::roundToHundredth(currentX + in_fixedIntervalValue);
		}
	}

	// ...repeat this for Y
	if (yGridWidth >= 1)
	{
		float currentY = IndependentUtils::roundToHundredth(((*yKeyValues.begin()) * in_fixedIntervalValue) + in_fixedIntervalValue);
		for (int y = 0; y < yGridWidth; y++)
		{
			returnLists.yList.insert(currentY);
			//std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted Y list value: " << currentY << std::endl;
			currentY = IndependentUtils::roundToHundredth(currentY + in_fixedIntervalValue);
		}
	}

	// ...and Z
	if (zGridWidth >= 1)
	{
		float currentZ = IndependentUtils::roundToHundredth(((*zKeyValues.begin()) * in_fixedIntervalValue) + in_fixedIntervalValue);
		for (int z = 0; z < zGridWidth; z++)
		{
			returnLists.zList.insert(currentZ);
			//std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted Z list value: " << currentZ << std::endl;
			currentZ = IndependentUtils::roundToHundredth(currentZ + in_fixedIntervalValue);
		}
	}

	return returnLists;
}

void FTriangleFracturerBase::buildAndRunFLineScanners()
{
	auto acceptablePermits = getValidPermits();
	auto XYZLists = getScanningIntervals(rayCastDimInterval);	// the parameter of in_fixedDimInterval needs to be the value of rayCastDimInterval from the base class.

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
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): criteria met for XDimLineScanner. (goes forward toward positive X)" << std::endl;
		int startBackwardXValue = getScanDimensionalStartKey(LineScanPermit::SCAN_X);
		int startForwardXValue = startBackwardXValue + 1;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): backward X start key is: " << startBackwardXValue << std::endl;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): forward X start key is: " << startForwardXValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> xScanner(new XDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_X] = xScanner;
		lineScannerMap[LineScanPermit::SCAN_X]->initializeLineScanner(XYZLists.xList,
			&fracturerPoints,
			&stagerMap,
			startBackwardXValue,
			startForwardXValue,
			rayCastDimInterval);
		//std::cout << "!! Start of run for SCAN_X..." << std::endl;
		lineScannerMap[LineScanPermit::SCAN_X]->runScan();
		//std::cout << "!! End of run for SCAN_X..." << std::endl;

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
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): criteria met for YDimLineScanner. (goes forward toward positive Y)" << std::endl;
		int startBackwardYValue = getScanDimensionalStartKey(LineScanPermit::SCAN_Y);
		int startForwardYValue = startBackwardYValue + 1;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): backward Y start key is: " << startBackwardYValue << std::endl;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): forward Y start key is: " << startForwardYValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> yScanner(new YDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_Y] = yScanner;
		lineScannerMap[LineScanPermit::SCAN_Y]->initializeLineScanner(XYZLists.yList,
			&fracturerPoints,
			&stagerMap,
			startBackwardYValue,
			startForwardYValue,
			rayCastDimInterval);
		//std::cout << "!! Start of run for SCAN_Y..." << std::endl;
		lineScannerMap[LineScanPermit::SCAN_Y]->runScan();
		//std::cout << "!! End of run for SCAN_Y..." << std::endl;
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
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): criteria met for ZDimLineScanner. (goes forward toward positive Z)" << std::endl;
		int startBackwardZValue = getScanDimensionalStartKey(LineScanPermit::SCAN_Z);
		int startForwardZValue = startBackwardZValue + 1;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): backward Z start key is: " << startBackwardZValue << std::endl;
		//std::cout << "(WorldFracturingMachine::buildAndRunFLineScanners()): forward Z start key is: " << startForwardZValue << std::endl;

		std::shared_ptr<DimensionalLineScannerBase> zScanner(new ZDimLineScanner);
		lineScannerMap[LineScanPermit::SCAN_Z] = zScanner;
		lineScannerMap[LineScanPermit::SCAN_Z]->initializeLineScanner(XYZLists.zList,
			&fracturerPoints,
			&stagerMap,
			startBackwardZValue,
			startForwardZValue,
			rayCastDimInterval);
		//std::cout << "!! Start of run for SCAN_Z..." << std::endl;
		lineScannerMap[LineScanPermit::SCAN_Z]->runScan();
		//std::cout << "!! End of run for SCAN_Z..." << std::endl;
	}

	/*
	std::cout << "!!! Done printing scan dims. " << std::endl;
	int scanDimsWait = 3;
	std::cin >> scanDimsWait;
	*/
	
}

void FTriangleFracturerBase::buildAndRunFRayCasters()
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

void FTriangleFracturerBase::runRaycastCollisionResolver()
{
	FExteriorRaycastCollisionResolver collResolver(&fracturerPoints, rayCastDimInterval);
}