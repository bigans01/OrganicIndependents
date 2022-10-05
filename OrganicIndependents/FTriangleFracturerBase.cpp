#include "stdafx.h"
#include "FTriangleFracturerBase.h"

void FTriangleFracturerBase::setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef)
{
	ftfOutputRef = in_outputRef;
};