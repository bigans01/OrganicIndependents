#pragma once

#ifndef THINFAN_H
#define THINFAN_H

#include "FanBase.h"

class ThinFan : public FanBase
{
	public:
		int getPointAtIndex(int in_pointArrayIndex);
		void fillPointIndex(int in_pointArrayIndex, int in_pointID);
		FanData getFanData();
		void printPoints();
		void buildFromFanData(FanData in_fanData);

		unsigned char thinArray[8] = { 0 };	// "Thin" means we use unsigned chars to index the points.

};

#endif
