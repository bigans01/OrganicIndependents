#pragma once

#ifndef BOUNDARYPOLYINDICATOR_H
#define BOUNDARYPOLYINDICATOR_H

#include "BoundaryOrientation.h"

class BoundaryPolyIndicator
{
	public:
		unsigned char indicatorData;
		void setBoundaryIndicator(BoundaryOrientation in_faceOrientation)
		{
			indicatorData = 0;
			int bitToSet = 0;
			switch (in_faceOrientation)
			{
				case(BoundaryOrientation::NEG_Z): { bitToSet = 8; break;} // NEG_Z		NORTHFACE
				case(BoundaryOrientation::POS_X): { bitToSet = 7; break; } // POS_X		EASTFACE
				case(BoundaryOrientation::POS_Z): { bitToSet = 6; break; } // POS_Z		SOUTHFACE
				case(BoundaryOrientation::NEG_X): { bitToSet = 5; break; } // NEG_X		WESTFACE
				case(BoundaryOrientation::POS_Y): { bitToSet = 4; break; } // POS_Y		TOPFACE
				case(BoundaryOrientation::NEG_Y): { bitToSet = 3; break; } // NEG_Y		BOTTOMFACE
			}

			int exponent = bitToSet - 1;
			indicatorData |= (1 << exponent);
		}
		BoundaryOrientation getBoundaryIndicatorValue()
		{
			if (indicatorData == 0)
			{
				return BoundaryOrientation::NONE;
			}
			else
			{

			}
		}
};

#endif
