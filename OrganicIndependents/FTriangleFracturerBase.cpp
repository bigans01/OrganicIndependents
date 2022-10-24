#include "stdafx.h"
#include "FTriangleFracturerBase.h"

void FTriangleFracturerBase::transferFTriangleMetadata(ECBPolyPoint in_fracturePoint0,
	ECBPolyPoint in_fracturePoint1,
	ECBPolyPoint in_fracturePoint2,
	ECBPolyPoint in_fractureEmptyNormal,
	BoundaryOrientation in_originBoundaryOrientation,
	PerfectClampEnum in_originPerfectClampValue)
{
	originFTrianglePoints[0] = in_fracturePoint0;
	originFTrianglePoints[1] = in_fracturePoint1;
	originFTrianglePoints[2] = in_fracturePoint2;
	originFTriangleEmptynormal = in_fractureEmptyNormal;
	originBoundaryOrientation = in_originBoundaryOrientation;
	originPerfectClampValue = in_originPerfectClampValue;
}

void FTriangleFracturerBase::setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef)
{
	ftfOutputRef = in_outputRef;
};

void FTriangleFracturerBase::printFracturerPoints()
{
	fracturerPoints.printAllPoints();
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

	int xGridWidth = maxX - minX;
	int yGridWidth = maxY - minY;
	int zGridWidth = maxZ - minZ;

	std::cout << "xGridWidth: " << xGridWidth << std::endl;
	std::cout << "yGridWidth: " << yGridWidth << std::endl;
	std::cout << "zGridWidth: " << zGridWidth << std::endl;

	// check for valid X grid (requires Y and Z grid width >= 1)
	if
	(
		(yGridWidth >= 1)
		&&
		(zGridWidth >= 1)
	)
	{
		std::cout << "Found valid X grid. " << std::endl;

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
		std::cout << "Found valid Y grid. " << std::endl;

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
		std::cout << "Found valid Z grid. " << std::endl;

		// requires: minX/maxX, minY/maxY, and the max of our target ray cast dim (so maxZ)
		FRayCasterInitData zData(minX, maxX, minY, maxY, rayCastDimInterval, minZ, maxZ);
		mappedInitData[FRayCasterTypeEnum::Z_RAY] = zData;
	}

	return mappedInitData;
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

		// Check for X clamp. Remove SCAN_X if true.
		case PerfectClampEnum::CLAMPED_TO_X: { validPermits.erase(LineScanPermit::SCAN_X); break; }
		case PerfectClampEnum::CLAMPED_TO_Y: { validPermits.erase(LineScanPermit::SCAN_Y); break; }
		case PerfectClampEnum::CLAMPED_TO_Z: { validPermits.erase(LineScanPermit::SCAN_Z); break; }

		

		// Second set of tests: matching dimensions in points. Can only be done if
		// the perfect clamp value is NONE.
		case PerfectClampEnum::NONE:
		{

			enum class DifferingDimValues
			{
				DIFFERENT_X,
				DIFFERENT_Y,
				DIFFERENT_Z
			};

			// Check line 0 (point0 -> point1, subtract point0 from point1)
			ECBPolyPoint line0Diff = (originFTrianglePoints[1] - originFTrianglePoints[0]);
			std::set<DifferingDimValues> line0DimValues;
			line0DimValues.insert(DifferingDimValues::DIFFERENT_X);
			line0DimValues.insert(DifferingDimValues::DIFFERENT_Y);
			line0DimValues.insert(DifferingDimValues::DIFFERENT_Z);
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



			// Check line 1 (point1 -> point2, subtract point1 from point2)
			ECBPolyPoint line1Diff = (originFTrianglePoints[2] - originFTrianglePoints[1]);
			std::set<DifferingDimValues> line1DimValues;
			line1DimValues.insert(DifferingDimValues::DIFFERENT_X);
			line1DimValues.insert(DifferingDimValues::DIFFERENT_Y);
			line1DimValues.insert(DifferingDimValues::DIFFERENT_Z);
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



			// Check line 2 (point2 -> point0, subtract point2 from point0)
			ECBPolyPoint line2Diff = (originFTrianglePoints[0] - originFTrianglePoints[2]);
			std::set<DifferingDimValues> line2DimValues;
			line2DimValues.insert(DifferingDimValues::DIFFERENT_X);
			line2DimValues.insert(DifferingDimValues::DIFFERENT_Y);
			line2DimValues.insert(DifferingDimValues::DIFFERENT_Z);
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

			break;
		}
	}
	std::cout << "Size of validPermits is: " << validPermits.size() << std::endl;
	return validPermits;
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

	// Determine the fixed X values.
	if (xGridWidth >= 1)
	{
		auto currentXKey = xKeyValues.begin();
		for (int x = 0; x < xGridWidth; x++)
		{
			float currentXListValue = IndependentUtils::roundToHundredth(((*currentXKey) * in_fixedIntervalValue) + in_fixedIntervalValue);
			returnLists.xList.insert(currentXListValue);
			std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted X list value: " << currentXListValue << std::endl;
			currentXKey++;
		}
	}

	// Determine the fixed Y values.
	if (yGridWidth >= 1)
	{
		auto currentYKey = yKeyValues.begin();
		for (int y = 0; y < yGridWidth; y++)
		{
			float currentYListValue = IndependentUtils::roundToHundredth(((*currentYKey) * in_fixedIntervalValue) + in_fixedIntervalValue);
			returnLists.yList.insert(currentYListValue);
			std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted Y list value: " << currentYListValue << std::endl;
			currentYKey++;
		}
	}

	// Determine the fixed Z values
	if (zGridWidth >= 1)
	{
		auto currentZKey = zKeyValues.begin();
		for (int z = 0; z < zGridWidth; z++)
		{
			float currentZListValue = IndependentUtils::roundToHundredth(((*currentZKey) * in_fixedIntervalValue) + in_fixedIntervalValue);
			returnLists.zList.insert(currentZListValue);
			std::cout << "(FTriangleFracturerBase::getScanningIntervals) -> inserted Z list value: " << currentZListValue << std::endl;
			currentZKey++;
		}
	}

	return returnLists;
}