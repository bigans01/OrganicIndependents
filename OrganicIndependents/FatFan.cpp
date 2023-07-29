#include "stdafx.h"
#include "FatFan.h"

int FatFan::getPointAtIndex(int in_pointArrayIndex)
{
	return thinArray[in_pointArrayIndex];
}

void FatFan::fillPointIndex(int in_pointArrayIndex, int in_pointID)
{
	thinArray[in_pointArrayIndex] = in_pointID;
}

FanData FatFan::getFanData()
{
	FanData returnData(
		thinArray,
		materialID,
		numberOfTertiaries,
		faceAlignment,
		emptyNormal);
	return returnData;
}

void FatFan::printPoints()
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

void FatFan::buildFromFanData(FanData in_fanData)
{
	numberOfTertiaries = in_fanData.numberOfTertiaries;

	// Remember: the number of points is equal to the number of tertiaries + 2.
	for (int x = 0; x < numberOfTertiaries + 2; x++)
	{
		thinArray[x] = in_fanData.pointArray[x];
	}

	materialID = in_fanData.materialID;
	faceAlignment = in_fanData.faceAlignment;
	emptyNormal = in_fanData.emptyNormal;
};