#pragma once

#ifndef BLOCKBORDERMDFACE_H
#define BLOCKBORDERMDFACE_H


#include "ECBPPOrientations.h"

class BlockBorderMDFace
{
public:
	EnclaveKeyDef::EnclaveKey borderLimits;		//  the +/- x/y/z limits (+1, -1, etc) -- two of these will always be 0
};

#endif

