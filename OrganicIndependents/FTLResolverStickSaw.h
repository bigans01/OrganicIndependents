#pragma once

#ifndef FTLRESOLVERSTICKSAW_H
#define FTLRESOLVERSTICKSAW_H

#include "FTriangleLineResolverBase.h"

/*

Description:

This class, derived from FTriangleLineResolverBase, attempts to handle the situation that occurs where a vector of FTriangleLine instances
ends up forming a "stick." A stick occurs when an FTriangleLine having a FTriangleLineType of EXTERIOR ends up being parallel/perpendicular to the 
bounds of a cell that the lines were produced in. This class will attempt to remove (saw-off) the "stick" and then check whether or not the vector of FTriangleLine instances
is now valid as a result. 

*/

class FTLResolverStickSaw : public FTriangleLineResolverBase
{
	public:
		bool runAttemptedResolution();	// attempt to clip out (saw-off) one of the EXTERIOR lines to make the triangle "whole"
};

#endif