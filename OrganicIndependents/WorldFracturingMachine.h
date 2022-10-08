#pragma once

#ifndef WORLDFRACTURINGMACHINE_H
#define WORLDFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
#include "IndependentUtils.h"
#include <iostream>
#include "FTriangleWorldTracer.h"

class WorldFracturingMachine : public FTriangleFracturerBase
{
	public:
		void runFracturing();
	private:
		void determineUncalibratedBlueprintKeys();	// Step 1: find the uncalibrated blueprint keys.
		void calibrateOriginBlueprintKeys();		// Step 2: calibrate the blueprint origin keys.
		// because we are working in world-space to produce triangles bound in a blueprint, and because there is no X/Y/Z limit, 
		// we should translate the triangle to a more manageable range (somewhere around 0,0,0). To do this, we will get the min X/Y/Z 
		// blueprint coords, and translate by that. The translateTriangle function will do this. This will also make debugging easier.
		void translateTriangleByBlueprintKeys();
		void runWorldTracing();
		
};

#endif
