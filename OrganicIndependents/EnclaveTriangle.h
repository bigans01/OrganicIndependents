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
	// core EnclaveTriangle components
	short enclaveTriangleMaterialID = 0;				// is the triangle dirt, stone, snow, wood, etc
	short isEnclaveTrianglePolyPerfectlyClamped = 0;	// determines if the entire polygon is perfectly clamped to x, y, or z azis
	bool isTriangleValid = true;						// determines if this EnclaveTriangle successfully ran its trace through the Enclave without any issues;
														// this value will be set to false if there are any issues with tracing.
	ECBPolyLine lineArray[3];							// the 3 ECBPolyLine that are used as a basis to trace the EnclaveTriangle through an Enclave.
	ECBPolyPoint emptyNormal;							// the empty normal that faces outward towards empty space.
	OrganicTriangleTertiary enclaveTriangleTertiary;	// stores the actual bb fans that this EnclaveTriangle produced in each block that it touched.
	BoundaryPolyIndicator enclaveTriangleBoundaryPolyIndicator;	// stores the BoundaryPolyIndicator value for the enclave triangle.

	void executeRun(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key, bool in_badRunFlag);
	void executeRunDebug(BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void reform(ECBPolyPoint in_polyPoint0, ECBPolyPoint in_polyPoint1, ECBPolyPoint in_polyPoint2);	// reform the triangle, for adhesion; 
																										// used by AdhesiveRunner in OrganicServerLib.

	// utility functions, for debugging and std::cout output
	void printExteriorBlocks();			// prints the block keys for each block that was spawned when the EnclaveTriangle's lines were traced through the Enclave's blocks.
	void printPoints();
	void printBlockKeys();				// prints the EnclaveKey block values of all blocks in the OrganicTriangleTertiary
	bool doBlocksExistAtY(int in_y);	// checks if any blocks exist in the enclaveTriangleTeritary, at a specific layer of Y


private:
	void purgeBadFans();	// cleanup all the OrganicWrappedBBFans in the enclaveTriangleTertiary instance, 
							// so that we don't end up inserting lines/points masquerading as triangles. If the 
							// OrganicWrappedBBFan has no valid triangles, it will be deleted.
	void runBoundaryOrientationPass();	// checks the existing OrganicWrappedBBFans in the OrganicTriangleTertiary, to apply any other boundary flags needed.
										// This is because the BoundaryPolyIndicator for an EnclaveTriangle only truly gets set if the EnclaveTriangle is on an
										// ORE's border (i.e, POS_X), not on the blocks. So all blocks need to have their fans checked for bordering.
										// This should be called immediately after purgeBadFans().
	void resetRunMetaData(); // must be called before any run occurs.

	// functions for ensuring the triangle will run OK
	void executeForwardRunTest(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	bool isEndpointValidForFreeSelection(int in_lineID, BlockCircuit* in_circuitRef);

	// poly line running functions
	void runPolyLinesThroughEnclave(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);	// run the exterior lines.
	void runPolyLinesThroughEnclaveReverse(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);	// run the exterior lines.
	void runPolyLinesForReverseTerminatingContainer(PrimaryLineT1Array* in_linkArrayRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);

	// interior tracing functions
	void runInteriorRunners(PrimaryLineT1Array* in_linkArrayRef, 
							BlockBorderLineList* in_blockBorderLineList, 
							BorderDataMap* in_borderDataMap, 
							PolyRunDirection in_polyRunDirection);

	void runInteriorRunnersDebug(PrimaryLineT1Array* in_linkArrayRef, 
								BlockBorderLineList* in_blockBorderLineList, 
								BorderDataMap* in_borderDataMap, 
								PolyRunDirection in_polyRunDirection);

	void runInteriorRunnerReverse(PrimaryLineT1Array* in_linkArrayRef, 
								BlockBorderLineList* in_blockBorderLineList, 
								BorderDataMap* in_borderDataMap, 
								PolyRunDirection in_polyRunDirection);

	// segment and circuit-filling functions.
	void generateExteriorLineSegmentsET(PrimaryLineT1Array* in_linkArrayRef, 
										BlockBorderLineList* in_blockBorderLineList, 
										BorderDataMap* in_borderDataMap, 
										EnclaveKeyDef::EnclaveKey in_key,
										bool in_badRunFlag);									// fill the SegmentTrackers for the outer lines
	void fillCircuitMetaData(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void fillCircuitMetaDataAndCheckValidity(BlockCircuit* in_circuitRef, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, EnclaveKeyDef::EnclaveKey in_key);
	void populateOrganicWrappedBBFanWithCircuitData(BlockCircuit* in_circuitRef, int in_secondaryID);
	int findIndexOfSegmentWithNonFreeEndpoint(BlockCircuit* in_circuitRef, EnclaveKeyDef::EnclaveKey in_key);
	bool isSegmentAndSlopeParallel(SegmentMeta in_segment, ECBPolyPoint in_slope);

	// centroid block checking.
	void performCentroidBlockCheck(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_pointC);	// checks whether or not the block that the EnclaveTriangle's centroid point
																											// would be found in actually exists.

	ECBPolyPoint getTraceResultForEndpointSegment(BlockCircuit* in_circuitRef, int in_segmentIndex, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
	PrimaryLineT1Array generatePrimaryLineT1Array(PolyRunDirection in_polyRunDirection);

	// tracing maps
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> exteriorTracedBlocks;	// a set to register exterior-traced blocks.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> allTracedBlocks;		// " " for all traced blocks (including exteriors)
	TerminatingSetContainer forwardTerminatingContainer;	// terminating set for when interior runners are run in the forward direction. Set up via call to executeRun();
	TerminatingSetContainer reverseTerminatingContainer;	// same as above, but for reverse direction.
	EnclaveKeyDef::EnclaveKey currentEnclaveKey;

	int attemptLimit = 16; // the number of times an EnclaveLineRunner may attempt its completion run before being halted, and the run has to begin again.
};

#endif
