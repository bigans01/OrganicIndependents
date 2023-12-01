#pragma once

#ifndef RDERIVEDTYPEENUM_H
#define RDERIVEDTYPEENUm_H

/*

Description: Used by derivatives of the RenderableTriangleBase class, to signal the associated 
derived class that is instantiated. Needed by the RenderableTriangleBase::setRenderingType function, and
utilized by the overloaded operators and constructors in RenderableTriangleContainer to determine what derivative
of RenderableTriangleBase to instantiate when pushing back a unique_ptr into the rtVector member of that class.

*/

enum class RDerivedTypeEnum
{
	DERIVED_TYPE_UNDEFINED,		// this would be the default value in a derived instance of RenderableTriangleBase; must be manually
								// overwritten by a call to the RenderableTriangleBase::setRenderingType function, whenever a new instance is created.

	R_TILED,		// Should be set by calling setRenderingType with this argument, for all RenderableTiledTriangle instances.
	R_TEXTURED
};

#endif
