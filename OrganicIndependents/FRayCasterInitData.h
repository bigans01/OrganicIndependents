#pragma once

#ifndef FRAYCASTERINITDATA_H
#define FRAYCASTERINITDATA_H

class FRayCasterInitData
{
	public:
		FRayCasterInitData() {};
		FRayCasterInitData(int in_dimOneMin,
			   			   int in_dimOneMax,
						   int in_dimTwoMin,
			               int in_dimTwoMax,
							float in_rayCastInterval) :
			dimOneMin(in_dimOneMin),
			dimOneMax(in_dimOneMax),
			dimTwoMin(in_dimTwoMin),
			dimTwoMax(in_dimTwoMax),
			rayCastInterval(in_rayCastInterval)
		{};

		int dimOneMin = 0;
		int dimOneMax = 0;
		int dimTwoMin = 0;
		int dimTwoMax = 0;
		float rayCastInterval = 0.0f;
	
};

#endif
