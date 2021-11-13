#include "stdafx.h"
#include "PRMA2DResolverXY.h"

void PRMA2DResolverXY::transferLinksAndSetStaticDimValue(ResolverLinkMap in_linkMap)
{
	resolverLinks = in_linkMap;
	staticDimensionValue = resolverLinks.links.begin()->second.originalPoint.z;	// remember, for XY, we get the Z-dimension as the static value.
}

ECBPolyPoint PRMA2DResolverXY::generateExpandedPoint(float in_dimA, float in_dimB)
{
	return ECBPolyPoint(in_dimA, in_dimB, staticDimensionValue);
}

void PRMA2DResolverXY::runResolutionAttempt()
{

}