#pragma once

#ifndef RCategoryEnum_H
#define RCategoryEnum_H

/*

Description: This enum class should contain all the possible categories for RenderableTriangle (R) style rendering. The 
RenderableTriangleHandler class uses this enum value in its rTypesMap and uniqueIDLookup members, the idea being that 
there will be potentially many values for RCategoryEnum -- but only a few for the derived-types of RenderableTraingleBase, which 
are stored in RDerivedTypeEnum. Each category should have one type of RDerivedTypeEnum associated with it.

*/

enum class RCategoryEnum
{
	TERRAIN_TILE_1,	// original terrain tiles (grass, dirt, etc)
	TEXTURED_1		// for textures that should be coherenent between FTriangle fractures (i.e, a rock face)
};

#endif
