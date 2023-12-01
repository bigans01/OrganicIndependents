#pragma once

#ifndef RENDERABLETILEDTRIANGLE_H
#define RENDERABLETILEDTRIANGLE_H

#include "RenderableTriangleBase.h"
#include "TriangleMaterial.h"
#include "OrganicTriangleTertiary.h"
#include "IndependentUtils.h"
#include "RenderableGenerationResult.h"

/*

Description: This class will produced terrain textures triangles, in the "tiled" style; the results, which will come from the call to generate(), 
can then be appened into individual blocks.

*/
	
class RenderableTiledTriangle : public RenderableTriangleBase
{
	public:
		void printStats();
		void setMaterialID(TriangleMaterial in_tilingMaterial);
		TriangleMaterial fetchMaterialID();
		RenderableGenerationResult generateData();	// this derived-class will use an OrganicTriangleTertiary to produce the BB fans that this tiled triangle produced,
													// along with any bad/incalculable blocks. The return value may be used in whatever way seems applicable, and is a
													// representation of the generated data from the function call.
	private:
		TriangleMaterial tilingMaterial = TriangleMaterial::NOVAL;
};

#endif
