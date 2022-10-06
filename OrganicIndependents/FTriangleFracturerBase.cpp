#include "stdafx.h"
#include "FTriangleFracturerBase.h"

void FTriangleFracturerBase::transferFTriangleMetadata(ECBPolyPoint in_fracturePoint0,
	ECBPolyPoint in_fracturePoint1,
	ECBPolyPoint in_fracturePoint2,
	ECBPolyPoint in_fractureEmptyNormal,
	BoundaryOrientation in_originBoundaryOrientation)
{
	originFTrianglePoints[0] = in_fracturePoint0;
	originFTrianglePoints[1] = in_fracturePoint1;
	originFTrianglePoints[2] = in_fracturePoint2;
	originFTriangleEmptynormal = in_fractureEmptyNormal;
	originBoundaryOrientation = in_originBoundaryOrientation;
}

void FTriangleFracturerBase::setOutputRef(std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* in_outputRef)
{
	ftfOutputRef = in_outputRef;
};