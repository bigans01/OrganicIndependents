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

	// begin cycling through the output container; for each found "block,"
	// we need to look at the FTriangleContainer in there; one FTriangleOutput in the container is == 1 fan 
	// in the OrganicWrappedBBFan.
	auto producedBlocks = fOutput.fetchOutputContainerRef();
	for (auto& currentBlockContainer : *producedBlocks)
	{
		// Convert the EnclaveKeyDef from the output container, into the corresponding single value.
		EnclaveKeyDef::EnclaveKey currentContainerKey = currentBlockContainer.first;

		/*
		std::cout << "!!! Attemptoing build of OrganicWrappedBBFan, for the FTriangleOutput existing at key ";
		currentContainerKey.printKey();
		std::cout << std::endl;
		*/

		int convertedEnclaveKey = PolyUtils::convertBlockCoordsToSingle(currentContainerKey);

		// Build the BBFan with the FTriangleOutput, and other needed meta data.
		BoundaryPolyIndicator newIndicator;
		newIndicator.setBoundaryIndicator(in_etRequiredOrientation);
		OrganicWrappedBBFan assembledFan(&currentBlockContainer.second,
										in_etTriangleMaterial,
										in_etEmptyNormal,
										newIndicator);

		// Finally, insert the BB fan into the necessary map element.
		triangleMap[convertedEnclaveKey] = assembledFan;
										
	}
}