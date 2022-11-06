#pragma once

#ifndef POLYLINEENDPOINTMETA_H
#define POLYLINEENDPOINTMETA_H

#include "BlockBorder.h"

class PolyLineEndpointMeta
{
public:
	ECBPolyPoint pointLocation;			// location of the point; values will range between 0.0f and 1.0f for x/y/z
	ECBPolyPoint moveDirectionValues;   // move values (0, 1 for positive, -1 for negative)
	ECBPolyPoint dimensionResetValues;	// reset values for when x/y/z hits a border
	void applyBlockBorder(BlockBorder in_blockBorder);
	void matchSlopesToDirections(ECBPolyPoint in_slope);
};

#endif