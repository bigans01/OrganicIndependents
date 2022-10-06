#pragma once

#ifndef FTRIANGLEFRACTURERBASE_H
#define FTRIANGLEFRACTURERBASE_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "FTriangleContainer.h"
#include "ECBPolyPoint.h"
#include "BoundaryOrientation.h"

class FTriangleFracturerBase
{
	public:
		// common public functions
		void transferFTriangleMetadata(ECBPolyPoint in_fracturePoint0,	// used to set the points + empty normal, which are required for fracturing.
			ECBPolyPoint in_fracturePoint1,
			ECBPolyPoint in_fracturePoint2,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_originBoundaryOrientation);
		void setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef);	// set a reference
																																		// to the map that we will output fracture results to.
		
		// required virtual functions
		virtual void runFracturing() = 0;
	protected:
		// metadata from the FTriangle that we will use as a basis for fracturing
		ECBPolyPoint originFTrianglePoints[3];
		ECBPolyPoint originFTriangleEmptynormal;
		BoundaryOrientation originBoundaryOrientation = BoundaryOrientation::NONE;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* ftfOutputRef = nullptr;

		EnclaveKeyDef::EnclaveKey originFTriangleKeys[3];	// the origin keys of the FTriangle points; these must be set appropriately by
															// the derived class of this base class (i.e, in a WorldFracturingMachine this would be 
															// the calibrated blueprint keys; in a BlueprintFracturingMachine it'be the EnclaveKey, etc)


};

#endif
