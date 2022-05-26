#pragma once

#ifndef ENCLAVEKEYPAIR_H
#define ENCLAVEKEYPAIR_H

#include "EnclaveKeyDef.h"

class EnclaveKeyPair
{
public:
	EnclaveKeyPair() {};
	EnclaveKeyPair(EnclaveKeyDef::EnclaveKey in_keyA, EnclaveKeyDef::EnclaveKey in_keyB) :
		keyA(in_keyA),
		keyB(in_keyB)
	{};
	EnclaveKeyDef::EnclaveKey keyA;
	EnclaveKeyDef::EnclaveKey keyB;
};

#endif
