#include "stdafx.h"
#include "FTriangleBlueprintTracer.h"

bool FTriangleBlueprintTracer::BlueprintLineTracer::checkIfRunComplete()
{
	bool isComplete = false;
	if (currentKey == endKey)
	{
		isComplete = true;
		shouldTraceStop = true;
	}
	return isComplete;
}

void FTriangleBlueprintTracer::BlueprintLineTracer::traverseLineOnce()
{
	//std::cout << "currentKey Value: " << currentKey.x << ", " << currentKey.y << ", " << currentKey.z << std::endl;	// test output only
	//std::cout << "nextKeyAdd Value: " << nextKeyAdd.x << ", " << nextKeyAdd.y << ", " << nextKeyAdd.z << std::endl;
	currentKey += nextKeyAdd;
	//std::cout << "########## Calling blueprint intersection (traverseLineOnce)" << std::endl;

	FIntersectMeta resultantIntersect = FTriangleUtils::findIntersectionData(currentIterationEndpoint,
		endPoint,
		currentKey,
		endKey,
		lineBoundingBox,
		FTraceType::ORE_TRACE);


	//std::cout << "--Resultant intersect at traverseLineOnce: " << resultantIntersect.intersectedPoint.x << ", " << resultantIntersect.intersectedPoint.y << ", " << resultantIntersect.intersectedPoint.z << std::endl;
	nextKeyAdd = resultantIntersect.incrementingKey;
	currentIterationBeginPoint = currentIterationEndpoint;			// set the begin point to be the previous end point
	currentIterationEndpoint = resultantIntersect.intersectedPoint; // set the new end point
}








void FTriangleBlueprintTracer::runLineTracing()
{
	for (int x = 0; x < 3; x++)
	{
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
		ECBPolyPoint tracingPointA = fTrianglePoints[beginPointIndex];
		ECBPolyPoint tracingPointB = fTrianglePoints[endPointIndex];

		// Remember, to guarantee 100% similiar tracing, we need to make sure that the two points 
		// consisting of a line either run as AB or BA (never both); which one to use is dependent on whichever
		// one is "positively oriented." If AB is not positively oriented, BA must be, so all we will have to do is swap the data, via
		// a call to swapValues below.
		//
		// Call the re-orientation check function here; 
		// Test for required swapping; if the return value is false -- which would indicate that the line isn't positively oriented --
		// then we need to postively orient the line, which is done by swapping the points and keys.
		if (!FTriangleUtils::isLinePositivelyOriented(tracingPointA, tracingPointB))
		{
			swapValues(&tracingKeyA, &tracingKeyB, &tracingPointA, &tracingPointB);
		}

		// Localization shouldn't be needed; the FTriangleBlueprintTracer class assumes that the X/Y/Z values of any point it is operating on
		// is within the range of 0.0f  to 32.0f.
		BlueprintLineTracer currentTracer(tracingKeyA,
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

			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentBeginPoint, FTrianglePointType::EXTERIOR));
			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(currentEndPoint, FTrianglePointType::EXTERIOR));
			FTriangleLine newTriangleLine(currentBeginPoint, currentEndPoint, FTriangleLineType::EXTERIOR);

			// Remember: we must always insert the exterior line at least once, before the special
			// logic that follows is done.
			currentCandidateAffectedKeys.insert(currentTracerKey);


			// FTriangleLine instances which are perfectly aligned to a grid line, must be inserted into both sections that the line borders on those dimension(s).
			// I.e, a line with points 4,1.5,0 and 4,2.5,4 is perfectly aligned to X, and in a BlueprintLineTracer this would need to exist at 
			// the keys 0,0,0, and 1,0,0, since the X = 4.0f sits between both of those keys. In this scenario, the positive-side key is
			// always inserted by default, so we must manually enter the negative value.
			//
			// Remember, the EnclaveKeyDef set to use for this operation is equivalent to what is produced by calling an instance of
			// FTriangleKeySetCalibrator, constructed with an FTriangleType::BLUEPRINT value, and using FKeyCalibrationMode::FTRIANGLE_LINE when calling
			// calibrate(). The values will be stored in the FTriangleFracturerBase::scannerKeys[] array.

			// For X
			float moduloXpointA = fmod(currentBeginPoint.x, 4.0f);
			float moduloXpointB = fmod(currentEndPoint.x, 4.0f);
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
				currentCandidateAffectedKeys.insert(negativeXKey);
			}

			// For Y
			float moduloYpointA = fmod(currentBeginPoint.y, 4.0f);
			float moduloYpointB = fmod(currentEndPoint.y, 4.0f);
			if
			(
				(moduloYpointA == 0.0f)
				&&
				(moduloYpointB == 0.0f)
				&&
				(currentBeginPoint.y == currentEndPoint.y)
			)
			{
				EnclaveKeyDef::EnclaveKey negativeYKey(currentTracerKey.x, currentTracerKey.y - 1, currentTracerKey.z);
				currentCandidateAffectedKeys.insert(negativeYKey);
			}

			// For Z
			float moduloZpointA = fmod(currentBeginPoint.z, 4.0f);
			float moduloZpointB = fmod(currentEndPoint.z, 4.0f);
			if
			(
				(moduloZpointA == 0.0f)
				&&
				(moduloZpointB == 0.0f)
				&&
				(currentBeginPoint.z == currentEndPoint.z)
			)
			{
				EnclaveKeyDef::EnclaveKey negativeZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z - 1);
				currentCandidateAffectedKeys.insert(negativeZKey);
			}

			// put the candidate data, into a new TracerLineRecord, and push that record back.
			TracerLineRecord newRecord(newTriangleLine, currentCandidateAffectedKeys);
			lineCandidates.push_back(newRecord);


			currentTracer.traverseLineOnce();
		}

		// once the current line has been finished, run the application logic. The application logic 
		// will take into account whether or not the 
		runCandidateApplicationLogic();

		//std::cout << "Completed traversal of line " << x << std::endl;
	}
}