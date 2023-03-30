#pragma once

#ifndef FTRIANGLECONTAINER_H
#define FTRIANGLECONTAINER_H

#include "FTriangleLine.h"
#include "FTriangleType.h"
#include "FTriangleOutput.h"
#include "FTriangleReverseTranslationMode.h"

/*

Description:

This class is meant to contain one or more FTriangleOutput instances.

In the produceFTriangles function, the parentEmptyNormal is used to organize the points of an FTriangleOutput in a way, 
such that the signage of the empty normal in the FTriangleOutput matches the signage
of the passed in empty normal (in_parentEmptyNormal).

The in_parentOrientation parameter of that same function, the passed in EnclaveKey, and the passed in
FTriangleType are used to determine if the points of the FTriangle are aligned in such a way
that they need a value for the BoundaryOrientation. The FTriangleOutput's orientation can then
be compared to the FTriangle's BoundaryOrientation, to see if they are different, allowing us
to check for anomalies.

*/

class FTriangleContainer
{
	public:
		std::map<int, FTriangleOutput> fracturedTriangles;
		void insertConstructionLines(std::vector<FTriangleLine> in_constructionLines);
		void printConstructionLines();
		void produceFTriangles(FTriangleType in_destinedTriangleType,
								ECBPolyPoint in_parentEmptyNormal,
								BoundaryOrientation in_parentOrientation,
								EnclaveKeyDef::EnclaveKey in_containerBounds,
								TriangleMaterial in_containerMaterial);

		// Below: this function will run the boundary tests for each FTriangleOutput, against
		// an instance of FTriangleContainerBounds. That instance will do boundary orientations,
		// check the empty normals against the boundary (if needed), and set the perfect clamp values.
		// The function should return true if the FTriangleContainer has no more FTriangleOutputs in it's map.
		bool runBoundaryTests(FTriangleReverseTranslationMode in_reverseTranslationMode,
								EnclaveKeyDef::EnclaveKey in_boundingKey,
								ECBPolyPoint in_parentEmptyNormal);

		void printProducedFTriangles();	// debug only: print the points contained within all FTriangleOutputs
	private:
		std::vector<FTriangleLine> constructionLines;
		static bool areNormalsDirectionallyAligned(ECBPolyPoint in_normalA, ECBPolyPoint in_normalB);	// takes in two normals, checks to see if their
																								// directions are at aligned in at least one of the dimensions (x,y,z)

		// Below: a simple struct that stores 3 points, calculates a normal, and can invert said normal by swapping points.
		struct OutputTriangleFrame
		{
			OutputTriangleFrame();	// default constructor
			OutputTriangleFrame(DoublePoint in_pointA,
				DoublePoint in_pointB,
				DoublePoint in_pointC);

			// the points that make up the frame
			DoublePoint framePoints[3];

			ECBPolyPoint getNormal();		// fetches the normal
			void swapForNormalChange();		// swaps points at indices 1 and 2, so that the normal gets flipped.
		};

		// Below: the bounds that represent the limits that the FTriangleOutput instances that have
		// been produced lie within.
		struct FTriangleContainerBounds
		{
			FTriangleContainerBounds();	// default constructor
			FTriangleContainerBounds(EnclaveKeyDef::EnclaveKey in_boundingKey,
									float in_boundingInterval);

			// the double values that represent the bounds.
			double boundsMinX = 0.0f;
			double boundsMaxX = 0.0f;
			double boundsMinY = 0.0f;
			double boundsMaxY = 0.0f;
			double boundsMinZ = 0.0f;
			double boundsMaxZ = 0.0f;

			// the following 3 functions below are run by FTriangleContainer::runBoundaryTests.
			bool runTriangleOrientationAnalysis(FTriangleOutput* in_fTriangleOutputRef);	// checks to to see if an FTriangleOutput is orientated
																							// to a boundary in this instance.
			void runBoundaryToNormalAnalysis(FTriangleOutput* in_fTriangleOutputRef, ECBPolyPoint in_parentEmptyNormal);	// checks to see if the empty normal
																															// of the given FTriagleOutput is aligned to its boundary;
																															// this should only be called if it was determined that it was aligned to a boundary.
			void runPerfectClampAnalysis(FTriangleOutput* in_fTriangleOutputRef);	// checks if the given FTriangleOutput has a perfect clamp value, and sets it when it detects it.
			void printBounds();	// debug only.
		};

		// Below: this function produces a raw, unconfigured FTriangleOutput instance, when given a frame.
		// The function also ensures the frame has it's appropriate normal set, as this must be done before th FTriangleOutput is formed.
		// The values of the orientation and clamp value are instantiated to NONE, as that is all handled by the call to the function
		// FTriangleContainer::runBoundaryTests.
		//
		// NOTE (3/29/2023): for the time being, the "old method" of organizing the points to produce the empty normal has been deprecated,
		// as it would on rare occasions, run into an issue where some outputs of the container get swapped, 
		// but others don't  (which leads to rendering anomalies). For this reason the FTriangleOutputs returned by this function will not be swapped,
		// and will intead have their normal value set to in_parentEmptyNormal. This is a temporary fix until a "smarter" fix is developed for the old method.
		// 
		// This anomaly case can be recreated by analyzing the crossProducts of these points. Notice how the signage doesn't match (it is off slightly).
		// The fact that one triangle gets swapped, but the other would not, is the problem.
		//	glm::vec3 pA(0, 0.55, 0.04);
		//	glm::vec3 pB(0.48, 0.74, 1);
		//	glm::vec3 pC(0.23, 0.84, 1);
		//	glm::vec3 pD(0, 0.56, 0.08);
		FTriangleOutput formOutput(OutputTriangleFrame in_triangleFrame,
			ECBPolyPoint in_parentEmptyNormal,
			FTriangleType in_typeForBoundingBasis,
			EnclaveKeyDef::EnclaveKey in_boundingKey,
			TriangleMaterial in_outputMaterial);

		bool normalsSwapped = false;	// set to true, if swapForNormalChange() is called even once.
										// If this gets set to true, the FTriangleOutput instances we produce
										// will have to be put into the fracturedTriangles in reverse order, to 
										// comply with the appropriate triangle fan point order.
										// See the FTriangleContainer::produceFTriangles function for more in-depth explanation
};

#endif
