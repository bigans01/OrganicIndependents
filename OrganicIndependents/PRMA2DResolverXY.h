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
		/*
		
		Remember, all Messages expect data in a particular order; 
		the Message format for the initializeFromMessage() function below MUST be:

		**ints**

		[0] = number of unbonded expansions per call of comparisonUnbondedMasses()
		[1] = number of points sent into this resolver
		[2 through [value at 1]-1] = the point IDs; 1 ID per point
		
		**ECBPolyPoint**

		N number of points, where N is the equivalent of the int at vector index [1] above

		
		*/
		void initializeFromMessage(Message in_messageToInitializeFrom);
		void runResolutionAttempt();
		void printPMassData();
		ResolverLinkMap generateResolverResult();
	private:
		ResolverLinkMap resolverLinks;
		std::map<int, std::shared_ptr<PMass>> pMassPtrMap;
		int expansionsPerUnbondedComparisonRun = 0;	// determines how many atomic expansions are done, per call of compareUnbondedMasses() below;
													// this value must be set by the call to initializeFromMessage()

		// comparison loop variables
		int comparisonsRemaining = 0;
		bool resolutionAcquired = false;

		void generateComparisonCount();
		bool areComparisonsDone();
		bool compareUnbondedMasses();	// compares PMasses to each other; returns true when a collision is found between two PMasses.
		void checkIfResolutionAchieved();	// needs to be called whenever a collision is detected, after the collision logic is done.
		bool produceAndRunComparisonSets(OperableIntSet in_unbondedMassSet, OperableIntSet in_allMassSet);
		void expandedUnbondedMasses();

		int getHighestMassPtrMapKeyPlusOne();
};

#endif
