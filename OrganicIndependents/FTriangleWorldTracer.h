#pragma once

#ifndef FTRIANGLEWORLDTRACER_H
#define FTRIANGLEWORLDTRACER_H

#include "FTriangleTracerBase.h"
#include "TracingLineBoundingBox.h"

/*

Description:

Used by the fracturing class, WorldFracturingMachine, this class is designed to trace an FTriangles lines through WORLD space,
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
		
		// This struct is designed to localize a line as close to 0,0,0 as can possibly be; using this will ensure that
		// a line consisting of two points, but has been translated due to the function WorldFracturingMachine::translateTriangleByBlueprintKeys().
		//
		// Because WorldFracturingMachine::translateTriangleByBlueprintKeys() translates by the minimum key values of a FTriangle, lines can get translated differently.
		// For example, consider FTriangle (F1) with points A (0,0,0), B (128, 16, 0) C  (128, 16, 32)
		// when compared to a FTriangle (F2) with points A (96,0,0), B (128, 16, 0), C (128, 16, 32).
		//
		// The line B->C for both F1 and F2 gets translated differently. 
		//
		// For F1, the B->C line will actually stay at  B (128, 16, 0) C (128, 16, 32); this is because point A is 0,0,0 -- which is the minimum of everything,
		// so no translation gets done, when calling WorldFracturingMachine::translateTriangleByBlueprintKeys(). However, the line would get translated
		// by (-4,0,0) when determineValues below is called.
		//
		// For F2, the B->C line will get translated by -96 (or EnclaveKey (-3, 0, 0); so the points become B(32, 16, 0) and C(32, 16, 32), 
		// when calling WorldFracturingMachine::translateTriangleByBlueprintKeys(). When determine values is called, the key will get further translated 
		// by another (-1,0,0) -- making the combined translation amount equal to (-4,0,0), like in F1 above. 
		//
		// In this way, we have the guarantee that B>C from both F1 and F2 will always produce the same results.
		struct LineLocalizer
		{
			LineLocalizer(EnclaveKeyDef::EnclaveKey in_beginKey,
						EnclaveKeyDef::EnclaveKey in_endKey,
						ECBPolyPoint in_beginPoint,
						ECBPolyPoint in_endPoint) :
					localizedBeginKey(in_beginKey),
					localizedEndKey(in_endKey),
					localizedStartPoint(in_beginPoint),
					localizedEndPoint(in_endPoint)
			{
				determineValues();
			};

			// Below: values that store the localized versions of keys and points, respectively,
			// once determineValues() has been called.
			EnclaveKeyDef::EnclaveKey localizedBeginKey;
			EnclaveKeyDef::EnclaveKey localizedEndKey;
			ECBPolyPoint localizedStartPoint;
			ECBPolyPoint localizedEndPoint;

			// The determined inverting values formed by the call to determineValues().
			EnclaveKeyDef::EnclaveKey invertingKey;
			ECBPolyPoint invertingPointValue;

			void determineValues();	// the function that determines the localized points, keys, and inversion values.
			ECBPolyPoint getRevertedPoint(ECBPolyPoint in_pointToRevert);	// calculates and returns the would-be reverted value for a point.
			EnclaveKeyDef::EnclaveKey getRevertedKey(EnclaveKeyDef::EnclaveKey in_keyToApplyTo);	// calculates and returns the would-be reverted value for a key.
		};

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
			bool checkIfRunComplete();
			void traverseLineOnce();	// used to traverse the line one time
		};
};

#endif
