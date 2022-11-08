#pragma once

#ifndef YFRAYCASTQUAD_H
#define YFRAYCASTQUAD_H

#include "FRayCasterQuadBase.h"

class YFRayCastQuad : public FRayCasterQuadBase
{
	public:
		void buildAndCastRays();
	private:
		struct YFRay
		{
			YFRay() {};
			YFRay(float in_xCoord,
				float in_zCoord,
				float in_minY,
				float in_maxY) :
				xCoord(in_xCoord),
				zCoord(in_zCoord),
				minY(in_minY),
				maxY(in_maxY)
			{};

			float xCoord = 0.0f;
			float zCoord = 0.0f;
			float minY = 0.0f;
			float maxY = 0.0f;
			ECBPolyPointPair getRayPoints();
		};

		std::vector<YFRay> yFRayVector;
};

#endif
