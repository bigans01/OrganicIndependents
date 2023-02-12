#pragma once

#ifndef FTRIANGLEPOINTTYPE_H
#define FTRIANGLEPOINTTYPE_H

enum class FTrianglePointType
{
	NOVAL,
	EXTERIOR,
	INTER_FROM_XRAYCAST,	// indicates the point was produced from an X-ray cast.
	INTER_FROM_YRAYCAST,	// indicates the point was produced from an Y-ray cast.
	INTER_FROM_ZRAYCAST,	// indicates the point was produced from an Z-ray cast.
};

#endif
