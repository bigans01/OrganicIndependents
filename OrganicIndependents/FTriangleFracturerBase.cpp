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

	glm::vec3 testVec3;

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