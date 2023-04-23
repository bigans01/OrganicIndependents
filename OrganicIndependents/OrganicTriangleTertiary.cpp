#include "stdafx.h"
#include "OrganicTriangleTertiary.h"

OrganicTriangleTertiary::OrganicTriangleTertiary()
{

}

OrganicTriangleTertiary::OrganicTriangleTertiary(ECBPolyPoint in_etPoint0,
												ECBPolyPoint in_etPoint1,
												ECBPolyPoint in_etPoint2,
												ECBPolyPoint in_etEmptyNormal,
												BoundaryOrientation in_etRequiredOrientation,
												PerfectClampEnum in_etPerfectClampEnum,
												TriangleMaterial in_etTriangleMaterial)
{
	// First, construct the FTriangle.
	FTriangle fOutput(in_etPoint0,
					in_etPoint1,
					in_etPoint2,
					FTriangleType::ORE,
					in_etEmptyNormal,
					in_etRequiredOrientation,
					in_etPerfectClampEnum,
					in_etTriangleMaterial);

	// run the FTriangle.
	fOutput.fracture();

	// We will also need to check for unresolved/incalculable keys.
	auto unresolvedKeys = fOutput.getUnresolvedOutputs();
	for (auto& curentUnresolvedKey : unresolvedKeys)
	{
		incalculableAttempts += curentUnresolvedKey;
		int convertedEnclaveKey = PolyUtils::convertBlockCoordsToSingle(curentUnresolvedKey);
		OrganicFanGroup builtGroup = buildFilledBlockFanGroup();
		triangleMap[convertedEnclaveKey] = builtGroup;
	}

	// begin cycling through the output container; for each found "block,"
	// we need to look at the FTriangleContainer in there; one FTriangleOutput in the container is == 1 fan 
	// in the OrganicWrappedBBFan.
	auto producedBlocks = fOutput.fetchOutputContainerRef();
	for (auto& currentBlockContainer : *producedBlocks)
	{
		// Convert the EnclaveKeyDef from the output container, into the corresponding single value.
		EnclaveKeyDef::EnclaveKey currentContainerKey = currentBlockContainer.first;
		auto wasKeyIncalculable = unresolvedKeys.find(currentContainerKey);

		/*
		// If the key was incalculable, we must record it, but also "fill" the block.
		if (wasKeyIncalculable != unresolvedKeys.end())
		{
			incalculableAttempts += (currentContainerKey);	// insert the bad key, for record-keeping.
			int convertedEnclaveKey = PolyUtils::convertBlockCoordsToSingle(currentContainerKey);

			//std::cout << "(OrganicTriangleTertiary): found key as unresolvable (double-check), going to insert... " << std::endl;
			//int doubleCheck = 3;
			//std::cin >> doubleCheck;

			// Placeholder: just create a dummy group.
			OrganicFanGroup dummyGroup;
			dummyGroup.setAsFilledBecauseIncalculable();
			triangleMap[convertedEnclaveKey] = dummyGroup;
		}
		*/

		// Check if the currentkey is out of bounds.
		bool isCurrentKeyOutofBounds = false;
		if
		(
			(currentContainerKey.x > 3)
			||
			(currentContainerKey.x < 0)

			||

			(currentContainerKey.y > 3)
			||
			(currentContainerKey.y < 0)

			||

			(currentContainerKey.z > 3)
			||
			(currentContainerKey.z < 0)
		)
		{
			isCurrentKeyOutofBounds = true;
			//incalculableAttempts += (currentContainerKey);	// insert the bad key, for record-keeping.
		}


		// Only insert if it was resolved.
		if
		(
			(wasKeyIncalculable == unresolvedKeys.end())
			&&
			(isCurrentKeyOutofBounds == false)
		)
		{




			int convertedEnclaveKey = PolyUtils::convertBlockCoordsToSingle(currentContainerKey);

			// Build the BBFan with the FTriangleOutput, and other needed meta data.
			BoundaryPolyIndicator newIndicator;
			newIndicator.setBoundaryIndicator(in_etRequiredOrientation);
			OrganicWrappedBBFan assembledFan(&currentBlockContainer.second,
				in_etTriangleMaterial,
				in_etEmptyNormal,
				newIndicator);

			OrganicFanGroup newGroup(assembledFan);


			// Finally, insert the BB fan into the necessary map element.
			triangleMap[convertedEnclaveKey] = newGroup;
		}
	}

}

OrganicFanGroup OrganicTriangleTertiary::buildFilledBlockFanGroup()
{
	OrganicFanGroup returnGroup;
	returnGroup.groupSubType = BlockSubType::BLOCK_NORMAL_FILLED;
	returnGroup.setAsFilledBecauseIncalculable();

	// Pos X face
	ECBPolyPoint posXFaceArray[4];
	ECBPolyPoint posX1(1.0f, 0.0f, 0.0f);
	ECBPolyPoint posX2(1.0f, 0.0f, 1.0f);
	ECBPolyPoint posX3(1.0f, 1.0f, 1.0f);
	ECBPolyPoint posX4(1.0f, 1.0f, 0.0f);
	ECBPolyPoint posXEmptyNormal(1.0f, 0.0f, 0.0f);
	posXFaceArray[0] = posX1;
	posXFaceArray[1] = posX2;
	posXFaceArray[2] = posX3;
	posXFaceArray[3] = posX4;

	BoundaryPolyIndicator posXIndicator;
	posXIndicator.setBoundaryIndicator(BoundaryOrientation::POS_X);
	OrganicWrappedBBFan posXFan(posXFaceArray, TriangleMaterial::DIRT, posXEmptyNormal, posXIndicator);
	
	returnGroup.insertFan(posXFan);

	// Pos Z face
	ECBPolyPoint posZFaceArray[4];
	ECBPolyPoint posZ1(0.0f, 0.0f, 1.0f);
	ECBPolyPoint posZ2(1.0f, 0.0f, 1.0f);
	ECBPolyPoint posZ3(1.0f, 1.0f, 1.0f);
	ECBPolyPoint posZ4(0.0f, 1.0f, 1.0f);
	ECBPolyPoint posZEmptyNormal(0.0f, 0.0f, 1.0f);
	posZFaceArray[0] = posZ1;
	posZFaceArray[1] = posZ2;
	posZFaceArray[2] = posZ3;
	posZFaceArray[3] = posZ4;

	BoundaryPolyIndicator posZIndicator;
	posZIndicator.setBoundaryIndicator(BoundaryOrientation::POS_Z);
	OrganicWrappedBBFan posZFan(posZFaceArray, TriangleMaterial::DIRT, posZEmptyNormal, posZIndicator);

	returnGroup.insertFan(posZFan);

	// Neg X face
	ECBPolyPoint negXFaceArray[4];
	ECBPolyPoint negX1(0.0f, 0.0f, 0.0f);
	ECBPolyPoint negX2(0.0f, 0.0f, 1.0f);
	ECBPolyPoint negX3(0.0f, 1.0f, 1.0f);
	ECBPolyPoint negX4(0.0f, 1.0f, 0.0f);
	ECBPolyPoint negXEmptyNormal(-1.0f, 0.0f, 0.0f);
	negXFaceArray[0] = negX1;
	negXFaceArray[1] = negX2;
	negXFaceArray[2] = negX3;
	negXFaceArray[3] = negX4;

	BoundaryPolyIndicator negXIndicator;
	negXIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_X);
	OrganicWrappedBBFan negXFan(negXFaceArray, TriangleMaterial::DIRT, negXEmptyNormal, negXIndicator);

	returnGroup.insertFan(negXFan);

	// Neg Z face
	ECBPolyPoint negZFaceArray[4];
	ECBPolyPoint negZ1(0.0f, 0.0f, 0.0f);
	ECBPolyPoint negZ2(1.0f, 0.0f, 0.0f);
	ECBPolyPoint negZ3(1.0f, 1.0f, 0.0f);
	ECBPolyPoint negZ4(0.0f, 1.0f, 0.0f);
	ECBPolyPoint negZEmptyNormal(0.0f, 0.0f, -1.0f);
	negZFaceArray[0] = negZ1;
	negZFaceArray[1] = negZ2;
	negZFaceArray[2] = negZ3;
	negZFaceArray[3] = negZ4;

	BoundaryPolyIndicator negZIndicator;
	negZIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_Z);
	OrganicWrappedBBFan negZFan(negZFaceArray, TriangleMaterial::DIRT, negZEmptyNormal, negZIndicator);

	returnGroup.insertFan(negZFan);

	// POS Y face 
	ECBPolyPoint posYFaceArray[4];
	ECBPolyPoint posY1(0.0f, 1.0f, 0.0f);
	ECBPolyPoint posY2(1.0f, 1.0f, 0.0f);
	ECBPolyPoint posY3(1.0f, 1.0f, 1.0f);
	ECBPolyPoint posY4(0.0f, 1.0f, 1.0f);
	ECBPolyPoint posYEmptyNormal(0.0f, 1.0f, 0.0f);
	posYFaceArray[0] = posY1;
	posYFaceArray[1] = posY2;
	posYFaceArray[2] = posY3;
	posYFaceArray[3] = posY4;
	BoundaryPolyIndicator posYIndicator;
	posYIndicator.setBoundaryIndicator(BoundaryOrientation::POS_Y);
	OrganicWrappedBBFan posYFan(posYFaceArray, TriangleMaterial::GRASS, posYEmptyNormal, posYIndicator);

	returnGroup.insertFan(posYFan);

	// Neg Y face 
	ECBPolyPoint negYFaceArray[4];
	ECBPolyPoint negY1(0.0f, 0.0f, 0.0f);
	ECBPolyPoint negY2(1.0f, 0.0f, 0.0f);
	ECBPolyPoint negY3(1.0f, 0.0f, 1.0f);
	ECBPolyPoint negY4(0.0f, 0.0f, 1.0f);
	ECBPolyPoint negYEmptyNormal(0.0f, -1.0f, 0.0f);
	negYFaceArray[0] = negY1;
	negYFaceArray[1] = negY2;
	negYFaceArray[2] = negY3;
	negYFaceArray[3] = negY4;
	BoundaryPolyIndicator negYIndicator;
	negYIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_Y);
	OrganicWrappedBBFan negYFan(negYFaceArray, TriangleMaterial::GRASS, negYEmptyNormal, negYIndicator);

	returnGroup.insertFan(posYFan);


	return returnGroup;
}