#include "stdafx.h"
#include "PrimaryLineT2Array.h"

void PrimaryLineT2Array::addNewPrimaryT2Line(PrimaryLineT2 in_primaryLineT2)
{
	type2LineArray[numberOfStoredLines] = in_primaryLineT2;	// add the new line to the array, increment the counter
	type2LineArray[numberOfStoredLines].trueLineID = currentLineID;
	currentLineID++;
	numberOfStoredLines++;
}

void PrimaryLineT2Array::printActiveLines()
{
	for (int x = 0; x < numberOfStoredLines; x++)
	{
		std::cout << "Line " << x << "is active: " << type2LineArray[x].beginPointRealXYZ.x << ", " << type2LineArray[x].beginPointRealXYZ.y << ", " << type2LineArray[x].beginPointRealXYZ.z << " | " << type2LineArray[x].endPointRealXYZ.x << ", " << type2LineArray[x].endPointRealXYZ.y << ", " << type2LineArray[x].endPointRealXYZ.z << std::endl;
	}
}

int PrimaryLineT2Array::getNumberOfActiveLines()
{
	return numberOfStoredLines;
}

void PrimaryLineT2Array::loadPrimaryT2LinesIntoMap(std::map<int, PrimaryLineT2>* in_map)
{
	for (int x = 0; x < numberOfStoredLines; x++)
	{
		in_map->insert(std::pair<int, PrimaryLineT2>(x, type2LineArray[x]));
	}
}