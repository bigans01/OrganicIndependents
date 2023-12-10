#include "stdafx.h"
#include "RenderableTiledTriangle.h"

void RenderableTiledTriangle::printStats()
{
	std::cout << "!!! Printing stats for RenderableTiledTriangle! " << std::endl;
	std::cout << "Points: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		rPoints[x].printPointData();
		std::cout << std::endl;
	}

	std::cout << "Empty normal: " << std::endl;
	rEmptyNormal.printPointCoords(); std::cout << std::endl;

	std::cout << "Value of tiling material: " << std::endl;
	switch (tilingMaterial)
	{
		case TriangleMaterial::NOVAL: { std::cout << "NOVAL" << std::endl; break; }
		case TriangleMaterial::DIRT: { std::cout << "DIRT" << std::endl; break; }
		case TriangleMaterial::GRASS: { std::cout << "GRASS" << std::endl; break; }
	}

	std::cout << "Rendering type: " << std::endl;
	switch (rDerived)
	{
		case RDerivedTypeEnum::DERIVED_TYPE_UNDEFINED: { std::cout << "DERIVED_TYPE_UNDEFINED" << std::endl; break;}
		case RDerivedTypeEnum::R_TILED: { std::cout << "R_TILED" << std::endl; break;}
		case RDerivedTypeEnum::R_TEXTURED: { std::cout << "R_TEXTURED" << std::endl; break;}
	}
}

void RenderableTiledTriangle::setMaterialID(TriangleMaterial in_tilingMaterial)
{
	tilingMaterial = in_tilingMaterial;
}

TriangleMaterial RenderableTiledTriangle::fetchMaterialID()
{
	return tilingMaterial;
}

RenderableGenerationResult RenderableTiledTriangle::generateData()
{
	RenderableGenerationResult returnResult;

	// The OrganicTriangleTertiary below will produce the tiled data, which will then be returned.
	OrganicTriangleTertiary fTertProducer(IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(rPoints[0].point),
		IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(rPoints[1].point),
		IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(rPoints[2].point),
		rEmptyNormal,
		rBoundaryIndicator.getBoundaryIndicatorValue(),
		rClampEnum,
		tilingMaterial);

	returnResult = RenderableGenerationResult(fTertProducer.incalculableAttempts, fTertProducer.triangleMap);

	return returnResult;
}