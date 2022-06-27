#pragma once

#ifndef BLOCKCOPYQUERY_H
#define BLOCKCOPYQUERY_H

#include "EnclaveBlock.h"

/*

class BlockCopyQuery

Description: this is a simple class that is designed to return an instance of EnclaveBlock; the bool value is used to indicate
			 if the result of the query contained a valid EnclaveBlock instance.
*/

class BlockCopyQuery
{
	public:
		BlockCopyQuery() {};
		BlockCopyQuery(EnclaveBlock in_enclaveBlockCopy) :
			wasFound(true)
		{
			blockCopy = in_enclaveBlockCopy;
		};

		bool wasFound = false;
		EnclaveBlock blockCopy;
};

#endif
