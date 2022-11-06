#pragma once

#ifndef ECBPOLYSKELETON_H
#define ECBPOLYSKELETON_H


class ECBPolySkeleton
{
public:
	short materialID = 0;
	short isPolyPerfectlyClamped = 0;
	ECBPolyPoint points[3];	// points of the Poly
	ECBPolyPoint emptyNormal;				// empty normal
	ECBPolyPoint mrp;						// mass reference point
	int polyType;			// a numerical representing the ECBPolyType; must be converted after it is read from the file.
};

#endif
