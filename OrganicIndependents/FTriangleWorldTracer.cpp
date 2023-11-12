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

	//localizedStartPoint = IndependentUtils::roundPolyPointToHundredths(localizedStartPoint + invertingPointValue);
	//localizedEndPoint = IndependentUtils::roundPolyPointToHundredths(localizedEndPoint + invertingPointValue);

	localizedStartPoint.point = IndependentUtils::roundDoublePointToHundredeths(localizedStartPoint.point + invertingPointValue);
	localizedEndPoint.point = IndependentUtils::roundDoublePointToHundredeths(localizedEndPoint.point + invertingPointValue);

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

	FIntersectMeta resultantIntersect = FTriangleUtils::findIntersectionDataV2(currentIterationEndpoint,
																			endPoint,
																			currentKey,
																			endKey,
																			lineBoundingBox, 
																			FTraceType::BLUEPRINT_TRACE);


	//std::cout << "--Resultant intersect at traverseLineOnce: " << resultantIntersect.intersectedPoint.x << ", " << resultantIntersect.intersectedPoint.y << ", " << resultantIntersect.intersectedPoint.z << std::endl;
	nextKeyAdd = resultantIntersect.incrementingKey;
	currentIterationBeginPoint = currentIterationEndpoint;			// set the begin point to be the previous end point
	//currentIterationEndpoint = FTriangleUtils::convertDoubleToECBPolyPoint(resultantIntersect.intersectedPoint.point); // set the new end point
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

		// Before the LineLocalizer is initialized, check to see if we need to re-orient (swap the points/keys) of the line we're looking at.
		// The easiest way to do this, is to get copies of the data we will use, and then swap those.
		EnclaveKeyDef::EnclaveKey tracingKeyA = tracingLineKeypairs[x].keyA;
		EnclaveKeyDef::EnclaveKey tracingKeyB = tracingLineKeypairs[x].keyB;
		//ECBPolyPoint tracingPointA = fTrianglePoints[beginPointIndex];
		//ECBPolyPoint tracingPointB = fTrianglePoints[endPointIndex];
		FTrianglePoint tracingPointA = fTrianglePoints[beginPointIndex];
		FTrianglePoint tracingPointB = fTrianglePoints[endPointIndex];

		// Remember, to guarantee 100% similiar tracing, we need to make sure that the two points 
		// consisting of a line either run as AB or BA (never both); which one to use is dependent on whichever
		// one is "positively oriented." If AB is not positively oriented, BA must be, so all we will have to do is swap the data, via
		// a call to swapValues below.
		//
		// Call the re-orientation check function here; 
		// Test for required swapping; if the return value is false -- which would indicate that the line isn't positively oriented --
		// then we need to postively orient the line, which is done by swapping the points and keys.
		if 
		(
			!FTriangleUtils::isLinePositivelyOriented(
				FTriangleUtils::convertDoubleToECBPolyPoint(tracingPointA.point), 
				FTriangleUtils::convertDoubleToECBPolyPoint(tracingPointB.point))
		)
		{
			swapKeysAndPoints(&tracingKeyA, &tracingKeyB, &tracingPointA, &tracingPointB);
		}

		// The values to use for the currentTracer, will have to come from the LineLocalizer.
		LineLocalizer newLocalizer(tracingKeyA,
									tracingKeyB,
									tracingPointA,
									tracingPointB);

		
		// The WorldLineTracer needs to be populated with the localized values produced by the LineLocalizer instance
		// that was created in the previous line.

		//WorldLineTracer currentTracer(newLocalizer.localizedBeginKey,
		//	newLocalizer.localizedEndKey,
		//	newLocalizer.localizedStartPoint,
		//	newLocalizer.localizedEndPoint);

		WorldLineTracer currentTracer(newLocalizer.localizedBeginKey,
									  newLocalizer.localizedEndKey,
									  newLocalizer.localizedStartPoint,
									  newLocalizer.localizedEndPoint);
		
		// we must always enter the below while loop at least once,
		// to put in at least one FTriangleLine. Therefore, we will check if the run is complete once
		// we are already in the while loop, and not before. If checkIfRunComplete does return true
		// on the very first iteration, then we will at least put set of data in for an FTriangleLine.
		int currentLineIteration = 0;
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


			//ECBPolyPoint revertedBeginPoint = newLocalizer.getRevertedPoint(currentBeginPoint);
			//ECBPolyPoint revertedEndPoint = newLocalizer.getRevertedPoint(currentEndPoint);
			ECBPolyPoint revertedBeginPoint = newLocalizer.getRevertedPoint(FTriangleUtils::convertDoubleToECBPolyPoint(currentBeginPoint.point));
			ECBPolyPoint revertedEndPoint = newLocalizer.getRevertedPoint(FTriangleUtils::convertDoubleToECBPolyPoint(currentEndPoint.point));

			// UV for begin point: only attempt an insert if we are on the very iteration; otherwise, proceed as normal.
			if (currentLineIteration == 0)
			{
				uniquePointsContainerRef->insertFTrianglePointAndFlagAsUVOrigin(FTrianglePoint(revertedBeginPoint, FTrianglePointType::EXTERIOR),
																							  currentTracer.beginPoint.fTextureU,
																							  currentTracer.beginPoint.fTextureV);
			}
			else
			{
				uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedBeginPoint, FTrianglePointType::EXTERIOR));
			}

			// UV for end point: only attempt to do this if we have confirmed this is the last iteration (i.e, currentTracer.shouldTraceStop == true)
			if (currentTracer.shouldTraceStop)
			{
				uniquePointsContainerRef->insertFTrianglePointAndFlagAsUVOrigin(FTrianglePoint(revertedEndPoint, FTrianglePointType::EXTERIOR),
																				currentTracer.endPoint.fTextureU,
																				currentTracer.endPoint.fTextureV);
			}
			else
			{
				uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedEndPoint, FTrianglePointType::EXTERIOR));
			}



			//uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedBeginPoint, FTrianglePointType::EXTERIOR));
			//uniquePointsContainerRef->insertFTrianglePoint(FTrianglePoint(revertedEndPoint, FTrianglePointType::EXTERIOR));
			
			//FTriangleLine newRevertedLine(revertedBeginPoint, revertedEndPoint, FTriangleLineType::EXTERIOR);

			FTrianglePoint testPoint(revertedBeginPoint, FTrianglePointType::EXTERIOR);

			FTriangleLine newRevertedLine(
											FTrianglePoint(DoublePoint(revertedBeginPoint), FTrianglePointType::EXTERIOR),
											FTrianglePoint(DoublePoint(revertedEndPoint), FTrianglePointType::EXTERIOR), 
											FTriangleLineType::EXTERIOR);

			// The key to insert into currentCandidateAffectedKeys is equal to the reverted key returned by newLocalizer.
			currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(currentTracerKey));

			// Remember, this class is tracing from the world grid (WorldFracturingMachine) into the blueprint grid, so use BLUEPRINT_TRACE
			// The limits should be equal to -> 32.0 * the EnclaveKey (curentTracerKey)
			FTraceBorderValues currentBlueprintTracingLimits = FTriangleUtils::getCurrentTracingLimits(currentTracerKey, FTraceType::BLUEPRINT_TRACE);

			// FTriangleLine instances which are perfectly aligned to a grid line, must be inserted into both sections that the line borders on those dimension(s).
			// I.e, a line with points 32,1.5,0 and 32,2.5,4 is perfectly aligned to X, and in a WorldLineTracer this would need to exist at 
			// the keys 0,0,0, and 1,0,0, since the X = 4.0f sits between both of those keys. In this scenario, the positive-side key is
			// always inserted by default, so we must manually enter the negative value.
			//
			// Remember, the EnclaveKeyDef set to use for this operation is equivalent to what is produced by calling an instance of
			// FTriangleKeySetCalibrator, constructed with an FTriangleType::WORLD value, and using FKeyCalibrationMode::FTRIANGLE_LINE when calling
			// calibrate(). The values will be stored in the FTriangleFracturerBase::scannerKeys[] array.

			// Check for grid-line alignment for X
			float moduloXpointA = fmod(currentBeginPoint.point.x, 32.0f);
			float moduloXpointB = fmod(currentEndPoint.point.x, 32.0f);
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
				if (currentBeginPoint.point.x == currentBlueprintTracingLimits.posXlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveXKey(currentTracerKey.x + 1, currentTracerKey.y, currentTracerKey.z);
					//(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeXKey)].insertLine(newRevertedLine);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(positiveXKey));
				}

				// If the alignment is at the negative X limit, insert a key at x - 1.
				else if (currentBeginPoint.point.x == currentBlueprintTracingLimits.negXlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeXKey(currentTracerKey.x - 1, currentTracerKey.y, currentTracerKey.z);
					//(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeXKey)].insertLine(newRevertedLine);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(negativeXKey));
				}
			}

			// Check for grid-line alignment for Y
			float moduloYpointA = fmod(currentBeginPoint.point.y, 32.0f);
			float moduloYpointB = fmod(currentEndPoint.point.y, 32.0f);
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
				if (currentBeginPoint.point.y == currentBlueprintTracingLimits.posYlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveYKey(currentTracerKey.x, currentTracerKey.y + 1, currentTracerKey.z);
					//(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeYKey)].insertLine(newRevertedLine);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(positiveYKey));
				}

				// If the alignment is at the negative Y limit, insert a key at y - 1.
				else if (currentBeginPoint.point.y == currentBlueprintTracingLimits.negYlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeYKey(currentTracerKey.x, currentTracerKey.y - 1, currentTracerKey.z);
					//(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeYKey)].insertLine(newRevertedLine);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(negativeYKey));
				}
			}

			// Check for grid-line alignment for Z
			float moduloZpointA = fmod(currentBeginPoint.point.z, 32.0f);
			float moduloZpointB = fmod(currentEndPoint.point.z, 32.0f);
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
				if (currentBeginPoint.point.z == currentBlueprintTracingLimits.posZlimit)
				{
					EnclaveKeyDef::EnclaveKey positiveZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z + 1);
					//(*tracerStagerRef)[newLocalizer.getRevertedKey(negativeZKey)].insertLine(newRevertedLine);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(positiveZKey));
				}

				// If the alignment is at the negative Z limit, insert a key at z - 1.
				else if (currentBeginPoint.point.z == currentBlueprintTracingLimits.negZlimit)
				{
					EnclaveKeyDef::EnclaveKey negativeZKey(currentTracerKey.x, currentTracerKey.y, currentTracerKey.z - 1);
					currentCandidateAffectedKeys.insert(newLocalizer.getRevertedKey(negativeZKey));
				}
			}
			
			// put the candidate data, into a new TracerLineRecord, and push that record back, but only if the points don't match
			// (yes, this can happen, and this is a lazy fix impelmented around 3/17/2023)
			if (newRevertedLine.pointA.point != newRevertedLine.pointB.point)
			{
				TracerLineRecord newRecord(newRevertedLine, currentCandidateAffectedKeys);
				lineCandidates.push_back(newRecord);
			}

			currentTracer.traverseLineOnce();

			currentLineIteration++;
		}

		// once the current line has been finished, run the application logic. The application logic 
		// will take into account whether or not the 
		runCandidateApplicationLogic();

		//std::cout << "Completed traversal of line " << x << std::endl;
	}
}