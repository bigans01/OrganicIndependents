#pragma once

#ifndef FTRIANGLEPOINTTYPE_H
#define FTRIANGLEPOINTTYPE_H

enum class FTrianglePointType
{
	NOVAL,
	ORIGINAL,	// indicates that this is one of the 3 original points that made up an FTriangle.
	EXTERIOR,	// indicates that this is a point that was produced as a result of tracing the FTriangle's lines.
	INTER_FROM_XRAYCAST,	// indicates the point was produced from an X-ray cast.
	INTER_FROM_YRAYCAST,	// indicates the point was produced from an Y-ray cast.
	INTER_FROM_ZRAYCAST,	// indicates the point was produced from an Z-ray cast.
};

#endif
