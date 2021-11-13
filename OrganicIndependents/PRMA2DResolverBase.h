#pragma once

#ifndef PRM2ARESOLVERBASE_H
#define PRM2ARESOLVERBASE_H

// eg, an example class names that derives this base would be: PRMA2DResolverXY, PRMA2DResolverYZ, PRMA2DREsolverXZ
#include "ECBPolyPoint.h"
#include "ResolverLinkMap.h"

class PRMA2DResolverBase
{
	public:
		virtual void transferLinksAndSetStaticDimValue(ResolverLinkMap in_linkMap) = 0;
		virtual ECBPolyPoint generateExpandedPoint(float in_dimA, float in_dimB) = 0;
		virtual void runResolutionAttempt() = 0;
	protected:
		ResolverLinkMap resolverLinks;
		float staticDimensionValue = 0.0f;	// the single dimension value that isn't affected by 2d operations (i.e, this would be  the value of "Z" when doing XY)

};

#endif
