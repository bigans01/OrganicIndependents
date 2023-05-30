#pragma once

#ifndef ENCLAVEPOLY_H
#define ENCLAVEPOLY_H

#include "BlockBorderLineList.h"
#include "FTriangleContainer.h"
#include "EnclaveTriangleContainer.h"

class EnclavePoly
{
	public:
		void produceEnclaveTrianglesFromFTriangleContainer(FTriangleContainer* in_fTriangleContainerRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);
		EnclaveTriangleContainer producedResultsContainer;
};

#endif
