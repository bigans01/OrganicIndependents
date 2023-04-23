#include "stdafx.h"
#include "FTLResolverDualStickSaw.h"

bool FTLResolverDualStickSaw::runAttemptedResolution()
{
	bool resolutionFound = false;
	
	if (resolverDebug)
	{
		std::cout << "(FTLResolverDualStickSaw) --> attempting resolution; size of originalLines is: "; std::cout << originalLines.size(); std::cout << std::endl;
	}


	if (originalLines.size() == 0)
	{
		int anomalousHalt = 3;
		std::cout << "WARNING: found number of originalLines as being 0! "; std::cout << std::endl;
		std::cin >> anomalousHalt;
	}

	if (originalLines.size() >= 4)
	{
		// We will have to go through each line, here. And find exactly one other line to remove.
		// Then, compare all the remaining lines to see if they are valid.

		// We only need to create the original line map once.
		std::map<int, FTriangleLine> originalLineMap;
		for (auto& lineToLoad : originalLines)
		{
			originalLineMap[originalLineMap.size()] = lineToLoad;
		}

		for (auto& currentLine : originalLines)
		{
			// For each line, we must do the following, as long as we haven't found a solution yet.
			if (!resolutionFound)
			{

				// 1.) copy the originalLineMap into the currentLineMap.
				std::map<int, FTriangleLine> currentLineMap = originalLineMap;

				// 2.) Find the index of the line we will be comparing; also load up an OperableIntSet that contains
				// all the indices to run against, then subtract the found one from it.
				OperableIntSet allIndices;
				int currentLineIndex = 0;
				for (auto& indexSearcher : currentLineMap)
				{
					allIndices += indexSearcher.first;
					// When we found the index of the matched line, that's the one to use for this.
					if (currentLine == indexSearcher.second)
					{
						currentLineIndex = indexSearcher.first;
					}
				}

				// 3.) loop through every index except the target one; simply subtract the currentLineIndex from the allIndices set to do this.
				allIndices -= currentLineIndex;

				for (auto& currentOtherIndex : allIndices)
				{
					// For every iteration, we need a copy of the map.
					auto currentIterMap = currentLineMap;

					if (resolverDebug)
					{
						std::cout << "||||||| Erasing -> currentLineIndex: " << currentLineIndex << " | currentOtherIndex: " << currentOtherIndex << std::endl;
					}

					// Now, subtract two indices: the one at the currentLineIndex, and the other at the currentOtherIndex.
					currentIterMap.erase(currentLineIndex);
					currentIterMap.erase(currentOtherIndex);

					//std::cout << "!! Done erasing..." << std::endl;

					// Now create a vector from these, and compare it to see if it's valid.
					std::vector<FTriangleLine> remainingLines;
					for (auto& currentRemainingLine : currentIterMap)
					{
						// push back the remaining line at the specifieed index.
						remainingLines.push_back(currentIterMap[currentRemainingLine.first]);
					}
					//std::cout << "!! Done with pushbacks..." << std::endl;

					// Finally, compare all the remaining lines to see if it did the job.

					bool isValid = checkLineValidity(remainingLines);
					//std::cout << "!! Done with validity check..." << std::endl;
					if (isValid)
					{
						std::cout << "(FTLResolverDualStickSaw) Found resolution!" << std::endl;
						resolutionFound = true;	// flag that we've found a solution, so we can stop analyzing each line.
						determinedResolutionStatus = FTLResolutionStatus::FTLR_VALID;
						break;	// break out of here, there's no point in continuing all this crap.
					}
				}
			}
		}
	}

	return resolutionFound;
}