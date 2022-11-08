#pragma once

#ifndef TERMINATINGSET_H
#define TERMINATINGSET_H


class TerminatingSet
{
public:
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> set;
};

#endif