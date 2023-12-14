#include "stdafx.h"
#include "RenderableTriangleBase.h"

void RenderableTriangleBase::setRenderingType(RDerivedTypeEnum in_enumType)
{
	rDerived = in_enumType;
}

RDerivedTypeEnum RenderableTriangleBase::getRenderingType()
{
	return rDerived;
}

void RenderableTriangleBase::initRenderable(FTrianglePoint in_point0,
	FTrianglePoint in_point1,
	FTrianglePoint in_point2,
	ECBPolyPoint in_rEmptyNormal,
	BoundaryPolyIndicator in_rBoundaryIndicator,
	PerfectClampEnum in_rClampEnum)
{
	rPoints[0] = in_point0;
	rPoints[1] = in_point1;
	rPoints[2] = in_point2;
	rEmptyNormal = in_rEmptyNormal;
	rBoundaryIndicator = in_rBoundaryIndicator;
	rClampEnum = in_rClampEnum;
}

FTrianglePoint RenderableTriangleBase::getPoint(int in_index)
{
	return rPoints[in_index];
}

void RenderableTriangleBase::setRFTriangleType(FTriangleType in_typeToSet)
{
	rfTriangleType = in_typeToSet;
}

ECBPolyPoint RenderableTriangleBase::getEmptyNormal()
{
	return rEmptyNormal;
}

BoundaryPolyIndicator RenderableTriangleBase::getRBoundaryIndicator()
{
	return rBoundaryIndicator;
}