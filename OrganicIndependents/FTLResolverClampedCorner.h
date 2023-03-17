#pragma once

#ifndef FTLRESOLVERCLAMPEDCORNER_H
#define FTLRESOLVERCLAMPEDCORNER_H

#include "FTriangleLineResolverBase.h"

/*

Description:

Attempts to resolve a scenarion in which exactly two FTriangleLines are within a stager, but they are both perfectly clamped to
either the X,Y, or Z plane.

This can be seen as occuring with the following triangle, when running through a BlueprintFracturingMachine:

A:	(5.59, 20.76, 25.59)
B:	(18.87, 13.95, 23.86)
C:	(18.34, 20.76, 6.51)

Ignore the usage of the FTLResolverClampedCorner resolution, in FTriangleLineResolutionMachine, to recreate the issue.

*/

class FTLResolverClampedCorner : public FTriangleLineResolverBase
{
	public:
		bool runAttemptedResolution();
};


#endif
