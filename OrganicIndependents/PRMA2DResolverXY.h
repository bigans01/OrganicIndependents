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
#include "OperableIntSet.h"

class PRMA2DResolverXY : public PRMA2DResolverBase
{
	public:
		void initializeFromMessage(Message in_messageToInitializeFrom);
		ECBPolyPoint generateExpandedPoint(float in_dimA, float in_dimB);
		void runResolutionAttempt();
	private:
		ResolverLinkMap resolverLinks;
		std::map<int, std::shared_ptr<PMass>> pMassPtrMap;

		// comparison loop variables
		int comparisonsRemaining = 0;
		bool resolutionAcquired = false;

		void generateComparisonCount();
		bool areComparisonsDone();
		bool compareUnbondedMasses();	// compares PMasses to each other; returns true when a collision is found between two PMasses.
		void checkIfResolutionAchieved();	// needs to be called whenever a collision is detected, after the collision logic is done.

};

#endif
