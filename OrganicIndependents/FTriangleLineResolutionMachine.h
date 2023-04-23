#pragma once

#ifndef FTRIANGLELINERESOLUTIONMACHINE_H
#define FTRIANGLELINERESOLUTIONMACHINE_H

#include "FTriangleLineResolverBase.h"
#include "FTLResolverExteriorStickSaw.h"
#include "FTLResolverScannedStickSaw.h"
#include "FTLResolutionStatus.h"
#include "FTLResolverClampedCorner.h"
#include "FTLResolverDualStickSaw.h"

/*

Description:

This class is designed to assist the FTriangleProductionStager class, when that class detects that it's vector of FTriangleLine instances
that it is operating on isn't valid. Upon being instantiated, this class should create one or more FTriangleLineResolverBase instances.
Each FTriangleLineResolverBase instance should be a different algorithm/derivative of that base class; When the runResolutionSequence function is called,
it will attempt to run each algorithm. If the algorithm corrects the lines, it should return true and stop the call of the remaining algorithms. A value
of true also indicats that the lines have been solved, the values of which should be put into the solutionLines member.

*/

class FTriangleLineResolutionMachine
{
	public:
		FTriangleLineResolutionMachine(std::vector<FTriangleLine> in_originalMachineLines, bool in_runResolversInDebugMode);	// the sole constructor for this class; 
																							// sets the value of originalMachineLines, and
																							// also calls setupResolvers().
	
		void setupResolvers();			// this will setup all instances of FTriangleResolverBase in the resolverMap; must be called by
										// the constructor of this class.
			
		void runResolutionSequence();	// run each instance of FTriangleLineResolverBase that is in the resolver map,
										// until a solution is found (which causes a break/stop), or until all algorithms have been exhausted.

		bool resolutionFound = false;	// set to true whenever we find a resolutioon.

		bool runResolversInDebugMode = false;	// passed to all instances of FTriangleLineResolverBase, to tell them where to direct output to.

		FTLResolutionStatus resolvedStatus = FTLResolutionStatus::FTLR_NOVAL;	// no val by default, must be correctly set by an underlying instance of 
																				// FTriangleLineResolverBase.
		std::vector<FTriangleLine> solutionLines;	// this will store the solution, if one is found.
	private:
		std::vector<FTriangleLine> originalMachineLines;	// must be initialized by constructor.
		std::map<int, std::unique_ptr<FTriangleLineResolverBase>> resolverMap;	// must be set up via call to setupResolvers()
};

#endif
