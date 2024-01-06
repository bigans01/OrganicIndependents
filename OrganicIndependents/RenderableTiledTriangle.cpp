#include "stdafx.h"
#include "RenderableTiledTriangle.h"

Message RenderableTiledTriangle::writeRTToMessage()
{
	Message tiledMessage;

	// second: add the 3 points.
	for (int x = 0; x < 3; x++)
	{
		tiledMessage.insertPoint(IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(getPoint(x).point));
	}

	// next: add the material ID:
	tiledMessage.insertInt(int(tilingMaterial));

	// next: add the perfect clamp enum value:
	tiledMessage.insertInt(int(rClampEnum));

	// next: add the empty normal:
	tiledMessage.insertPoint(getEmptyNormal());

	// next: convert te unsigned char of the BoundaryPolyIndicator value, to int, and put that in.
	tiledMessage.insertInt(int(getRBoundaryIndicator().getIndicatorData()));

	return tiledMessage;
}

void RenderableTiledTriangle::initFromMessage(Message* in_messageREf)
{
	// Message should already be open; just read from it.

	// For points of a RenderableTiledTriangle, there is only the point data, no UV data is necessary.
	for (int x = 0; x < 3; x++)
	{
		rPoints[x] = in_messageREf->readPoint();
	}

	// next: set the material ID.
	tilingMaterial = TriangleMaterial(in_messageREf->readInt());

	// next: set the perfect clamp enum value.
	rClampEnum = PerfectClampEnum(in_messageREf->readInt());

	// next: set the empty normal.
	rEmptyNormal = in_messageREf->readPoint();

	// next: set the indicator data.
	rBoundaryIndicator.setIndicatorData(unsigned char(in_messageREf->readInt()));
}

void RenderableTiledTriangle::printStats()
{
	std::cout << "!!! Printing stats for RenderableTiledTriangle! " << std::endl;
	std::cout << "Points: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		rPoints[x].printPointData();
		std::cout << std::endl;
	}

	std::cout << "Empty normal: ";
	rEmptyNormal.printPointCoords(); 
	std::cout << std::endl;

	std::cout << "Value of tiling material: ";
	switch (tilingMaterial)
	{
		case TriangleMaterial::NOVAL: { std::cout << "NOVAL" << std::endl; break; }
		case TriangleMaterial::DIRT: { std::cout << "DIRT" << std::endl; break; }
		case TriangleMaterial::GRASS: { std::cout << "GRASS" << std::endl; break; }
	}

	std::cout << "Rendering type: ";
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