#pragma once

#ifndef ORGANICRAWENCLAVESTATE_H
#define ORGANICRAWENCALVESTATE_H

enum class OrganicRawEnclaveState
{
	LOD_ENCLAVE,		// the ORE is renderable, produces all of it's blocks and mass from EnclaveTriangles, and is at shallowest LOD (enclave); this the initial value 
						// if the second constructor of the ORE isn't used.
	LOD_BLOCK,			// the ORE is renderable, produces all of it's blocks and mass from EnclaveBlocks, and is at the deepest LOD (block);
						// this is also one of the states that is searched for when doing a post-collision check.
	FULL				// the entire area mass that the ORE covers is full; it will have exactly 64 EnclaveBlockSkeletons, 
						// and exactly 0 EnclaveTriangles, EnclaveTriangleSkeletons, and OrganicTriangles;
};
#endif

