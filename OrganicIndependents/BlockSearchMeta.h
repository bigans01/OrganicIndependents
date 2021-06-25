#pragma once

#ifndef BLOCKSEARCHMETA_H
#define BLOCKSEARCHMETA_H

#include "ECBPolyPoint.h"

class BlockSearchMeta
{
public:
	int searchResult = 0;	// if search result was 0, nothing was found.
	ECBPolyPoint foundPoint;
};

#endif
