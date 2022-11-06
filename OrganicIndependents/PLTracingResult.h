#pragma once

#ifndef PLTRACINGRESULT_H
#define PLTRACINGRESULT_H

#include "BorderMDFaceList.h"
#include "ECBPPOrientationResults.h"

class PLTracingResult
{
public:
	ECBPolyPoint resultingEndPoint;
	BorderMDFaceList resultingFaceList;
	ECBPPOrientationResults resultingOrientation;
	void setResults(ECBPolyPoint in_polyPoint, BorderMDFaceList in_BorderMDFaceList);
};

#endif
