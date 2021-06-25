#include "stdafx.h"
#include "PLTracingResult.h"

void PLTracingResult::setResults(ECBPolyPoint in_polyPoint, BorderMDFaceList in_BorderMDFaceList)
{
	resultingEndPoint = in_polyPoint;
	resultingFaceList = in_BorderMDFaceList;
}