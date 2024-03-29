#pragma once

#ifndef ORGANICTRIANGLETERTIARY_H
#define ORGANICTRIANGLETERTIARY_H


#include "OrganicWrappedBBFan.h"
#include "FTriangle.h"
#include "PolyUtils.h"
#include "OrganicFanGroup.h"
#include "Operable3DEnclaveKeySet.h"

/*

Description:

This class is used by the EnclaveTriangle class to store the relevant triangles for each block
that an EnclaveTriangle touched.

*/

class OrganicTriangleTertiary
{
	public:
		OrganicTriangleTertiary();		// default constructor.

		// Below: this constructor uses the given parameters to create an
		// FTriangle instance that runs an OREFracturingMachine, which should subsequently produce all necessary triangles
		// for each block that the FTriangle touched.
		OrganicTriangleTertiary(ECBPolyPoint in_etPoint0,		
								ECBPolyPoint in_etPoint1,
								ECBPolyPoint in_etPoint2,
								ECBPolyPoint in_etEmptyNormal,
								BoundaryOrientation in_etRequiredOrientation,
								PerfectClampEnum in_etPerfectClampEnum,
								TriangleMaterial in_etTriangleMaterial);
		

		//std::map<int, OrganicWrappedBBFan> triangleMap;	// contains all relevant OrganicWrappedBBFan instances; the int value is an EnclaveKeyDef value
														// that has been converted to an int.

		std::map<int, OrganicFanGroup> triangleMap;		// under certain conditions, blocks produced by this instance can contain multiple OrganicWrappedBBFans;
														// hence, we need to use OrganicFanGroup in case that happens.

		Operable3DEnclaveKeySet incalculableAttempts;	// stores the correleating EnclaveKeyDef values of any blocks that couldn't be produced.
	private:
		OrganicFanGroup buildFilledBlockFanGroup();		// builds an OrganicFanGroup that has exactly 6 
														// OrganicWrappedBBFan instances, one for each face of the block.
};

#endif
