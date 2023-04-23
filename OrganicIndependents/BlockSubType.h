#pragma once

#ifndef BLOCKSUBTYPE_H
#define BLOCKSUBTYPE_H

/*

Description:

This enum class is used to store a value that determines whether or not an EnclaveBlock can be utilized by an SPolyShellProducer, in order to produce
exposed faces. The value of BLOCK_NORMAL indicates that it is OK to use this block in an SPolyShellProducer. The BLOCK_INDEPENDENT value indicates
that the block has customized triangles/fans, that will not work in an SPolyShellProducer. This is useful in cases where blocks are deleted, and you are 
analyzing neighboring blocks: an EnclaveBlock that is in a BLOCK_INDEPENDENT state cannot be relied upon to produce a reliable shell from a SPolyShellProducer
instance; we must ignore it for a comparison.

*/

enum class BlockSubType
{
	BLOCK_NORMAL,			// the default value for EnclaveBlock; most EnclaveBlocks should be in this state.

	BLOCK_NORMAL_FILLED,	// indicates the the EnclaveBlock has been completely filled on all 6 sides. This
							// is needed in cases where some blocks spawned by an FTriangle running in an OREFracturingMachine didn't produce blocks.
							// When the machine can't produce blocks or find a solution, this value must be used to indicate that.
							// You can see how it is set, in the function OrganicTriangleTertiary::buildFilledBlockFanGroup() in this library.
	BLOCK_INDEPENDENT
};

#endif
