#include "stdafx.h"
#include "TertiaryTriangleProducer.h"

TertiaryTriangleProducer::TertiaryTriangleProducer(EnclaveBlock* in_enclaveBlockPtr, FanBase* in_enclaveBlockTrianglePtr)
{
	enclaveBlockPtr = in_enclaveBlockPtr;
	enclaveBlockTrianglePtr = in_enclaveBlockTrianglePtr;
}

EnclaveBlockVertexTri TertiaryTriangleProducer::getTrianglePointsAndIterateToNext()
{
	EnclaveBlockVertexTri blockVertexTriToReturn;

	int pointA_index = enclaveBlockTrianglePtr->getPointAtIndex(0);
	int pointB_index = enclaveBlockTrianglePtr->getPointAtIndex(current_second_point);
	int pointC_index = enclaveBlockTrianglePtr->getPointAtIndex(current_third_point);

	blockVertexTriToReturn.pointA = enclaveBlockPtr->fetchPoint(pointA_index);
	blockVertexTriToReturn.pointB = enclaveBlockPtr->fetchPoint(pointB_index);
	blockVertexTriToReturn.pointC = enclaveBlockPtr->fetchPoint(pointC_index);


	current_second_point++;
	current_third_point++;

	return blockVertexTriToReturn;

}
