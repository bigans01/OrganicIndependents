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

		// Below: the analyzeAndReorganize function takes in 3 parameters, in the event that it needs to print debug output:
		// -the key that the stager is working for
		// -the FTriangleType of the parent FTriangle (so we can identify the machine that the bug/issue occured in)
		// -the localized points of the FTriangle, so that we can reconstruct it directly if needed
		FTLResolutionStatus analyzeAndReorganize(EnclaveKeyDef::EnclaveKey in_stagerKey, 
												FTriangleType in_fTriangleTypeForDebug,
												ECBPolyPoint in_localizedFTrianglePointsArrayRef[3]);
		std::vector<FTriangleLine> fetchStagerLines();
	private:
		std::vector<FTriangleLine> stagerLines;

		// flags to indicate certain things happened (duplicates, bad triangles, etc)
		bool duplicatesFound = false;

};

#endif
