#include "stdafx.h"
#include "BorderMDFaceList.h"

BorderMDFaceList::BorderMDFaceList()
{
	for (int x = 0; x < 3; x++)
	{
		faceList[x] = ECBPPOrientations::NOVAL;	// set the default values to be NOVAL (these will be overwritten)
	}
}