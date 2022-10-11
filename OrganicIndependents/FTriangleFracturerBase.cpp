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
	for (int x = 0; x < 3; x++)
	{
		minX = std::min(originFTriangleKeys[x].x, minX);
		maxX = std::max(originFTriangleKeys[x].x, maxX);

		minY = std::min(originFTriangleKeys[x].y, minY);
		maxY = std::max(originFTriangleKeys[x].y, maxY);

		minZ = std::min(originFTriangleKeys[x].z, minZ);
		maxZ = std::max(originFTriangleKeys[x].z, maxZ);
	}

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
		FRayCasterInitData xData(minY, maxY, minZ, maxZ, rayCastDimInterval);
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
		FRayCasterInitData yData(minX, maxX, minZ, maxZ, rayCastDimInterval);
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
		FRayCasterInitData zData(minX, maxX, minY, maxY, rayCastDimInterval);
		mappedInitData[FRayCasterTypeEnum::Z_RAY] = zData;
	}

	return mappedInitData;
}