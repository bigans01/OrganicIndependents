#include "stdafx.h"
#include "OrganicTriangleSecondary.h"

void OrganicTriangleSecondary::loadDataFromEnclaveTriangleContainer(EnclaveTriangleContainer* in_containerRef)
{
	auto trianglesBegin = in_containerRef->triangles.begin();
	auto trianglesEnd = in_containerRef->triangles.end();
	for (trianglesBegin; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		//std::cout << "!! Stats for EnclaveTriangle " << trianglesBegin->first << std::endl;
		secondaryMapContainer[trianglesBegin->first] = trianglesBegin->second.secondaryMap;

		/*
		auto secondaryMapContainerBegin = secondaryMapContainer[trianglesBegin->first].triangleMap.begin();
		auto secondaryMapContainerEnd = secondaryMapContainer[trianglesBegin->first].triangleMap.end();
		for (secondaryMapContainerBegin; secondaryMapContainerBegin != secondaryMapContainerEnd; secondaryMapContainerBegin++)
		{
			EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(secondaryMapContainerBegin->first);
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
				int numberOfPoints = secondaryMapContainerBegin->second.poly.numberOfTertiaries + 2;
				for (int x = 0; x < numberOfPoints; x++)
				{
					//std::cout << ": point " << x << " |> " << (int(secondaryMapContainerBegin->second.vertices[x].x)) / 100 << ", " << (int(secondaryMapContainerBegin->second.vertices[x].y)) / 100 << ", " << (int(secondaryMapContainerBegin->second.vertices[x].z)) / 100 << std::endl;
				}

				//std::cout << "Material ID is: " << secondaryMapContainerBegin->second.poly.materialID << std::endl;
			}
		}
		*/
	}
}