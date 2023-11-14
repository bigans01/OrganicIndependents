#pragma once

#ifndef FTRIANGLEFRACTURERBASE_H
#define FTRIANGLEFRACTURERBASE_H

#include "FTriangleProductionStager.h"
#include "EnclaveKeyPair.h"
#include "CalibratableBlueprintKeyPair.h"
#include "FTriangleTracerBase.h"
#include "FRayCasterInitData.h"
#include "FRayCasterQuadBase.h"
#include "XFRayCastQuad.h"
#include "YFRayCastQuad.h"
#include "ZFRayCastQuad.h"
#include "DimensionalLineScannerBase.h"
#include "XDimLineScanner.h"
#include "YDimLineScanner.h"
#include "ZDimLineScanner.h"
#include "FTriangleContainer.h"
#include "FTriangleReverseTranslationMode.h"
#include "FTriangleKeySetCalibrator.h"
#include "FExteriorRaycastCollisionResolver.h"
#include "FTLResolutionStatus.h"
#include "OutputDirector.h"
#include "FTriangleUtils.h"

class FTriangleFracturerBase
{
	public:
		// common public functions
		void transferFTriangleMetadata(
			FTrianglePoint in_fracturePoint0,
			FTrianglePoint in_fracturePoint1,
			FTrianglePoint in_fracturePoint2,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_originBoundaryOrientation,
			PerfectClampEnum in_originPerfectClampValue,
			TriangleMaterial in_originMaterial,
			OutputDirector* in_fracturerBaseWriterRef);
		void printFracturerPoints();

		// required virtual functions
		virtual void setPointTranslationMode(FTriangleReverseTranslationMode in_translationModeToRunAs) = 0;	// only works with WORLD based fracturers; must be called before runFracturing() is called,
																												// if wishing to use the desired value.
		virtual void runFracturing() = 0;

	protected:
		friend class FTriangle;

		// Below: the pointer to the OutputDirector that resides in the parent FTriangle that owns an instance of 
		// this derived-class. Can be used anywhere we please in this class and in the derived classes of this base class. Items of interest where this is used
		// include:
		//	-the function FTriangleProductionStager::analyzeAndReorganize
		//	-any instantiation of FTriangleLineResolutionMachine, so that it may pass this value onto the underlying FTriangleLineResolverBase instances that get run
		OutputDirector* fracturerBaseWriterRef = nullptr;

		// metadata from the FTriangle that we will use as a basis for fracturing
		FTrianglePoint originFTrianglePoints[3];
		FTrianglePoint localizedFTrianglePoints[3];	// stores the localized points of an FTriangle (used mainly by WorldFracturingMachine; may be moved later)

		ECBPolyPoint originFTriangleEmptynormal;
		BoundaryOrientation originBoundaryOrientation = BoundaryOrientation::NONE;	// must be set by constructor
		PerfectClampEnum originPerfectClampValue = PerfectClampEnum::NONE;	// must be set by constructor
		TriangleMaterial originMaterial = TriangleMaterial::NOVAL;
		float rayCastDimInterval = 0.0f;	// the distance, measured in float, between each ray cast. Must be set by the derived class. Should be set to 32.0f, 4.0f, or 1.0f;
											// The chosen value must be set manually by the specified class derived from this base class (such as WorldFracturingMachine).

		EnclaveKeyPair originFTriangleLineKeypairs[3];		// These are the keypair values used to trace an FTriangleLine through it's respective space;
															// this must be handled by the derived class of this base class. These should have an initial value,
															// but then the keys may be calibrated (i.e, such as with WorldFracturingMachine::calibrateFTriangleLineAndScannerBlueprintKeys())
		EnclaveKeyDef::EnclaveKey originFTriangleKeys[3];	// the origin keys of the FTriangle points; these must be set appropriately by
															// the derived class of this base class (i.e, in a WorldFracturingMachine this would be 
															// the calibrated blueprint keys; in a BlueprintFracturingMachine it'be the EnclaveKey, etc)
															// which will update their values.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* ftfOutputRef = nullptr;	// a pointer to the map that stores
																																// the output triangles that are a result of the 
																																// fracturing operation.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher> stagerMap;	// a map of stagers; this must be passed as a reference
																														// to the selected tracing class (i.e, FTriangleWorldTracer, FTriangleBlueprintTracer, etc)

		EnclaveKeyPair scanningKeypairs[3];		// These are keypairs used to determine the fixed ray-cast interval ranges;
												// they are used to populate the appropriate values of the scanningKeys[3] member of this class,
												// which is done with a specialized function of each derived class.
												// (i.e, the function BlueprintFracturingMachine::calibrateFTriangleLineAndScannerOREKeys()).

		EnclaveKeyDef::EnclaveKey scanningKeys[3];	// Setup by functions as BlueprintFracturingMachine::calibrateFTriangleLineAndScannerOREKeys(),
													// This key set represents the valid range of fixed raycasting dimensions for X/Y/Z.
													// Currently required for use by the virtual function FTriangleTracerBase::runLineTracing()

		UniquePointContainer fracturerPoints;				// any point that will be used by any line, or anywhere in the triangle for that matter, should go here.
															// This includes but is not limited to: the original points of the triangle, traced exterior points, and ray-casted points.
															// which FRayCasterQuadBase-derived classes to build.
		std::map<FRayCasterTypeEnum, std::shared_ptr<FRayCasterQuadBase>> selectedRayCasters;	// a map of selected ray casters, that must be executed.
																								// Must be called by derived class that is based off this one.

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> incalculableKeys; // this is a set of keys that correlate to FTriangleProductionStager instances 
																								  // in which a set of triangles destined for output were not produced. 

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

		enum class DifferingDimValues
		{
			DIFFERENT_X,
			DIFFERENT_Y,
			DIFFERENT_Z
		};

		std::map<LineScanPermit, std::shared_ptr<DimensionalLineScannerBase>> lineScannerMap;

		std::set<LineScanPermit> getValidPermits();
		int getScanDimensionalStartKey(LineScanPermit in_dimensionToFetch);
		void erasePermitFromSet(DifferingDimValues in_dimValueForErase, std::set<LineScanPermit>* in_permitMapRef);
		std::map<FRayCasterTypeEnum, FRayCasterInitData> getUsableRayCasters();		// uses the values of the originFTriangleKeys (which must be calibrated before this) to determine;
																					// the value of the interval used for the FRayCasterInitData is equivalent to the rayCastDimInterval that must be already set,
																					// before this function is called.
		void setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef);	// set a reference
																																		// to the map that we will output fracture results to.
		void analyzeAndCleanupStagers(FTriangleType in_fTriangleTypeForDebug); // check the stagerMap for any bad instances (bad number of lines, etc);
										 // for valid stagers, the lines of said stagers are put into the proper sequential order.

		void buildAndRunFRayCasters();

		void runRaycastCollisionResolver();	// (NEW, as of 2/10/2023) this function calls an instance of FExteriorRaycastCollisionResolver, which is a 
											// class that is used to see if any FTrianglePoint instances that are of an EXTERIOR type, actually cross paths
											// with a ray that is used to cast an INTERIOR point (for instance, an FTrianglePoint having a FTrianglePointType of INTER_FROM_YRAYCAST).
											// If a collision is detected between the EXTERIOR point and the INTER* type point (which is known as a External-InteriorRayCast collision),
											// the EXTERIOR point takes precedence, and the INTER* point must be erased.

		void buildAndRunFLineScanners();	// builds and runs the FLineScanners, based on the value of rayCastDimInterval that represents the fixed grid length.



};

#endif
