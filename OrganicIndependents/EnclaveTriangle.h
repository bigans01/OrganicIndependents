#pragma once

#ifndef ENCLAVETRIANGLE_H
#define ENCLAVETRIANGLE_H

#include "ECBPolyLine.h"
#include "OrganicWrappedBBFan.h"
#include "PolyRunDirection.h"
#include "PrimarySegmentMeta.h"
#include "BorderDataMap.h"
#include "BlockBorderLineList.h"
#include "BlockCircuit.h"
#include "EnclaveLineRunner.h"
#include "PolyUtils.h"
#include "SecondarySearchResult.h"
#include "OrganicTriangleTertiary.h"
#include "EnclaveTriangleInteriorRunner.h"
#include "TerminatingSetContainer.h"
#include "SegmentResult.h"
//#include "PrimaryLineT1Array.h"
#include "FTriangleOutput.h"
#include "PolyUtils.h"
#include "Operable3DEnclaveKeySet.h"

/*

Description: When generated, an EnclaveTriangle will attempt to trace through its own area in order to produce OrganicWrappedBBFan instances.
			 However, there is a possibility that the generated fan instances have anomalies; these anomalies must be checked in each fan,
			 allowing for corrections and possible removal of the fan. To ensure this is done, the purgeBadFans function must be called at the
			 very end of both the executeRun and executeRunDebug functions.

*/

class PrimaryLineT1Array;	// this class forwarding could be cleaned up later.
class IndependentUtils;
class EnclaveTriangle
{
public:
	// custom constructor, meant for use with FTriangleOutput
	EnclaveTriangle();
	EnclaveTriangle(FTriangleOutput in_fTriangleOutput);

	// core EnclaveTriangle components
	TriangleMaterial enclaveTriangleMaterialID = TriangleMaterial::NOVAL;				// is the triangle dirt, stone, snow, wood, etc
	PerfectClampEnum isEnclaveTrianglePolyPerfectlyClamped = PerfectClampEnum::NONE;	// determines if the entire polygon is perfectly clamped to x, y, or z azis
	bool isTriangleValid = true;						// determines if this EnclaveTriangle successfully ran its trace through the Enclave without any issues;
														// this value will be set to false if there are any issues with tracing.
	//ECBPolyLine lineArray[3];							// the 3 ECBPolyLine that are used as a basis to trace the EnclaveTriangle through an Enclave.
	ECBPolyPoint points[3];							// the 3 ECBPolyLine that are used as a basis to trace the EnclaveTriangle through an Enclave.
	ECBPolyPoint emptyNormal;							// the empty normal that faces outward towards empty space.
	OrganicTriangleTertiary enclaveTriangleTertiary;	// stores the actual bb fans that this EnclaveTriangle produced in each block that it touched.
	BoundaryPolyIndicator enclaveTriangleBoundaryPolyIndicator;	// stores the BoundaryPolyIndicator value for the enclave triangle.

	// The below function takes the data of this EnclaveTriangle, and puts in into an FTriangle to break it down.
	// The return value of this function indicates which correlating blocks the FTriangle failed to produce data for.
	Operable3DEnclaveKeySet executeRun(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key, bool in_badRunFlag);


	// utility functions, for debugging and std::cout output
	void printPoints();
	void printBlockKeys();				// prints the EnclaveKey block values of all blocks in the OrganicTriangleTertiary
	bool doBlocksExistAtY(int in_y);	// checks if any blocks exist in the enclaveTriangleTeritary, at a specific layer of Y


private:
	ECBPolyPoint convertDoublePointToECBPolyPointForEnclaveTriangle(DoublePoint in_doublePointToConvert);
	

	int attemptLimit = 16; // the number of times an EnclaveLineRunner may attempt its completion run before being halted, and the run has to begin again.


};

#endif
