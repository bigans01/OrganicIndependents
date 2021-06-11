#include "stdafx.h"
#include "LinePointSynchronizer.h"

LinePointSynchronizer::LinePointSynchronizer(ECBPolyPoint in_pointToSync, int in_coordToSyncTo)
{
	basePoint = in_pointToSync;
	coordToSyncTo = in_coordToSyncTo;
}

ECBPolyPoint LinePointSynchronizer::sync()
{
	ECBPolyPoint pointToReturn = basePoint;	// return value -- starts as base 
	// first pass, .9999f
	float highBarrier = 0.9999f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;				// this value must be 1 for a valid return value

	// -------------------------------------> X CHECKS
	if (coordToSyncTo == 0)		// sync to x; meaning x is either 0 or 1. We must check if .9999f/.0001f is valid for rounding either the y or z coordinate.
	{
		// check for y
		if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.y = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.y < lowBarrier)
		{
			pointToReturn.y = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.z = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.z < lowBarrier)
		{
			pointToReturn.z = 0.0f;
			matchCount++;
		}

		// did it match? if so, return.
		if (matchCount == 1)
		{
			return pointToReturn;
		}	// if it did not...try this
		else
		{
			highBarrier = .99f;
			lowBarrier = .01f;
			matchCount = 0;
			// check for y
			if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.y = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.y < lowBarrier)
			{
				pointToReturn.y = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.z = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.z < lowBarrier)
			{
				pointToReturn.z = 0.0f;
				matchCount++;
			}
			return pointToReturn;
		}
	}

	// -------------------------------------> Y CHECKS
	else if (coordToSyncTo == 1) // sync to x;
	{
		// check for x
		if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.x = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.x < lowBarrier)
		{
			pointToReturn.x = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.z = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.z < lowBarrier)
		{
			pointToReturn.z = 0.0f;
			matchCount++;
		}

		// did it match? if so, return.
		if (matchCount == 1)
		{
			return pointToReturn;
		}
		else
		{
			highBarrier = .99f;
			lowBarrier = .01f;
			matchCount = 0;

			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.x = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.z = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.z < lowBarrier)
			{
				pointToReturn.z = 0.0f;
				matchCount++;
			}
			return pointToReturn;
		}
	}

	// -------------------------------------> Z CHECKS
	else if (coordToSyncTo == 2)
	{
		// check for x
		if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.x = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.x < lowBarrier)
		{
			pointToReturn.x = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.y > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
		{
			pointToReturn.y = 1.0f;
			matchCount++;
		}
		else if (pointToReturn.y < lowBarrier)
		{
			pointToReturn.y = 0.0f;
			matchCount++;
		}

		// did it match? if so, return.
		if (matchCount == 1)
		{
			return pointToReturn;
		}
		else
		{
			highBarrier = .99f;
			lowBarrier = .01f;
			matchCount = 0;

			// check for x
			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.x = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.y > highBarrier) // is z greater than 0.5f? if yes, then round to 1.0f
			{
				pointToReturn.y = 1.0f;
				matchCount++;
			}
			else if (pointToReturn.y < lowBarrier)
			{
				pointToReturn.y = 0.0f;
				matchCount++;
			}

			return pointToReturn;
		}
	}

	return pointToReturn;
}