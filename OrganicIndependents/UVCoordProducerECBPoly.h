#pragma once

#ifndef UVCOORDPRODUCERECBPOLY_H
#define UVCOORDPRODUCERECBPOLY_H

#include "UVTriangleCoords.h"
#include "TileCoordinateProducer.h"
#include "AtlasMap.h"

class UVCoordProducerECBPoly
{
	public:
		UVCoordProducerECBPoly(TriangleMaterial in_materialID, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, AtlasMap* in_mapRef, int in_debugFlag, EnclaveKeyDef::EnclaveKey in_blueprintKey);
		UVTriangleCoords getCoords();
	private:
		ECBPolyPoint point0;
		ECBPolyPoint point1;
		ECBPolyPoint point2;
		AtlasMap* atlasMapRef;
		TriangleMaterial atlasTileID = TriangleMaterial::NOVAL;		// the ID of the atlas tile to look up -- equivalent to polyMaterialID
		EnclaveKeyDef::EnclaveKey blueprintKey;
};

#endif
