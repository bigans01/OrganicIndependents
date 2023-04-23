#pragma once

#ifndef ORGANICWRAPPEDBBFAN_H
#define ORGANICWRAPPEDBBFAN_H

#include "EnclaveBlockVertex.h"
#include "PrimarySegmentTracker.h"
#include "BlockCircuit.h"
#include "BlockBorderLineList.h"
#include "BorderDataMap.h"
#include "PolyRunDirection.h"
#include "SegmentResult.h"
#include "FatFan.h"
#include "FTriangleContainer.h"

/*

Description: The OrganicWrappedBBFan stores a single fan triangle (FatFan) that may or may not be eventually inserted
			 into an instance of EnclaveBlock. Before an instance of this class is inserted, the function checkForAndEraseAnomalousTriangles()
			 needs to be called to ensure that any anomalous occurences (such as first/last point match, or duplicate points) 
			 are managed. This means it is possible for the fan to have 0 valid triangles after it is done, which is what
			 the bool value returned by the call is meant to represent. Failing to do this will result in a higher chance
			 of "missing" triangles in the OpenGL rendering, and potential issues when these triangles are used in an instance of 
			 BlockShellProducer (OrganicCoreLib). 

*/

class IndependentUtils;
class OrganicWrappedBBFan
{
	public:
		OrganicWrappedBBFan();
		OrganicWrappedBBFan(FTriangleContainer* in_fTriangleContainerRef,
			TriangleMaterial in_materialID,
			ECBPolyPoint in_emptyNormal,
			BoundaryPolyIndicator in_boundaryPolyIndicator);

		// The below function is primarily used to take in 4 points to 
		// produce two triangles for the face of a block. See it's usage in 
		// the function OrganicTriangleTertiary::buildFilledBlockFanGroup(). 
		// This function may be replaced at a later point with a templated function. (noted on (4/21/2023)
		OrganicWrappedBBFan(ECBPolyPoint in_pointsArray[4],
							TriangleMaterial in_materialID,
							ECBPolyPoint in_emptyNormal,
							BoundaryPolyIndicator in_boundaryPolyIndicator);

		int keyID;
		PrimarySegmentTracker blockSegmentTracker;
		PrimarySegmentTracker reverseSegmentTracker;
		FatFan poly;
		EnclaveBlockVertex vertices[8];

		void buildBBFan(BlockCircuit* in_blockCircuitRef, TriangleMaterial in_materialID, ECBPolyPoint in_emptyNormal);
		void buildBBFanWithBoundaryIndicator(BlockCircuit* in_blockCircuitRef, 
											TriangleMaterial in_materialID,
											 ECBPolyPoint in_emptyNormal,
											 BoundaryPolyIndicator in_boundaryPolyIndicator);

		int checkIfRunIsValidForTwoSegments(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
		bool checkForAndEraseAnomalousTriangles();	// Check for anomalous triangle conditions, such as first/last points matching, or duplicate points in the same TemporalTriangle.
													// There is also a possibility, for some unknown reason, that the poly member of this class contains no tertiaries. 

		SegmentResult checkIfRunIsValidForTwoSegmentsSpecial(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
		int checkIfRunIsValidForTwoSegmentsViaCopy(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PrimarySegmentTracker in_trackerCopy, PolyRunDirection in_direction);
		int checkIfRunIsValidForTwoSegmentsReverse(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap);
		void runBoundaryChecks();	// checks the first 3 points in a FatFan (poly member), to see if any bordering flags need to be set for the fans.
									// should only be done after checkForAndEraseAnomalousTriangles() has been called. Should not be called if there are no tertiaries;
									// (this function assumes there are tertiaries to operate on)
	private:
		// TemporalTriangle -- used to determine what the points of each fan in a triangle are; 
		// instances of this class should only be instantiated/managed after the call to performFirstLastMatchCheck()
		// in the checkForAndEraseAnomalousTriangles function. 
		struct TemporalTriangle
		{
			public:
				TemporalTriangle() {};
				TemporalTriangle(ECBPolyPoint in_sharedPoint,
								ECBPolyPoint in_leadingPoint0,
								ECBPolyPoint in_leadingPoint1,
								int in_leadingPoint0Index,
								int in_leadingPoint1Index) :
					sharedPoint(in_sharedPoint),
					leadingPoint0(in_leadingPoint0),
					leadingPoint1(in_leadingPoint1),
					leadingPoint0Index(in_leadingPoint0Index),
					leadingPoint1Index(in_leadingPoint1Index)
				{};

				// Remember, a TemporalTriangle is invalid if
				// any 2 points are equal OR all 3 points are equal to each other.
				bool isTemporalValid()
				{
					bool isValid = true; // assume valid
					//printPoints();
					if
					(
						(sharedPoint == leadingPoint0)
						||
						(sharedPoint == leadingPoint1)
						||
						(leadingPoint0 == leadingPoint1)
					)
					{
						//std::cout << "!!! NOTE: TemporalTriangle is INVALID! " << std::endl;
						isValid = false;
					}
					return isValid;
				}

				int fetchTemporalMidPointIndex()	// the index for leading point 0 would be the point that exists between the shared point,
													// and the 3rd point. I.e, if this was the very first TemporalTriangle before any modification,
													// the triangle would consist of the points at indices 0,1,2. 1 would be the midpoint.
				{
					return leadingPoint0Index;
				}

				void printPoints()
				{
					std::cout << "Points of this TemporalTriangle are: " << std::endl;
					std::cout << "sharedPoint: ";
					sharedPoint.printPointCoords(); std::cout << std::endl;

					std::cout << "leadingPoint0: ";
					leadingPoint0.printPointCoords(); std::cout << std::endl;

					std::cout << "leadingPoint1: ";
					leadingPoint1.printPointCoords(); std::cout << std::endl;
				}

				void downshiftLeadingPointIndices()
				{
					leadingPoint0Index--;
					leadingPoint1Index--;
				}

				

				// the original points of the triangle, before the cleanup is done;
				// used when checking if triangle is valid, during call to isTemporalValid(), but that should be all that's needed.
				ECBPolyPoint sharedPoint;
				ECBPolyPoint leadingPoint0;
				ECBPolyPoint leadingPoint1;

				// leading point indexes; indexes should match an element in the vertices[] array of the OrganicWrappedBBFan class, and 
				// always be valued between 0 and 7; these should be initialized with the non-default constructor, but if the call to 
				// isTemporalValid() determines that the triangle is "bad" (that is, the 3 points are a line or point), the points can be slid 
				// down in other instances of TemporalTriangle, by calling downshiftLeadingPointIndices().
				int leadingPoint0Index = 0;
				int leadingPoint1Index = 0;


		};

	
		void performFirstLastMatchCheck();	// Checks to see if the shared point (that is, the point at index 0 of vertices) is equal to the last point in the fan.
											// There is a possibility this can happen after a BlockCircuit does its run and rounds the points; this function must be called
											// at the beginning of checkForAndEraseAnomalousTriangles(), before the TemporalTriangles are created.

		void chopOutPointAndShiftVertices(int in_targetIndexToChop);	// "chops" out the midpoint of a triangle fan, and moves down all points above it
																		// (that is, points that have an index greater than that of the chopped out point) down
																		// by 1. Used by checkForAndEraseAnomalousTriangles().
};

#endif
