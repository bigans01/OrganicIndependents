#pragma once

#ifndef FTRIANGLEOUTPUT_H
#define FTRIANGLEOUTPUT_H

#include "FTriangleType.h"
#include "PerfectClampEnum.h"

/*

Description:

Meant to store a single triangle that is produced as a result of the fracturing process that occurs from 
an FTriangle. This class can also be used to instantiate an FTriangle, as it's data members are a 1:1 copy.

This class has to be used in order to avoid a circular redundancy problem that was occuring. Before this class,
the FTriangle class contained a map of FTriangleContainers, and FTriangleContainers contained a vector of FTriangles; 
this resulted in very strange compilation artifacts, as a result of a bad decision.

This class addresses that circular dependency.

*/

class FTriangleOutput
{
	public:
		FTriangleOutput() {};
		FTriangleOutput(ECBPolyPoint in_fracturePoint0,
			ECBPolyPoint in_fracturePoint1,
			ECBPolyPoint in_fracturePoint2,
			FTriangleType in_destinedGridType,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_requiredOrientation,
			PerfectClampEnum in_perfectClampValue)
		{
			// each of the below ECBPolyPoints is converted to DoublePoint, via the special DoublePoint operator.
			fracturePoints[0] = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1] = in_fracturePoint1;	// "" 
			fracturePoints[2] = in_fracturePoint2;	// ""
			destinedGridType = in_destinedGridType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;
		};

		FTriangleOutput(DoublePoint in_fracturePoint0,
			DoublePoint in_fracturePoint1,
			DoublePoint in_fracturePoint2,
			FTriangleType in_destinedGridType,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_requiredOrientation,
			PerfectClampEnum in_perfectClampValue)
		{
			fracturePoints[0] = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1] = in_fracturePoint1;	// "" 
			fracturePoints[2] = in_fracturePoint2;	// ""
			destinedGridType = in_destinedGridType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;
		}


		DoublePoint fracturePoints[3];
		FTriangleType destinedGridType = FTriangleType::NOVAL;	// the value that determines the type of grid this FTriangleOutput is intended to be.
																// Valid values would be:
																//	-BLUEPRINT
		ECBPolyPoint fractureEmptyNormal;
		BoundaryOrientation fractureRequiredOrientation = BoundaryOrientation::NONE;	// must be set by constructor.
		PerfectClampEnum fractureRequiredClampValue = PerfectClampEnum::NONE;	// must be set by constructor
};

#endif