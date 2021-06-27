#pragma once

#ifndef TERMINATINGSET_H
#define TERMINATINGSET_H

#include "EnclaveKeyDef.h"
#include <unordered_set>

class TerminatingSet
{
public:
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> set;
};

#endif