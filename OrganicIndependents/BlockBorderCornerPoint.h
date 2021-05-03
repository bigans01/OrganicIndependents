#pragma once

#ifndef BLOCKBORDERCORNERPOINT_H
#define BLOCKBORDERCORNERPOINT_H

#include "BlockBorder.h"
#include "BlockPolyPoint.h"

class BlockBorderCornerPoint
{
public:
	BlockBorder borderValues;
	BlockPolyPoint pointXYZ;
};

#endif