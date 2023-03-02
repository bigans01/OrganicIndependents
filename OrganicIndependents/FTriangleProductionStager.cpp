#include "stdafx.h"
#include "FTriangleProductionStager.h"

void FTriangleProductionStager::insertLine(FTriangleLine in_lineToInsert)
{
	// scan to ensure that the line doesn't exist already.
	bool lineExists = false;
	for (auto& lineToScan : stagerLines)
	{
		if (lineToScan == in_lineToInsert)
		{
			//std::cout << "NOTICE: duplicate line found, will not insert line. " << std::endl;
			lineExists = true;
			duplicatesFound = true;
			break;
		}
	}

	if (!lineExists)	// the line was not detected as a duplicate
	{
		stagerLines.push_back(in_lineToInsert);
	}
}

void FTriangleProductionStager::translateLines(EnclaveKeyDef::EnclaveKey in_translationKey, float in_fixedDimensionInterval)
{
	DoublePoint calculatedTranslationValue(in_translationKey.x*in_fixedDimensionInterval,
										in_translationKey.y*in_fixedDimensionInterval,
										in_translationKey.z*in_fixedDimensionInterval);

	// FTDEBUG (uncomment when needed)
	/*
	std::cout << "--Old line values: " << std::endl;
	for (auto& currentLine : stagerLines)
	{
		currentLine.printLine();
	}
	*/
	
	for (auto& currentLine : stagerLines)
	{
		currentLine.translateLine(calculatedTranslationValue);
	}

	// FTDEBUG (uncomment when needed)
	/*
	std::cout << "--New line values (after translation): " << std::endl;
	for (auto& currentLine : stagerLines)
	{
		currentLine.printLine();
	}
	*/
}

void FTriangleProductionStager::printLines()
{
	for (auto& currentLine : stagerLines)
	{
		currentLine.printLine();
	}
}

std::vector<FTriangleLine> FTriangleProductionStager::fetchStagerLines()
{
	return stagerLines;
}

bool FTriangleProductionStager::analyzeAndReorganize()
{
	// assume it's valid.
	bool isStagerValid = true;

	//std::cout << "(FTriangleProductionStager::analyzeAndReorganize(): cleaning up..." << std::endl;
	std::string dupesFound = "";

	// remember, a duplicate doesn't necessarily mean it's bad. This is just for debug information.
	switch (duplicatesFound)
	{
		case (false): { dupesFound = "false"; break;}
		case (true): { dupesFound = "true"; break;}
	}

	//std::cout << "Number of lines: " << int(stagerLines.size()) << " | duplicatesFound: " << dupesFound << std::endl;

	// if the number of lines is just 1, it's definitely bad.
	if (int(stagerLines.size()) == 1)
	{
		isStagerValid = false;
	}

	// but if there are multiple lines, do the analysis.

	else
	{
		std::vector<FTriangleLine> newLineVector;

		// a copy of the lines, used for debugging, in case something goes wrong.
		std::vector<FTriangleLine> debugCopy = stagerLines;

		// FTDEBUG, (uncomment when needed for debugging)
		
		/*
		std::cout << "############ Before operating, lines are: " << std::endl;
		for (auto& prePrint : stagerLines)
		{
			prePrint.printLine();
		}
		int debugPrint = 3;
		std::cin >> debugPrint;
		*/

		// get the very first line from the vector, push it into the newLineVector, and then erase it from stagerLines.
		auto currentFTriangleLine = stagerLines.front();
		newLineVector.push_back(currentFTriangleLine);
		stagerLines.erase(stagerLines.begin());

		// load the remaining lines into a map.
		std::map<int, FTriangleLine> remainingLineMap;
		for (auto& remainingStagerLine : stagerLines)
		{
			remainingLineMap[remainingLineMap.size()] = remainingStagerLine;
		}
		
		// FTDEBUG, (uncomment when needed for debugging)
		/*
		std::cout << "Remaining stagerLines to run are: " << remainingLineMap.size() << std::endl;
		for (auto& remaining : remainingLineMap)
		{
			remaining.second.printLine();
		}
		int debugPrint = 3;
		*/

		// we must loop a number of times, where the number to loop is equal to the number of remaining lines in stagerLines (we already removed one already).
		int initialRemaining = remainingLineMap.size();
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
				//remainingLineMap[remainingLineIndex].printLine();
				remainingLineMap.erase(remainingLineIndex);
			}
		}

		//std::cout << "Size of remainingLineMap.size(), after loops: " << remainingLineMap.size() << std::endl;

		
		// set stagerLines to be the newLineVector.
		stagerLines = newLineVector;

		// the final checks, before attempting to build FTriangles; if any of the following is true,
		// it's bad.
		// -there are lines remaining in the remainingLineMap.
		// -pointB of the rbegin() of the newLineVector DOES NOT match pointA of the begin() of the vector.
		// -there aren't at least 3 lines in the newLineVector.
		if
		(
			(remainingLineMap.size() != 0)
			||
			(stagerLines.rbegin()->pointB != stagerLines.begin()->pointA)
			||
			(newLineVector.size() < 3)
		)
		{
			std::cout << "!! -> Bad line sequence detected!" << std::endl;
			std::cout << "!! -> Original lines were: " << std::endl;
			for (auto& currentDebugLine : debugCopy)
			{
				currentDebugLine.printLine();
			}
			
			std::cout << "!! Reasons: ";

			if (remainingLineMap.size() != 0)
			{
				std::cout << " :: lineMapSize not 0 :: | ";
			}

			if (stagerLines.rbegin()->pointB != stagerLines.begin()->pointA)
			{
				std::cout << " :: begin and end points don't match :: | ";
			}

			if (newLineVector.size() < 3)
			{
				std::cout << " :: number of lines is less than 3 :: | ";
			}
			std::cout << std::endl;

			//int badSequenceWait = 3;
			//std::cin >> badSequenceWait;
			std::cout << "!! -> Done printing bad line metadata. " << std::endl;
			
			isStagerValid = false;	// we will set this to false,
									// but if the FTriangleLineResolutionMachine figures out a solution,
									// it will get set back to true.

			//FTLResolverExteriorStickSaw sawResolver;
			//sawResolver.initLineResolver(debugCopy);
			//sawResolver.runAttemptedResolution();

			// Because this stager instance couldn't find a solution in the lines,
			// we must delegate that responsibility to an instance of
			// FTriangleLineResolutionMachine.
			FTriangleLineResolutionMachine solver(debugCopy);
			solver.runResolutionSequence();
			if (solver.resolutionFound)
			{
				//std::cout << "!! Solution was found in FTriangleLineResolutionMachine; setting that as the new set of lines. " << std::endl;
				//int solutionFound = 3;
				//std::cin >> solutionFound;
				stagerLines = solver.solutionLines;
				isStagerValid = true;
			}
			else if (!solver.resolutionFound)
			{
				std::cout << "!!! WARNING: solution was not found in the FTriangleLineResolutionMachine! Input number to continue." << std::endl;
				int notFound = 3;
				std::cin >> notFound;
			}
		}

		// FTDEBUG, (uncomment when needed for debugging)
		/*
		// otherwise, everything checked out ok.
		else
		{
			// print out the newly organized vector.
			std::cout << "(FTriangleProductionStager::analyzeAndReorganize): line re-organization checked out OK, lines are: " << std::endl;
			for (auto& newLine : newLineVector)
			{
				newLine.printLine();
			}

		}
		*/
	}

	return isStagerValid;
}