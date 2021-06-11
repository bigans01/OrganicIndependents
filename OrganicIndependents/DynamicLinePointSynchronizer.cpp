#include "stdafx.h"
#include "DynamicLinePointSynchronizer.h"

DynamicLinePointSynchronizer::DynamicLinePointSynchronizer(ECBPolyPoint in_pointToSync, int in_coordToSyncTo, float in_dimLength)
{
	basePoint = in_pointToSync;
	coordToSyncTo = in_coordToSyncTo;
	dimLimit = in_dimLength;
}

ECBPolyPoint DynamicLinePointSynchronizer::sync()
{
	ECBPolyPoint pointToReturn = basePoint;	// return value -- starts as base 
	// first pass, .9999f
	float highBarrier = dimLimit - 0.0001f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;				// this value must be 1 for a valid return value

	// -------------------------------------> X CHECKS
	if (coordToSyncTo == 0)		// sync to x; meaning x is either 0 or 1. We must check if .9999f/.0001f is valid for rounding either the y or z coordinate.
	{
		// check for y
		if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.y = dimLimit;
			matchCount++;
		}
		else if (pointToReturn.y < lowBarrier)
		{
			pointToReturn.y = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.z = dimLimit;
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
			highBarrier = dimLimit - .001f;
			lowBarrier = .001f;
			matchCount = 0;
			// check for y
			if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.y = dimLimit;
				matchCount++;
			}
			else if (pointToReturn.y < lowBarrier)
			{
				pointToReturn.y = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.z = dimLimit;
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
		if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.x = dimLimit;
			matchCount++;
		}
		else if (pointToReturn.x < lowBarrier)
		{
			pointToReturn.x = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.z = dimLimit;
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
			highBarrier = dimLimit - .001f;
			lowBarrier = .001f;
			matchCount = 0;

			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.x = dimLimit;
				matchCount++;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.z = dimLimit;
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
		if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.x = dimLimit;
			matchCount++;
		}
		else if (pointToReturn.x < lowBarrier)
		{
			pointToReturn.x = 0.0f;
			matchCount++;
		}

		// check for z
		if (pointToReturn.y > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
		{
			pointToReturn.y = dimLimit;
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
			highBarrier = dimLimit - .001f;
			lowBarrier = .001f;
			matchCount = 0;

			// check for x
			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.x = dimLimit;
				matchCount++;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
			}

			// check for z
			if (pointToReturn.y > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.y = dimLimit;
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