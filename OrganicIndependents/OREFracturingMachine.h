#pragma once

#ifndef OREFRACTURINGMACHINE_H
#define OREFRACTURINGMACHINE_H

#include "FTriangleFracturerBase.h"
#include "FTriangleORETracer.h"

class OREFracturingMachine : public FTriangleFracturerBase
{
	public:
		void runFracturing();
	private:
		void determineUncalibratedBlockKeys();
		void calibrateFTriangleLineAndScannerBlockKeys();
		void loadLocalizedOREPoints();

		void runORETracing();

		EnclaveKeyDef::EnclaveKey getUncalibratedBlockKeyForPoint(DoublePoint in_point);
};

#endif
