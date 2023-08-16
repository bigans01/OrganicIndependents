#include "stdafx.h"
#include "FTriangleLineResolverBase.h"

void FTriangleLineResolverBase::initLineResolver(std::vector<FTriangleLine> in_originalLines,
												OutputDirector* in_resolverWriterRef)
{
	originalLines = in_originalLines;
	resolverWriterRef = in_resolverWriterRef;
}

bool FTriangleLineResolverBase::checkLineValidity(std::vector<FTriangleLine> in_linesToCheck)
{
	bool areLinesValid = true;


	resolverWriterRef->logLine("(FTriangleLineResolverBase::checkLineValidity): entered checkLineValidity, size of lines to check is: ", int(in_linesToCheck.size()));

	// Check 1: If the number of lines is 1, it's definitely not valid.
	if (int(in_linesToCheck.size()) == 1)
	{
		areLinesValid = false;
	}
	else
	{
		std::vector<FTriangleLine> newLineVector;	// this vector will contain an appropriate sequential ordering of the lines, as we attempt to build it.

		std::vector<FTriangleLine> debugCopy = in_linesToCheck;	// a copy of the original input vector, in case we need it for debug output.
															
		// get the very first line from the vector, push it into the newLineVector, and then erase it from stagerLines.
		auto currentFTriangleLine = in_linesToCheck.front();
		newLineVector.push_back(currentFTriangleLine);
		in_linesToCheck.erase(in_linesToCheck.begin());

		// load the remaining lines from the input vector into a map.
		std::map<int, FTriangleLine> remainingLineMap;
		for (auto& remainingStagerLine : in_linesToCheck)
		{
			remainingLineMap[remainingLineMap.size()] = remainingStagerLine;
		}

		// we must loop a number of times, where the number to loop is equal to the number of remaining lines in stagerLines (we already removed one already).
		int initialRemaining = remainingLineMap.size();


		resolverWriterRef->logLine("(FTriangleLineResolverBase::checkLineValidity): value of initialRemaining: ", initialRemaining);


		for (int x = 0; x < initialRemaining; x++)
		{
			bool nextLineFound = false;
			int remainingLineIndex = 0;

			//std::cout << "Remaining stager lines to scan: " << remainingLineMap.size() << std::endl;

			bool lastLineScan = false;
			if (remainingLineMap.size() <= 2)
			{
				lastLineScan = true;
			}

			for (auto& currentRemainingLine : remainingLineMap)
			{
				// compare the current remaining line against the rbegin of the newLineVector (as that would be the latest inserted line);
				// do this by comparing pointB of the latestNewLine, to both points of the currentRemainingLine.
				FTriangleLine latestNewLine = *newLineVector.rbegin();

				// remember, pointB of the latestNewLine is always the leader; it must always be used 
				// when finding the next line. Even more important, we must break out of the current 
				// for loop as soon as we detect a matching line.
				if (latestNewLine.pointB == currentRemainingLine.second.pointB)
				{
					FTriangleLine lineCopy = currentRemainingLine.second;
					lineCopy.swapPoints();
					remainingLineIndex = currentRemainingLine.first;
					newLineVector.push_back(lineCopy);
					nextLineFound = true;
					break;
				}
				else if (latestNewLine.pointB == currentRemainingLine.second.pointA)
				{
					FTriangleLine lineCopy = currentRemainingLine.second;
					remainingLineIndex = currentRemainingLine.first;
					newLineVector.push_back(lineCopy);
					nextLineFound = true;
					break;
				}
			}

			// if there was a nextLineFound, erase it.
			if (nextLineFound)
			{
				//std::cout << "!!! Will now erase this line: ";
				//remainingecbPolyPoints[remainingLineIndex].printLine();
				remainingLineMap.erase(remainingLineIndex);
			}
		}

		// the final checks, before attempting to build FTriangles; if any of the following is true,
		// it's bad.
		// -there are lines remaining in the remainingLineMap.
		// -pointB of the rbegin() of the newLineVector DOES NOT match pointA of the begin() of the vector.
		// -there aren't at least 3 lines in the newLineVector.
		if
		(
			(remainingLineMap.size() != 0)
			||
			(newLineVector.rbegin()->pointB != newLineVector.begin()->pointA)
			||
			(newLineVector.size() < 3)
		)
		{
			// //|||||||||||||||||||||||||| START: DEBUG block


			resolverWriterRef->logLine("(FTriangleLineResolverBase) !! Bad line sequence detected!");
			resolverWriterRef->logLine("(FTriangleLineResolverBase) !! Original lines were: ");
			for (auto& currentDebugLine : debugCopy)
			{
				// need code to print lines to std::string here (again)
				resolverWriterRef->logLine(currentDebugLine.printLineToString());
			}

			resolverWriterRef->log("!! Reasons: ");
			if (remainingLineMap.size() != 0)
			{
				resolverWriterRef->log(" :: lineMapSize not 0 :: | ");
			}

			if (newLineVector.rbegin()->pointB != newLineVector.begin()->pointA)
			{
				resolverWriterRef->log(" :: begin and end points don't match :: | ");
			}

			if (newLineVector.size() < 3)
			{
				resolverWriterRef->log(" :: number of lines is less than 3 :: | ");
			}
			resolverWriterRef->log("\n");
			//|||||||||||||||||||||||||| END: DEBUG block

			areLinesValid = false;
		}
		else
		{
			// Otherwise, we're all good.
			solutionLines = newLineVector;
		}
	}

	return areLinesValid;
}