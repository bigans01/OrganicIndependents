#pragma once

#ifndef TERTIARYTRIANGLEPRODUCER_H
#define TERTIARYTRIANGLEPRODUCER_H

#include "EnclaveBlock.h"
#include "EnclaveBlockVertexTri.h"
#include "EnclaveBlockVertex.h"
#include "FanBase.h"

class TertiaryTriangleProducer
{
public:
	TertiaryTriangleProducer(EnclaveBlock* in_enclaveBlockPtr, FanBase* in_enclaveBlockTrianglePtr);
	EnclaveBlock* enclaveBlockPtr;
	FanBase* enclaveBlockTrianglePtr;
	int current_first_point = 0;
	int current_second_point = 1;
	int current_third_point = 2;
	int currentIteration = 0;
	int isActive = 0;			// for testing only; will eventually be replaced
	EnclaveBlockVertexTri getTrianglePointsAndIterateToNext();
};

#endif