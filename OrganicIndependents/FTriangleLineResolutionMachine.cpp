#include "stdafx.h"
#include "FTriangleLineResolutionMachine.h"

FTriangleLineResolutionMachine::FTriangleLineResolutionMachine(std::vector<FTriangleLine> in_originalMachineLines)
{
	originalMachineLines = in_originalMachineLines;
	setupResolvers();
}

void FTriangleLineResolutionMachine::setupResolvers()
{
	resolverMap[0] = std::unique_ptr<FTriangleLineResolverBase>(new FTLResolverStickSaw());
}

void FTriangleLineResolutionMachine::runResolutionSequence()
{
	// Step 1: setup all resolvers
	for (auto& currentResolver : resolverMap)
	{
		currentResolver.second->initLineResolver(originalMachineLines);
	}

	// Step 2: attempt resolution through all resolvers. If we find a resolution, our output machine lines become the solution.
	for (auto& currentResolver : resolverMap)
	{
		bool wasResolved = currentResolver.second->runAttemptedResolution();
		if (wasResolved)
		{
			solutionLines = currentResolver.second->solutionLines;
			resolutionFound = true;
			break;	// we're done, no need to continue.
		}
	}
}
