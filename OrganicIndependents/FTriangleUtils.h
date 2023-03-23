#pragma once

#ifndef FTRIANGLEUTILS_H
#define FTRIANGLEUTILS_H

#include "TracingLineBoundingBox.h"
#include "FTraceType.h"
#include "FIntersectMeta.h"
#include "ECBBorderValues.h"
#include "FTraceBorderValues.h"
#include "FTraceBorderLineList.h"
#include "FTraceOrientationResults.h"

class FTriangleUtils
{
	public:
		static FIntersectMeta findIntersectionData(ECBPolyPoint in_pointA,
									ECBPolyPoint in_pointB,
									EnclaveKeyDef::EnclaveKey in_pointAKey,
									EnclaveKeyDef::EnclaveKey in_pointBKey,
									TracingLineBoundingBox in_boundingBox,
									FTraceType in_fTraceType);

		static FIntersectMeta findIntersectionDataV2(ECBPolyPoint in_pointA,
									ECBPolyPoint in_pointB,
									EnclaveKeyDef::EnclaveKey in_pointAKey,
									EnclaveKeyDef::EnclaveKey in_pointBKey,
									TracingLineBoundingBox in_boundingBox,
									FTraceType in_fTraceType);

		static FIntersectMeta calculateIntersection(EnclaveKeyDef::EnclaveKey in_Key1,
									ECBPolyPoint in_originPoint,
									ECBPolyPoint in_distanceValues,
									ECBPolyPoint in_slopeDirection,
									ECBPolyPointTri in_XYZinterceptCoords,
									TracingLineBoundingBox in_boundingBox,
									FTraceType in_fTraceType);


		static ECBBorderValues getBlueprintLimits(EnclaveKeyDef::EnclaveKey in_Key);
		static ECBPolyPoint findNormalizedPoint(ECBPolyPoint in_pointA);
		static ECBPolyPoint roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ);
		static ECBCalibratedPointPair compareAndCalibrateDistances(ECBPolyPointTri* in_polyPointTriRef,
			ECBPolyPoint in_distanceValues,
			ECBPolyPoint in_currentLineSlope,
			EnclaveKeyDef::EnclaveKey in_currentBlueprintKey);

		static FTraceBorderValues getCurrentTracingLimits(EnclaveKeyDef::EnclaveKey in_currentTracingKey, FTraceType in_fTraceType);
		static FTraceBorderLineList getCurrentBorderLines(EnclaveKeyDef::EnclaveKey in_currentTracingKey, FTraceType in_fTraceType);

		static ECBPolyPoint roundToNearestLineOrCorner(int in_xoryorz,
			ECBPolyPoint in_polyPoint,
			int in_lineOrCorner,
			FTraceBorderValues* in_blueprintBorderValuesRef,
			FTraceType in_fTraceType);

		static ECBPolyPoint roundToAppropriatePrecisionForHundredths(ECBPolyPoint in_polyPoint, EnclaveKeyDef::EnclaveKey in_blueprintKey);		// rounds a point to appropriate precision, based off blueprint coords
		static ECBPolyPoint determinePolyPointPrecisionLimits(ECBPolyPoint in_polyPoint);
		static float roundToHundredth(float in_float);
		static ECBPolyPoint roundPointToHundredth(ECBPolyPoint in_polyPoint);
		static float roundToHundredthSpecial(float in_float, float in_lowerLimit, float in_upperLimit);
		static EnclaveKeyDef::EnclaveKey getBorderShiftResult(ECBBorder in_Border, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		static EnclaveKeyDef::EnclaveKey getBorderShiftResultV2(ECBBorder in_Border, 
																ECBPolyPoint in_originPoint, 
																ECBPolyPoint in_selectedPoint,
																ECBPolyPoint in_cornerPointA,
																ECBPolyPoint in_cornerPointB);
		static FTraceOrientationResults getFTracePointOrientation(ECBPolyPoint in_pointToCheck, FTraceBorderLineList* in_borderLineList);
		static EnclaveKeyDef::EnclaveKey getFTraceCalibratedKey(ECBPolyPoint in_originPoint,
			ECBPolyPoint in_interceptPoint,
			FTraceBorderLineList* in_borderLineListRef);

		static EnclaveKeyDef::EnclaveKey lookupBlueprintBorderKey(FTraceOrientationResults in_results,
																FTraceBorderLineList* in_borderLineListRef,
																ECBPolyPoint in_originPoint, 
																ECBPolyPoint in_interceptPoint);

		static ECBPolyPoint findNormalizedSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		static bool isLinePositivelyOriented(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);	// checks if the given Line A>B is positively oriented; returns false if not.


		
};

#endif