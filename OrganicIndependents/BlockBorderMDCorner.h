#pragma once

#ifndef BLOCKBORDERMDCORNER_H
#define BLOCKBORDERMDCORNER_H

#include "EnclaveKeyDef.h"
#include "ECBPPOrientations.h"

class BlockBorderMDCorner
{
public:
	BlockBorderMDCorner();						// default constructor
	EnclaveKeyDef::EnclaveKey borderLimits;		// the +/- x/y/z limits (+1, -1, etc)
	ECBPPOrientations adjacentCorners[6];		// the adjacent corners to this corner
	ECBPPOrientations diagonalCorner;			// the corner that is diagonal to this one
	ECBPPOrientations linkedLines[3];			// the 3 lines that are linked to this corner
	ECBPPOrientations linkedFaces[3];			// the 3 faces this corner belongs to
};

#endif
