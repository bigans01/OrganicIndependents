#pragma once

#ifndef RENDERABLETRIANGLE_H
#define RENDERABLETRIANGLE_H

#include "FTriangleType.h"
#include "FTrianglePoint.h"
#include "BoundaryPolyIndicator.h"
#include "PerfectClampEnum.h"
#include "TriangleMaterial.h"
#include "Operable3DEnclaveKeySet.h"
#include "RenderableGenerationResult.h"
#include "RDerivedTypeEnum.h"

/*

Description: The intent behind the RenderableTriangleBase class is to replace the existing EnclaveTriangle structure that currently exists in
OrganicRawEnclaves. This base class will allow for more flexibility, for different types of rendering: for example, the ability to create derived
classes that deal with tiled data (as is the case with RenderableTiledTriangle), and a separate one for textured data.

This allows the RenderableTriangleContainer class, which contains a vector of unique_ptr instances of this base class, to effectively store
multiple types derived from this class. 

In theory, this should allow for an easy way to access multiple rendering types within the same ORE, within the same class (see the associated class,
RenderableTriangleHandler)

*/

class RenderableTriangleBase
{
	public:
		// ******************************************************************
		// R_TILED-based derived class virtual functions
		// ******************************************************************
		virtual void setMaterialID(TriangleMaterial in_tilingMaterial) = 0;
		virtual TriangleMaterial fetchMaterialID() = 0;


		// Common virtual functions for all derived classes
		virtual RenderableGenerationResult generateData() = 0;	// this is the function that would produce data that would be put into EnclaveBlocks
															//
															// See constructor, OrganicTriangleTertiary::OrganicTriangleTertiary, as a starting point.
															// Rough description of steps:
															//													
															//	1.	Need a way to generate a std::map<int, OrganicFanGroup> member that contains produced data, AND
															//		return a set of any incalculable blocks. This can be done by calling an FTriangle that takes in
															//		FTriangleType::ORE as its origin grid space. See how the function EnclaveTriangle::executeRun calls
															//		an OrganicTriangleTertiary object to do this.

		virtual void printStats() = 0;	// print the stats (points, normal, etc) -- each derived class must define in it's own way.

		// Below: set up the common components inherent to all derivatives of RenderableTriangleBase.
		void initRenderable(FTrianglePoint in_point0,	// initRenderable should always be called immediately after a new object derived from this base class is created.
			FTrianglePoint in_point1,
			FTrianglePoint in_point2,
			ECBPolyPoint in_rEmptyNormal,
			BoundaryPolyIndicator in_rBoundaryIndicator,
			PerfectClampEnum in_rClampEnum);
		
		void setRFTriangleType(FTriangleType in_typeToSet);	// set the type that determines how the FTriangle will perform its fracturing. Call immediately 
															// after initRenderable above. 

		void setRenderingType(RDerivedTypeEnum in_enumType);	// flags the appropriate rendering type, so that code that queries this object knowas what instance it is;
																// i.e, pass in a value of RDerivedTypeEnum::R_TILED if the derived class of this base class is RenderableTiledTriangle.
																// Alternatively, pass in R_TEXTURED if dealing with a RenderableTexturedTriangle

		RDerivedTypeEnum getRenderingType();	// used to fetch the type associated with classes derived from this base class; needed in cases where code needs to check
												// the associated class of the derived_ptr (since this isn't easily accessible). 


		FTrianglePoint getPoint(int in_index); // ranges between 0 and 2, obviously.
		ECBPolyPoint getEmptyNormal();	// fetch the empty normal value

	protected:
		FTrianglePoint rPoints[3];	// all 3 points that constituted the triangle. Note that not all data members of each FTrianglePoint may be used;
									// it is completely up to the derived class how it wants to use the data in these points.												

		FTriangleType rfTriangleType = FTriangleType::NOVAL;	// needs to be set by a call to setFType; will determine how executeFRun operates.
		RDerivedTypeEnum rDerived = RDerivedTypeEnum::DERIVED_TYPE_UNDEFINED;	// must be set by call to setRenderingType.
	
		ECBPolyPoint rEmptyNormal;
		BoundaryPolyIndicator rBoundaryIndicator;	// indicates if the renderable triangle is clamped to a border
		PerfectClampEnum rClampEnum = PerfectClampEnum::NONE;	// indicates if the renderable triangle is clamped to a plane
};

#endif
