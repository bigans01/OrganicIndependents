#pragma once

#ifndef PRMA2DRESOLVERXY_H
#define PRMA2DRESOLVERXY_H

#include "PRMA2DResolverBase.h"

class PRMA2DResolverXY : public PRMA2DResolverBase
{
	public:
		void transferLinksAndSetStaticDimValue(ResolverLinkMap in_linkMap);
		ECBPolyPoint generateExpandedPoint(float in_dimA, float in_dimB);
		void runResolutionAttempt();
};

#endif
