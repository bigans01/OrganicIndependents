#pragma once

#ifndef QUATROTATIONPOINTSV2_H
#define QUATROTATIONPOINTSV2_H

#include "FTrianglePoint.h"

class QuatRotationPointsV2
{
	public:
		template<typename FirstOption, typename ...RemainingOptions> void insertPointRefs(FirstOption&& firstOption, RemainingOptions && ...optionParams)
		{
			//pointsRefVector.push_back(std::forward<FirstOption>(firstOption));
			insertPointRefs(std::forward<RemainingOptions>(optionParams)...);
			
		}
		void insertPointRefs() {};

	private:

		struct PointProxy
		{
			PointProxy() {};

		};
};


#endif