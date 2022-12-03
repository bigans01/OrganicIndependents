#include "stdafx.h"
#include "OrganicTransformUtils.h"

EnclaveTriangleSkeleton OrganicTransformUtils::deflateEnclaveTriangle(EnclaveTriangle in_enclaveTriangle)
{
	EnclaveTriangleSkeleton returnSkeleton;
	for (int x = 0; x < 3; x++)
	{
		returnSkeleton.points[x] = in_enclaveTriangle.lineArray[x].pointA;
	}
	returnSkeleton.materialID = in_enclaveTriangle.enclaveTriangleMaterialID;
	returnSkeleton.isPolyPerfectlyClamped = in_enclaveTriangle.isEnclaveTrianglePolyPerfectlyClamped;
	returnSkeleton.emptyNormal = in_enclaveTriangle.emptyNormal;
	returnSkeleton.skeletonBoundaryIndicatorValue = in_enclaveTriangle.enclaveTriangleBoundaryPolyIndicator;

	return returnSkeleton;
}

EnclaveTriangleSkeleton OrganicTransformUtils::buildTriangleSkeletonRaw(ECBPolyPoint in_pointA, 
																		ECBPolyPoint in_pointB, 
																		ECBPolyPoint in_pointC, 
																		TriangleMaterial in_materialID,
																		PerfectClampEnum in_isPolyPerfectlyClamped, 
																		ECBPolyPoint in_emptyNormal,
																		BoundaryPolyIndicator in_skeletonBoundaryIndicator)
{
	EnclaveTriangleSkeleton returnSkeleton;
	returnSkeleton.points[0] = in_pointA;
	returnSkeleton.points[1] = in_pointB;
	returnSkeleton.points[2] = in_pointC;

	returnSkeleton.materialID = in_materialID;
	returnSkeleton.isPolyPerfectlyClamped = in_isPolyPerfectlyClamped;
	returnSkeleton.emptyNormal = in_emptyNormal;
	returnSkeleton.skeletonBoundaryIndicatorValue = in_skeletonBoundaryIndicator;

	return returnSkeleton;
}

EnclaveTriangle OrganicTransformUtils::inflateEnclaveTriangle(EnclaveTriangleSkeleton in_enclaveTriangleSkeleton)
{
	EnclaveTriangle returnTriangle;
	TriangleLine line1, line2, line3;

	ECBPolyPoint point0 = in_enclaveTriangleSkeleton.points[0];
	ECBPolyPoint point1 = in_enclaveTriangleSkeleton.points[1];
	ECBPolyPoint point2 = in_enclaveTriangleSkeleton.points[2];

	ECBPolyPoint roundedPoint1 = IndependentUtils::roundPolyPointToHundredths(point0);
	ECBPolyPoint roundedPoint2 = IndependentUtils::roundPolyPointToHundredths(point1);
	ECBPolyPoint roundedPoint3 = IndependentUtils::roundPolyPointToHundredths(point2);

	line1.pointA = roundedPoint1;
	line1.pointB = roundedPoint2;

	line2.pointA = roundedPoint2;
	line2.pointB = roundedPoint3;

	line3.pointA = roundedPoint3;
	line3.pointB = roundedPoint1;

	bool isValid1 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line1, roundedPoint3, point0, point1, point2);
	bool isValid2 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line2, roundedPoint1, point0, point1, point2);
	bool isValid3 = PolyUtils::determineLineInterceptSlopesDebugForEnclaveTriangle(&line3, roundedPoint2, point0, point1, point2);

	returnTriangle.lineArray[0] = PolyUtils::convertToECBPolyLine(line1, roundedPoint3);
	returnTriangle.lineArray[1] = PolyUtils::convertToECBPolyLine(line2, roundedPoint1);
	returnTriangle.lineArray[2] = PolyUtils::convertToECBPolyLine(line3, roundedPoint2);
	returnTriangle.enclaveTriangleMaterialID = in_enclaveTriangleSkeleton.materialID;
	returnTriangle.isEnclaveTrianglePolyPerfectlyClamped = in_enclaveTriangleSkeleton.isPolyPerfectlyClamped;
	returnTriangle.emptyNormal = in_enclaveTriangleSkeleton.emptyNormal;
	returnTriangle.enclaveTriangleBoundaryPolyIndicator = in_enclaveTriangleSkeleton.skeletonBoundaryIndicatorValue;

	return returnTriangle;
}