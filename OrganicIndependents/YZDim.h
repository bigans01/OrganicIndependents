#pragma once

#ifndef YZDIM_H
#define YZDIM_H

#include "YZDimMoveVal.h"

class YZDim
{
	public:
		YZDim() {};
		YZDim(int in_y, int in_z) : y(in_y), z(in_z) {}
		YZDim& operator +=(const YZDimMoveVal& in_moveVal)
		{
			y += in_moveVal.y;
			z += in_moveVal.z;
			return *this;
		};
		int y = 0;
		int z = 0;
};

#endif
