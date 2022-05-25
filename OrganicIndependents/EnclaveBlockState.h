#pragma once

#ifndef ENCLAVEBLOCKSTATE_H
#define ENCLAVEBLOCKSTATE_H

enum class EnclaveBlockState
{
	NONEXISTENT,			// returned if the block doesn't exist.
	EXPOSED,		// return if it's exposed (i.e, it's being rendered)
	UNEXPOSED,		// return if it exists, but is unexposed (hidden from rendering)
	NONEXISTENT_BECAUSE_FULL // return for an ORE that is in a FULL state, but doesn't have an unexposed block at a given block key.
};

#endif
