#include "stdafx.h"
#include "OrganicTriangleSecondary.h"

void OrganicTriangleSecondary::loadDataFromEnclaveTriangleContainer(EnclaveTriangleContainer* in_containerRef)
{
	auto trianglesBegin = in_containerRef->triangles.begin();
	auto trianglesEnd = in_containerRef->triangles.end();
	for (trianglesBegin; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		//std::cout << "!! Stats for EnclaveTriangle " << trianglesBegin->first << std::endl;
		enclaveTriangleTertiaryContainer[trianglesBegin->first] = trianglesBegin->second.enclaveTriangleTertiary;

		/*
		auto enclaveTriangleTertiaryContainerBegin = enclaveTriangleTertiaryContainer[trianglesBegin->first].triangleMap.begin();
		auto enclaveTriangleTertiaryContainerEnd = enclaveTriangleTertiaryContainer[trianglesBegin->first].triangleMap.end();
		for (enclaveTriangleTertiaryContainerBegin; enclaveTriangleTertiaryContainerBegin != enclaveTriangleTertiaryContainerEnd; enclaveTriangleTertiaryContainerBegin++)
		{
			EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(enclaveTriangleTertiaryContainerBegin->first);
			if
				(
				(blockKey.x == 3)
					&&
					(blockKey.y == 3)
					&&
					(blockKey.z == 0)
					)
			{
				//std::cout << "! Block Key: " << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << std::endl;
				int numberOfPoints = enclaveTriangleTertiaryContainerBegin->second.poly.numberOfTertiaries + 2;
				for (int x = 0; x < numberOfPoints; x++)
				{
					//std::cout << ": point " << x << " |> " << (int(enclaveTriangleTertiaryContainerBegin->second.vertices[x].x)) / 100 << ", " << (int(enclaveTriangleTertiaryContainerBegin->second.vertices[x].y)) / 100 << ", " << (int(enclaveTriangleTertiaryContainerBegin->second.vertices[x].z)) / 100 << std::endl;
				}

				//std::cout << "Material ID is: " << enclaveTriangleTertiaryContainerBegin->second.poly.materialID << std::endl;
			}
		}
		*/
	}
}