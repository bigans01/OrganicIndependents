#pragma once

#ifndef FTRIANGLEFRACTURERBASE_H
#define FTRIANGLEFRACTURERBASE_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "FTriangleContainer.h"

class FTriangleFracturerBase
{
	public:
		void setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef);
	private:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* ftfOutputRef = nullptr;
};

#endif
