#include "stdafx.h"
#include "FTriangleTracerBase.h"

void FTriangleTracerBase::swapValues(EnclaveKeyDef::EnclaveKey* in_keyA,
	EnclaveKeyDef::EnclaveKey* in_keyB,
	ECBPolyPoint* in_pointA,
	ECBPolyPoint* in_pointB)
{
	// swap keys
	EnclaveKeyDef::EnclaveKey tempKeyA = *in_keyA;
	*in_keyA = *in_keyB;
	*in_keyB = tempKeyA;

	// swap points
	ECBPolyPoint tempPointA = *in_pointA;
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