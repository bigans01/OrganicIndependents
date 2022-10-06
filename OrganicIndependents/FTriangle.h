#pragma once

#ifndef FTRIANGLE_H
#define FTRIANGLE_H

#include "FTriangleType.h"
#include "ECBPolyPoint.h"
#include <mutex>
#include <unordered_map>
#include "FTriangleContainer.h"
#include "EnclaveKeyDef.h"
#include "FTriangleFracturerBase.h"
#include "WorldFracturingMachine.h"
#include "BoundaryOrientation.h"

/*

Description:

The fracturing solution to end them all. And in the errors, bind them.

*/

class FTriangle
{
	public:
		FTriangle() {};	// should only ever be used for copying into maps.
		FTriangle(ECBPolyPoint in_fracturePoint0,
				  ECBPolyPoint in_fracturePoint1,
				  ECBPolyPoint in_fracturePoint2,
				  FTriangleType in_originType,
				  ECBPolyPoint in_fractureEmptyNormal,
				  BoundaryOrientation in_requiredOrientation)
		{
			fracturePoints[0] = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1] = in_fracturePoint1;	// "" 
			fracturePoints[2] = in_fracturePoint2;	// ""
			triangleOriginGrid = in_originType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
		}

		void fracture();

	private:
		ECBPolyPoint fracturePoints[3];
		FTriangleType triangleOriginGrid = FTriangleType::NOVAL;	// the original grid type for the FTriangle (the grid type it was built in)
		FTriangleType triangleOutputGrid = FTriangleType::NOVAL;	// the grid type that would be put into any FTriangles produced by a
																	// FTriangleFracturerBase-derived class.
		ECBPolyPoint fractureEmptyNormal;
		BoundaryOrientation fractureRequiredOrientation = BoundaryOrientation::NONE;	// must be set by constructor.

		std::shared_ptr<FTriangleFracturerBase> fracturerMachine;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher> outputContainers;

		void determineOutputLevel();	// sets the value of the triangleOutputGrid, based on what the value of the triangleOriginGrid is.
										// The value of triangleOutputGrid is used to determine the value of the FTriangleFracturerBase-derived instance.
		void setupAndRunFracturerMachine();		// instantiates the appropriate class for the fracturerMachine, based on the value of triangleOutputGrid.


};

#endif
