#pragma once

#ifndef ORGANICTRANSFORMUTILS_H
#define ORGANICTRANSFORMUTILS_H

#include "EnclaveTriangle.h"
#include "EnclaveTriangleSkeleton.h"
#include "IndependentUtils.h"
#include "BoundaryPolyIndicator.h"

class OrganicTransformUtils
{
public:
	static EnclaveTriangleSkeleton deflateEnclaveTriangle(EnclaveTriangle in_enclaveTriangle);
	static EnclaveTriangleSkeleton buildTriangleSkeletonRaw(ECBPolyPoint in_pointA, 
															ECBPolyPoint in_pointB, 
															ECBPolyPoint in_pointC, 
															short in_materialID, 
															short in_isPolyPerfectlyClamped, 
															ECBPolyPoint in_emptyNormal,
															BoundaryPolyIndicator in_skeletonBoundaryIndicator);
	static EnclaveTriangle inflateEnclaveTriangle(EnclaveTriangleSkeleton in_enclaveTriangleSkeleton);
};

#endif
