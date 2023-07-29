#pragma once

#ifndef FATFAN_H
#define FATFAN_H

#include "FanBase.h"

class FatFan : public FanBase
{
	public:
		int getPointAtIndex(int in_pointArrayIndex);
		void fillPointIndex(int in_pointArrayIndex, int in_pointID);
		FanData getFanData();
		void printPoints();
		void buildFromFanData(FanData in_fanData);

		unsigned int thinArray[8] = { 0 };	// "Fat" means we use unsigned ints to reference the points.
};

#endif
