#include "stdafx.h"
#include "OrganicTriangle.h"

OrganicTriangle::OrganicTriangle()
{

}

OrganicTriangle::OrganicTriangle(ECBPoly* in_ecbPolyRef,  
								 int in_polyID)
{
	ecbPolyRef = in_ecbPolyRef;
	polyID = in_polyID;

	produceEnclaveTriangles();				// step 3: after the interior filler has run (both original and reverse), go to each EnclavePoly and produce the EnclaveTriangles.
}

void OrganicTriangle::produceEnclaveTriangles()
{
	// NOTE: as of 1/31/2023, "OPTION 2" still has anomalous edge cases detected when it runs some FTriangles.
	// Use caution, and stick to "OPTION 1" above unless testing this explicitly.

	// ecbPolyRef->materialID, ecbPolyRef->isPolyPerfectlyClamped, ecbPolyRef->emptyNormal

	// UPDATE 7
	originalPoints[0] = ecbPolyRef->ecbPolyPoints[0];
	originalPoints[1] = ecbPolyRef->ecbPolyPoints[1];
	originalPoints[2] = ecbPolyRef->ecbPolyPoints[2];

	//std::cout << "Start: FTriangle processing. " << std::endl;
	// UPDATE 8
	FTriangle fPoly(ecbPolyRef->ecbPolyPoints[0],
		ecbPolyRef->ecbPolyPoints[1],
		ecbPolyRef->ecbPolyPoints[2],
		FTriangleType::BLUEPRINT,
		ecbPolyRef->emptyNormal,
		ecbPolyRef->polyBoundaryOrientation,
		ecbPolyRef->isPolyPerfectlyClamped,
		ecbPolyRef->materialID);

	fPoly.fracture();

	auto outputsMap = fPoly.fetchOutputContainerRef();
	//std::cout << "Start: FTriangle processing. " << std::endl;
	for (auto& currentContainer : *outputsMap)
	{
		polyMap.enclavePolyMap[currentContainer.first].produceEnclaveTrianglesFromFTriangleContainer(&currentContainer.second, currentContainer.first);
		polyMap.allTracedEnclaves.insert(currentContainer.first);
	}
	//std::cout << "End: FTriangle processing. " << std::endl;	
}

void OrganicTriangle::debugCheckIfKeyExists(EnclaveKeyDef::EnclaveKey in_key)
{
	auto keyFind = polyMap.enclavePolyMap.find(in_key);
	if (keyFind != polyMap.enclavePolyMap.end())
	{
		std::cout << "! Key was found: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") | In OrganicTriangle for PolyID: " << polyID << std::endl;
	}
}