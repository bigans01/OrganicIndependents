#pragma once

#ifndef BLOCKBORDERMDLINE_H
#define BLOCKBORDERMDLINE_H


#include "ECBPPOrientations.h"

class BlockBorderMDLine
{
public:
	BlockBorderMDLine();
	EnclaveKeyDef::EnclaveKey borderLimits;		//  the +/- x/y/z limits (+1, -1, etc) -- one of these will always be 0
	ECBPPOrientations diagonalLine;				// the line that is diagonal to this one
	ECBPPOrientations linkedLines[6];			// the 6 lines that are on the same 2 planes as this line's planes
	ECBPPOrientations linkedFaces[2];			// the 2 planes this line belongs to

};

#endif
