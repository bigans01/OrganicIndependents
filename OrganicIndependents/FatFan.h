#pragma once

#ifndef FATFAN_H
#define FATFAN_H

#include "FanBase.h"

class FatFan : public FanBase
{
public:
	int getPointAtIndex(int in_pointArrayIndex)
	{
		return thinArray[in_pointArrayIndex];
	}
	void fillPointIndex(int in_pointArrayIndex, int in_pointID)
	{
		thinArray[in_pointArrayIndex] = in_pointID;
	}
	FanData getFanData()
	{
		FanData returnData(
			thinArray,
			materialID,
			numberOfTertiaries,
			faceAlignment,
			emptyNormal);
		return returnData;
	}
	void printPoints()
	{
		{
			if (numberOfTertiaries != 0)
			{
				int numberOfPoints = 2 + numberOfTertiaries;
				for (int x = 0; x < numberOfPoints; x++)
				{
					std::cout << "Fan point index at [" << x << "]: " << thinArray[x] << std::endl;
				}
			}
		}
	}
	unsigned int thinArray[8] = { 0 };	// "Fat" means we use unsigned ints to reference the points.
};

#endif
