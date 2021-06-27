#pragma once

#ifndef TRIANGLELINE_H
#define TRIANGLELINE_H

#include "ECBPolyPoint.h"

class TriangleLine
{
public:
	// some comment
	TriangleLine() {};
	TriangleLine(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC) :
		pointA(in_pointA),
		pointB(in_pointB),
		pointC(in_pointC)
	{};
	ECBPolyPoint pointA;
	ECBPolyPoint pointB;
	ECBPolyPoint pointC;
	ECBPolyPoint x_interceptSlope;
	ECBPolyPoint y_interceptSlope;
	ECBPolyPoint z_interceptSlope;
	char clamped_to_x = 0;			// cache optimized, 8 bytes used only (2 DWORDS)
	char clamped_to_y = 0;
	char clamped_to_z = 0;
	char x_clamp_direction = 0;
	short y_clamp_direction = 0;
	short z_clamp_direction = 0;
};

#endif
