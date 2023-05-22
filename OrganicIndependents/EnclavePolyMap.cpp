#include "stdafx.h"
#include "EPolyMap.h"

PrimarySegmentMeta EPolyMap::buildPrimarySegment(int in_lineID, ECBPolyPointPair in_pair, PrimaryLineT1Array* in_t1ArrayRef, EnclaveBorderLineList* in_enclaveBorderLineListRef, BorderDataMap* in_borderDataMapRef)
{
	PrimarySegmentMeta returnSegmentMeta;
	for (int x = 0; x < 3; x++)
	{
		if (in_t1ArrayRef->linkArray[x].IDofLine == in_lineID)
		{
			returnSegmentMeta.x_slope = in_t1ArrayRef->linkArray[x].x_int;
			returnSegmentMeta.y_slope = in_t1ArrayRef->linkArray[x].y_int;
			returnSegmentMeta.z_slope = in_t1ArrayRef->linkArray[x].z_int;
			returnSegmentMeta.intendedFaces = in_t1ArrayRef->linkArray[x].intendedFaces;
		}
	}
	returnSegmentMeta.startPoint = in_pair.pointA;
	returnSegmentMeta.endPoint = in_pair.pointB;

	ECBPPOrientationResults beginResults = IndependentUtils::GetEnclavePointOrientation(in_pair.pointA, in_enclaveBorderLineListRef);
	ECBPPOrientationResults endResults = IndependentUtils::GetEnclavePointOrientation(in_pair.pointB, in_enclaveBorderLineListRef);
	BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(beginResults, in_borderDataMapRef);
	BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(endResults, in_borderDataMapRef);

	returnSegmentMeta.startFaces = segmentBeginFaceList;
	returnSegmentMeta.endFaces = segmentEndFaceList;

	return returnSegmentMeta;
}

PrimarySegmentMeta EPolyMap::buildPrimarySegment(int in_lineID, ECBPolyPointPair in_pair, PrimaryLineT1* in_primaryLineRef, EnclaveBorderLineList* in_enclaveBorderLineListRef, BorderDataMap* in_borderDataMapRef)
{
	PrimarySegmentMeta returnSegmentMeta;

	returnSegmentMeta.x_slope = in_primaryLineRef->x_int;
	returnSegmentMeta.y_slope = in_primaryLineRef->y_int;
	returnSegmentMeta.z_slope = in_primaryLineRef->z_int;
	returnSegmentMeta.intendedFaces = in_primaryLineRef->intendedFaces;

	returnSegmentMeta.startPoint = in_pair.pointA;
	returnSegmentMeta.endPoint = in_pair.pointB;

	ECBPPOrientationResults beginResults = IndependentUtils::GetEnclavePointOrientation(in_pair.pointA, in_enclaveBorderLineListRef);
	ECBPPOrientationResults endResults = IndependentUtils::GetEnclavePointOrientation(in_pair.pointB, in_enclaveBorderLineListRef);
	BorderMDFaceList segmentBeginFaceList = IndependentUtils::getFaceList(beginResults, in_borderDataMapRef);
	BorderMDFaceList segmentEndFaceList = IndependentUtils::getFaceList(endResults, in_borderDataMapRef);

	returnSegmentMeta.startFaces = segmentBeginFaceList;
	returnSegmentMeta.endFaces = segmentEndFaceList;

	return returnSegmentMeta;
}

EnclavePoly* EPolyMap::getEnclavePolyRef(EnclaveKeyDef::EnclaveKey in_key)
{
	return &enclavePolyMap[in_key];
}