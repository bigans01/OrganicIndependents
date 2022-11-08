#pragma once

#ifndef ECBPOLYLINESLOPESARRAY_H
#define ECBPOLYLINESLOPESARRAY_H

#include "ECBPolyLineSlopes.h"
#include "ECBPolyLine.h"

class ECBPolyLineSlopesArray
{
public:
	ECBPolyLineSlopes slopesArray[3];
	void addLineSlopes(int in_lineID, ECBPolyLine in_polyLine);
};

#endif
