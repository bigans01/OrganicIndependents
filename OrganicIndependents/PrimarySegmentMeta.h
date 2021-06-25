#pragma once

#ifndef PRIMARYSEGMENTMETA_H
#define PRIMARYSEGMENTMETA_H

#include "BorderMDFaceList.h"
#include "EnclaveBlockVertex.h"
#include "ECBPolyPoint.h"

class PrimarySegmentMeta
{
public:
	ECBPolyPoint startPoint;	// the start vertex point 
	BorderMDFaceList startFaces;	// start faces
	ECBPolyPoint endPoint;	// end point
	BorderMDFaceList endFaces;		// end faces
	ECBPolyPoint intendedFaces;		// intendedFaces
	ECBPolyPoint x_slope;
	ECBPolyPoint y_slope;
	ECBPolyPoint z_slope;

	bool isEndpointNotFree()
	{
		bool result = false;	// assume false
		int count = 0;
		for (int x = 0; x < 3; x++)
		{
			if (endFaces.faceList[x] == ECBPPOrientations::NOVAL)
			{
				count++;
			}
		}

		if (count != 3)
		{
			result = true;
		}
		return result;
	};

	bool isBeginpointNotFree()
	{
		bool result = false;	// assume false
		int count = 0;
		for (int x = 0; x < 3; x++)
		{
			if (startFaces.faceList[x] == ECBPPOrientations::NOVAL)
			{
				count++;
			}
		}

		if (count != 3)
		{
			result = true;
		}
		return result;
	};
};

#endif