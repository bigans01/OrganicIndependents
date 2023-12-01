#pragma once

#ifndef ORETERRAINTRIANGLE_H
#define ORETERRAINTRIANGLE_H

#include "ECBPolyPoint.h"
//#include "ECBPolyPointTri.h"
#include "TriangleMaterial.h"

class ORETerrainTriangle
{
	public:
		ORETerrainTriangle() {};
		ORETerrainTriangle(ECBPolyPoint in_otEmptyNormal,
			TriangleMaterial in_otMaterial,
			ECBPolyPoint in_point0,
			ECBPolyPoint in_point1,
			ECBPolyPoint in_point2)
		{
			otEmptyNormal = in_otEmptyNormal;
			otMaterial = in_otMaterial;
			otPoints[0] = in_point0;
			otPoints[1] = in_point1;
			otPoints[2] = in_point2;
		}

		ECBPolyPoint otEmptyNormal;
		TriangleMaterial otMaterial = TriangleMaterial::NOVAL;
		ECBPolyPoint otPoints[3];
};

#endif
