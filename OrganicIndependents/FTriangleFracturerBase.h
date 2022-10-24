#pragma once

#ifndef FTRIANGLEFRACTURERBASE_H
#define FTRIANGLEFRACTURERBASE_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "FTriangleContainer.h"
#include "FTriangleProductionStager.h"
#include "ECBPolyPoint.h"
#include "BoundaryOrientation.h"
#include "EnclaveKeyPair.h"
#include "CalibratableBlueprintKeyPair.h"
#include "UniquePointContainer.h"
#include "FTriangleTracerBase.h"
#include "PerfectClampEnum.h"
#include <map>
#include "FRayCasterTypeEnum.h"
#include "FRayCasterInitData.h"
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>
#include <mutex>
#include "FRayCasterQuadBase.h"
#include "XFRayCastQuad.h"
#include "YFRayCastQuad.h"
#include "ZFRayCastQuad.h"
#include <set>

class FTriangleFracturerBase
{
	public:
		// common public functions
		void transferFTriangleMetadata(ECBPolyPoint in_fracturePoint0,	// used to set the points + empty normal, which are required for fracturing.
			ECBPolyPoint in_fracturePoint1,
			ECBPolyPoint in_fracturePoint2,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_originBoundaryOrientation,
			PerfectClampEnum in_originPerfectClampValue);
		void setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef);	// set a reference
																																		// to the map that we will output fracture results to.
		void printFracturerPoints();
		// required virtual functions
		virtual void runFracturing() = 0;
	protected:
		// metadata from the FTriangle that we will use as a basis for fracturing
		ECBPolyPoint originFTrianglePoints[3];
		ECBPolyPoint originFTriangleEmptynormal;
		BoundaryOrientation originBoundaryOrientation = BoundaryOrientation::NONE;	// must be set by constructor
		PerfectClampEnum originPerfectClampValue = PerfectClampEnum::NONE;	// must be set by constructor
		float rayCastDimInterval = 0.0f;	// the distance, measured in float, between each ray cast. Must be set by the derived class.

		EnclaveKeyPair originFTriangleLineKeypairs[3];		// These are the keypair values used to trace an FTriangleLine through it's respective space;
															// this must be handled by the derived class of this base class. These should have an initial value,
															// but then the keys may be calibrated (i.e, such as with WorldFracturingMachine::calibrateOriginBlueprintKeys())
															// which will update their values.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* ftfOutputRef = nullptr;	// a pointer to the map that stores
																																// the output triangles that are a result of the 
																																// fracturing operation.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher> stagerMap;	// a map of stagers; this must be passed as a reference
																														// to the selected tracing class (i.e, FTriangleWorldTracer, FTriangleBlueprintTracer, etc)

		EnclaveKeyDef::EnclaveKey originFTriangleKeys[3];	// the origin keys of the FTriangle points; these must be set appropriately by
															// the derived class of this base class (i.e, in a WorldFracturingMachine this would be 
															// the calibrated blueprint keys; in a BlueprintFracturingMachine it'be the EnclaveKey, etc)

		UniquePointContainer fracturerPoints;				// any point that will be used by any line, or anywhere in the triangle for that matter, should go here.
															// This includes but is not limited to: the original points of the triangle, traced exterior points, and ray-casted points.
		std::map<FRayCasterTypeEnum, FRayCasterInitData> getUsableRayCasters();		// uses the values of the originFTriangleKeys (which must be calibrated before this) to determine
																// which FRayCasterQuadBase-derived classes to build.
		std::map<FRayCasterTypeEnum, std::shared_ptr<FRayCasterQuadBase>> selectedRayCasters;	// a map of selected ray casters, that must be executed.
																								// Must be called by derived class that is based off this one.

		struct LineScanLists
		{
			LineScanLists() {};
			std::set<float> xList;
			std::set<float> yList;
			std::set<float> zList;
		};

		LineScanLists getScanningIntervals(float in_fixedIntervalValue);

		enum class LineScanPermit
		{
			SCAN_X,
			SCAN_Y,
			SCAN_Z
		};

		std::set<LineScanPermit> getValidPermits();
};

#endif
