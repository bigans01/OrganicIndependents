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
			std::cout << "NOTICE: dupliate line found, will not insert line. " << std::endl;
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

void FTriangleProductionStager::printLines()
{
	for (auto& currentLine : stagerLines)
	{
		currentLine.printLine();
	}
}