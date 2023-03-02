#pragma once

#ifndef FTLRESOLVERSCANNEDSTICKSAW_H
#define FTLRESOVLERSCANNEDSTICKSAW_H

#include "FTriangleLineResolverBase.h"

/*

Description:

This class, derived from FTriangleLineResolverBase, attempts to handle the situation that occurs where a vector of FTriangleLine instances
ends up forming a "stick." This class is designed to handle the situation in which an FTriangleLine having a FTriangleLineType that is NOT EXTERIOR ends up being parallel/perpendicular to the
bounds of a cell that the lines were produced in. This class will attempt to remove (saw-off) the non-EXTERIOR (aka, a FTriangleLine produced as a 
result of scanning) "stick" and then check whether or not the vector of FTriangleLine instances
is now valid as a result.

*/

class FTLResolverScannedStickSaw : public FTriangleLineResolverBase
{
	public:
		bool runAttemptedResolution();	// attempt to clip out (saw-off) one of the non-EXTERIOR lines to make the triangle "whole";
										// one non-EXTERIOR line will be clipped each pass. The total number of passes that can possibly occur
										// is equal to the number of non-EXTERIOR lines.
};

#endif