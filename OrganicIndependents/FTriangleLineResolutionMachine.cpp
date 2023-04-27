#include "stdafx.h"
#include "FTriangleLineResolutionMachine.h"

FTriangleLineResolutionMachine::FTriangleLineResolutionMachine(std::vector<FTriangleLine> in_originalMachineLines, OutputDirector* in_machineWriterRef)
{
	originalMachineLines = in_originalMachineLines;
	machineWriterRef = in_machineWriterRef;
	setupResolvers();
}

void FTriangleLineResolutionMachine::setupResolvers()
{
	resolverMap[0] = std::unique_ptr<FTriangleLineResolverBase>(new FTLResolverExteriorStickSaw());
	resolverMap[1] = std::unique_ptr<FTriangleLineResolverBase>(new FTLResolverScannedStickSaw());
	resolverMap[2] = std::unique_ptr<FTriangleLineResolverBase>(new FTLResolverClampedCorner());
	resolverMap[3] = std::unique_ptr<FTriangleLineResolverBase>(new FTLResolverDualStickSaw());
}

void FTriangleLineResolutionMachine::runResolutionSequence()
{
	// Step 1: setup all resolvers
	for (auto& currentResolver : resolverMap)
	{
		currentResolver.second->initLineResolver(originalMachineLines, machineWriterRef);
	}

	// Step 2: attempt resolution through all resolvers. If we find a resolution, our output machine lines become the solution.
	for (auto& currentResolver : resolverMap)
	{
		bool wasResolved = currentResolver.second->runAttemptedResolution();
		if (wasResolved)
		{
			solutionLines = currentResolver.second->solutionLines;
			resolutionFound = true;
			resolvedStatus = currentResolver.second->determinedResolutionStatus;
			break;	// we're done, no need to continue.
		}
	}
}
