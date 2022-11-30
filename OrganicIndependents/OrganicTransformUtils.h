#pragma once

#ifndef ORGANICTRANSFORMUTILS_H
#define ORGANICTRANSFORMUTILS_H

#include "EnclaveTriangle.h"
#include "EnclaveTriangleSkeleton.h"

class OrganicTransformUtils
{
public:
	static EnclaveTriangleSkeleton deflateEnclaveTriangle(EnclaveTriangle in_enclaveTriangle);
	static EnclaveTriangleSkeleton buildTriangleSkeletonRaw(ECBPolyPoint in_pointA, 
															ECBPolyPoint in_pointB, 
															ECBPolyPoint in_pointC, 
															TriangleMaterial in_materialID,
															short in_isPolyPerfectlyClamped, 
															ECBPolyPoint in_emptyNormal,
															BoundaryPolyIndicator in_skeletonBoundaryIndicator);
	static EnclaveTriangle inflateEnclaveTriangle(EnclaveTriangleSkeleton in_enclaveTriangleSkeleton);
};

#endif
