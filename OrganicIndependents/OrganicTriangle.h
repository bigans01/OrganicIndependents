#pragma once

#ifndef ORGANICTRIANGLE_H
#define ORGANICTRIANGLE_H

#include "EPolyMap.h"
#include "OrganicTriangleState.h"
#include "PrimaryLineT1Array.h"
#include "ECBPoly.h"
#include "EnclaveBorderLineList.h"
#include "BorderDataMap.h"
#include "PLTracingResult.h"
#include "OrganicTriangleSecondary.h"
#include "FTriangle.h"

class OrganicTriangle
{
	public:
		OrganicTriangle();
		OrganicTriangle(ECBPoly* in_ecbPolyRef,
			PrimaryLineT1Array in_forwardPrimaryLineCopy,
			PrimaryLineT1Array in_reversePrimaryLineCopy,
			EnclaveKeyDef::EnclaveKey in_blueprintKey,
			BorderDataMap* in_borderDataMapRef,
			BlockBorderLineList* in_blockBorderLineListRef,
			int in_polyID);

		OrganicTriangle& operator=(const OrganicTriangle& organicTriangle_b)	// copy constructor; must be defined because we use smart pointers.
		{
			borderDataMapRef = organicTriangle_b.borderDataMapRef;
			polyMap = organicTriangle_b.polyMap;					// " "
			ecbPolyRef = organicTriangle_b.ecbPolyRef;
			polyID = organicTriangle_b.polyID;
			return *this;
		}

		BorderDataMap* borderDataMapRef;
		EPolyMap polyMap;
		ECBPoly* ecbPolyRef = NULL;										// a reference to the ECB poly

		int polyID = 0;			// the ID of the poly that this triangle will be produced from.

		ECBPolyPoint originalPoints[3];	// a utility array that stores the points of the original OrganicTriangle.

	private:
		void produceEnclaveTriangles();
		void debugCheckIfKeyExists(EnclaveKeyDef::EnclaveKey in_key);
		void checkIfBlockWasTraced(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
		bool checkCurrentPointAgainstSegmentCorrelation(ECBPolyPoint in_currentPoint, PrimarySegmentMeta in_segment);
};

#endif
