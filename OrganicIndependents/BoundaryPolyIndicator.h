#pragma once

#ifndef BOUNDARYPOLYINDICATOR_H
#define BOUNDARYPOLYINDICATOR_H


/*

Description: This class is used by multiple classes, such as PTriangle (OrganicGLWinLib), EnclaveTriangle, and SPoly (OrganicGLWinLib)
             to store positional metadata + scab indicators for a triangle primitive. Each bit in the unsigned char represents a flag:

			 bit 1: scab child flag.
			 bit 2: scab parent flag.
			 bit 3: NEG_Y border flag.
			 bit 4: POS_Y border flag.
			 bit 5: NEG_X border flag.
			 bit 6: POS_Z border flag.
			 bit 7: POS_X border flag.
			 bit 8: NEG_Z border flag.

			 Only one border flag bit should be set at any given time, BUT either the "scab child" or "scab parent" flag may be 
			 set at the same time as an individual border flag (but both scab flags shouldn't be set simultaneously)

*/

class BoundaryPolyIndicator
{
	public:
		void setBoundaryIndicator(BoundaryOrientation in_faceOrientation)
		{
			int bitToSet = 0;
			switch (in_faceOrientation)
			{
				case(BoundaryOrientation::NEG_Z): { bitToSet = 8; break; } // NEG_Z		NORTHFACE
				case(BoundaryOrientation::POS_X): { bitToSet = 7; break; } // POS_X		EASTFACE
				case(BoundaryOrientation::POS_Z): { bitToSet = 6; break; } // POS_Z		SOUTHFACE
				case(BoundaryOrientation::NEG_X): { bitToSet = 5; break; } // NEG_X		WESTFACE
				case(BoundaryOrientation::POS_Y): { bitToSet = 4; break; } // POS_Y		TOPFACE
				case(BoundaryOrientation::NEG_Y): { bitToSet = 3; break; } // NEG_Y		BOTTOMFACE
			}

			int numberOfShifts = bitToSet - 1;
			indicatorData |= (1 << numberOfShifts);
		}

		void setScabParentIndicator()	// set the 2nd bit, which is the SCAB_PARENT indicator
		{
			indicatorData |= (1 << 1);
		}

		bool isScabParentIndicatorSet()
		{
			bool set = false;
			if (((indicatorData >> 1) & 1) == 1)	// checks if the 2nd bit is set, returns true if so.
			{
				set = true;
			}
			return set;
		}

		void setScabChildIndicator()	// set the 1st bit, which is the SCAB_CHILD indicator
		{
			indicatorData |= 1;
		}

		bool isScabChildIndicatorSet()
		{
			bool set = false;
			if ((indicatorData & 1) == 1)	// checks if the 1st bit is set, returns true if so.
			{
				set = true;
			}
			return set;
		}

		unsigned char getIndicatorData()
		{
			return indicatorData;
		}

		void setIndicatorData(unsigned char in_indicatorData)
		{
			indicatorData = in_indicatorData;
		}

		BoundaryOrientation getBoundaryIndicatorValue()
		{
			BoundaryOrientation returnOrientation = BoundaryOrientation::NONE;

			/*
			if (indicatorData == 0)
			{
				returnOrientation = BoundaryOrientation::NONE;
			}
			else
			{
				switch (indicatorData)
				{
					case 128:	{ returnOrientation = BoundaryOrientation::NEG_Z; break;}
					case 64:	{ returnOrientation = BoundaryOrientation::POS_X; break;}
					case 32:	{ returnOrientation = BoundaryOrientation::POS_Z; break;}
					case 16:	{ returnOrientation = BoundaryOrientation::NEG_X; break;}
					case 8:		{ returnOrientation = BoundaryOrientation::POS_Y; break;}
					case 4:		{ returnOrientation = BoundaryOrientation::NEG_Y; break;}
				}
			}
			*/

			// cycle through bits 8 through 3; which are for faces. The first result of 1 for a bitwise
			// comparison counts as the signaled BoundaryOrientation, so make sure multiple boundary bits aren't set.

			// Check NEG_Z (bit 8, 7 shifts.)
			if (((indicatorData >> 7) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::NEG_Z;
			}

			// Check POS_X (bit 7, 6 shifts.)
			else if (((indicatorData >> 6) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::POS_X;
			}

			// Check POS_Z (bit 6, 5 shifts.)
			else if (((indicatorData >> 5) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::POS_Z;
			}

			// Check NEG_X (bit 5, 4 shifts.)
			else if (((indicatorData >> 4) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::NEG_X;
			}

			// Check POS_Y (bit 4, 3 shifts.)
			else if (((indicatorData >> 3) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::POS_Y;
			}

			// Check NEG_Y (bit 4, 3 shifts.)
			else if (((indicatorData >> 2) & 1) == 1)
			{
				returnOrientation = BoundaryOrientation::NEG_Y;
			}


			return returnOrientation;
		}

		std::string getPrintableIndicatorValue()
		{
			std::string returnString = "BoundaryOrientation::NONE";

			/*
			switch (indicatorData)
			{
				case 128: { returnString = "BoundaryOrientation::NEG_Z"; break;}
				case 64: { returnString = "BoundaryOrientation::POS_X"; break;}
				case 32: { returnString = "BoundaryOrientation::POS_Z"; break;}
				case 16: { returnString = "BoundaryOrientation::NEG_X"; break;}
				case 8: { returnString = "BoundaryOrientation::POS_Y"; break;}
				case 4: { returnString = "BoundaryOrientation::NEG_Y"; break;}
			}
			*/

			// Check NEG_Z (bit 8, 7 shifts.)
			if (((indicatorData >> 7) & 1) == 1)
			{
				returnString = "BoundaryOrientation::NEG_Z";
			}

			// Check POS_X (bit 7, 6 shifts.)
			else if (((indicatorData >> 6) & 1) == 1)
			{
				returnString = "BoundaryOrientation::POS_X";
			}

			// Check POS_Z (bit 6, 5 shifts.)
			else if (((indicatorData >> 5) & 1) == 1)
			{
				returnString = "BoundaryOrientation::POS_Z";
			}

			// Check NEG_X (bit 5, 4 shifts.)
			else if (((indicatorData >> 4) & 1) == 1)
			{
				returnString = "BoundaryOrientation::NEG_X";
			}

			// Check POS_Y (bit 4, 3 shifts.)
			else if (((indicatorData >> 3) & 1) == 1)
			{
				returnString = "BoundaryOrientation::POS_Y";
			}

			// Check NEG_Y (bit 4, 3 shifts.)
			else if (((indicatorData >> 2) & 1) == 1)
			{
				returnString = "BoundaryOrientation::NEG_Y";
			}

			// Print the SCAB_PARENT and SCAB_CHILD flags if they are set.

			// SCAB_PARENT
			if (((indicatorData >> 1) & 1) == 1)
			{
				returnString += " | SCAB_PARENT ";
			}

			// SCAB_CHILD
			if ((indicatorData & 1) == 1)
			{
				returnString += " | SCAB_CHILD ";
			}

			return returnString;
		}
	private:
		unsigned char indicatorData = 0;
};

#endif
