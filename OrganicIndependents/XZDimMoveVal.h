#pragma once

#ifndef XZDIMMOVEVAL_H
#define XZDIMMOVEVAL_H

class XZDimMoveVal
{
	public:
		XZDimMoveVal() {};
		XZDimMoveVal(int in_x, int in_z) : x(in_x), z(in_z) {}
		int x = 0;
		int z = 0;
};

#endif