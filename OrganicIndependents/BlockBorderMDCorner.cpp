#include "stdafx.h"
#include "BlockBorderMDCorner.h"

BlockBorderMDCorner::BlockBorderMDCorner()
{
	// set adjacent corner default values
	for (int x = 0; x < 6; x++)
	{
		adjacentCorners[x] = ECBPPOrientations::NOVAL;
	}

	diagonalCorner = ECBPPOrientations::NOVAL;	// set diagonal default

	// set lines and faces
	for (int x = 0; x < 3; x++)
	{
		linkedLines[x] = ECBPPOrientations::NOVAL;
		linkedFaces[x] = ECBPPOrientations::NOVAL;
	}
}