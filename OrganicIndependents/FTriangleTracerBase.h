#pragma once

#ifndef FTRIANGLETRACERBASE_H
#define FTRIANGLETRACERBASE_H

#include "FTriangleProductionStager.h"
#include "EnclaveKeyPair.h"
#include "FTriangleLine.h"
#include "FTriangleUtils.h"
#include "FIntersectMeta.h"

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

		struct TracerLineRecord
		{
			TracerLineRecord() {};
			TracerLineRecord(FTriangleLine in_candidateLine,
				std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_applicableSet) :
				candidateLine(in_candidateLine),
				applicableSet(in_applicableSet)
			{}

			FTriangleLine candidateLine;
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> applicableSet;
		};

		std::vector<TracerLineRecord> lineCandidates;

		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* tracerStagerRef = nullptr;
		UniquePointContainer* uniquePointsContainerRef = nullptr;
		EnclaveKeyPair tracingLineKeypairs[3];
		ECBPolyPoint fTrianglePoints[3];
		bool reorientRequired = false;		// if true, it means we had to make the line "positive oriented", and we'll have to insert
											// the lines into the tracerStagerRef in reverse order, when we call runCandidateApplicationLogic.
		void swapValues(EnclaveKeyDef::EnclaveKey* in_keyA,
						EnclaveKeyDef::EnclaveKey* in_keyB,
						ECBPolyPoint* in_pointA,
						ECBPolyPoint* in_pointB);

		void runCandidateApplicationLogic();
};

#endif
