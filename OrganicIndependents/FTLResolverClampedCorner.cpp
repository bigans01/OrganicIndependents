#include "stdafx.h"
#include "FTLResolverClampedCorner.h"

bool FTLResolverClampedCorner::runAttemptedResolution()
{

	resolverWriterRef->logLine("(FTLResolverClampedCorner) --> attempting resolution.... ");
	bool resolutionFound = false;

	// This resolver can only operate under the condition that there are two lines. So check the original lines.
	if 
	(
		(originalLines.size() >= 2)
		&&
		(originalLines.size() <= 3)
	)
	{
		// Create 3 sets of floats: one for each dimension.
		std::set<float> xFloats;
		std::set<float> yFloats;
		std::set<float> zFloats;

		// Go through each point in the line, insert it.
		for (auto& currentLine : originalLines)
		{
			// pointA
			xFloats.insert(currentLine.pointA.x);
			yFloats.insert(currentLine.pointA.y);
			zFloats.insert(currentLine.pointA.z);

			// pointB
			xFloats.insert(currentLine.pointB.x);
			yFloats.insert(currentLine.pointB.y);
			zFloats.insert(currentLine.pointB.z);
		}

		// Now, check which dimension is clamped. There should only be 1.
		if (xFloats.size() == 1)
		{

			resolverWriterRef->logLine("(FTLResolverClampedCorner): found resolution; all points were clamped to an X value of: ", *xFloats.begin());	
			resolutionFound = true;
			determinedResolutionStatus = FTLResolutionStatus::FTLR_PURGABLE;
		}

		else if (yFloats.size() == 1)
		{

			resolverWriterRef->logLine("(FTLResolverClampedCorner): found resolution; all points were clamped to an X value of: ", *yFloats.begin());
			resolutionFound = true;
			determinedResolutionStatus = FTLResolutionStatus::FTLR_PURGABLE;
		}

		else if (zFloats.size() == 1)
		{

			resolverWriterRef->logLine("(FTLResolverClampedCorner): found resolution; all points were clamped to an Z value of: ", *zFloats.begin());
			resolutionFound = true;
			determinedResolutionStatus = FTLResolutionStatus::FTLR_PURGABLE;
		}

	}

	return resolutionFound;
}