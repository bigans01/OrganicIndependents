#include "stdafx.h"
#include "FTriangleORETracer.h"


bool FTriangleORETracer::ORELineTracer::checkIfRunComplete()
{
	bool isComplete = false;
	if (currentKey == endKey)
	{
		isComplete = true;
		shouldTraceStop = true;
	}
	return isComplete;
}

void FTriangleORETracer::ORELineTracer::traverseLineOnce()
{
	//std::cout << "currentKey Value: " << currentKey.x << ", " << currentKey.y << ", " << currentKey.z << std::endl;	// test output only
	//std::cout << "nextKeyAdd Value: " << nextKeyAdd.x << ", " << nextKeyAdd.y << ", " << nextKeyAdd.z << std::endl;
	currentKey += nextKeyAdd;
	//std::cout << "########## Calling blueprint intersection (traverseLineOnce)" << std::endl;

	FIntersectMeta resultantIntersect = FTriangleUtils::findIntersectionDataV2(
		currentIterationEndpoint,
		endPoint,
		currentKey,
		endKey,
		lineBoundingBox,
		FTraceType::BLOCK_TRACE);


	//std::cout << "--Resultant intersect at traverseLineOnce: " << resultantIntersect.intersectedPoint.x << ", " << resultantIntersect.intersectedPoint.y << ", " << resultantIntersect.intersectedPoint.z << std::endl;
	nextKeyAdd = resultantIntersect.incrementingKey;
	currentIterationBeginPoint = currentIterationEndpoint;			// set the begin point to be the previous end point
	currentIterationEndpoint = resultantIntersect.intersectedPoint; // set the new end point
}

void FTriangleORETracer::runLineTracing()
{
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "(FTriangleORETracer): running line tracing, for line at index " << x << std::endl;

		int beginPointIndex = 0;
		int endPointIndex = 0;

		switch (x)
		{
			case 0: { beginPointIndex = 0; endPointIndex = 1; break;}
			case 1: { beginPointIndex = 1; endPointIndex = 2; break;}
			case 2: { beginPointIndex = 2; endPointIndex = 0; break;}
		}

		// Before the LineLocalizer is initialized, check to see if we need to re-orient (swap the points/keys) of the line we're looking at.
		// The easiest way to do this, is to get copies of the data we will use, and then swap those.
		EnclaveKeyDef::EnclaveKey tracingKeyA = tracingLineKeypairs[x].keyA;
		EnclaveKeyDef::EnclaveKey tracingKeyB = tracingLineKeypairs[x].keyB;
		FTrianglePoint tracingPointA = fTrianglePoints[beginPointIndex];
		FTrianglePoint tracingPointB = fTrianglePoints[endPointIndex];

		//std::cout << "!! Initial tracingKeyA: "; tracingKeyA.printKey(); std::cout << std::endl;
		//std::cout << "!! Initial tracingKeyB: "; tracingKeyB.printKey(); std::cout << std::endl;

		//std::cout << "!! Initial tracingPointA: " ; tracingPointA.printPointCoords(); std::cout << std::endl;
		//std::cout << "!! Initial tracingPointB: " ; tracingPointB.printPointCoords(); std::cout << std::endl;

		// Remember, to guarantee 100% similiar tracing, we need to make sure that the two points 
		// consisting of a line either run as AB or BA (never both); which one to use is dependent on whichever
		// one is "positively oriented." If AB is not positively oriented, BA must be, so all we will have to do is swap the data, via
		// a call to swapValues below.
		//
		// Call the re-orientation check function here; 
		// Test for required swapping; if the return value is false -- which would indicate that the line isn't positively oriented --
		// then we need to postively orient the line, which is done by swapping the points and keys.
		//if (!FTriangleUtils::isLinePositivelyOriented(tracingPointA, tracingPointB))
		if 
		(
			!FTriangleUtils::isLinePositivelyOriented(FTriangleUtils::convertDoubleToECBPolyPoint(tracingPointA.point), 
													  FTriangleUtils::convertDoubleToECBPolyPoint(tracingPointB.point))
		)
		{
			swapKeysAndPoints(&tracingKeyA, &tracingKeyB, &tracingPointA, &tracingPointB);
		}

		// Localization shouldn't be needed; the FTriangleBlueprintTracer class assumes that the X/Y/Z values of any point it is operating on
		// is within the range of 0.0f  to 32.0f.
		ORELineTracer currentTracer(tracingKeyA,
			tracingKeyB,
			tracingPointA,
			tracingPointB);

		// we must always enter the below while loop at least once,
		// to put in at least one FTriangleLine. Therefore, we will check if the run is complete once
		// we are already in the while loop, and not before. If checkIfRunComplete does return true
		// on the very first iteration, then we will at least put set of data in for an FTriangleLine.
		while (currentTracer.shouldTraceStop == false)
		{
			currentTracer.checkIfRunComplete();

			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> currentCandidateAffectedKeys;


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

			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentBeginPoint.point, FTrianglePointType::EXTERIOR));
			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentEndPoint.point, FTrianglePointType::EXTERIOR));
			FTriangleLine newTriangleLine(currentBeginPoint, currentEndPoint, FTriangleLineType::EXTERIOR);

			// Remember: we must always insert the exterior line at least once, before the special
			// logic that follows is done.
			currentCandidateAffectedKeys.insert(currentTracerKey);

			//FTraceBorderValues FTriangleUtils::getCurrentTracingLimits(EnclaveKeyDef::EnclaveKey in_currentTracingKey, FTraceType in_fTraceType)

			// Remember, this class is traces from the ORE grid (OREFracturingMachine) into a block grid, so use BLOCK_TRACE
			// The limits should be equal to -> 1.0 * the EnclaveKey (curentTracerKey)
			FTraceBorderValues currentBlockTracingLimits = FTriangleUtils::getCurrentTracingLimits(currentTracerKey, FTraceType::BLOCK_TRACE);


			// FTriangleLine instances which are perfectly aligned to a grid line, must be inserted into both sections that the line borders on those dimension(s).
			// I.e, a line with points 4,1.5,0 and 4,2.5,4 is perfectly aligned to X, and in a BlueprintLineTracer this would need to exist at 
			// the keys 0,0,0, and 1,0,0, since the X = 4.0f sits between both of those keys. In this scenario, the positive-side key is
			// always inserted by default, so we must manually enter the negative value.
			//
			// Remember, the EnclaveKeyDef set to use for this operation is equivalent to what is produced by calling an instance of
			// FTriangleKeySetCalibrator, constructed with an FTriangleType::BLUEPRINT value, and using FKeyCalibrationMode::FTRIANGLE_LINE when calling
			// calibrate(). The values will be stored in the FTriangleFracturerBase::scannerKeys[] array.

			// Check for grid-line alignment for X
			float moduloXpointA = fmod(currentBeginPoint.point.x, 1.0f);
			float moduloXpointB = fmod(currentEndPoint.point.x, 1.0f);
			if
			(
				(moduloXpointA == 0.0f)
				&&
				(moduloXpointB == 0.0f)
				&&
				(currentBeginPoint.point.x == currentEndPoint.point.x)
			)
			{
				// If the alignment is at the positive X limit, insert a key at x + 1.
				if (currentBeginPoint.point.x == currentBlockTracingLimits.posXlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveXKey(currentTracerKey.x + 1, currentTracerKey.y, currentTracerKey.z);
					currentCandidateAffectedKeys.insert(positiveXKey);
				}

				// If the alignment is at the negative X limit, insert a key at x - 1.
				else if (currentBeginPoint.point.x == currentBlockTracingLimits.negXlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeXKey(currentTracerKey.x - 1, currentTracerKey.y, currentTracerKey.z);
					currentCandidateAffectedKeys.insert(negativeXKey);
				}
			}

			// Check for grid-line alignment for Y
			float moduloYpointA = fmod(currentBeginPoint.point.y, 1.0f);
			float moduloYpointB = fmod(currentEndPoint.point.y, 1.0f);
			if
			(
				(moduloYpointA == 0.0f)
				&&
				(moduloYpointB == 0.0f)
				&&
				(currentBeginPoint.point.y == currentEndPoint.point.y)
			)
			{
				// If the alignment is at the positive Y limit, insert a key at y + 1.
				if (currentBeginPoint.point.y == currentBlockTracingLimits.posYlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveYKey(currentTracerKey.x, currentTracerKey.y + 1, currentTracerKey.z);
					currentCandidateAffectedKeys.insert(positiveYKey);
				}

				// If the alignment is at the negative Y limit, insert a key at y - 1.
				else if (currentBeginPoint.point.y == currentBlockTracingLimits.negYlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeYKey(currentTracerKey.x, currentTracerKey.y - 1, currentTracerKey.z);
					currentCandidateAffectedKeys.insert(negativeYKey);
				}
			}

			// Check for grid-line alignment for Z
			float moduloZpointA = fmod(currentBeginPoint.point.z, 1.0f);
			float moduloZpointB = fmod(currentEndPoint.point.z, 1.0f);
			if
			(
				(moduloZpointA == 0.0f)
				&&
				(moduloZpointB == 0.0f)
				&&
				(currentBeginPoint.point.z == currentEndPoint.point.z)
			)
			{
				// If the alignment is at the positive Z limit, insert a key at z + 1.
				if (currentBeginPoint.point.z == currentBlockTracingLimits.posZlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z + 1);
					currentCandidateAffectedKeys.insert(positiveZKey);
				}

				// If the alignment is at the negative Z limit, insert a key at z - 1.
				else if (currentBeginPoint.point.z == currentBlockTracingLimits.negZlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z - 1);
					currentCandidateAffectedKeys.insert(negativeZKey);
				}
			}


			// put the candidate data, into a new TracerLineRecord, and push that record back, but only if the points don't match
			// (yes, this can happen, and this is a lazy fix impelmented around 3/17/2023)
			if (newTriangleLine.pointA.point != newTriangleLine.pointB.point)
			{
				//std::cout << "(FTriangleORETracer) -> inserting exterior line, with points: ";
				//newTriangleLine.printLine();
				TracerLineRecord newRecord(newTriangleLine, currentCandidateAffectedKeys);
				lineCandidates.push_back(newRecord);
			}

			currentTracer.traverseLineOnce();
		}

		// once the current line has been finished, run the application logic. The application logic 
		// will take into account whether or not the 
		runCandidateApplicationLogic();

		//std::cout << "Completed traversal of line " << x << std::endl;
	}
}