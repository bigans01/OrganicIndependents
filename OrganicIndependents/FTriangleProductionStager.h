#pragma once

#ifndef FTRIANGLEPRODUCTIONSTAGER_H
#define FTRIANGLEPRODUCTIONSTAGER_H

#include "FTriangleLine.h"
#include "FTriangleLineResolutionMachine.h"
#include "FTLResolutionStatus.h"

/*

Description:

This class is meant to store all the FTriangleLine instances that the fracturing process
of an FTriangle produces, for a particular cell in a regular 3d grid. The stager is later analyzed for items such as duplicate lines, 
its own validity, and also reorganizes the lines to be in sequential order, to allow
for easy construction of triangle fans.

See it's map and usage in FTriangle.h/cpp for more clarification.

*/

class FTriangleProductionStager
{
	public:
		void insertLine(FTriangleLine in_lineToInsert);
		void translateLines(EnclaveKeyDef::EnclaveKey in_translationKey, float in_fixedDimensionInterval);
		void printLines();
		FTLResolutionStatus analyzeAndReorganize(EnclaveKeyDef::EnclaveKey in_stagerKey);
		std::vector<FTriangleLine> fetchStagerLines();
	private:
		std::vector<FTriangleLine> stagerLines;

		// flags to indicate certain things happened (duplicates, bad triangles, etc)
		bool duplicatesFound = false;

};

#endif
