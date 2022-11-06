#pragma once

#ifndef YZDIMMOVEVAL_H
#define YZDIMMOVEVAL_H

class YZDimMoveVal
{
	public:
		YZDimMoveVal() {};
		YZDimMoveVal(int in_y, int in_z) : y(in_y), z(in_z) {}
		int y = 0;
		int z = 0;
};

#endif
