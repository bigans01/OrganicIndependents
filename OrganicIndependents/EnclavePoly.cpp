#include "stdafx.h"
#include "EnclavePoly.h"

void EnclavePoly::produceEnclaveTrianglesFromFTriangleContainer(FTriangleContainer* in_fTriangleContainerRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	EnclaveTriangleContainer tempContainer(*in_fTriangleContainerRef);
	BlockBorderLineList testList;
	for (auto& currentTriangleToRun : tempContainer.triangles)
	{
		currentTriangleToRun.second.executeRun(&testList, epBorderDataMapRef, in_enclaveKey, false);
	}

	producedResultsContainer = tempContainer;
}