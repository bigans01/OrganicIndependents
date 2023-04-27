#pragma once

#ifndef FTRIANGLE_H
#define FTRIANGLE_H

#include "FTriangleContainer.h"
#include "FTriangleType.h"
#include "FTriangleFracturerBase.h"
#include "WorldFracturingMachine.h"
#include "BlueprintFracturingMachine.h"
#include "OREFracturingMachine.h"
#include "OutputDirector.h"

/*

Description:

The fracturing solution to end them all. And in the errors, bind them.

The FTriangle class breaks itself down into a lower-level dimension, based off the value of the triangleOriginGrid.
i.e, an FTriangle of the type FTriangleType::WORLD, will break down into one or more FTriangleOutput instances of the type FTriangle::BLUEPRINT.

The output of an FTriangle is an std::unordered_map of keyed FTriangleContainers, where each FTriangleContainer contains one or more
FTriangleOutput instances. The FTriangleOutput instances will have their FTriangleTypes be equivalent to the next level down from the FTriangle 
that spawned them. These FTriangleOutput instances can also be used to instantiate an FTriangle (see constructor below). 

For instance, an FTriangle that has a type of FTriangleType::WORLD, will potentially produce many FTriangleContainers; all FTriangleOutput instances
in said containers will be of the type FTriangleType::BLUEPRINT, as that is the next level down.

*/

class FTriangle
{
	public:
		FTriangle() {};	// should only ever be used for copying into maps.

		// constructor for small points (floats)
		FTriangle(ECBPolyPoint in_fracturePoint0,
				  ECBPolyPoint in_fracturePoint1,
				  ECBPolyPoint in_fracturePoint2,
				  FTriangleType in_originType,
				  ECBPolyPoint in_fractureEmptyNormal,
				  BoundaryOrientation in_requiredOrientation,
				  PerfectClampEnum in_perfectClampValue,
				  TriangleMaterial in_fractureMaterial,
				  OutputDirectorMode in_outputMode = OutputDirectorMode::OUTPUT_NONE)
		{
			// each of the below ECBPolyPoints is converted to DoublePoint, via the special DoublePoint operator.
			fracturePoints[0] = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1] = in_fracturePoint1;	// "" 
			fracturePoints[2] = in_fracturePoint2;	// ""
			triangleOriginGrid = in_originType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;

			fractureMaterial = in_fractureMaterial;

			writingMode = in_outputMode;
			outputWriter.setOutputMode(writingMode);
		}

		// constructor for big points (double); does the same stuff as above; 
		// will be needed by OrganicServerLib for very far-out OSContouredTriangles.
		FTriangle(DoublePoint in_fracturePoint0,
				DoublePoint in_fracturePoint1,
				DoublePoint in_fracturePoint2,
				FTriangleType in_originType,
				ECBPolyPoint in_fractureEmptyNormal,
				BoundaryOrientation in_requiredOrientation,
				PerfectClampEnum in_perfectClampValue,
				TriangleMaterial in_fractureMaterial,
				OutputDirectorMode in_outputMode = OutputDirectorMode::OUTPUT_NONE)
		{
			fracturePoints[0] = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1] = in_fracturePoint1;	// "" 
			fracturePoints[2] = in_fracturePoint2;	// ""
			triangleOriginGrid = in_originType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;

			fractureMaterial = in_fractureMaterial;

			writingMode = in_outputMode;
			outputWriter.setOutputMode(writingMode);
		}

		// constructor for creating an FTriangle from an FTriangleOutput
		FTriangle(FTriangleOutput in_outputTriangle) 
		{
			fracturePoints[0] = in_outputTriangle.fracturePoints[0];
			fracturePoints[1] = in_outputTriangle.fracturePoints[1];
			fracturePoints[2] = in_outputTriangle.fracturePoints[2];
			triangleOriginGrid = in_outputTriangle.destinedGridType;
			fractureEmptyNormal = in_outputTriangle.fractureEmptyNormal;

			fractureRequiredOrientation = in_outputTriangle.fractureRequiredOrientation;
			fractureRequiredClampValue = in_outputTriangle.fractureRequiredClampValue;

			fractureMaterial = in_outputTriangle.outputTriangleMaterial;
		};

		void fracture();	// fractures the FTriangle down into the next level; 
							// i.e., fracture an FTriangle of the type WORLD, into a bunch of FTriangleOutput instances that are of the type BLUEPRINT.

		void printProducedPoints();		// print the unique points that are found in the underlying FTriangleFraturer base instance; fracture() MUST be called before this,
										// or there will be undefined behavior. 

		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* fetchOutputContainerRef();
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> getUnresolvedOutputs();
		OutputDirector outputWriter;	// used to write FTriangle output to std::cout, or save it for later use/analysis.
	private:
		DoublePoint fracturePoints[3];		// the original points of the FTriangle, before anything is done; these values 
											// may or may not get translated.
											
		FTriangleType triangleOriginGrid = FTriangleType::NOVAL;	// the original grid type for the FTriangle (the grid type it was built in)
		FTriangleType triangleOutputGrid = FTriangleType::NOVAL;	// the grid type that would be put into any FTriangles produced by a
																	// FTriangleFracturerBase-derived class.
		ECBPolyPoint fractureEmptyNormal;	// the empty normal to compare against when building the FTriangleContainers.
		BoundaryOrientation fractureRequiredOrientation = BoundaryOrientation::NONE;	// must be set by constructor.
		PerfectClampEnum fractureRequiredClampValue = PerfectClampEnum::NONE;	// must be set by constructor

		TriangleMaterial fractureMaterial = TriangleMaterial::NOVAL;	// in reality, this will need to be set to something other than NOVAL. (GRASS, DIRT, etc)

		std::shared_ptr<FTriangleFracturerBase> fracturerMachine;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher> outputContainers;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> unresolvedOutputs;	// this map stores the EnclaveKeyDef values that correlate
																									// with FTriangleProductionStagers that couldn't produce a valid solution,
																									// when the FTriangleProductionStager::analyzeAndReorganize function got called.
																									// This class does not directly modify this member; instead, the underlying FTriangleFracturerBase-derived
																									// class that is used must produce it, which instances of this class will then store in this member.

		bool hasFracturingCompleted = false;

		void determineOutputLevel();	// sets the value of the triangleOutputGrid, based on what the value of the triangleOriginGrid is.
										// The value of triangleOutputGrid is used to determine the value of the FTriangleFracturerBase-derived instance.
		void setupAndRunFracturerMachine();		// instantiates the appropriate class for the fracturerMachine, based on the value of triangleOutputGrid.

		OutputDirectorMode writingMode = OutputDirectorMode::OUTPUT_NONE;	// used to determine what the FTriangle will do with the debug output;
																			// This should always be set to off/false by default.


};

#endif
