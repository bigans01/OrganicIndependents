#pragma once

#ifndef FTRIANGLEORETRACER_H
#define FTRIANGLEORETRACER_H

#include "FTriangleTracerBase.h"
#include "TracingLineBoundingBox.h"

/*

Description:

Used by the fracturing class, OREFracturingMachine, this class is designed to trace an FTriangle's lines through a singular ORE space,
in order to produce FTriangles that would exist in BLOCK space. The lines (FTriangleLines) produced by the ORELineTracer struct below
are fed into the stager map referenced by tracerStagerRef (see base class for details).

*/

class FTriangleORETracer : public FTriangleTracerBase
{
	public:
		void runLineTracing();

	private:

		// A struct that traces the lines of a triangle that exists in ORE space.
		struct ORELineTracer
		{
			ORELineTracer(EnclaveKeyDef::EnclaveKey in_beginKey,
				EnclaveKeyDef::EnclaveKey in_endKey,
				ECBPolyPoint in_beginPoint,
				ECBPolyPoint in_endPoint) :
				beginKey(in_beginKey),
				currentKey(in_beginKey),
				endKey(in_endKey),
				beginPoint(in_beginPoint),
				endPoint(in_endPoint)
			{
				TracingLineBoundingBox bbToUse(beginKey, endKey);
				lineBoundingBox = bbToUse;

				// As we are tracing a triangle that exists in ORE space into BLOCK space, we need to ensure that the bounds of each cell are that of a BLOCK (so 1.0f)
				//std::cout << "(ORELineTracer): begin point is: " << beginPoint.x << ", " << beginPoint.y << ", " << beginPoint.z << std::endl;
				FIntersectMeta resultantIntersect = FTriangleUtils::findIntersectionDataV2(beginPoint,
					endPoint,
					beginKey,
					endKey,
					lineBoundingBox,
					FTraceType::BLOCK_TRACE);

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