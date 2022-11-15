#pragma once

#ifndef XYDIM_H
#define XYDIM_H

#include "XYDimMoveVal.h"

class XYDim
{
	public:
		XYDim() {};
		XYDim(int in_x, int in_y) : x(in_x), y(in_y) {}
		XYDim& operator +=(const XYDimMoveVal& in_moveVal)
		{
			x += in_moveVal.x;
			y += in_moveVal.y;
			return *this;
		}
		int x = 0;
		int y = 0;
};

#endif
