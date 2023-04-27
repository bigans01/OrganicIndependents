#include "stdafx.h"
#include "FTLResolverExteriorStickSaw.h"

bool FTLResolverExteriorStickSaw::runAttemptedResolution()
{

	resolverWriterRef->logLine("(FTLResolverExteriorStickSaw) --> attempting resolution....");

	bool resolutionFound = false;

	// the number of "passes" to perform this on, is equal to the number of EXTERIOR lines in the originalLines.
	// So just iterate through the vector, to get that number.
	//
	// We also need to keep track of a set that contains the indexes in the vector that we will need to attempt to erase,
	// to make a good resolution. So as we detect the indices of the EXTERIOR lines, we'll put them into the set.
	int numberOfPasses = 0;
	std::set<int> foundExteriorIndices;
	for (auto& currentLine : originalLines)
	{
		if (currentLine.getType() == FTriangleLineType::EXTERIOR)
		{
			foundExteriorIndices.insert(numberOfPasses);	// the current value of number of passes reflects the offset into the vector.
		}
		numberOfPasses++;
	}

	// The maximum number of times we will need to loop is equal to the size of the foundExteriorIndices set, but can break if we find a solution.
	for (auto& currentExteriorToSaw : foundExteriorIndices)
	{
		// Remember, for each pass, we will need a fresh copy of the originalLines.
		std::vector<FTriangleLine> passCopy = originalLines;

		// Attempt to erase the exterior line at the specified offset
		passCopy.erase(passCopy.begin() + currentExteriorToSaw);

		// Then, check it with checkLineValidity.
		bool isValid = checkLineValidity(passCopy);
		if (isValid)
		{
			resolutionFound = true;
			determinedResolutionStatus = FTLResolutionStatus::FTLR_VALID;

			// ||||||||||||||||| START DEBUG BLOCK

			resolverWriterRef->logLine("(FTLResolverStickRaw): !!! Resolution was found. Ordered lines are:");
			for (auto& currentLine : solutionLines)
			{
				// need to write line to std::string (again)
				resolverWriterRef->logLine(currentLine.printLineToString());
			}
			resolverWriterRef->logLine("(FTLResolverStickRaw): Done printing resolution lines.");
			// ||||||||||||||||| END DEBUG BLOCK

			//std::cout << "(FTLResolverStickRaw): Done printing resolution lines. Enter a number to continue." << std::endl;
			//int foundSolution = 3;
			//std::cin >> foundSolution;

			break;
		}
	}

	return resolutionFound;
}