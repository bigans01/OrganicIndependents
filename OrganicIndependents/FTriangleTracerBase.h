#pragma once

#ifndef FTRIANGLETRACERBASE_H
#define FTRIANGLETRACERBASE_H

#include "EnclaveKeyDef.h"
#include <unordered_map>
#include "FTriangleProductionStager.h"
#include "UniquePointContainer.h"
#include "EnclaveKeyPair.h"


/*

Description:

This class serves as a base class for all the tracing-derived classes, such as FTriangleWorldTracer, FTriangleBlueprintTracer and FTriangleORETracer.

*/

class FTriangleTracerBase
{
	public:
		void initialize(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* in_tracerStagerRef,
						UniquePointContainer* in_uniquePointsContainerRef,
						EnclaveKeyPair in_tracingLineKeyPairs[3],
						ECBPolyPoint in_fTrianglePoints[3])
		{
			tracerStagerRef = in_tracerStagerRef;
			uniquePointsContainerRef = in_uniquePointsContainerRef;
			for (int x = 0; x < 3; x++)
			{
				tracingLineKeypairs[x] = in_tracingLineKeyPairs[x];
				fTrianglePoints[x] = in_fTrianglePoints[x];
			}

		};

		virtual void runLineTracing() = 0;
	protected:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* tracerStagerRef = nullptr;
		UniquePointContainer* uniquePointsContainerRef = nullptr;
		EnclaveKeyPair tracingLineKeypairs[3];
		ECBPolyPoint fTrianglePoints[3];
};

#endif
