#pragma once

#ifndef FTRIANGLEWORLDTRACER_H
#define FTRIANGLEWORLDTRACER_H

#include "FTriangleFracturerBase.h"

/*

Description:

Used by the fractuing class, WorldFracturingMachine, this class is designed to trace an FTriangles lines through WORLD space,
in order to produce FTriangles that would exist in BLUEPRINT space. The lines (FTriangleLines) produced by this class are fed into the 
stager map referenced by tracerStagerRef (see base class for details). The calling WorldFracturingMachine will then analyze the contents
of its stagerMap member, to determine what to do. In other words, this class simply produces exterior line segments for an FTriangle,
and feeds the results back into the stager map.

*/

class FTriangleWorldTracer : public FTriangleTracerBase
{
	public:
		void runLineTracing();
	private:
};

#endif
