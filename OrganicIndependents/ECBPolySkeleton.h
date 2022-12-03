#pragma once

#ifndef ECBPOLYSKELETON_H
#define ECBPOLYSKELETON_H

class ECBPolySkeleton
{
public:
	TriangleMaterial materialID = TriangleMaterial::NOVAL;
	PerfectClampEnum isPolyPerfectlyClamped = PerfectClampEnum::NONE;
	ECBPolyPoint points[3];	// points of the Poly
	ECBPolyPoint emptyNormal;				// empty normal
	ECBPolyPoint mrp;						// mass reference point
	int polyType;			// a numerical representing the ECBPolyType; must be converted after it is read from the file.
};

#endif
