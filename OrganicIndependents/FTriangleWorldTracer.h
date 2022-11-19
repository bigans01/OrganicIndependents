#pragma once

#ifndef FTRIANGLEWORLDTRACER_H
#define FTRIANGLEWORLDTRACER_H

#include "FTriangleTracerBase.h"
#include "TracingLineBoundingBox.h"

/*

Description:

Used by the fractuing class, WorldFracturingMachine, this class is designed to trace an FTriangles lines through WORLD space,
in order to produce FTriangles that would exist in BLUEPRINT space. The lines (FTriangleLines) produced by the WorldLineTracer struct below
are fed into the stager map referenced by tracerStagerRef (see base class for details). The calling WorldFracturingMachine will 
then analyze the contents of its stagerMap member, to determine what to do. In other words, this class simply produces exterior 
line segments for an FTriangle, and feeds the results back into the stager map; the raycasted points are not done here nor should
they exist in the UniquePointContainer used by the base class (that comes later).

*/

class FTriangleWorldTracer : public FTriangleTracerBase
{
	public:
		void runLineTracing();
	private:

		// A struct that traces the lines of triangle that exists in World space. It is loosely based on the
		// OSTriangleLineTraverserBare class from OrganicServerLib.
		struct WorldLineTracer
		{
			WorldLineTracer(EnclaveKeyDef::EnclaveKey in_beginKey,
				EnclaveKeyDef::EnclaveKey in_endKey,
				ECBPolyPoint in_beginPoint,
				ECBPolyPoint in_endPoint) :
					beginKey(in_beginKey),
					currentKey(in_beginKey),
					endKey(in_endKey),
					beginPoint(in_beginPoint),
					endPoint(in_endPoint)
			{
				// NEW: the FTriangleWorldTracer should use the advanced blueprint tracing function, findCBIv2 (this function uses a tracing bounding box
				// for safety against cases where the incrementing key goes out of the bounds of the keys that represent the points of the line).
				// This feature will probably eventually be extended to other points of the overall Organic libraries, but it is being tested here for now.
				TracingLineBoundingBox bbToUse(beginKey, endKey);
				lineBoundingBox = bbToUse;
				ECBIntersectMeta resultantIntersect = IndependentUtils::findCBIv2(beginPoint, 
																				  endPoint, 
																				  beginKey, 
																				  endKey,
																				lineBoundingBox);	// do the initial set up; beginKey will be replaced by currentKey in later function calls
				nextKeyAdd = resultantIntersect.incrementingKey;					// the next key add will be a result from previous function call
				currentIterationBeginPoint = beginPoint;							// set the initial value of the begin point
				currentIterationEndpoint = resultantIntersect.intersectedPoint;		// set the incrementing point
			};

			EnclaveKeyDef::EnclaveKey beginKey;		// the beginning key = the key that point A lies in
			EnclaveKeyDef::EnclaveKey currentKey;	// the current key value
			EnclaveKeyDef::EnclaveKey nextKeyAdd;	// how much will be added to currentKey in the next iteration
			EnclaveKeyDef::EnclaveKey endKey;		// the ending key = the key that point B lies in (will be done when this happens)

			ECBPolyPoint beginPoint;					// equals point A of line
			ECBPolyPoint currentIterationBeginPoint;	// equals whatever the begin point of the line is
			ECBPolyPoint currentIterationEndpoint;		// equals whatever the point is when this line hits an ECB border
			ECBPolyPoint endPoint;						//	 equals point B of line

			TracingLineBoundingBox lineBoundingBox;		// must be set by constructor function

			bool shouldTraceStop = false;

			// check if the run is complete
			bool checkIfRunComplete()
			{
				bool isComplete = false;
				if (currentKey == endKey)
				{
					isComplete = true;
					shouldTraceStop = true;
				}
				return isComplete;
			}

			// used to traverse the line one time
			void traverseLineOnce()
			{
				//std::cout << "currentKey Value: " << currentKey.x << ", " << currentKey.y << ", " << currentKey.z << std::endl;	// test output only
				//std::cout << "nextKeyAdd Value: " << nextKeyAdd.x << ", " << nextKeyAdd.y << ", " << nextKeyAdd.z << std::endl;
				currentKey += nextKeyAdd;
				//std::cout << "########## Calling blueprint intersection (traverseLineOnce)" << std::endl;
				ECBIntersectMeta resultantIntersect = IndependentUtils::findCBIv2(currentIterationEndpoint,
																										endPoint, 
																										currentKey, 
																										endKey,
																										lineBoundingBox);
				//std::cout << "--Resultant intersect at traverseLineOnce: " << resultantIntersect.intersectedPoint.x << ", " << resultantIntersect.intersectedPoint.y << ", " << resultantIntersect.intersectedPoint.z << std::endl;
				nextKeyAdd = resultantIntersect.incrementingKey;
				currentIterationBeginPoint = currentIterationEndpoint;			// set the begin point to be the previous end point
				currentIterationEndpoint = resultantIntersect.intersectedPoint; // set the new end point
			}
		};
};

#endif
