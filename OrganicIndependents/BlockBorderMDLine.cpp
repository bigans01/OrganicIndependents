#include "stdafx.h"
#include "BlockBorderMDLine.h"

BlockBorderMDLine::BlockBorderMDLine()
{
	// set linked line default values
	for (int x = 0; x < 6; x++)
	{
		linkedLines[x] = ECBPPOrientations::NOVAL;
	}

	diagonalLine = ECBPPOrientations::NOVAL;

	// set faces
	for (int x = 0; x < 2; x++)
	{
		linkedFaces[x] = ECBPPOrientations::NOVAL;
	}
}