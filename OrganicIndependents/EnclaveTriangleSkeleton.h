#pragma once

#ifndef ENCLAVETRIANGLESKELETON_H
#define ENCLAVETRIANGLESKELETON_H

#include "EnclaveTriangle.h"
#include "TriangleLine.h"

class IndependentUtils;
class EnclaveTriangleSkeleton
{
public:
	ECBPolyPoint points[3];
	TriangleMaterial materialID = TriangleMaterial::NOVAL;			// is the triangle dirt, stone, snow, wood, etc
	PerfectClampEnum isPolyPerfectlyClamped = PerfectClampEnum::NONE;					// determines if the entire polygon is perfectly clamped to x, y, or z axis
	ECBPolyPoint emptyNormal;
	BoundaryPolyIndicator skeletonBoundaryIndicatorValue;
	
	EnclaveTriangle inflateToEnclaveTriangle();
	void printSkeletonTriangle();
	Message convertETSkeletonToMessage();	// copies the data of the skeleton into a generic Message that may be appended
											// to other Messages. Currently only really utilized by EnclaveTriangleSkeletonSupergroupManager::convertSkeletonSGMToBDM.
};

#endif
