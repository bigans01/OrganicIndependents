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
		std::cout << "(OrganicTriangleTertiary): discovered incalculable key; must be mitigated and transformed to a whole block. Enter a number to continue." << std::endl;
		int incalcWait = 3;
		std::cin >> incalcWait;
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
		}


		// Only insert if it was resolved.
		if
		(
			(wasKeyIncalculable == unresolvedKeys.end())
			&&
			(isCurrentKeyOutofBounds == false)
		)
		{

			/*
			std::cout << "!!! Attemptoing build of OrganicWrappedBBFan, for the FTriangleOutput existing at key ";
			currentContainerKey.printKey();
			std::cout << std::endl;
			std::cout << "!!! Triangles of the container are: "; std::cout << std::endl;
			currentBlockContainer.second.printProducedFTriangles();
			*/


			int convertedEnclaveKey = PolyUtils::convertBlockCoordsToSingle(currentContainerKey);

			// Build the BBFan with the FTriangleOutput, and other needed meta data.
			BoundaryPolyIndicator newIndicator;
			newIndicator.setBoundaryIndicator(in_etRequiredOrientation);
			OrganicWrappedBBFan assembledFan(&currentBlockContainer.second,
				in_etTriangleMaterial,
				in_etEmptyNormal,
				newIndicator);

			OrganicFanGroup newGroup(assembledFan);

			// Test what happens when we create a FanManager, and insert this fan:
			//FanManager testManager;


			// Finally, insert the BB fan into the necessary map element.
			//triangleMap[convertedEnclaveKey] = assembledFan;
			triangleMap[convertedEnclaveKey] = newGroup;
		}
	}

}