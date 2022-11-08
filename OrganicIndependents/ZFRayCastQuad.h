#pragma once

#ifndef ZFRAYCASTQUAD_H
#define ZFRAYCASTQUAD_H

#include "FRayCasterQuadBase.h"

class ZFRayCastQuad : public FRayCasterQuadBase
{
	public:
		void buildAndCastRays();
	private:
		struct ZFRay
		{
			ZFRay() {};
			ZFRay(float in_xCoord,
				float in_yCoord,
				float in_minZ,
				float in_maxZ) :
				xCoord(in_xCoord),
				yCoord(in_yCoord),
				minZ(in_minZ),
				maxZ(in_maxZ)
			{};

			float xCoord = 0.0f;
			float yCoord = 0.0f;
			float minZ = 0.0f;
			float maxZ = 0.0f;
			ECBPolyPointPair getRayPoints();
		};

		std::vector<ZFRay> zFRayVector;
};

#endif
