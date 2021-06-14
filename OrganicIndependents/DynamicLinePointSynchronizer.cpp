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

	float dimLimitIncremental = dimLimit / 100;	// increment by 1% every failed pass

	// first pass, .9999f
	float highBarrier = dimLimit - 0.0001f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;				// this value must be 1 for a valid return value

	// std::cout << "!!! Base point is: " << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << std::endl;

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
			//std::cout << "high bar z hit. " << std::endl;
			pointToReturn.z = dimLimit;
			matchCount++;
		}
		else if (pointToReturn.z < lowBarrier)
		{
			//std::cout << "low bar z hit. " << std::endl;
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
			bool foundY = false;
			bool foundZ = false;
			//float dimLimitDifference = .001f;
			//float dimLimitAdd = .001f;

			float dimLimitDifference = dimLimitIncremental;
			float dimLimitAdd = dimLimitIncremental;

			std::cout << "!!! trying else branch. " << std::endl;
			std::cout << "!!! dimLimit is: " << dimLimit << std::endl;
			std::cout << "::: point to return: " << pointToReturn.x << ", " << pointToReturn.y << ", " << pointToReturn.z << std::endl;

			while (matchCount != 1)
			{

				highBarrier = dimLimit - dimLimitDifference;	// high barrier = dimLimit - current value of dimLimitDifference
				lowBarrier = dimLimitDifference;
				//matchCount = 0;
				// check for y
				if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
				{
					//std::cout << " y > high " << std::endl;
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

				// iterate the dimLimitDifference.
				dimLimitDifference += dimLimitAdd;

				//std::cout << "while loop wait: " << std::endl;
				//std::cout << "high barrier was: " << highBarrier << std::endl;
				//std::cout << "low barrier was: " << lowBarrier << std::endl;
				//std::cout << "matchCount value is now: " << matchCount << std::endl;
				//int whileWait = 3;
				//std::cin >> whileWait;
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

DynamicLineSyncResult DynamicLinePointSynchronizer::syncToX()
{
	DynamicLineSyncResult xResult;
	ECBPolyPoint pointToReturn = basePoint;	// return value -- starts as base 
	float dimLimitIncremental = dimLimit / 100;	// increment by 1% every failed pass

	// first pass, .9999f
	float highBarrier = dimLimit - 0.0001f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;				// this value must be 1 for a valid return value

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
		//std::cout << "high bar z hit. " << std::endl;
		pointToReturn.z = dimLimit;
		matchCount++;
	}
	else if (pointToReturn.z < lowBarrier)
	{
		//std::cout << "low bar z hit. " << std::endl;
		pointToReturn.z = 0.0f;
		matchCount++;
	}

	// did it match? if so, return.
	if (matchCount == 1)
	{
		//return pointToReturn;
		xResult.syncPoint = pointToReturn;
	}	// if it did not...try this
	else
	{
		bool foundY = false;
		bool foundZ = false;
		//float dimLimitDifference = .001f;
		//float dimLimitAdd = .001f;

		float dimLimitDifference = dimLimitIncremental;
		float dimLimitAdd = dimLimitIncremental;

		std::cout << "!!! trying else branch. " << std::endl;
		std::cout << "!!! dimLimit is: " << dimLimit << std::endl;
		std::cout << "::: point to return: " << pointToReturn.x << ", " << pointToReturn.y << ", " << pointToReturn.z << std::endl;

		while (matchCount != 1)
		{

			highBarrier = dimLimit - dimLimitDifference;	// high barrier = dimLimit - current value of dimLimitDifference
			lowBarrier = dimLimitDifference;
			//matchCount = 0;
			// check for y
			if (pointToReturn.y > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				//std::cout << " y > high " << std::endl;
				pointToReturn.y = dimLimit;
				matchCount++;
				foundY = true;
			}
			else if (pointToReturn.y < lowBarrier)
			{
				pointToReturn.y = 0.0f;
				matchCount++;
				foundY = true;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.z = dimLimit;
				matchCount++;
				foundZ = true;
			}
			else if (pointToReturn.z < lowBarrier)
			{
				pointToReturn.z = 0.0f;
				matchCount++;
				foundZ = true;
			}

			// iterate the dimLimitDifference.
			dimLimitDifference += dimLimitAdd;

			//std::cout << "while loop wait: " << std::endl;
			//std::cout << "high barrier was: " << highBarrier << std::endl;
			//std::cout << "low barrier was: " << lowBarrier << std::endl;
			//std::cout << "matchCount value is now: " << matchCount << std::endl;
			//int whileWait = 3;
			//std::cin >> whileWait;
		}

		if
		(
			(foundY == true)
			&&
			(foundZ == true)
		)
		{
			xResult.syncType = DynamicLineSyncType::CORNER;
		}
		else if
		(
			(foundY == true)
			||
			(foundZ == true)
		)
		{
			xResult.syncType = DynamicLineSyncType::LINE;
		}
		xResult.syncPoint = pointToReturn;

		//return pointToReturn;
	}

	return xResult;
}

DynamicLineSyncResult DynamicLinePointSynchronizer::syncToY()
{
	DynamicLineSyncResult yResult;
	ECBPolyPoint pointToReturn = basePoint;	// return value -- starts as base 
	float dimLimitIncremental = dimLimit / 100;	// increment by 1% every failed pass

	// first pass, .9999f
	float highBarrier = dimLimit - 0.0001f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;
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
		yResult.syncPoint = pointToReturn;
		//return pointToReturn;
	}
	else
	{
		bool foundX = false;
		bool foundZ = false;

		float dimLimitDifference = dimLimitIncremental;
		float dimLimitAdd = dimLimitIncremental;

		std::cout << "!!! trying else branch. " << std::endl;
		std::cout << "!!! dimLimit is: " << dimLimit << std::endl;
		std::cout << "::: point to return: " << pointToReturn.x << ", " << pointToReturn.y << ", " << pointToReturn.z << std::endl;

		while (matchCount != 1)
		{

			highBarrier = dimLimit - dimLimitDifference;	// high barrier = dimLimit - current value of dimLimitDifference
			lowBarrier = dimLimitDifference;
			//matchCount = 0;
			// check for y
			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				//std::cout << " y > high " << std::endl;
				pointToReturn.x = dimLimit;
				matchCount++;
				foundX = true;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
				foundX = true;
			}

			// check for z
			if (pointToReturn.z > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.z = dimLimit;
				matchCount++;
				foundZ = true;
			}
			else if (pointToReturn.z < lowBarrier)
			{
				pointToReturn.z = 0.0f;
				matchCount++;
				foundZ = true;
			}

			// iterate the dimLimitDifference.
			dimLimitDifference += dimLimitAdd;

			//std::cout << "while loop wait: " << std::endl;
			//std::cout << "high barrier was: " << highBarrier << std::endl;
			//std::cout << "low barrier was: " << lowBarrier << std::endl;
			//std::cout << "matchCount value is now: " << matchCount << std::endl;
			//int whileWait = 3;
			//std::cin >> whileWait;
			}

			if
			(
				(foundX == true)
				&&
				(foundZ == true)
			)
			{
				yResult.syncType = DynamicLineSyncType::CORNER;
			}
			else if
			(
				(foundX == true)
				||
				(foundZ == true)
			)
			{
				yResult.syncType = DynamicLineSyncType::LINE;
			}
			yResult.syncPoint = pointToReturn;
		//return pointToReturn;
	}

	return yResult;
}

DynamicLineSyncResult DynamicLinePointSynchronizer::syncToZ()
{
	DynamicLineSyncResult zResult;
	ECBPolyPoint pointToReturn = basePoint;	// return value -- starts as base 
	float dimLimitIncremental = dimLimit / 100;	// increment by 1% every failed pass

	// first pass, .9999f
	float highBarrier = dimLimit - 0.0001f;
	float lowBarrier = 0.0001f;
	int matchCount = 0;
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
		zResult.syncPoint = pointToReturn;
		//return pointToReturn;
	}
	else
	{
		bool foundX = false;
		bool foundY = false;

		float dimLimitDifference = dimLimitIncremental;
		float dimLimitAdd = dimLimitIncremental;

		//std::cout << "!!! trying else branch. " << std::endl;
		//std::cout << "!!! dimLimit is: " << dimLimit << std::endl;
		//std::cout << "::: point to return: " << pointToReturn.x << ", " << pointToReturn.y << ", " << pointToReturn.z << std::endl;

		while (matchCount != 1)
		{

			highBarrier = dimLimit - dimLimitDifference;	// high barrier = dimLimit - current value of dimLimitDifference
			lowBarrier = dimLimitDifference;
			//matchCount = 0;
			// check for y
			if (pointToReturn.x > highBarrier)	// is y greater than 0.5f? if yes, then round to dimLimit
			{
				//std::cout << " y > high " << std::endl;
				pointToReturn.x = dimLimit;
				matchCount++;
				foundX = true;
			}
			else if (pointToReturn.x < lowBarrier)
			{
				pointToReturn.x = 0.0f;
				matchCount++;
				foundX = true;
			}

			// check for z
			if (pointToReturn.y > highBarrier) // is z greater than 0.5f? if yes, then round to dimLimit
			{
				pointToReturn.y = dimLimit;
				matchCount++;
				foundY = true;
			}
			else if (pointToReturn.y < lowBarrier)
			{
				pointToReturn.y = 0.0f;
				matchCount++;
				foundY = true;
			}

			// iterate the dimLimitDifference.
			dimLimitDifference += dimLimitAdd;

			//std::cout << "while loop wait: " << std::endl;
			//std::cout << "high barrier was: " << highBarrier << std::endl;
			//std::cout << "low barrier was: " << lowBarrier << std::endl;
			//std::cout << "matchCount value is now: " << matchCount << std::endl;
			//int whileWait = 3;
			//std::cin >> whileWait;
		}

		if
		(
			(foundX == true)
			&&
			(foundY == true)
		)
		{
			zResult.syncType = DynamicLineSyncType::CORNER;
		}
		else if
		(
			(foundX == true)
			||
			(foundY == true)
		)
		{
			zResult.syncType = DynamicLineSyncType::LINE;
		}
		zResult.syncPoint = pointToReturn;
		//return pointToReturn;
	}
	return zResult;
}
