#include "stdafx.h"
#include "FTriangleContainer.h"

void FTriangleContainer::insertConstructionLines(std::vector<FTriangleLine> in_constructionLines)
{
	constructionLines = in_constructionLines;
}

void FTriangleContainer::printConstructionLines()
{
	std::cout << "Printing construction lines..." << std::endl;
	for (auto& currentLine : constructionLines)
	{
		currentLine.printLine();
	}
}

void FTriangleContainer::produceFTriangles(FTriangleType in_destinedTriangleType,
										ECBPolyPoint in_parentEmptyNormal,
										BoundaryOrientation in_parentOrientation,
										EnclaveKeyDef::EnclaveKey in_containerBounds,
										TriangleMaterial in_containerMaterial)
{
	// Below: a big triangle that is way too close to a grid's line border; should still run fine.
	
	/*
	FTriangleOutput testFTriangle(ECBPolyPoint(31.99f, 16.0f, -0.01f),
		ECBPolyPoint(112.0f, 48.0f, 0.0f),
		ECBPolyPoint(64.0f, 32.0f, 80.0f),
		in_parentTriangleType,
		ECBPolyPoint(0, 1.0f, 0.0f),
		BoundaryOrientation::NONE,
		PerfectClampEnum::NONE);

	fracturedTriangles[0] = testFTriangle;
	*/


	// the number of triangles to produce is always equal to the number of lines - 2.
	int totalTrianglesToProduce = constructionLines.size() - 2;

	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "Producing " << totalTrianglesToProduce  << " initial FTriangles for key at: ";
	in_containerBounds.printKey();
	std::cout << std::endl;
	*/

	//std::cout << "DEBUG: these lines are: " << std::endl;
	//printConstructionLines();

	// the root point will be point A of the very first line.
	DoublePoint rootPoint = constructionLines.begin()->pointA.point;

	// the auto iterator for the leading line will be at begin() + 1.
	auto leadingLineIter = constructionLines.begin();
	leadingLineIter++;

	for (int x = 0; x < totalTrianglesToProduce; x++)
	{
		OutputTriangleFrame newFrame(rootPoint, leadingLineIter->pointA.point, leadingLineIter->pointB.point);

		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "Created new frame, with points: " << std::endl;
		std::cout << "0: " << rootPoint.x << ", " << rootPoint.y << ", " << rootPoint.z << std::endl;
		std::cout << "1: " << leadingLineIter->pointA.x << ", " << leadingLineIter->pointA.y << ", " << leadingLineIter->pointA.z << std::endl;
		std::cout << "2: " << leadingLineIter->pointB.x << ", " << leadingLineIter->pointB.y << ", " << leadingLineIter->pointB.z << std::endl;
		*/

		// Form a new output, but swap the point order if necessary to auto-produce the 
		// correct normal value. 
		//
		// Remember, the normalsSwapped value neeeds to be set to true 
		// if we do swap points for the normal for any of the FTriangleOutput instances. This is because swapping the points
		// at triangle point indices 1 and 2 also means that we are reversing the original
		// intended triangle fan point order -- which was assumed to be set correctly prior to this call. When the points of the triangle fan
		// are reversed in this way, we will have to insert the FTriangleOutput
		// instances into the fracturedTriangles member in reverse order. (see the if/else block below)
		FTriangleOutput newOutput = formOutput(newFrame, 
											in_parentEmptyNormal, 
											in_destinedTriangleType, 
											in_containerBounds,
											in_containerMaterial);
		fracturedTriangles[fracturedTriangles.size()] = newOutput;

		leadingLineIter++;
	}

	// NEW TEST	(uncomment when needed)
	// 
	// This function will attempt to set the empty normals for each FTriangleOutput,
	// by utilizing their cross products (it will also attempt to swap the points if needed).
	// The goal is to have the unit-vectored cross products of the FOutputTriangle instances
	// be as close as possible to the unit-vectored empty normal of the parent triangle.
	checkForPointReversalAndSetNormals(in_parentEmptyNormal);


	// if we ever had to swap the normals at all, we will need to reverse the
	// fracturedTriangles map (this is so that we appropriately follow a true triangle fan)
	if (normalsSwapped)
	{
		// Below line is for DEBUG only
		//std::cout << "!!! NOTICE: normals were swapped; doing change for key at "; in_containerBounds.printKey(); std::cout << std::endl;

		// Copy the produced FTriangleOutput instances into reversedTriangles,
		// but do it in reverse order.
		std::map<int, FTriangleOutput> reversedTriangles;
		auto currentOutputsBegin = fracturedTriangles.rbegin();
		auto currentOutputsEnd = fracturedTriangles.rend();
		for (; currentOutputsBegin != currentOutputsEnd; currentOutputsBegin++)
		{
			reversedTriangles[reversedTriangles.size()] = currentOutputsBegin->second;
		}

		/*
		// Below block is for DEBUG only
		for (auto& currentReversed : reversedTriangles)
		{
			currentReversed.second.printOutputTrianglePoints();
		}
		*/

		fracturedTriangles = reversedTriangles;
	}
}

void FTriangleContainer::checkForPointReversalAndSetNormals(ECBPolyPoint in_parentEmptyNormal)
{
	// Step 1: Get the unit vector of the parent empty normal.
	glm::vec3 parentUnitVector = glm::normalize(glm::vec3(in_parentEmptyNormal.x, in_parentEmptyNormal.y, in_parentEmptyNormal.z));

	// Step 2: Check the cross products of all existing FTriangleOutput instances. If there is one case where the getReversedCrossProduct
	// is closest to the parentUnitVector, then the normalsSwapped flag needs to get set.
	bool reverseRequired = false;
	for (auto& currentFracturedTriangle : fracturedTriangles)
	{
		glm::vec3 originalUnitVector = glm::normalize(currentFracturedTriangle.second.getCurrentCrossProduct());
		glm::vec3 reversedUnitVector = glm::normalize(currentFracturedTriangle.second.getReversedCrossProduct());

		float distToOriginal = glm::distance(parentUnitVector, originalUnitVector);
		float distToReverse = glm::distance(parentUnitVector, reversedUnitVector);

		// If the reverse distance is closest to the parent unit vectorm, we must swap the points 
		if (distToReverse < distToOriginal)
		{
			reverseRequired = true;
			normalsSwapped = true;
			break;
		}
	}

	// If a reverse was required, we will need to reverse ALL the triangles
	if (reverseRequired)
	{
		for (auto& currentTriangleToReverse : fracturedTriangles)
		{
			currentTriangleToReverse.second.swapPointsForNormalAlignment();
		}
	}

	// Regardless of if we had to swap, use the normal contained within each triangle output.
	for (auto& currentTriangletoSetNormalFor : fracturedTriangles)
	{
		currentTriangletoSetNormalFor.second.setBuiltInEmptyNormal();
	}
}

void FTriangleContainer::printProducedFTriangles()
{
	for (auto& currentTriangle : fracturedTriangles)
	{
		currentTriangle.second.printOutputTrianglePoints();
	}
}

bool FTriangleContainer::areNormalsDirectionallyAligned(ECBPolyPoint in_normalA, ECBPolyPoint in_normalB)
{
	bool areNormalsAligned = false;
	ECBPolyPoint normalADirections = IndependentUtils::findNormalizedPoint(in_normalA);
	ECBPolyPoint normalBDirections = IndependentUtils::findNormalizedPoint(in_normalB);
	int signageMatchCounter = 0;

	// remember, if the counter is > 0, then the current point organization of the frame is OK, and we don't have to swap it.
	if
	(

		(
			(normalADirections.x == normalBDirections.x)
			&&
			(normalADirections.x != 0.0f)
		)

		||

		(
			(normalADirections.y == normalBDirections.y)
			&&
			(normalADirections.y != 0.0f)
		)

		||

		(
			(normalADirections.z == normalBDirections.z)
			&&
			(normalADirections.z != 0.0f)
		)
	)
	{
		signageMatchCounter++;
	}

	if (signageMatchCounter > 0)
	{
		//std::cout << "!! ---> No swap required." << std::endl;
		areNormalsAligned = true;
	}
	else if (signageMatchCounter == 0)
	{
		//std::cout << "!! Had to do point swap for normal change." << std::endl;
	}

	return areNormalsAligned;
}

FTriangleOutput FTriangleContainer::formOutput(OutputTriangleFrame in_triangleFrame,
	ECBPolyPoint in_parentEmptyNormal,
	FTriangleType in_typeForBoundingBasis,
	EnclaveKeyDef::EnclaveKey in_boundingKey, 
	TriangleMaterial in_outputMaterial)
{
	// Step 1: Finding the empty normal.
	//
	// Before forming the FTriangleOutput, the frame needs to have its points orientated in such a way that 
	// the normal produced by the points is directionally aligned to the parent's empty normal.

	// ||||||||||||||||| START OLD METHOD |||||
	//
	//	The "old method" is related to F-002 in the task tracker spreadsheet.
	//
	// Not 100& reliable, can cause rare cases where some triangles in the container, but others do not. 
	// (this is because of the signane matching but would drastically save data space because the normal
	// could be generated from the points alone.
	/*
	if (!areNormalsDirectionallyAligned(in_parentEmptyNormal,in_triangleFrame.getNormal()))
	{
		//std::cout << "!! Had to do point swap for normal change." << std::endl;
		in_triangleFrame.swapForNormalChange();
		normalsSwapped = true;
	}
	*/
	//auto finalNormal = in_triangleFrame.getNormal();
	// ||||||||||||||||| END OLD METHOD |||||

	// ||||||||||||||||| NEW METHOD (temp fix), just assign the final normal as the parent FTriangle's empty normal.
	// This is meant to be temporary, until a smarter fix is developed.
	auto finalNormal = in_parentEmptyNormal;

	//std::cout << "Calculated empty normal is: " << finalNormal.x << ", " << finalNormal.y << ", " << finalNormal.z << std::endl;

	// with the normal out of the way, find the other items required for the FOutputTriangle

	// 2. Find the BoundaryOrientation.
	BoundaryOrientation selectedOrientation = BoundaryOrientation::NONE;

	// 3. Find the perfect clamp value.
	PerfectClampEnum selectedPerfectClampValue = PerfectClampEnum::NONE;

	// 4. Form the FTriangleOutput, and then return it.
	FTriangleOutput outputTriangle(in_triangleFrame.framePoints[0],
		in_triangleFrame.framePoints[1],
		in_triangleFrame.framePoints[2],
		in_typeForBoundingBasis,
		finalNormal,
		selectedOrientation,
		selectedPerfectClampValue,
		in_outputMaterial);

	return outputTriangle;
}

bool FTriangleContainer::runBoundaryTests(FTriangleReverseTranslationMode in_reverseTranslationMode,
											EnclaveKeyDef::EnclaveKey in_boundingKey, 
											ECBPolyPoint in_parentEmptyNormal)
{
	// the folloiwing bool should be set to true, if there are no FTriangleOutput instances remaining in the container.
	bool shouldContainerBeDeleted = false;

	//std::cout << "!! Starting run of boundary tests. " << std::endl;

	// first, get the directional signs of the parent normal.
	ECBPolyPoint parentNormalDirections = IndependentUtils::findNormalizedPoint(in_parentEmptyNormal);

	// build the boundary to check against. We can just sample the destinedOutputType of the first FTriangleOutput value
	// to build this. The appropriate FTriangleType values can be BLUEPRINT, ORE, or BLOCK.
	FTriangleType sampledType = fracturedTriangles.begin()->second.destinedGridType;

	// Determine the container bounds, based on function parameters.
	//
	// NOTE:the value of the key to use for calculating the bounds will be based on the FTriangleReverseTranslationMode,
	// if the sampledType is BLUEPRINT.
	FTriangleContainerBounds bounds;
	switch (sampledType)
	{
		case FTriangleType::BLUEPRINT:
		{
			//std::cout << "!! Selecting BLUEPRINT bounds. " << std::endl;
			float boundaryInterval = 32.0f;
			EnclaveKeyDef::EnclaveKey blueprintBoundaryMultiplier;

			// if we're localized, the key is just going to be 0,0,0.
			if (in_reverseTranslationMode == FTriangleReverseTranslationMode::LOCALIZED_TRANSLATE)
			{
				blueprintBoundaryMultiplier = EnclaveKeyDef::EnclaveKey(0, 0, 0);
			}
			else if (in_reverseTranslationMode == FTriangleReverseTranslationMode::ABSOLUTE_TRANSLATE)
			{
				//std::cout << "selecting bounding mode for ABSOLUTE. " << std::endl;
				blueprintBoundaryMultiplier = in_boundingKey;
			}

			FTriangleContainerBounds decidedBounds(blueprintBoundaryMultiplier, boundaryInterval);
			bounds = decidedBounds;
			break;
		}

		// For ORE bounds, we always assume that the output is LOCALIZED; so just use a key of 0,0,0 and interval of 4.0f.
		case FTriangleType::ORE:
		{
			//std::cout << "!! Selecting ORE bounds. " << std::endl;
			float boundaryInterval = 4.0f;
			FTriangleContainerBounds decidedBounds(EnclaveKeyDef::EnclaveKey(0, 0, 0), boundaryInterval);
			bounds = decidedBounds;
			break;
		}

		// Fore BLOCK bounds,  we always assume that the output is LOCALIZED; so just use a key of 0,0,0 and interval of 1.0f.
		case FTriangleType::BLOCK:
		{
			float boundaryInterval = 1.0f;
			FTriangleContainerBounds decidedBounds(EnclaveKeyDef::EnclaveKey(0, 0, 0), boundaryInterval);
			bounds = decidedBounds;
			break;
		}
	}

	// Below: FTDEBUG (uncomment when needed)
	// bounds.printBounds();

	std::set<int> trianglesToRemove;
	for (auto& currentOutputTriangle : fracturedTriangles)
	{
		bool wasCurrentTriangleOrientated = bounds.runTriangleOrientationAnalysis(&currentOutputTriangle.second);

		// Below: FTDEBUG (uncomment when needed)
		/*
		if (wasCurrentTriangleOrientated)
		{
			std::cout << "!!!! NOTICE: triangle was orientated." << std::endl;
		}
		*/

		// if a BoundaryOrientation was detected, that Boundary must be compared against the emptyNormal of the parent FTriangle.
		// If these two values do not aligned, that corresponding FTriangleOutput will have to be purged (aka, mark its nonAlignedBoundaryDetected as true)
		if (wasCurrentTriangleOrientated)
		{
			bounds.runBoundaryToNormalAnalysis(&currentOutputTriangle.second, in_parentEmptyNormal);

			// if the current FTriangleOutput has an invalid alignment, it needs to be deleted.
			if (currentOutputTriangle.second.nonAlignedBoundaryDetected)
			{
				trianglesToRemove.insert(currentOutputTriangle.first);
			}
		}

		// lastly, run perfect clamp analysis.
		bounds.runPerfectClampAnalysis(&currentOutputTriangle.second);
	}

	// Remove any triangles that need to are not appropriately aligned.
	for (auto& removable : trianglesToRemove)
	{
		fracturedTriangles.erase(removable);
	}

	if (fracturedTriangles.size() == 0)
	{
		//std::cout << "!!! NOTICE: no output triangles contained in this instance of FTriangleContainer; it should be removed/deleted." << std::endl;
		shouldContainerBeDeleted = true;
	}

	//std::cout << "!! Ending run of boundary tests. " << std::endl;
	return shouldContainerBeDeleted;
}

FTriangleContainer::OutputTriangleFrame::OutputTriangleFrame()
{

}

FTriangleContainer::OutputTriangleFrame::OutputTriangleFrame(DoublePoint in_pointA,
	DoublePoint in_pointB,
	DoublePoint in_pointC)
{
	framePoints[0] = in_pointA;
	framePoints[1] = in_pointB;
	framePoints[2] = in_pointC;
}

ECBPolyPoint FTriangleContainer::OutputTriangleFrame::getNormal()
{
	DoublePoint doubleU = framePoints[1] - framePoints[0];
	DoublePoint doubleV = framePoints[2] - framePoints[0];
	glm::vec3 u(doubleU.x, doubleU.y, doubleU.z);
	glm::vec3 v(doubleV.x, doubleV.y, doubleV.z);
	glm::vec3 calcedNormal = cross(u, v);	// the normal of the triangle
	ECBPolyPoint N(calcedNormal.x, calcedNormal.y, calcedNormal.z);
	return N;
}

void FTriangleContainer::OutputTriangleFrame::swapForNormalChange()
{
	/*
	std::cout << "!! Pre-swap values: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		framePoints[x].printPointCoords();
		std::cout << std::endl;
	}
	*/
	DoublePoint point1Copy = framePoints[1];
	framePoints[1] = framePoints[2];
	framePoints[2] = point1Copy;
	/*
	std::cout << "!! Post-swap values: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		framePoints[x].printPointCoords();
		std::cout << std::endl;
	}
	*/
}

FTriangleContainer::FTriangleContainerBounds::FTriangleContainerBounds()
{

}

FTriangleContainer::FTriangleContainerBounds::FTriangleContainerBounds(EnclaveKeyDef::EnclaveKey in_boundingKey,
	float in_boundingInterval)
{
	boundsMinX = IndependentUtils::roundToHundredth(in_boundingKey.x * in_boundingInterval);
	boundsMaxX = IndependentUtils::roundToHundredth((in_boundingKey.x * in_boundingInterval) + in_boundingInterval);
	boundsMinY = IndependentUtils::roundToHundredth(in_boundingKey.y * in_boundingInterval);
	boundsMaxY = IndependentUtils::roundToHundredth((in_boundingKey.y * in_boundingInterval) + in_boundingInterval);
	boundsMinZ = IndependentUtils::roundToHundredth(in_boundingKey.z * in_boundingInterval);
	boundsMaxZ = IndependentUtils::roundToHundredth((in_boundingKey.z * in_boundingInterval) + in_boundingInterval);
};

bool FTriangleContainer::FTriangleContainerBounds::runTriangleOrientationAnalysis(FTriangleOutput* in_fTriangleOutputRef)
{
	bool wasTriangleOrientated = false;

	// Test for POS_X
	if 
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.x == boundsMaxX)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.x == boundsMaxX)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.x == boundsMaxX)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::POS_X;
		wasTriangleOrientated = true;
	}

	// Test for NEG_X
	else if
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.x == boundsMinX)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.x == boundsMinX)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.x == boundsMinX)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::NEG_X;
		wasTriangleOrientated = true;
	}

	// Test for POS_Y
	else if 
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.y == boundsMaxY)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.y == boundsMaxY)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.y == boundsMaxY)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::POS_Y;
		wasTriangleOrientated = true;
	}

	// Test for NEG_Y
	else if
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.y == boundsMinY)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.y == boundsMinY)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.y == boundsMinY)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::NEG_Y;
		wasTriangleOrientated = true;
	}

	// Test for POS_Z
	else if 
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.z == boundsMaxZ)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.z == boundsMaxZ)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.z == boundsMaxZ)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::POS_Z;
		wasTriangleOrientated = true;
	}

	// Test for NEG_Z
	else if 
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.z == boundsMinZ)
		&&
		(in_fTriangleOutputRef->fracturePoints[1].point.z == boundsMinZ)
		&&
		(in_fTriangleOutputRef->fracturePoints[2].point.z == boundsMinZ)
	)
	{
		in_fTriangleOutputRef->fractureRequiredOrientation = BoundaryOrientation::NEG_Z;
		wasTriangleOrientated = true;
	}

	return wasTriangleOrientated;
}

void FTriangleContainer::FTriangleContainerBounds::runBoundaryToNormalAnalysis(FTriangleOutput* in_fTriangleOutputRef, ECBPolyPoint in_parentEmptyNormal)
{
	switch (in_fTriangleOutputRef->fractureRequiredOrientation)
	{
		case BoundaryOrientation::POS_X:
		{
			ECBPolyPoint posXBoundaryNormal(1, 0, 0);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, posXBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, POS_X bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the POS_X direction. " << std::endl;
				*/

				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}

		case BoundaryOrientation::NEG_X:
		{
			ECBPolyPoint negXBoundaryNormal(-1, 0, 0);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, negXBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, NEG_X bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the NEG_X direction. " << std::endl;
				*/

				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}

		case BoundaryOrientation::POS_Y:
		{
			// Below: FTDEBUG (uncomment when needed)
			/*
			std::cout << "! Attempting POS_Y case handle. " << std::endl;
			std::cout << "! Target triangle empty normal is: ";
			in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
			std::cout << std::endl;
			*/

			ECBPolyPoint posYBoundaryNormal(0, 1, 0);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, posYBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, POS_Y bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the POS_Y direction. " << std::endl;
				*/

				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}

		case BoundaryOrientation::NEG_Y:
		{
			// Below: FTDEBUG (uncomment when needed)
			/*
			std::cout << "! Attempting NEG_Y case handle. " << std::endl;
			std::cout << "! Target triangle empty normal is: ";
			in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
			std::cout << std::endl;
			*/

			ECBPolyPoint negYBoundaryNormal(0, -1, 0);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, negYBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, NEG_Y bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the POS_Y direction. " << std::endl;
				*/
				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}

		case BoundaryOrientation::POS_Z:
		{
			ECBPolyPoint posZBoundaryNormal(0, 0, 1);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, posZBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, POS_Z bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the POS_Z direction. " << std::endl;
				*/
				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}

		case BoundaryOrientation::NEG_Z:
		{
			ECBPolyPoint negZBoundaryNormal(0, 0, -1);
			bool aligned = FTriangleContainer::areNormalsDirectionallyAligned(in_fTriangleOutputRef->fractureEmptyNormal, negZBoundaryNormal);
			if (!aligned)
			{
				// Below: FTDEBUG (uncomment when needed)
				/*
				std::cout << "!!! Warning, NEG_Z bound triangle's normal, ";
				in_fTriangleOutputRef->fractureEmptyNormal.printPointCoords();
				std::cout << " is not in the NEG_Z direction. " << std::endl;
				*/
				in_fTriangleOutputRef->nonAlignedBoundaryDetected = true;
			}
			break;
		}
	}
}

void FTriangleContainer::FTriangleContainerBounds::runPerfectClampAnalysis(FTriangleOutput* in_fTriangleOutputRef)
{
	// Test for perfect clamp on X
	if 
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.x == in_fTriangleOutputRef->fracturePoints[1].point.x)
		&&
		(in_fTriangleOutputRef->fracturePoints[0].point.x == in_fTriangleOutputRef->fracturePoints[2].point.x)
	)
	{
		// Below: FTDEBUG (uncomment when needed)
		//std::cout << "(FTriangleContainerBounds::runPerfectClampAnalysis) -> found FTirangleOutput as being CLAMPED_TO_X. " << std::endl;

		in_fTriangleOutputRef->fractureRequiredClampValue = PerfectClampEnum::CLAMPED_TO_X;
	}

	// Test for perfect clamp on Y
	else if
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.y == in_fTriangleOutputRef->fracturePoints[1].point.y)
		&&
		(in_fTriangleOutputRef->fracturePoints[0].point.y == in_fTriangleOutputRef->fracturePoints[2].point.y)
	)
	{
		// Below: FTDEBUG (uncomment when needed)
		// std::cout << "(FTriangleContainerBounds::runPerfectClampAnalysis) -> found FTirangleOutput as being CLAMPED_TO_Y. " << std::endl;

		in_fTriangleOutputRef->fractureRequiredClampValue = PerfectClampEnum::CLAMPED_TO_Y;
	}

	// Test for perfect clamp on Z
	else if
	(
		(in_fTriangleOutputRef->fracturePoints[0].point.z == in_fTriangleOutputRef->fracturePoints[1].point.z)
		&&
		(in_fTriangleOutputRef->fracturePoints[0].point.z == in_fTriangleOutputRef->fracturePoints[2].point.z)
	)
	{
		// Below: FTDEBUG (uncomment when needed)
		//std::cout << "(FTriangleContainerBounds::runPerfectClampAnalysis) -> found FTirangleOutput as being CLAMPED_TO_Z. " << std::endl;

		in_fTriangleOutputRef->fractureRequiredClampValue = PerfectClampEnum::CLAMPED_TO_Z;
	}
}

void FTriangleContainer::FTriangleContainerBounds::printBounds()
{
	std::cout << "Bounds values are: " << std::endl;
	std::cout << "pos x: " << boundsMaxX << std::endl;
	std::cout << "neg x: " << boundsMinX << std::endl;
	std::cout << "pos y: " << boundsMaxY << std::endl;
	std::cout << "neg y: " << boundsMinY << std::endl;
	std::cout << "pos z: " << boundsMaxZ << std::endl;
	std::cout << "neg z: " << boundsMinZ << std::endl;
}