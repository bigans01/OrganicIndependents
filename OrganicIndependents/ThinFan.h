#pragma once

#ifndef THINFAN_H
#define THINFAN_H

#include "FanBase.h"

class ThinFan : public FanBase
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
					std::cout << "Fan point index at [" << x << "]: " << int(thinArray[x]) << std::endl;
				}
			}
		}
	}

	unsigned char thinArray[8] = { 0 };	// "Thin" means we use unsigned chars to index the points.

};

#endif
