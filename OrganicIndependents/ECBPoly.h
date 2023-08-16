#pragma once

#ifndef ECBPOLYLINE_NEW_H
#define ECBPOLYLINE_NEW_H

#include "ECBPolyLine.h"
#include "ECBPolyType.h"
#include "FTriangleOutput.h"
#include "TriangleLine.h"

/*

Description:

This class is meant to represent a single triangle; it represents the triangles that are stored in an EnclaveCollectionBlueprint (OrganicCoreLib).

*/

class ECBPoly
{
	public:
		ECBPoly() {};	// default constructor, for copying.
		ECBPoly(ECBPolyType in_polyType) : polyType(in_polyType) {};	// instantiate an ECBPoly with a type; will be deprecated in the future.
		ECBPoly(FTriangleOutput in_completeTriangleOutput);		// constructor designed for building the ECBPoly off an FTriangleOutput.
		ECBPolyType polyType = ECBPolyType::FREE;		// types that are required so that ContourPlan instances (OrganicServerLib) know which ECBPoly instances
														// are used for operations such as mass driving.

		ECBPolyPoint ecbPolyPoints[3];	// the 3 points that make up the ECBPoly.

		TriangleMaterial materialID = TriangleMaterial::NOVAL;				// is the triangle dirt, stone, snow, wood, etc
		PerfectClampEnum isPolyPerfectlyClamped = PerfectClampEnum::NONE;	// determines if the entire polygon is perfectly clamped to x, y, or z azis
		ECBPolyPoint emptyNormal;	// the empty normal that represents the direction of empty space in relation to this ECBPoly.
		BoundaryOrientation polyBoundaryOrientation = BoundaryOrientation::NONE;	// must be set by the FTriangleOutput constructor.

		void printLineData();	// print all the point data of each ECBPolyLine
		Message produceBDMFormatECBPolyMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey, int in_mappedPolyKey);	// produces a Message of the type BDM_BLUEPRINT_ECBPOLY
	private:
		// NOTE: for the below functions, these are duplicates of what is in the IndependentUtils utility class;
		// the copies had to be put here to avoid a circular dependency (IndependentUtils utilizes ECBPoly class, so ECBPoly can utilize IndependentUtils)
		ECBPolyLine convertToECBPolyLine(TriangleLine in_line, ECBPolyPoint in_thirdPoint);
		ECBPolyPoint convertDoublePointToEcBPolyPoint(DoublePoint in_doublePointToConvert);
		ECBPolyPoint findSlope(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
		void determineLineInterceptSlopes(OrganicLineBase* in_LinePtr, ECBPolyPoint in_thirdPoint);
};

#endif

