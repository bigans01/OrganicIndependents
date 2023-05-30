#pragma once

#ifndef ORGANICTRIANGLE_H
#define ORGANICTRIANGLE_H

#include "EPolyMap.h"
#include "OrganicTriangleState.h"
#include "ECBPoly.h"
#include "EnclaveBorderLineList.h"
#include "BorderDataMap.h"
#include "FTriangle.h"

class OrganicTriangle
{
	public:
		OrganicTriangle();
		OrganicTriangle(ECBPoly* in_ecbPolyRef,
						int in_polyID);

		EPolyMap polyMap;
		ECBPoly* ecbPolyRef = NULL;		// a reference to the ECB poly
		int polyID = 0;					// the ID of the poly that this triangle will be produced from.
		ECBPolyPoint originalPoints[3];	// a utility array that stores the points of the original OrganicTriangle.
	private:
		void produceEnclaveTriangles();
		void debugCheckIfKeyExists(EnclaveKeyDef::EnclaveKey in_key);
};

#endif
