#pragma once

#ifndef FTLRESOLVERDUALSTICKSAW_H
#define FTLRESOLVERDUALSTICKSAW_H

#include "FTriangleLineResolverBase.h"

/*

Description:

Attempts to resolve a scenario in which two stick saws exist in the same stager. This is evidenced by the following triangle being run through a
BlueprintFracturingMachine (just turn off/ignore this resolution in the FTriangleLineResolutionMachine, and run that triangle to see it):

A:	(0, 11.33, 16)
B:	(20, 13.33, 0)
C:	(12.18, 10.69, 0)

*/

class FTLResolverDualStickSaw : public FTriangleLineResolverBase
{
	public:
		bool runAttemptedResolution();
};

#endif