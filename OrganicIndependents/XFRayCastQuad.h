#pragma once

#ifndef XFRAYCASTQUAD_H
#define XFRAYCASTQUAD_H

#include "FRayCasterQuadBase.h"

class XFRayCastQuad : public FRayCasterQuadBase
{
	public:
		void buildAndCastRays();
	private:
		struct XFRay
		{
			XFRay() {};
			XFRay(float in_yCoord,
				float in_zCoord,
				float in_minX,
				float in_maxX) :
				yCoord(in_yCoord),
				zCoord(in_zCoord),
				minX(in_minX),
				maxX(in_maxX)
			{};

			float yCoord = 0.0f;
			float zCoord = 0.0f;
			float minX = 0.0f;
			float maxX = 0.0f;
			ECBPolyPointPair getRayPoints();
		};

		std::vector<XFRay> xFRayVector;
};

#endif