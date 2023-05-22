#pragma once

#ifndef MASSDRIVERLIMITARRAY_H
#define MASSDRIVERLIMITARRAY_H

#include "MassDriverLimit.h"

class MassDriverLimitArray
{
	public:
		MassDriverLimit limitArray[32][32];		// a 32 x 32 array; for dimensions X and Z.
		void updateLimit(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
		void updateLimitDebug(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
};

#endif
