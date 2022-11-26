#include "stdafx.h"
#include "FTriangleWorldTracer.h"
#include "FTrianglePoint.h"

void FTriangleWorldTracer::runLineTracing()
{
	for (int x = 0; x < 3; x++)
	{
		int beginPointIndex = 0;
		int endPointIndex = 0;

		switch(x)
		{
			case 0: { beginPointIndex = 0; endPointIndex = 1; break;}
			case 1: { beginPointIndex = 1; endPointIndex = 2; break;}
			case 2: { beginPointIndex = 2; endPointIndex = 0; break;}
		}

		//TracingLineBoundingBox currentTracerBoundingBox(tracingLineKeypairs[x].keyA,
		//											    tracingLineKeypairs[x].keyB);
		WorldLineTracer currentTracer(tracingLineKeypairs[x].keyA,
											tracingLineKeypairs[x].keyB,
											fTrianglePoints[beginPointIndex],
											fTrianglePoints[endPointIndex]);

		// we must always enter the below while loop at least once,
		// to put in at least one FTriangleLine. Therefore, we will check if the run is complete once
		// we are already in the while loop, and not before. If checkIfRunComplete does return true
		// on the very first iteration, then we will at least put set of data in for an FTriangleLine.
		while (currentTracer.shouldTraceStop == false)
		{
			currentTracer.checkIfRunComplete();

			// do stuff here
			// ...
			// ...
			// ...
			// get the currentKey, the currentIterationBeginPoint, and currentIterationEndpoint, to construct a new
			// FTriangleLine (having the FTriangleLineType::EXTERIOR type) and put it into the referenced FTriangleProductionStager map.
			// Ensure that the new points have been inserted into the UniquePointContainer.
			auto currentTracerKey = currentTracer.currentKey;
			auto currentBeginPoint = currentTracer.currentIterationBeginPoint;
			auto currentEndPoint = currentTracer.currentIterationEndpoint;

			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentBeginPoint, FTrianglePointType::EXTERIOR));
			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentEndPoint, FTrianglePointType::EXTERIOR));
			FTriangleLine newExteriorLine(currentBeginPoint, currentEndPoint, FTriangleLineType::EXTERIOR);

			

			std::cout << "(FTriangleWorldTracer): inserting line with points: A -> ";
			currentBeginPoint.printPointCoords();
			std::cout << " | B -> ";
			currentEndPoint.printPointCoords();
			std::cout << std::endl;

			(*tracerStagerRef)[currentTracerKey].insertLine(newExteriorLine);

			// check for additional key adjustments, for x/y/z, when the line is perfectly clamped to any of those.
			// For X
			float moduloXpointA = fmod(currentBeginPoint.x, 32.0f);
			float moduloXpointB = fmod(currentEndPoint.x, 32.0f);
			if 
			(
				(moduloXpointA == 0.0f)
				&&
				(moduloXpointB == 0.0f)
				&&
				(currentBeginPoint.x == currentEndPoint.x)
			)
			{
				EnclaveKeyDef::EnclaveKey negativeXKey(currentTracerKey.x - 1, currentTracerKey.y, currentTracerKey.z);
				(*tracerStagerRef)[negativeXKey].insertLine(newExteriorLine);
			}

			// For Y
			float moduloYpointA = fmod(currentBeginPoint.y, 32.0f);
			float moduloYpointB = fmod(currentEndPoint.y, 32.0f);
			if
			(
				(moduloYpointA == 0.0f)
				&&
				(moduloYpointB == 0.0f)
				&&
				(currentBeginPoint.y == currentEndPoint.y)
			)
			{
				EnclaveKeyDef::EnclaveKey negativeYKey(currentTracerKey.x, currentTracerKey.y -1, currentTracerKey.z);
				(*tracerStagerRef)[negativeYKey].insertLine(newExteriorLine);
			}

			// For Z
			float moduloZpointA = fmod(currentBeginPoint.z, 32.0f);
			float moduloZpointB = fmod(currentEndPoint.z, 32.0f);
			if 
			(
				(moduloZpointA == 0.0f)
				&&
				(moduloZpointB == 0.0f)
				&&
				(currentBeginPoint.z == currentEndPoint.z)
			)
			{
				/*
				std::cout << "!! Alert: found matching Z values; points are:" << std::endl;
				currentBeginPoint.printPointCoords();
				std::cout << std::endl;
				currentEndPoint.printPointCoords();
				std::cout << std::endl;

				std::cout << "!! currentTracerKey is: ";
				currentTracerKey.printKey();
				std::cout << std::endl;

				int waitVal = 3;
				std::cin >> waitVal;
				*/

				EnclaveKeyDef::EnclaveKey negativeZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z - 1);
				(*tracerStagerRef)[negativeZKey].insertLine(newExteriorLine);
			}

			currentTracer.traverseLineOnce();
		}
		std::cout << "Completed traversal of line " << x << std::endl;
	}
}