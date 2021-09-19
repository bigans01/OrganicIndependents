#pragma once

#ifndef PRIMARYLINET1ARRAY_H
#define PRIMARYLINET1ARRAY_H

#include "PrimaryLineT1.h"
#include "EnclaveKeyDef.h"
#include "ECBTrianglePointArray.h"
#include "ECBPolyPointLocation.h"
#include "ECBPolyLineSlopesArray.h"
#include "PrimaryLineT2Array.h"
#include "PrimaryLineT2IsolatedTracer.h"
#include "PrimaryLineT2MultiTracer.h"
#include "BorderDataMap.h"
#include "ECBPolyPoint.h"
#include <map>

class IndependentUtils;
class PrimaryLineT1Array
{
public:
	PrimaryLineT1Array();
	PrimaryLineT1 linkArray[3];	// contains up to 3 instances of PrimaryLineT1
	BorderDataMap* borderDataMapRef;	// initialized during call to produceT2Lines

	void adjustForEnclavePerfectClamping(short in_perfectClampValue);	// needed by EnclaveTriangle, when calling
																		// EnclaveTriangle::executeRun; "pulls back" EnclaveTriangles that exist on the 
																		// border of positive X/Y/Z in an Enclave.
																		

	bool areLinesValid = true;		// default is true, 99.99% of the time
	void addNewPrimaryLine(PrimaryLineT1 in_Link);
	int checkForTrianglePoints(ECBTrianglePointArray* in_trianglePointArrayPtr);		// cycles through the array, comparing begin points and end points to determine if there is a triangle corner point here; returns a value equal to the number of points found
	void produceT2Lines(EnclaveKeyDef::EnclaveKey in_key, int in_perfectClampFlag, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef, BorderDataMap* in_borderDataMapRef);
	int checkIfLineHasTwoPrimaryT1Points(int in_lineID);		// checks to see if a line in the array has two t1 points as its endpoints
	int compareLineEndpointsForCommonT1Points(int in_lineID, ECBPolyPoint* in_beginPolyPointARef, ECBPolyPoint* in_endPolyPointBRef);	// check if a line has two common t1 points (endpoint and beginpoint)
	int checkIfPointsMatch(ECBPolyPoint* in_pointA, ECBPolyPoint* in_pointB);
	void runT2LinesStandard(EnclaveKeyDef::EnclaveKey in_key, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef);		// generates a T2 line by using a standard trace (used when there is one line)
	void runT2LinesMulti(EnclaveKeyDef::EnclaveKey in_key, ECBPolyLineSlopesArray* in_polyLineSlopesPtr, ECBBorderLineList* in_blueprintBorderLines, PrimaryLineT2Array* in_primaryLineT2ArrayRef, int in_perfectClampFlag);			// generates T2 lines by using a multi-line trace (when there is more than one line)
	void swapLines(ECBBorderValues* in_borderLinesRef);

	void swapLinesForBlueprintTracing();
	void reverseLinePointsAndIntendedFaceForBlueprintTracing(int in_x);

	void reverseLinePointsAndIntendedFace(int in_x, ECBBorderValues* in_borderLinesRef);
	void invertSlopesDuringSwap(int in_index, ECBPolyPoint in_oldFaces);
	void printActiveLines();
	int getNumberOfActiveLines();
	void loadPrimaryT1LinesIntoMap(std::map<int, PrimaryLineT1>* in_map);

	void printBlueprintTracingLines();
};

#endif