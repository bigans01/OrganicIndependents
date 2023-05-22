#pragma once

#ifndef ENCLAVEPOLY_H
#define ENCLAVEPOLY_H

#include "PrimarySegmentTracker.h"
#include "PrimaryLineT1Array.h"
#include "PrimarySegmentMeta.h"
#include "EnclaveBorderLineList.h"
#include "BorderDataMap.h"
#include "BlockBorderLineList.h"
#include "FTriangleContainer.h"
#include "EnclaveTriangleContainer.h"

class EnclavePoly
{
	public:
		BorderDataMap* epBorderDataMapRef = NULL; // for getting trace results in enclaves
		void produceEnclaveTrianglesFromFTriangleContainer(FTriangleContainer* in_fTriangleContainerRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);
		EnclaveTriangleContainer producedResultsContainer;
};

#endif
