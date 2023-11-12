#include "stdafx.h"
#include "FTriangleTracerBase.h"

void FTriangleTracerBase::swapKeysAndPoints(EnclaveKeyDef::EnclaveKey* in_keyA,
	EnclaveKeyDef::EnclaveKey* in_keyB,
	FTrianglePoint* in_pointA,
	FTrianglePoint* in_pointB)
{
	// swap keys
	EnclaveKeyDef::EnclaveKey tempKeyA = *in_keyA;
	*in_keyA = *in_keyB;
	*in_keyB = tempKeyA;

	// swap points 
	FTrianglePoint tempPointA = *in_pointA;
	*in_pointA = *in_pointB;
	*in_pointB = tempPointA;
}

void FTriangleTracerBase::runCandidateApplicationLogic()
{
	for (auto& currentLineData : lineCandidates)
	{
		for (auto& currentLineDataKey : currentLineData.applicableSet)
		{
			(*tracerStagerRef)[currentLineDataKey].insertLine(currentLineData.candidateLine);
		}
	}
}