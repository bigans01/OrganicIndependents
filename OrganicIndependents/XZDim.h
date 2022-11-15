#pragma once

#ifndef XZDIM_H
#define XZDIM_H

#include "XZDimMoveVal.h"

class XZDim
{
	public:
		XZDim() {};
		XZDim(int in_x, int in_z) : x(in_x), z(in_z) {}
		XZDim& operator +=(const XZDimMoveVal& in_moveVal)
		{
			x += in_moveVal.x;
			z += in_moveVal.z;
			return *this;
		};
		int x = 0;
		int z = 0;
};

#endif
