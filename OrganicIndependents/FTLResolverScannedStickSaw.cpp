#include "stdafx.h"
#include "FTLResolverScannedStickSaw.h"

bool FTLResolverScannedStickSaw::runAttemptedResolution()
{
	resolverWriterRef->logLine("(FTLResolverScannedStickSaw) --> attempting resolution....");

	bool resolutionFound = false;

	// the number of passes is equal to the number of non EXTERIR lines.
	int numberOfPasses = 0;
	std::set<int> foundNonExteriorIndices;
	for (auto& currentLine : originalLines)
	{
		// anything that isn't an exterior line, needs to be checked.
		if (currentLine.getType() != FTriangleLineType::EXTERIOR)
		{
			foundNonExteriorIndices.insert(numberOfPasses);
		}
		numberOfPasses++;
	}

	// The maximum number of times we will need to loop is equal to the size of the foundNonExteriorIndices set, but can break if we find a solution.
	for (auto& currentScannedToSaw : foundNonExteriorIndices)
	{
		// Remember, for each pass, we will need a fresh copy of the originalLines.
		std::vector<FTriangleLine> passCopy = originalLines;

		// Attempt to erase the non-exterior line at the specified offset.
		passCopy.erase(passCopy.begin() + currentScannedToSaw);

		// Then, check it with checkLineValidity.
		bool isValid = checkLineValidity(passCopy);
		if (isValid)
		{
			resolutionFound = true;
			determinedResolutionStatus = FTLResolutionStatus::FTLR_VALID;

			// ||||||||||||||||||||||||||||||| START DEBUG BLOCK

			resolverWriterRef->logLine("(FTLResolverScannedStickSaw): !!! Resolution was found: Ordered lines are: ");
			for (auto& currentLine : solutionLines)
			{
				resolverWriterRef->logLine(currentLine.printLineToString());
			}
			resolverWriterRef->logLine("(FTLResolverScannedStickSaw): Done printing resolution lines.");

			// ||||||||||||||||||||||||||||||| END DEBUG BLOCK

			break;

		}
	}

	return resolutionFound;
}