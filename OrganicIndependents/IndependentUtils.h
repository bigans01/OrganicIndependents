#pragma once

#ifndef INDEPENDENTUTILS_H
#define INDEPENDENTUTILS_H

#include "ECBPPOrientationResults.h"
#include "ECBPolyPoint.h"
#include "BlockBorderLineList.h"
#include "EnclaveBorderLineList.h"
#include "EnclaveKeyDef.h"
#include "ECBBorderLineList.h"
#include "BorderDataMap.h"
#include "BorderMDFaceList.h"
#include "EnclaveBlockVertex.h"
#include "FaceListMeta.h"
#include "InterceptValidity.h"
#include "ECBPolyPointTri.h"
#include "PolyLineEndpointMeta.h"
#include "LinePointSynchronizer.h"
#include "PLTracingResult.h"
#include "CursorPathTraceContainer.h"
#include "ECBBorderValues.h"
#include "ECBPolyPointLocation.h"
#include "ECBCalibratedPointPair.h"
#include "ECBIntersectMeta.h"
#include "ECBPolyType.h"
//#include "EnclaveTriangle.h"
#include "ECBPoly.h"
#include "EnclaveBlockVertexTri.h"
#include "DynamicBorderLineList.h"
#include "BoundaryOrientation.h"

class EnclaveTriangle;
class IndependentUtils
{
	public:
		static int checkIfPointsExistOnSameFace(BorderMDFaceList in_faceListA, BorderMDFaceList in_faceListB, BorderMDFaceList in_faceListC, int in_debugFlag);
		static ECBPolyPointLocation getPolyPointLocation(ECBPolyPoint in_point, ECBBorderValues in_borderValues);
		static ECBCalibratedPointPair compareAndCalibrateDistances(ECBPolyPointTri* in_polyPointTriRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_currentLineSlope, EnclaveKeyDef::EnclaveKey in_currentBlueprintKey);
		static ECBIntersectMeta findClosestBlueprintIntersection(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, EnclaveKeyDef::EnclaveKey in_pointAKey, EnclaveKeyDef::EnclaveKey in_pointBKey);
		static ECBIntersectMeta findBlueprintBorderMoveMeta(EnclaveKeyDef::EnclaveKey in_Key1, ECBPolyPoint in_originPoint, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slope, ECBPolyPointTri in_XYZinterceptCoords);		// determines the full key shift, by checking if the poly point's slope is on a border line or a border corner point; called by findClosestBlueprintIntersection
		static ECBPolyPoint roundToAppropriatePrecisionForHundredths(ECBPolyPoint in_polyPoint, EnclaveKeyDef::EnclaveKey in_blueprintKey);		// rounds a point to appropriate precision, based off blueprint coords
		static float roundToHundredthSpecial(float in_float, float in_lowerLimit, float in_upperLimit);
		static ECBPolyPoint roundToNearestBlueprintLineOrCorner(int in_xoryorz, ECBPolyPoint in_polyPoint, int in_lineOrCorner, ECBBorderValues* in_blueprintBorderValuesRef);
		static ECBPolyPoint determinePolyPointPrecisionLimits(ECBPolyPoint in_polyPoint);
		static EnclaveKeyDef::EnclaveKey getBlueprintCalibratedKey(ECBPolyPoint in_pointToCheckA, ECBPolyPoint in_pointToCheckB, ECBBorderLineList* in_borderLineListRef);
		static EnclaveKeyDef::EnclaveKey lookupBlueprintBorderKey(ECBPPOrientationResults in_results, ECBBorderLineList* in_borderLineListRef, ECBPolyPoint in_originPoint, ECBPolyPoint in_interceptPoint);
		static EnclaveKeyDef::EnclaveKey getBorderShiftResult(ECBBorder in_Border, ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		static EnclaveKeyDef::EnclaveKey getUncalibratedBlueprintKeyForPoint(ECBPolyPoint in_point);
		static ECBPolyPoint getBlueprintTracingEndpointForIsolatedPrimaryT2(ECBPolyPoint in_pointA, ECBPolyPoint in_slope, ECBBorderLineList* in_borderLineList, EnclaveKeyDef::EnclaveKey in_blueprintKey);

		static ECBBorderValues getBlueprintLimits(EnclaveKeyDef::EnclaveKey in_Key);
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList in_blockBorders);	// default version of GetPointOrientation
		static ECBPPOrientationResults GetPointOrientation(ECBPolyPoint in_pointToCheck, BlockBorderLineList* in_blockBorders);	// version of GetPointOrientation which takes a pointer instead of a copy of in_blockBorders
		static ECBPPOrientationResults getDynamicPointOrientation(ECBPolyPoint in_pointToCheck, DynamicBorderLineList* in_dynamicBorderLineListRef);

		static ECBPPOrientationResults GetEnclavePointOrientation(ECBPolyPoint in_pointToCheck, EnclaveBorderLineList* in_enclaveBorderLineList);
		static ECBPolyPoint applyEnclaveResetValuesToPoint(ECBPolyPoint in_pointToApplyTo, ECBPolyPoint in_resetValues, ECBPolyPoint in_moveValues);
		static ECBPolyPointLocation getEnclavePolyPointLocation(ECBPolyPoint in_point, ECBBorderValues in_borderValues);

		static ECBPolyPoint findCommonMoveValues(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB);

		static ECBPolyPoint determineTriangleCentroid(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);

		static ECBPPOrientationResults GetBlueprintPointOrientation(ECBPolyPoint in_pointToCheck, ECBBorderLineList* in_borderLineList);
		static ECBBorderLineList determineBorderLines(EnclaveKeyDef::EnclaveKey in_Key);
		static ECBPolyPoint roundPolyPointToHundredths(ECBPolyPoint in_pointToCheck);
		static BorderMDFaceList getFaceList(ECBPPOrientationResults in_beginOrientation, BorderDataMap* in_borderDataMapRef);
		static BorderMDFaceList getFaceListDebug(ECBPPOrientationResults in_beginOrientation, BorderDataMap* in_borderDataMapRef);
		static ECBPolyPoint findNormalizedPoint(ECBPolyPoint in_pointA);
		static ECBPolyPoint findNormalizedSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		static float findNormalizedDirection(float in_dimValueA, float in_dimValueB);
		static void printOrientationEnum(ECBPPOrientations in_pointOrientation);
		static ECBPolyPoint snapPointToOrganicGrid(ECBPolyPoint in_polyPoint, float in_gridLimit);
		static EnclaveBlockVertex convertPolyPointToBlockVertex(ECBPolyPoint in_polyPoint);
		static ECBPolyPoint roundPolyPointToThousandths(ECBPolyPoint in_polyPoint);
		static ECBPolyPoint roundPolyPointToTenThousandths(ECBPolyPoint in_polyPoint);
		static int checkIfFaceListsMatch(BorderMDFaceList in_faceListA, BorderMDFaceList in_faceListB, int in_numberOfMatches);
		static float roundToHundredth(float in_float);
		static float roundToThousandths(float in_float);
		static float roundToTenThousandths(float in_float);
		static EnclaveKeyDef::EnclaveKey retrieveBorderDirection(ECBPPOrientationResults in_results, BorderDataMap* in_dataMapRef);

		static ECBPolyPoint getAppropriateSlopeToUse(BorderDataMap* in_dataMapRef, ECBPPOrientationResults in_beginOrientationResults, ECBPolyPoint in_xInt, ECBPolyPoint in_yInt, ECBPolyPoint in_zInt, EnclaveKeyDef::EnclaveKey in_moveVals, int in_perfectClampValue, int in_debugFlag);
		static ECBPolyPoint getAppropriateSlopeToUseWithIntendedFaceCheck(BorderDataMap* in_dataMapRef, ECBPPOrientationResults in_beginOrientationResults, ECBPolyPoint in_xInt, ECBPolyPoint in_yInt, ECBPolyPoint in_zInt, EnclaveKeyDef::EnclaveKey in_moveVals, int in_perfectClampValue, int in_debugFlag, ECBPolyPoint in_intendedFaces);
		static ECBPolyPoint getAppropriateSlopeToUseWithIntendedFaceCheckIgnoreWarning(BorderDataMap* in_dataMapRef, ECBPPOrientationResults in_beginOrientationResults, ECBPolyPoint in_xInt, ECBPolyPoint in_yInt, ECBPolyPoint in_zInt, EnclaveKeyDef::EnclaveKey in_moveVals, int in_perfectClampValue, int in_debugFlag, ECBPolyPoint in_intendedFaces);

		static void checkForSecondarySlopeInversion(ECBPolyPoint in_intendedFaces, EnclaveKeyDef::EnclaveKey in_moveVals, ECBPolyPoint* in_xIntRef, ECBPolyPoint* in_yIntRef, ECBPolyPoint* in_zIntRef);

		static ECBPolyPoint invertSlope(ECBPolyPoint in_polyPoint);
		static ECBPolyPoint getSlopeToUse(ECBPPOrientations in_interceptType, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope);
		static ECBPolyPoint findSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);

		static ECBPolyType convertIntToPolyType(int in_polyTypeInt);


		static InterceptValidity determineInterceptValidity(ECBPolyPoint in_xInt, ECBPolyPoint in_yInt, ECBPolyPoint in_zInt, ECBPolyPoint in_slopeToCheck, int in_perfectClampValue);
		static int checkIfPolyPointsMatch(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		static ECBPolyPoint getInterceptToUseFromLine(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2, InterceptValidity in_firstInterceptValidity, InterceptValidity in_secondInterceptValidity, EnclaveKeyDef::EnclaveKey in_moveVals);
		static ECBPolyPoint getInterceptToUseFromLineDebug(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2, InterceptValidity in_firstInterceptValidity, InterceptValidity in_secondInterceptValidity, EnclaveKeyDef::EnclaveKey in_moveVals);
		static ECBPolyPoint getInterceptToUseFromCorner(ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope, int in_perfectClampValue, EnclaveKeyDef::EnclaveKey in_moveVals);
		static bool checkIfInterceptIsValid(ECBPolyPoint in_interceptToCheck);
		static ECBPolyPoint getBlockTracingEndpoint(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef);
		static ECBPolyPoint getBlockTracingEndpointDebug(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef);

		// for block endpoints
		static PolyLineEndpointMeta getCalculatedEndpointMetadata(ECBPolyPoint in_originPoint, BlockBorderLineList* in_blockBorderRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords);
		static PolyLineEndpointMeta getCalculatedEndpointMetadataDebug(ECBPolyPoint in_originPoint, BlockBorderLineList* in_blockBorderRef, ECBPolyPoint in_distanceValues, ECBPolyPoint in_slopeDirection, ECBPolyPointTri in_XYZinterceptCoords);

		static ECBPolyPoint roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ);
		static ECBPolyPoint roundXYZInterceptDistancesToAppropriatePrecisionDebug(float in_distX, float in_distY, float in_distZ);

		static ECBPolyPoint roundToNearestBlockLineOrCorner(int in_xoryorz, ECBPolyPoint in_polyPoint, int in_lineOrCorner);

		static PLTracingResult getBlockTracingResult(ECBPolyPoint in_beginPoint, ECBPolyPoint in_interceptToUse, BlockBorderLineList* in_borderLineListRef, BorderDataMap* in_borderDataMapRef, int in_debugFlag);
		static PolyLineEndpointMeta getBlockTracingEndpointMeta(ECBPolyPoint in_beginPoint, ECBPolyPoint in_slope, BlockBorderLineList* in_blockBorderRef);

		static float convertPreciseCoordToFloat(unsigned char in_unsignedCharToCheck);

		static int isUnsignedCharBitSet(unsigned char in_unsignedChar, int in_bitToCheck);
		static void setUnsignedCharBit(unsigned char* in_unsignedCharPtr, int in_bitToSet, int in_bitValue);		// sets a bit in an unsigned char; 8 = 8th bit, 1 = 1st bit (do not use 0 for a bit)

		static CursorPathTraceContainer getPreciseCoordinate(float x);
		static int calibrateEnclaveBlockKeys(float in_remainder, float in_slope);									// used to calibrate enclave and block keys; used during PrimaryLineT1::calibrate()

		static ECBPolyPoint determineIntendedFaces(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB, ECBPolyPoint in_polyPointC);
		static ECBPolyPoint determineIntendedFacesV2(ECBPolyPoint in_polyPointA, ECBPolyPoint in_polyPointB, ECBPolyPoint in_polyPointC, ECBPolyPoint in_xintercept, ECBPolyPoint in_yintercept, ECBPolyPoint in_zintercept);	// needs more clarification on what this does; but is necessary. (6/27/2021)
		static int determineIntendedFaceValidity(int in_xyorz, float in_suggestedIntendedFace, float in_normalizedDirectionFloat, ECBPolyPoint in_xyorzIntercept, ECBPolyPoint in_slopeOfAB);

		static ECBPoly buildECBPolyFromEnclaveTriangle(EnclaveTriangle in_enclaveTriangle, 
													   ECBPolyPoint in_ECBPolyMRP,
			                                           EnclaveKeyDef::EnclaveKey in_blueprintKeyForTranslation,
			                                           EnclaveKeyDef::EnclaveKey in_oreKeyForTranslation);
		static ECBPolyPoint translateEnclavePointToWorldSpace(ECBPolyPoint in_pointToTranslate,
															EnclaveKeyDef::EnclaveKey in_blueprintKeyForTranslation,
															EnclaveKeyDef::EnclaveKey in_oreKeyForTranslation);
		static ECBPolyPoint convertEnclaveBlockVertexToFloats(EnclaveBlockVertex in_vertex);
		static ECBPolyPointTri convertEnclaveBlockVertexesToFloats(EnclaveBlockVertexTri in_vertexTri);
		static ECBPolyPointTri combineClampedCoordsWithPrecise(ECBPolyPointTri in_precisePolyPointTri, EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blueprintKey);
		static ECBPolyPointTri adjustEnclaveTriangleCoordsToWorldSpace(ECBPolyPointTri in_precisePolyPointTri, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blueprintKey);

		static BoundaryOrientation getOppositeOrientation(BoundaryOrientation in_boundaryOrientation);
		static void printBoundaryOrientation(BoundaryOrientation in_boundaryOrientation);
		static std::string getBoundaryOrientationString(BoundaryOrientation in_boundaryOrientation);
		static int convertBoundaryOrientationToInt(BoundaryOrientation in_boundaryOrientation);
		static BoundaryOrientation convertIntToBoundaryOrientation(int in_indexValueToConvert);
};

#endif
