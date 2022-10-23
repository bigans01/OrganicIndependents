#pragma once

#ifndef TRACINGLINEBOUNDINGBOX_H
#define TRACINGLINEBOUNDINGBOX_H

#include "EnclaveKeyDef.h"
#include <set>
#include <iostream>

/*

Description:

There are some edge cases where the result of adding an incrementing key to a current point key, extends beyond the limits of what it should actually be
(see the edge case description in the case of 2022_4_005, in the OrganicSystem_Tasks google doc). An easy way to prevent this is to use a 3D bounding box,
which is a simple calculated by using the min/max of the EnclaveKeys that correspond to the points. 

The bounding box only needs to be constructed one time: before the line is actually traced, when the original keys of both points that constitute the line are known.
This instance can then be subsequently used in all tracing calls, such as in the case with the FTriangleWorldTracer class.

Doing this should ensure that the line terminates completely, and that there is no infinite while loop that occurs while tracing the line in world space.

*/

class TracingLineBoundingBox
{
	public:
		TracingLineBoundingBox() {};	// needed because we'll have to copy the contents across functions once it's been made

		// Below: this is the function that is used to setup the bounding box
		TracingLineBoundingBox(EnclaveKeyDef::EnclaveKey in_boundingLimitA,	
							   EnclaveKeyDef::EnclaveKey in_boundingLimitB)
		{
			xLimits.insert(in_boundingLimitA.x);
			xLimits.insert(in_boundingLimitB.x);

			yLimits.insert(in_boundingLimitA.y);
			yLimits.insert(in_boundingLimitB.y);

			zLimits.insert(in_boundingLimitA.z);
			zLimits.insert(in_boundingLimitB.z);
		};

		// Below: this is the function that actually runs the proposed key (formed by combinining in_keyToIncrement and in_incrementingKey);
		// The value of in_keyToIncrement is the key we will be incrementing. The in_incrementingKey value is the value calculated by
		// the movement meta calculation function, such as findBBMv2.
		// 
		// If any dimensions of the futureIncrementedKey fall out of the bounds of the box, the same dimension in the key that was added to 
		// futureIncrementingKey (which would be candidateIncrementingKey below), is also subtracted from the finalIncrementingKey. The
		// return value of this function should be finalIncrementingKey, where it's values are within the bounds of the bounding box that was 
		// formed.
		EnclaveKeyDef::EnclaveKey applyBoundingCorrectionToKeys(EnclaveKeyDef::EnclaveKey in_keyToIncrement,
																EnclaveKeyDef::EnclaveKey in_incrementingKey)
		{
			EnclaveKeyDef::EnclaveKey correctedIncrementingKey;
			EnclaveKeyDef::EnclaveKey candidateIncrementingKey = in_incrementingKey;
			
			EnclaveKeyDef::EnclaveKey futureIncrementedKey = in_keyToIncrement;
			futureIncrementedKey += candidateIncrementingKey;
			EnclaveKeyDef::EnclaveKey originalIntendedKey = futureIncrementedKey;	// this is the intended key before any alterations/tests were made.

			// Remember, for all sets, the min value should be the least bounds (begin), the max value should be the most bounding (rbegin)
			// (std::set automatically sorts least to greatest for us)


			// Use the future incremented key, before any corrections, to see if it would go out of the bounds.

			// Check X: 
			int leastX = *xLimits.begin();
			int mostX = *xLimits.rbegin();
			bool withinXLimits = true;	// assume it will be within the limits.
			if
			(
				!(futureIncrementedKey.x >= leastX)
				||
				!(futureIncrementedKey.x <= mostX)
			)
			{
				withinXLimits = false;
			}

			// Check Y:
			int leastY = *yLimits.begin();
			int mostY = *yLimits.rbegin();
			bool withinYLimits = true;
			if
			(
				!(futureIncrementedKey.y >= leastY)
				||
				!(futureIncrementedKey.y <= mostY)
			)
			{
				withinYLimits = false;
			}

			// Check Z:
			int leastZ = *zLimits.begin();
			int mostZ = *zLimits.rbegin();
			bool withinZLimits = true;
			if
			(
				!(futureIncrementedKey.z >= leastZ)
				||
				!(futureIncrementedKey.z <= mostZ)
			)
			{
				withinZLimits = false;
			}

			// Get a copy of the original intended incrementing key.
			auto finalIncrementingKey = candidateIncrementingKey;

			// If X was out of limits, subtract the original X value we applied.
			if (withinXLimits == false)
			{
				//std::cout << "(TracingLineBoundingBox): had to decrement x bounds. " << std::endl;
				futureIncrementedKey.x -= candidateIncrementingKey.x;
				finalIncrementingKey.x -= finalIncrementingKey.x;
			}

			// ... do the same for y
			if (withinYLimits == false)
			{
				//std::cout << "(TracingLineBoundingBox): had to decrement y bounds. " << std::endl;
				futureIncrementedKey.y -= candidateIncrementingKey.y;
				finalIncrementingKey.y -= finalIncrementingKey.y;
			}

			// ... do the same for z
			if (withinZLimits == false)
			{
				//std::cout << "(TracingLineBoundingBox): had to decrement z bounds. " << std::endl;
				futureIncrementedKey.z -= candidateIncrementingKey.z;
				finalIncrementingKey.z -= finalIncrementingKey.z;
			}

			// Below: These lines are for debug only.
			//std::cout << "Original intended incrementing key key was: " << candidateIncrementingKey.x << ", " << candidateIncrementingKey.y << ", " << candidateIncrementingKey.z << std::endl;
			//std::cout << "Original intended value of key was: " << originalIntendedKey.x << ", " << originalIntendedKey.y << ", " << originalIntendedKey.z << std::endl;

			//std::cout << "Final value of incrementing key is: " << finalIncrementingKey.x << ", " << finalIncrementingKey.y << ", " << finalIncrementingKey.z << std::endl;
			//std::cout << "Final value of key is: " << futureIncrementedKey.x << ", " << futureIncrementedKey.y << ", " << futureIncrementedKey.z << std::endl;

			return finalIncrementingKey;
		}
		
	private:
		std::set<int> xLimits;
		std::set<int> yLimits;
		std::set<int> zLimits;
};

#endif
