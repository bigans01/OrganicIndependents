#include "stdafx.h"
#include "FTriangleWorldTracer.h"

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

			uniquePointsContainerRef->insertPoint(currentBeginPoint);
			uniquePointsContainerRef->insertPoint(currentEndPoint);
			FTriangleLine newExteriorLine(currentBeginPoint, currentEndPoint, FTriangleLineType::EXTERIOR);
			(*tracerStagerRef)[currentTracerKey].insertLine(newExteriorLine);

			currentTracer.traverseLineOnce();
		}
		std::cout << "Completed traversal of line " << x << std::endl;
	}
}