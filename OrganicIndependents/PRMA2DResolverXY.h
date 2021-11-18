#pragma once

#ifndef PRMA2DRESOLVERXY_H
#define PRMA2DRESOLVERXY_H

#include "PRMA2DResolverBase.h"
#include "ResolverLinkMap.h"
#include "ECBPolyPoint.h"
#include "PMass.h"
#include <map>
#include <mutex>
#include <iostream>

class PRMA2DResolverXY : public PRMA2DResolverBase
{
	public:
		void initializeFromMessage(Message in_messageToInitializeFrom);
		ECBPolyPoint generateExpandedPoint(float in_dimA, float in_dimB);
		void runResolutionAttempt();
	private:
		ResolverLinkMap resolverLinks;
		std::map<int, std::shared_ptr<PMass>> pMassPtrMap;

		void compareMasses();
};

#endif
