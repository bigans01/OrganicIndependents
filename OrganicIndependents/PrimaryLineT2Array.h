#pragma once

#ifndef PRIMARYLINET2ARRAY_H
#define PRIMARYLINET2ARRAY_H

#include "PrimaryLineT2.h"
#include "EnclaveKeyDef.h"
#include "ECBTrianglePointArray.h"
#include "ECBPolyPointLocation.h"
#include "ECBPolyLineSlopesArray.h"
#include <map>

class PrimaryLineT2Array
{
public:
	PrimaryLineT2 type2LineArray[8];
	short numberOfStoredLines = 0;								// the number of lines that exist here
	short currentLineID = 3;									// the first line inserted into here will always start at 3
	void addNewPrimaryT2Line(PrimaryLineT2 in_primaryLineT2);
	void printActiveLines();
	int getNumberOfActiveLines();
	void loadPrimaryT2LinesIntoMap(std::map<int, PrimaryLineT2>* in_map);
};

#endif

