#pragma once

#ifndef POLYGROUPRANGE_H
#define POLYGROUPRANGE_H

class PolyGroupRange
{
public:
	//PolyGroupRange(int begin, int end) : beginPolyID(begin), endPolyID(end) {}
	int beginPolyID = 0;
	int endPolyID = 0;
	int numberOfPolys = 0;	// the number of polys in the range.
};

#endif