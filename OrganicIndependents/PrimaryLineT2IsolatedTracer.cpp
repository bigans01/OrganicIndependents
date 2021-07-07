#include "stdafx.h"
#include "PrimaryLineT2IsolatedTracer.h"
#include "IndependentUtils.h"

PrimaryLineT2IsolatedTracer::PrimaryLineT2IsolatedTracer(int in_lineID,
	EnclaveKeyDef::EnclaveKey in_blueprintKey,
	ECBPPOrientationResults in_beginOrientation,
	ECBPPOrientationResults in_endOrientation,
	ECBPolyPoint in_beginPoint,
	ECBPolyPoint in_endPoint,
	ECBPolyPoint in_thirdPoint,
	ECBBorderLineList* in_borderLineListRef,
	ECBPolyLineSlopesArray* in_polyLineSlopesArrayRef,
	BorderDataMap* in_borderDataMapRef,
	ECBPolyPoint in_intendedFaces,
	PrimaryLineT2Array* in_primaryLineT2ArrayRef,
	int in_perfectClampFlag,
	int in_x_clamp,
	int in_y_clamp,
	int in_z_clamp
)
{
	parentLineID = in_lineID;
	blueprintKey = in_blueprintKey;
	rootBeginOrientation = in_beginOrientation;					// set root orientation (should never have to change)
	rootBeginPoint = in_beginPoint;								// set begin point
	currentEndPoint = in_endPoint;								// set end point
	currentThirdPoint = in_thirdPoint;
	currentEndOrientation = in_endOrientation;					// set the end orientation
	borderDataMapRef = in_borderDataMapRef;
	intendedFaces = in_intendedFaces;							// set intended faces
	rootBeginFaceList = IndependentUtils::getFaceList(in_beginOrientation, borderDataMapRef);
	rootEndFaceList = IndependentUtils::getFaceList(in_endOrientation, borderDataMapRef);
	lineSlope = IndependentUtils::findNormalizedSlope(in_beginPoint, in_endPoint);	// get the normalized slope
	blueprintBorderListRef = in_borderLineListRef;
	primaryLineT2ArrayRef = in_primaryLineT2ArrayRef;
	perfectClampValue = in_perfectClampFlag;
	x_int_slope = in_polyLineSlopesArrayRef->slopesArray[in_lineID].x_intercept_slope;		// set the slopes for x,y,z
	y_int_slope = in_polyLineSlopesArrayRef->slopesArray[in_lineID].y_intercept_slope;
	z_int_slope = in_polyLineSlopesArrayRef->slopesArray[in_lineID].z_intercept_slope;
	if (in_x_clamp == 1)
	{
		x_int_slope = IndependentUtils::invertSlope(x_int_slope);
		//std::cout << "!! x clamp detected, inverting..." << std::endl;
	}
	if (in_y_clamp == 1)
	{
		y_int_slope = IndependentUtils::invertSlope(y_int_slope);
		//std::cout << "!! y clamp detected, inverting..." << std::endl;
	}
	if (in_z_clamp == 1)
	{
		z_int_slope = IndependentUtils::invertSlope(z_int_slope);
		//std::cout << "!! z clamp detected, inverting..." << std::endl;
	}
	determineInitialTrace(perfectClampValue);
	//std::cout << "::::: Post secondary line complete. (3) " << std::endl;
}

void PrimaryLineT2IsolatedTracer::determineInitialTrace(int in_perfectClampFlag)
{
	moveVals = IndependentUtils::retrieveBorderDirection(currentEndOrientation, borderDataMapRef);	// get the move vals

	//std::cout << "::: Base point before move: " << std::endl;
	//std::cout << "0: " << rootBeginPoint.x << ", " << rootBeginPoint.y << ", " << rootBeginPoint.z << std::endl;
	//std::cout << "1: " << currentEndPoint.x << ", " << currentEndPoint.y << ", " << currentEndPoint.z << std::endl;
	//std::cout << "2: " << currentThirdPoint.x << ", " << currentThirdPoint.y << ", " << currentThirdPoint.z << std::endl;




	currentBeginPoint = currentEndPoint;			// for the initial trace, the begin point to trace from is equal to the end point

	//std::cout << "||||||||| Starting initial trace logic..." << std::endl;

	ECBBorderValues tempLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	//std::cout << "### Blueprint Limits " << std::endl;
	//std::cout << "Pos X: " << tempLimits.posXlimit << std::endl;
	//std::cout << "Neg X: " << tempLimits.negXlimit << std::endl;
	//std::cout << "Pos Y: " << tempLimits.posYlimit << std::endl;
	//std::cout << "Neg Y: " << tempLimits.negYlimit << std::endl;
	//std::cout << "Pos Z: " << tempLimits.posZlimit << std::endl;
	//std::cout << "Neg Z: " << tempLimits.negZlimit << std::endl;


	ECBPolyPoint currentNormalizedSlope = IndependentUtils::findNormalizedSlope(rootBeginPoint, currentEndPoint);
	//std::cout << "||| normalized direction of root line is: " << currentNormalizedSlope.x << ", " << currentNormalizedSlope.y << ", " << currentNormalizedSlope.z << ", " << std::endl;
	//std::cout << "||| intended faces: " << intendedFaces.x << ", " << intendedFaces.y << ", " << intendedFaces.z << std::endl;
	//std::cout << "||| initial move vals: " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
	//std::cout << "|| Initial X: " << x_int_slope.x << ", " << x_int_slope.y << ", " << x_int_slope.z << ", " << std::endl;
	//std::cout << "|| Initial Y: " << y_int_slope.x << ", " << y_int_slope.y << ", " << y_int_slope.z << ", " << std::endl;
	//std::cout << "|| Initial Z: " << z_int_slope.x << ", " << z_int_slope.y << ", " << z_int_slope.z << ", " << std::endl;

	//std::cout << "Perfect clamp value is: " << int(perfectClampValue) << std::endl;
	// determine slope to use below; define the struct first
	struct interceptStorage
	{
		//ECBPPOrientations orientationArray[3];
		BorderMDFaceList tempFaceList;
		int numberOfIntercepts = 0;
	};
	interceptStorage tempStorage;
	currentBeginOrientation = currentEndOrientation; // transfer current end into current begin
	if (currentBeginOrientation.otype == ECBPPOrientations::FACE)	// the hit is a face
	{
		tempStorage.tempFaceList.faceList[0] = currentBeginOrientation.osubtype;	// set the value to be the face
		tempStorage.numberOfIntercepts = 1;			// 1 intercept only
	}
	else if (currentBeginOrientation.otype == ECBPPOrientations::LINE)		// ...get the linked faces for the line
	{
		tempStorage.tempFaceList.faceList[0] = borderDataMapRef->lineMap[currentBeginOrientation.osubtype].linkedFaces[0];
		tempStorage.tempFaceList.faceList[1] = borderDataMapRef->lineMap[currentBeginOrientation.osubtype].linkedFaces[1];
		tempStorage.numberOfIntercepts = 2;
	}
	else if (currentBeginOrientation.otype == ECBPPOrientations::CORNER)	// ...get the linked faces for the corner
	{
		tempStorage.tempFaceList.faceList[0] = borderDataMapRef->cornerMap[currentBeginOrientation.osubtype].linkedFaces[0];
		tempStorage.tempFaceList.faceList[1] = borderDataMapRef->cornerMap[currentBeginOrientation.osubtype].linkedFaces[1];
		tempStorage.tempFaceList.faceList[2] = borderDataMapRef->cornerMap[currentBeginOrientation.osubtype].linkedFaces[2];
		tempStorage.numberOfIntercepts = 3;
	}

	// based on previous result, do some thing:
	ECBPolyPoint interceptToUse;
	EnclaveKeyDef::EnclaveKey moveValsToPass;
	// an intercept count of 1 will always mean a face is use
	if (tempStorage.numberOfIntercepts == 1)
	{
		//std::cout << ">>>>> Face encountered! " << std::endl;
		// an intercept count of 1 will always mean a face is used
		ECBPPOrientations slopeToUse = borderDataMapRef->faceInterceptMap[currentBeginOrientation.osubtype];
		interceptToUse = getSlopeToUse(slopeToUse);
		//std::cout << "|||| (face) Slope to use is: " << interceptToUse.x << ", " << interceptToUse.y << ", " << interceptToUse.z << std::endl;
	}

	// an intercept count of 2 means we need to get the appropriate slope from the line
	else if (tempStorage.numberOfIntercepts == 2)
	{
		//std::cout << ">>>>>> Line encountered! " << std::endl;
		// get the first face value
		ECBPPOrientations face1 = borderDataMapRef->faceInterceptMap[tempStorage.tempFaceList.faceList[0]];
		ECBPolyPoint intercept1 = getSlopeToUse(face1);
		//std::cout << "intercept 1 is : " << intercept1.x << ", " << intercept1.y << ", " << intercept1.z << std::endl;

		// get the second face value
		ECBPPOrientations face2 = borderDataMapRef->faceInterceptMap[tempStorage.tempFaceList.faceList[1]];
		ECBPolyPoint intercept2 = getSlopeToUse(face2);
		//std::cout << "intercept 2 is : " << intercept2.x << ", " << intercept2.y << ", " << intercept2.z << std::endl;
		interceptToUse = getInterceptToUseFromLine(intercept1, intercept2);
	}

	// an intercept count of 3 means we need to get the appropriate slope from the corner (in theory, there should only be 1 valid slope ever)
	else if (tempStorage.numberOfIntercepts == 3)
	{
		//std::cout << ">>>>>>>> Corner encountered! " << std::endl;
		moveValsToPass = borderDataMapRef->cornerMap[currentBeginOrientation.osubtype].borderLimits;
		ECBPolyPoint polyPointToPass;
		polyPointToPass.x = float(moveValsToPass.x);
		polyPointToPass.y = float(moveValsToPass.y);
		polyPointToPass.z = float(moveValsToPass.z);
		interceptToUse = getInterceptToUseFromCorner(polyPointToPass);
	}
	//std::cout << "Intercept to use for isolated T2 is: " << interceptToUse.x << "," << interceptToUse.y << "," << interceptToUse.z << std::endl;
	if
		(
		(interceptToUse.x == 0)
			&&
			(interceptToUse.y == 0)
			&&
			(interceptToUse.z == 0)
			)
	{
		std::cout << "!!! WARNING, isolated run is INVALID! flagging... " << std::endl;

		std::cout << "Root begin point was: " << rootBeginPoint.x << ", " << rootBeginPoint.y << ", " << rootBeginPoint.z << std::endl;

		isRunValid = false;
	}
	// send the chosen intercept to getBlueprintTracingResult

	if (isRunValid == true)
	{
		PLTracingResult currentTraceResults = getBlueprintTracingResult(interceptToUse);
		currentEndPoint = currentTraceResults.resultingEndPoint;
		//std::cout << "::::::::::::::::::::::::::::::::Resulting endpoint is: " << currentEndPoint.x << ", " << currentEndPoint.y << ", " << currentEndPoint.z << ", " << std::endl;
		currentEndOrientation = currentTraceResults.resultingOrientation;

		// setup variables for creation of new primary T2 line
		ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
		ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentBeginPoint, PT2blueprintLimits);		// 
		ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentTraceResults.resultingEndPoint, PT2blueprintLimits);		// 
		PrimaryLineT2 newPLT2;
		newPLT2.IDofParentLine = parentLineID;
		newPLT2.perfectClampValue = perfectClampValue;
		newPLT2.beginPointRealXYZ = currentBeginPoint;
		newPLT2.endPointRealXYZ = currentTraceResults.resultingEndPoint;
		newPLT2.thirdPointRealXYZ = rootBeginPoint;
		newPLT2.intendedFaces = IndependentUtils::determineIntendedFaces(currentBeginPoint, currentEndPoint, currentThirdPoint);
		//std::cout << "::::: New secondary line intended faces: " << newPLT2.intendedFaces.x << ", " << newPLT2.intendedFaces.y << ", " << newPLT2.intendedFaces.z << std::endl;
		newPLT2.beginPointMeta = PT2pointALocation;
		newPLT2.endPointMeta = PT2pointBLocation;
		newPLT2.calibrate(rootBeginPoint);
		newPLT2.applySlopes(intendedFaces, x_int_slope, y_int_slope, z_int_slope);

		//std::cout << "::::: Post secondary line complete. (6) " << std::endl;
		//	std::cout << "::::: Post secondary line complete. " << std::endl;
		// add the new primary T2 line into the referenced primary T2 line array
		primaryLineT2ArrayRef->addNewPrimaryT2Line(newPLT2);

		// check if the first 3 points have any common faces (rootBegin, rootEnd, currentTraceResults)...if this is true, set the  requried match count to 2.
		if (perfectClampValue >= 1)		// remember: 1 = x perfect clamp, 2 = y perfect clamp, 3 = z perfect clamp
		{
			int hasCommonFaces = IndependentUtils::checkIfPointsExistOnSameFace(rootBeginFaceList, rootEndFaceList, currentTraceResults.resultingFaceList, 0);
			if (hasCommonFaces == 1)
			{
				//std::cout << "Perfect clamp has common faces!" << std::endl;
				matchRequiredCount = 2;
			}
		}

		// check if there is a match in the faces
		int isMatchFound = IndependentUtils::checkIfFaceListsMatch(rootBeginFaceList, currentTraceResults.resultingFaceList, matchRequiredCount);
		faceMatchFlag = isMatchFound;		// store result here


		//std::cout << "checking face lists: " << std::endl;
		//std::cout << "Root face list: " << std::endl;
		/*
		for (int x = 0; x < 3; x++)
		{
			if (rootBeginFaceList.faceList[x] == ECBPPOrientations::WESTFACE)
			{
				std::cout << "WESTFACE" << std::endl;
			}
			else if (rootBeginFaceList.faceList[x] == ECBPPOrientations::NORTHFACE)
			{
				std::cout << "NORTHFACE" << std::endl;
			}
			else if (rootBeginFaceList.faceList[x] == ECBPPOrientations::EASTFACE)
			{
				std::cout << "EASTFACE" << std::endl;
			}
			else if (rootBeginFaceList.faceList[x] == ECBPPOrientations::SOUTHFACE)
			{
				std::cout << "SOUTHFACE" << std::endl;
			}
			else if (rootBeginFaceList.faceList[x] == ECBPPOrientations::TOPFACE)
			{
				std::cout << "TOPFACE" << std::endl;
			}
			else if (rootBeginFaceList.faceList[x] == ECBPPOrientations::BOTTOMFACE)
			{
				std::cout << "BOTTOMFACE" << std::endl;
			}
		}
		std::cout << "current resultant face list: " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::WESTFACE)
			{
				std::cout << "WESTFACE" << std::endl;
			}
			else if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::NORTHFACE)
			{
				std::cout << "NORTHFACE" << std::endl;
			}
			else if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::EASTFACE)
			{
				std::cout << "EASTFACE" << std::endl;
			}
			else if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::SOUTHFACE)
			{
				std::cout << "SOUTHFACE" << std::endl;
			}
			else if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::TOPFACE)
			{
				std::cout << "TOPFACE" << std::endl;
			}
			else if (currentTraceResults.resultingFaceList.faceList[x] == ECBPPOrientations::BOTTOMFACE)
			{
				std::cout << "BOTTOMFACE" << std::endl;
			}
		}
		*/
		//std::cout << "::::: Post secondary line complete. (7) " << std::endl;
		if (faceMatchFlag != 1)
		{
			while (faceMatchFlag != 1)
			{
				determineTrace();
				//std::cout << "determining trace..." << std::endl;
			}
		}
		//std::cout << "::::: Post secondary line complete. (8) " << std::endl;
		//std::cout << "Point A data: " << std::endl;
		//std::cout << "Chunk/Enclave: " << newPLT2.beginPointMeta.enclaveKey.x << ", " << newPLT2.beginPointMeta.enclaveKey.y << ", " << newPLT2.beginPointMeta.enclaveKey.z << std::endl;
		//std::cout << "Block: " << newPLT2.beginPointMeta.blockKey.x << ", " << newPLT2.beginPointMeta.blockKey.y << ", " << newPLT2.beginPointMeta.blockKey.z << std::endl;
		//std::cout << "Precise loc: " << newPLT2.beginPointMeta.preciseCoord.x << ", " << newPLT2.beginPointMeta.preciseCoord.y << ", " << newPLT2.beginPointMeta.preciseCoord.z << std::endl;
		//determineTrace();	// do the last trace
		determineFinalTrace();
	}
	//std::cout << "--->>>> Isolated tracer end. " << std::endl;

	//std::cout << "::::: Post secondary line complete. (2) " << std::endl;
}

void PrimaryLineT2IsolatedTracer::determineTrace()
{
	// set the current third point, before the new begin point gets reset
	//std::cout << "hello? " << std::endl;
	currentThirdPoint = currentBeginPoint;
	currentBeginPoint = currentEndPoint;
	currentBeginOrientation = currentEndOrientation;

	// set the base slopes (can potentially be modified)
	ECBPolyPoint current_x_slope = x_int_slope;
	ECBPolyPoint current_y_slope = y_int_slope;
	ECBPolyPoint current_z_slope = z_int_slope;

	//std::cout << "|||>>> Current x slope: " << current_x_slope.x << ", " << current_x_slope.y << ", " << current_x_slope.z << std::endl;
	//std::cout << "|||>>> Current y slope: " << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
	//std::cout << "|||>>> Current z slope: " << current_z_slope.x << ", " << current_z_slope.y << ", " << current_z_slope.z << std::endl;

	// retrieve move vals of appropriate type
	moveVals = IndependentUtils::retrieveBorderDirection(currentBeginOrientation, borderDataMapRef);
	//std::cout << "hello??" << std::endl;

	//std::cout << "Determine trace:: move vals-> " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;

	// std::cout << "Intended faces: " << intendedFaces.x << ", " << intendedFaces.y << ", " << intendedFaces.z << ", " << std::endl;
	// check to see what slopes need to be inverted
	if (intendedFaces.x != moveVals.x)
	{

		current_x_slope = IndependentUtils::invertSlope(current_x_slope);	// invert x
		//std::cout << "|||>>> x invert requirement detected " << current_x_slope.x << ", " << current_x_slope.y << ", " << current_x_slope.z << std::endl;

	}
	if (intendedFaces.y != moveVals.y)
	{

		current_y_slope = IndependentUtils::invertSlope(current_y_slope);
		//std::cout << "|||>>> y invert requirement detected " << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
	}
	if (intendedFaces.z != moveVals.z)
	{

		current_z_slope = IndependentUtils::invertSlope(current_z_slope);
		//std::cout << "|||>>> z invert requirement detected " << current_y_slope.x << ", " << current_y_slope.y << ", " << current_y_slope.z << std::endl;
	}

	// choose slope to use
	ECBPolyPoint slopeToUse;
	if (currentBeginOrientation.otype == ECBPPOrientations::FACE)	// the hit is a face (should be impossible; remove this later)
	{
		//std::cout << "|||| acquiring slope from face...?" << std::endl;
	}
	else if (currentBeginOrientation.otype == ECBPPOrientations::LINE)		// ...get the linked faces for the line
	{
		//std::cout << "|||| acquiring slope from line... " << std::endl;
		ECBPPOrientations face1 = borderDataMapRef->faceInterceptMap[borderDataMapRef->lineMap[currentBeginOrientation.osubtype].linkedFaces[0]];
		ECBPolyPoint intercept1 = getSlopeToUse(face1, current_x_slope, current_y_slope, current_z_slope);

		ECBPPOrientations face2 = borderDataMapRef->faceInterceptMap[borderDataMapRef->lineMap[currentBeginOrientation.osubtype].linkedFaces[1]];
		ECBPolyPoint intercept2 = getSlopeToUse(face2, current_x_slope, current_y_slope, current_z_slope);
		//std::cout << " |||| !!! --Post face calls: " << std::endl;
		slopeToUse = getInterceptToUseFromLine(intercept1, intercept2);
		//std::cout << " |||| !!! --chosen line slope is: " << slopeToUse.x << ", " << slopeToUse.y << ", " << slopeToUse.z << std::endl;
	}
	else if (currentBeginOrientation.otype == ECBPPOrientations::CORNER)	// ...get the linked faces for the corner
	{
		//std::cout << "|||| acquiring slope from corner... " << std::endl;
		slopeToUse = getInterceptToUseFromCorner(current_x_slope, current_y_slope, current_z_slope);
	}
	//std::cout << "hello???" << std::endl;
	//std::cout << "slope to use is: " << slopeToUse.x << ", " << slopeToUse.y << ", " << slopeToUse.z << std::endl;

	// get the tracing result
	//std::cout << ">>>> beginning new trace attempt " << std::endl;
	PLTracingResult currentTraceResults = getBlueprintTracingResult(slopeToUse);
	//std::cout << "hello????" << std::endl;
	currentEndPoint = currentTraceResults.resultingEndPoint;
	//std::cout << "::::::::::::::::::::::::::::::::Resulting endpoint is: " << currentEndPoint.x << ", " << currentEndPoint.y << ", " << currentEndPoint.z << ", " << std::endl;

	// std::cout << "<<<<  Current turn endpoint is: " << currentEndPoint.x << ", " << currentEndPoint.y << ", " << currentEndPoint.z << ", " << std::endl;
	currentEndOrientation = currentTraceResults.resultingOrientation;
	//std::cout << "-----------post initial match attempt begin-------------" << std::endl;
	//std::cout << "root point: " << rootBeginPoint.x << ", " << rootBeginPoint.y << ", " << rootBeginPoint.z << std::endl;
	//std::cout << "current end point: " << currentEndPoint.x << ", " << currentEndPoint.y << ", " << currentEndPoint.z << std::endl;

	// set up values for the Primary T2 line
	//std::cout << "beefy " << std::endl;
	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentBeginPoint, PT2blueprintLimits);		// 
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentTraceResults.resultingEndPoint, PT2blueprintLimits);		// 
	PrimaryLineT2 newPLT2;
	newPLT2.IDofParentLine = parentLineID;
	newPLT2.perfectClampValue = perfectClampValue;
	newPLT2.beginPointRealXYZ = currentBeginPoint;
	newPLT2.endPointRealXYZ = currentTraceResults.resultingEndPoint;
	newPLT2.thirdPointRealXYZ = currentThirdPoint;
	newPLT2.intendedFaces = IndependentUtils::determineIntendedFaces(currentBeginPoint, currentEndPoint, currentThirdPoint);
	newPLT2.beginPointMeta = PT2pointALocation;
	newPLT2.endPointMeta = PT2pointBLocation;
	newPLT2.calibrate(currentThirdPoint);
	newPLT2.applySlopes(intendedFaces, x_int_slope, y_int_slope, z_int_slope);
	//std::cout << "chewy " << std::endl;
	// add the new primary T2 line into the referenced primary T2 line array
	primaryLineT2ArrayRef->addNewPrimaryT2Line(newPLT2);

	int isMatchFound = IndependentUtils::checkIfFaceListsMatch(rootBeginFaceList, currentTraceResults.resultingFaceList, matchRequiredCount);
	//std::cout << "-----------post initial match attempt end-------------" << std::endl;
	faceMatchFlag = isMatchFound;		// store result here

	if (faceMatchFlag == 1)
	{
		//std::cout << ">>>> :::::: Blueprint Trace has completed, faceMatchflag is SET to 1!! ::: <<<<" << std::endl;
		//std::cout << "Why isn't this printing??!?!" << std::endl;
	}
}

void PrimaryLineT2IsolatedTracer::determineFinalTrace()
{
	// set the current third point, before the new begin point gets reset
	currentThirdPoint = currentBeginPoint;
	currentBeginPoint = currentEndPoint;
	currentBeginOrientation = currentEndOrientation;

	// set current end point to the root begin point
	currentEndPoint = rootBeginPoint;

	ECBBorderValues PT2blueprintLimits = IndependentUtils::getBlueprintLimits(blueprintKey);
	ECBPolyPointLocation PT2pointALocation = IndependentUtils::getPolyPointLocation(currentBeginPoint, PT2blueprintLimits);		// point A is the last end point (set up in the previous call to determineTrace())
	ECBPolyPointLocation PT2pointBLocation = IndependentUtils::getPolyPointLocation(currentEndPoint, PT2blueprintLimits);		// point B is the root begin point
	PrimaryLineT2 newPLT2;
	newPLT2.IDofParentLine = parentLineID;
	newPLT2.perfectClampValue = perfectClampValue;
	newPLT2.beginPointRealXYZ = currentBeginPoint;
	newPLT2.endPointRealXYZ = currentEndPoint;
	newPLT2.thirdPointRealXYZ = currentThirdPoint;
	newPLT2.intendedFaces = IndependentUtils::determineIntendedFaces(currentBeginPoint, currentEndPoint, currentThirdPoint);
	newPLT2.beginPointMeta = PT2pointALocation;
	newPLT2.endPointMeta = PT2pointBLocation;
	newPLT2.calibrate(currentThirdPoint);
	newPLT2.applySlopes(intendedFaces, x_int_slope, y_int_slope, z_int_slope);

	// add the new primary T2 line into the referenced primary T2 line array
	primaryLineT2ArrayRef->addNewPrimaryT2Line(newPLT2);
}

PLTracingResult PrimaryLineT2IsolatedTracer::getBlueprintTracingResult(ECBPolyPoint in_interceptToUse)
{
	PLTracingResult resultToReturn;
	//std::cout << ">>>>>> Called blueprint tracing result... " << std::endl;
	ECBPolyPoint resultEndpoint = IndependentUtils::getBlueprintTracingEndpointForIsolatedPrimaryT2(currentBeginPoint, in_interceptToUse, blueprintBorderListRef, blueprintKey);
	//std::cout << "oh hello! " << std::endl;
	ECBPPOrientationResults orientationResults = IndependentUtils::GetBlueprintPointOrientation(resultEndpoint, blueprintBorderListRef);
	//std::cout << "oh hello!! " << std::endl;
	BorderMDFaceList resultFaceList = IndependentUtils::getFaceList(orientationResults, borderDataMapRef);					// get resultant face list by looking up the map with the orientation
	//std::cout << "oh hello!!! " << std::endl;
	resultToReturn.resultingEndPoint = resultEndpoint;
	resultToReturn.resultingFaceList = resultFaceList;
	resultToReturn.resultingOrientation = orientationResults;
	return resultToReturn;
}

ECBPolyPoint PrimaryLineT2IsolatedTracer::getSlopeToUse(ECBPPOrientations in_interceptType)
{
	if (in_interceptType == ECBPPOrientations::X_INT)
	{
		//std::cout << "!!! Returning x-int..." << std::endl;
		return x_int_slope;

	}
	else if (in_interceptType == ECBPPOrientations::Y_INT)
	{
		//std::cout << "!!! Returning y-int..." << std::endl;
		return y_int_slope;
	}
	else if (in_interceptType == ECBPPOrientations::Z_INT)
	{
		//std::cout << "!!! Returning z-int..." << std::endl;
		return z_int_slope;
	}
	else
	{
		ECBPolyPoint badPoint;
		return badPoint;	// ++should never return, makes compiler happy (won't produce a warning)
	}
}

ECBPolyPoint PrimaryLineT2IsolatedTracer::getInterceptToUseFromLine(ECBPolyPoint in_intercept1, ECBPolyPoint in_intercept2)
{
	int first_intercept_invalid = 0;		// for storing if the first intercept value is valid selection, start at 0 (meaning its valid)
	int second_intercept_invalid = 0;		// for storing if the second intercept value is valid selection, start at 0 ""
	ECBPolyPoint normalized_first_intercept = IndependentUtils::findNormalizedPoint(in_intercept1);
	ECBPolyPoint normalized_second_intercept = IndependentUtils::findNormalizedPoint(in_intercept2);
	ECBPolyPoint returnPoint;				// the point to return
											// compare values for first intercept


											//std::cout << "++++++++++++ printing get intercept line data... " << std::endl;
											//std::cout << "move vals: " << moveVals.x << ", " << moveVals.y << ", " << moveVals.z << std::endl;
											//std::cout << "original first int: " << in_intercept1.x << ", " << in_intercept1.y << ", " << in_intercept1.z << std::endl;
											//std::cout << "original second int: " << in_intercept2.x << ", " << in_intercept2.y << ", " << in_intercept2.z << std::endl;
											//std::cout << "normalized first int: " << normalized_first_intercept.x << ", " << normalized_first_intercept.y << ", " << normalized_first_intercept.z << std::endl;
											//std::cout << "normalized second int: " << normalized_second_intercept.x << ", " << normalized_second_intercept.y << ", " << normalized_second_intercept.z << std::endl;
	if (
		(
		(moveVals.x == normalized_first_intercept.x)
			&&
			(moveVals.x != 0.0f)
			)
		||
		(
		(moveVals.y == normalized_first_intercept.y)
			&&
			(moveVals.y != 0.0f)
			)
		||
		(
		(moveVals.z == normalized_first_intercept.z)
			&&
			(moveVals.z != 0.0f)
			)
		)
	{
		//std::cout << "First intercept set to invalid..." << std::endl;
		first_intercept_invalid = 1;
	}

	// compare values for second intercept
	if (
		(
		(moveVals.x == normalized_second_intercept.x)
			&&
			(moveVals.x != 0.0f)
			)
		||
		(
		(moveVals.y == normalized_second_intercept.y)
			&&
			(moveVals.y != 0.0f)
			)
		||
		(
		(moveVals.z == normalized_second_intercept.z)
			&&
			(moveVals.z != 0.0f)
			)
		)
	{
		//std::cout << "Second intercept set to invalid..." << std::endl;
		second_intercept_invalid = 1;
	}

	// use the second intercept
	if ((first_intercept_invalid == 1) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept2;
		//std::cout << "Second slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// use the first intercept
	else if ((second_intercept_invalid == 1) && (first_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "First slope is valid, choosing: " << returnPoint.x << ", " << returnPoint.y << ", " << returnPoint.z << std::endl;
	}

	// if both are valid, use the first one
	else if ((first_intercept_invalid == 0) && (second_intercept_invalid == 0))
	{
		returnPoint = in_intercept1;
		//std::cout << "Both slopes technically valid, returning first. " << std::endl;
	}
	return returnPoint;
}

ECBPolyPoint PrimaryLineT2IsolatedTracer::getInterceptToUseFromCorner(ECBPolyPoint in_moveValues)
{
	ECBPolyPoint returnPoint;
	int x_intercept_invalid = 0;	// for storing if the x intercept value is valid selection, start at 0 (meaning its valid)
	int y_intercept_invalid = 0;	// ""  y intercept
	int z_intercept_invalid = 0;	// ""  z intercept
	ECBPolyPoint normalized_x_intercept = IndependentUtils::findNormalizedPoint(x_int_slope);
	ECBPolyPoint normalized_y_intercept = IndependentUtils::findNormalizedPoint(y_int_slope);
	ECBPolyPoint normalized_z_intercept = IndependentUtils::findNormalizedPoint(z_int_slope);

	// check for x intercept invalid
	if ((moveVals.y == normalized_x_intercept.y)
		||
		(moveVals.z == normalized_x_intercept.z)
		)
	{
		x_intercept_invalid = 1;
		//std::cout << "Corner calc: x int is invalid!!" << std::endl;
	}

	// check for y intercept invalid
	if ((moveVals.x == normalized_y_intercept.x)
		||
		(moveVals.z == normalized_y_intercept.z)
		)
	{
		y_intercept_invalid = 1;
		//std::cout << "Corner calc: y int is invalid!!" << std::endl;
	}

	// check for z intercept invalid
	if ((moveVals.x == normalized_z_intercept.x)
		||
		(moveVals.y == normalized_z_intercept.y)
		)
	{
		z_intercept_invalid = 1;
		//std::cout << "Corner calc: z int is invalid!!" << std::endl;
	}

	// return appropriate point
	if (x_intercept_invalid == 0)
	{
		//std::cout << "Returning valid x-slope!" << std::endl;
		returnPoint = x_int_slope;
	}
	else if (y_intercept_invalid == 0)
	{
		//std::cout << "Returning valid y-slope!" << std::endl;
		returnPoint = y_int_slope;
	}
	else if (z_intercept_invalid == 0)
	{
		//std::cout << "Returning valid z-slope!" << std::endl;
		returnPoint = z_int_slope;
	}
	return returnPoint;
}

ECBPolyPoint PrimaryLineT2IsolatedTracer::getInterceptToUseFromCorner(ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope)
{
	ECBPolyPoint returnPoint;
	int x_intercept_invalid = 0;	// for storing if the x intercept value is valid selection, start at 0 (meaning its valid)
	int y_intercept_invalid = 0;	// ""  y intercept
	int z_intercept_invalid = 0;	// ""  z intercept
	ECBPolyPoint normalized_x_intercept = IndependentUtils::findNormalizedPoint(in_xSlope);
	ECBPolyPoint normalized_y_intercept = IndependentUtils::findNormalizedPoint(in_ySlope);
	ECBPolyPoint normalized_z_intercept = IndependentUtils::findNormalizedPoint(in_zSlope);

	// check for x intercept invalid
	if ((moveVals.y == normalized_x_intercept.y)
		||
		(moveVals.z == normalized_x_intercept.z)
		||
		(perfectClampValue == 1)	// x clamp check
		)
	{
		x_intercept_invalid = 1;
		//std::cout << "Corner calc: x int is invalid!!" << std::endl;
	}

	// check for y intercept invalid
	if ((moveVals.x == normalized_y_intercept.x)
		||
		(moveVals.z == normalized_y_intercept.z)
		||
		(perfectClampValue == 2)	// y clamp check
		)
	{
		y_intercept_invalid = 1;
		//std::cout << "Corner calc: y int is invalid!!" << std::endl;
	}

	// check for z intercept invalid
	if ((moveVals.x == normalized_z_intercept.x)
		||
		(moveVals.y == normalized_z_intercept.y)
		||
		(perfectClampValue == 3)	// z clamp check
		)
	{
		z_intercept_invalid = 1;
		//std::cout << "Corner calc: z int is invalid!!" << std::endl;
	}

	// return appropriate point
	if (x_intercept_invalid == 0)
	{
		//std::cout << "Returning valid x-slope!" << std::endl;
		returnPoint = in_xSlope;
	}
	else if (y_intercept_invalid == 0)
	{
		//std::cout << "Returning valid y-slope!" << std::endl;
		returnPoint = in_ySlope;
	}
	else if (z_intercept_invalid == 0)
	{
		//std::cout << "Returning valid z-slope!" << std::endl;
		returnPoint = in_zSlope;
	}
	return returnPoint;
}

ECBPolyPoint PrimaryLineT2IsolatedTracer::getSlopeToUse(ECBPPOrientations in_interceptType, ECBPolyPoint in_xSlope, ECBPolyPoint in_ySlope, ECBPolyPoint in_zSlope)
{
	if (in_interceptType == ECBPPOrientations::X_INT)
	{
		//std::cout << "!!! Returning x-int..." << std::endl;
		return in_xSlope;

	}
	else if (in_interceptType == ECBPPOrientations::Y_INT)
	{
		//std::cout << "!!! Returning y-int..." << std::endl;
		return in_ySlope;
	}
	else if (in_interceptType == ECBPPOrientations::Z_INT)
	{
		//std::cout << "!!! Returning z-int..." << std::endl;
		return in_zSlope;
	}
	else
	{
		std::cout << "!!!! WARNING: no match found. " << std::endl;
		ECBPolyPoint badPoint;
		return badPoint;	// ++should never return, makes compiler happy (won't produce a warning)
	}
}