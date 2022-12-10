#include "stdafx.h"
#include "FTriangleWorldTracer.h"
#include "FTrianglePoint.h"

void FTriangleWorldTracer::LineLocalizer::determineValues()
{
	std::set<int> keyedXValues;
	std::set<int> keyedYValues;
	std::set<int> keyedZValues;

	keyedXValues.insert(localizedBeginKey.x);
	keyedYValues.insert(localizedBeginKey.y);
	keyedZValues.insert(localizedBeginKey.z);

	keyedXValues.insert(localizedEndKey.x);
	keyedYValues.insert(localizedEndKey.y);
	keyedZValues.insert(localizedEndKey.z);

	// create an EnclaveKey that has the min values (just use the begin of each set, since the minimum value is there.)
	EnclaveKeyDef::EnclaveKey minimumKey(*keyedXValues.begin(), *keyedYValues.begin(), *keyedZValues.begin());

	// fetch the inverted key; this will be the value of the translationKey.
	invertingKey = minimumKey.getInvertedKey();

	ECBPolyPoint pointAdjustmentValue(invertingKey.x*32.0f, invertingKey.y*32.0f, invertingKey.z*32.0f);
	invertingPointValue = IndependentUtils::roundPolyPointToHundredths(pointAdjustmentValue);

	// apply the inverted values
	localizedBeginKey += invertingKey;
	localizedEndKey += invertingKey;

	localizedStartPoint = IndependentUtils::roundPolyPointToHundredths(localizedStartPoint + invertingPointValue);
	localizedEndPoint = IndependentUtils::roundPolyPointToHundredths(localizedEndPoint + invertingPointValue);

}

ECBPolyPoint FTriangleWorldTracer::LineLocalizer::getRevertedPoint(ECBPolyPoint in_pointToRevert)
{
	ECBPolyPoint revertedValue = invertingPointValue * -1;
	return IndependentUtils::roundPolyPointToHundredths(in_pointToRevert + revertedValue);
	return revertedValue;
}

EnclaveKeyDef::EnclaveKey FTriangleWorldTracer::LineLocalizer::getRevertedKey(EnclaveKeyDef::EnclaveKey in_keyToApplyTo)
{
	return in_keyToApplyTo + (invertingKey.getInvertedKey());
}









bool FTriangleWorldTracer::WorldLineTracer::checkIfRunComplete()
{
	bool isComplete = false;
	if (currentKey == endKey)
	{
		isComplete = true;
		shouldTraceStop = true;
	}
	return isComplete;
}

void FTriangleWorldTracer::WorldLineTracer::traverseLineOnce()
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

		// The values to use for the currentTracer, will have to come from the LineLocalizer.
		
		LineLocalizer newLocalizer(tracingLineKeypairs[x].keyA,
									tracingLineKeypairs[x].keyB,
									fTrianglePoints[beginPointIndex],
									fTrianglePoints[endPointIndex]);

		WorldLineTracer currentTracer(newLocalizer.localizedBeginKey,
									  newLocalizer.localizedEndKey,
									  newLocalizer.localizedStartPoint,
									  newLocalizer.localizedEndPoint);
		
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


			ECBPolyPoint revertedBeginPoint = newLocalizer.getRevertedPoint(currentBeginPoint);
			ECBPolyPoint revertedEndPoint = newLocalizer.getRevertedPoint(currentEndPoint);
			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedBeginPoint, FTrianglePointType::EXTERIOR));
			uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedEndPoint, FTrianglePointType::EXTERIOR));
			FTriangleLine newRevertedLine(revertedBeginPoint, revertedEndPoint, FTriangleLineType::EXTERIOR);
			(*tracerStagerRef)[newLocalizer.getRevertedKey(currentTracerKey)].insertLine(newRevertedLine);

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
				(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeXKey)].insertLine(newRevertedLine);
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
				EnclaveKeyDef::EnclaveKey negativeYKey(currentTracerKey.x, currentTracerKey.y - 1, currentTracerKey.z);
				(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeYKey)].insertLine(newRevertedLine);
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
				EnclaveKeyDef::EnclaveKey negativeZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z - 1);
				(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeZKey)].insertLine(newRevertedLine);
			}
			
			currentTracer.traverseLineOnce();
		}
		std::cout << "Completed traversal of line " << x << std::endl;
	}
}