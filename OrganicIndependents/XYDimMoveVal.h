#pragma once

#ifndef XYDIMMOVEVAL_H
#define XYDIMMOVEVAL_H

class XYDimMoveVal
{
	public:
		XYDimMoveVal() {};
		XYDimMoveVal(int in_x, int in_y) : x(in_x), y(in_y) {}
		int x = 0;
		int y = 0;
};

#endif