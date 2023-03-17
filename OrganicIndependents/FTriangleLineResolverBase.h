#pragma once

#ifndef FTRIANGLELINERESOLVERBASE_H
#define FTRIANGLELINERESOLVERBASE_H

#include "FTriangleLine.h"
#include "FTLResolutionStatus.h"

/*

Description:

This is the base class that all of the FTriangleLine resolution algorithms are based on (i.e, the FTLResolverExteriorStickSaw class)

*/

class FTriangleLineResolverBase
{
	public:
		void initLineResolver(std::vector<FTriangleLine> in_originalLines);
		virtual bool runAttemptedResolution() = 0;	// the virtual function that calls the algorithm that will attempt to do a resolution. If it returns true,
													// we should assume a usable solution was found. If false, a solution wasn't found, and it should be up to the 
													// calling function/class to decide what to do.
		std::vector<FTriangleLine> solutionLines;	// the lines that contain the resolved solution, if it was found.
		FTLResolutionStatus determinedResolutionStatus = FTLResolutionStatus::FTLR_NOVAL;	// needs to be set appropriately, by the derived class,
																							// when runAttemptedResolution() function would return true.

	protected:
		std::vector<FTriangleLine> originalLines;	// contains the original lines that need to be scanned for a resolution; all base classes need 
													// this to work with. Must be set by the initLineResolver function, after the child class has 
													// been instantiated.


		bool checkLineValidity(std::vector<FTriangleLine> in_linesToCheck);	// analyzes the given input vector, to see if it is an acceptable solution


		
};

#endif
