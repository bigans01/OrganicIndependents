#pragma once

#ifndef FTRIANGLELINETARGETS_H
#define FTRIANGLELINETARGETS_H

//#include <unordered_set>
//#include "EnclaveKeyDef.h"
#include "FTriangleLine.h"

class FTriangleLineTargets
{
	public:
		FTriangleLineTargets() {};
		FTriangleLineTargets(FTriangleLine in_insertableLine,
							std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_affectedKeys) :
							insertableLine(in_insertableLine),
							affectedKeys(in_affectedKeys)
		{};

		FTriangleLine insertableLine;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> affectedKeys;


};

#endif
