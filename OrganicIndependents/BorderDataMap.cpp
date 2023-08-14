#include "stdafx.h"
#include "BorderDataMap.h"

BorderDataMap::BorderDataMap()
{
	//CORNER_LOWERNW
	//CORNER_LOWERSW,
	//CORNER_LOWERSE,
	//CORNER_LOWERNE,
	//CORNER_UPPERNW,
	//CORNER_UPPERSW,
	//CORNER_UPPERSE,
	//CORNER_UPPERNE,
	//cornerMap[ECBPPOrientations::CORNER_LOWERNE] = 5;
	//cornerMap[ECBPPOrientations::CORNER_LOWERNW] = 3;
	//cornerMap[ECBPPOrientations::CORNER_LOWERSE] = 2;

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CORNER MAP SET UP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// >>>>>>>>>> LOWER NW corner set up
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].borderLimits.x = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].borderLimits.y = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].borderLimits.z = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[0] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[1] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[2] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[3] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[4] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].adjacentCorners[5] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].diagonalCorner = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedFaces[1] = ECBPPOrientations::NORTHFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedFaces[2] = ECBPPOrientations::BOTTOMFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHWEST;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWERWEST;
	cornerMap[ECBPPOrientations::CORNER_LOWERNW].linkedLines[2] = ECBPPOrientations::XAXIS_LOWERNORTH;

	// >>>>>>>>>> LOWER SW corner set up
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].borderLimits.x = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].borderLimits.y = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].borderLimits.z = 1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[0] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[1] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[2] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[3] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[4] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].adjacentCorners[5] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].diagonalCorner = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedFaces[2] = ECBPPOrientations::BOTTOMFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedLines[0] = ECBPPOrientations::YAXIS_SOUTHWEST;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWERWEST;
	cornerMap[ECBPPOrientations::CORNER_LOWERSW].linkedLines[2] = ECBPPOrientations::XAXIS_LOWERSOUTH;

	// >>>>>>>>>> LOWER SE corner set up
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].borderLimits.x = 1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].borderLimits.y = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].borderLimits.z = 1;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[0] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[1] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[2] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[3] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[4] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].adjacentCorners[5] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].diagonalCorner = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedFaces[2] = ECBPPOrientations::BOTTOMFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedLines[0] = ECBPPOrientations::YAXIS_SOUTHEAST;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWEREAST;
	cornerMap[ECBPPOrientations::CORNER_LOWERSE].linkedLines[2] = ECBPPOrientations::XAXIS_LOWERSOUTH;

	// >>>>>>>>>> LOWER NE corner set up
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].borderLimits.x = 1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].borderLimits.y = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].borderLimits.z = -1;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[0] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[1] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[2] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[3] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[4] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].adjacentCorners[5] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].diagonalCorner = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedFaces[1] = ECBPPOrientations::NORTHFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedFaces[2] = ECBPPOrientations::BOTTOMFACE;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHEAST;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWEREAST;
	cornerMap[ECBPPOrientations::CORNER_LOWERNE].linkedLines[2] = ECBPPOrientations::XAXIS_LOWERNORTH;

	// >>>>>>>>>> UPPER NW corner set up
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].borderLimits.x = -1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].borderLimits.y = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].borderLimits.z = -1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[0] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[1] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[2] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[3] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[4] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].adjacentCorners[5] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].diagonalCorner = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedFaces[1] = ECBPPOrientations::NORTHFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedFaces[2] = ECBPPOrientations::TOPFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHWEST;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedLines[1] = ECBPPOrientations::ZAXIS_UPPERWEST;
	cornerMap[ECBPPOrientations::CORNER_UPPERNW].linkedLines[2] = ECBPPOrientations::XAXIS_UPPERNORTH;

	// >>>>>>>>>> UPPER SW corner set up
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].borderLimits.x = -1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].borderLimits.y = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].borderLimits.z = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[0] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[1] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[2] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[3] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[4] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].adjacentCorners[5] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].diagonalCorner = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedFaces[2] = ECBPPOrientations::TOPFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedLines[0] = ECBPPOrientations::YAXIS_SOUTHWEST;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedLines[1] = ECBPPOrientations::ZAXIS_UPPERWEST;
	cornerMap[ECBPPOrientations::CORNER_UPPERSW].linkedLines[2] = ECBPPOrientations::XAXIS_UPPERSOUTH;

	// >>>>>>>>>> UPPER SE corner set up
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].borderLimits.x = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].borderLimits.y = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].borderLimits.z = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[0] = ECBPPOrientations::CORNER_UPPERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[1] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[2] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[3] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[4] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].adjacentCorners[5] = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].diagonalCorner = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedFaces[2] = ECBPPOrientations::TOPFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedLines[0] = ECBPPOrientations::YAXIS_SOUTHEAST;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedLines[1] = ECBPPOrientations::ZAXIS_UPPEREAST;
	cornerMap[ECBPPOrientations::CORNER_UPPERSE].linkedLines[2] = ECBPPOrientations::XAXIS_UPPERSOUTH;

	// >>>>>>>>>> UPPER NE corner set up
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].borderLimits.x = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].borderLimits.y = 1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].borderLimits.z = -1;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[0] = ECBPPOrientations::CORNER_UPPERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[1] = ECBPPOrientations::CORNER_UPPERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[2] = ECBPPOrientations::CORNER_UPPERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[3] = ECBPPOrientations::CORNER_LOWERNE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[4] = ECBPPOrientations::CORNER_LOWERNW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].adjacentCorners[5] = ECBPPOrientations::CORNER_LOWERSE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].diagonalCorner = ECBPPOrientations::CORNER_LOWERSW;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedFaces[1] = ECBPPOrientations::NORTHFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedFaces[2] = ECBPPOrientations::TOPFACE;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHEAST;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedLines[1] = ECBPPOrientations::ZAXIS_UPPEREAST;
	cornerMap[ECBPPOrientations::CORNER_UPPERNE].linkedLines[2] = ECBPPOrientations::XAXIS_UPPERNORTH;




	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LINE MAP SET UP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ** if a line being checked is not in linkedLines or is the diagonalLine, it is an "adjacent/opposite" line

	// ------------------------------------------------------------------------------------------------BEGIN Z AXIS initializations
	// >>>>>>>>>>>>>>>>>>>> Z AXIS, LOWER WEST
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].borderLimits.x = -1;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].borderLimits.y = -1;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].borderLimits.z = 0;	// z is not applicable
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[0] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[4] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedLines[5] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].diagonalLine = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWERWEST].linkedFaces[1] = ECBPPOrientations::BOTTOMFACE;

	// >>>>>>>>>>>>>>>>>>>> Z AXIS, UPPER WEST
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].borderLimits.x = -1;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].borderLimits.y = 1;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].borderLimits.z = 0;	// z is not applicable
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[0] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[1] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[4] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedLines[5] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].diagonalLine = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPERWEST].linkedFaces[1] = ECBPPOrientations::TOPFACE;

	// >>>>>>>>>>>>>>>>>>>> Z AXIS, UPPER EAST
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].borderLimits.x = 1;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].borderLimits.y = 1;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].borderLimits.z = 0;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[0] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[4] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedLines[5] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].diagonalLine = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	bdLinesMap[ECBPPOrientations::ZAXIS_UPPEREAST].linkedFaces[1] = ECBPPOrientations::TOPFACE;

	// >>>>>>>>>>>>>>>>>>>> Z AXIS, LOWER EAST
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].borderLimits.x = 1;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].borderLimits.y = -1;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].borderLimits.z = 0;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[0] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[1] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[4] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedLines[5] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].diagonalLine = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	bdLinesMap[ECBPPOrientations::ZAXIS_LOWEREAST].linkedFaces[1] = ECBPPOrientations::BOTTOMFACE;

	// ------------------------------------------------------------------------------------------------BEGIN Y AXIS initializations
	// >>>>>>>>>>>>>>>>>>>>> Y AXIS, NORTH WEST
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].borderLimits.x = -1;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].borderLimits.y = 0;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].borderLimits.z = -1;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[1] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[2] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[3] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[4] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedLines[5] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].diagonalLine = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHWEST].linkedFaces[1] = ECBPPOrientations::NORTHFACE;

	// >>>>>>>>>>>>>>>>>>>>> Y AXIS, SOUTH WEST
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].borderLimits.x = -1;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].borderLimits.y = 0;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].borderLimits.z = 1;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[1] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[2] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[3] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[4] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedLines[5] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].diagonalLine = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedFaces[0] = ECBPPOrientations::WESTFACE;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHWEST].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;

	// >>>>>>>>>>>>>>>>>>>>> Y AXIS, SOUTH EAST
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].borderLimits.x = 1;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].borderLimits.y = 0;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].borderLimits.z = 1;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[0] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[1] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[2] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[3] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[4] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedLines[5] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].diagonalLine = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	bdLinesMap[ECBPPOrientations::YAXIS_SOUTHEAST].linkedFaces[1] = ECBPPOrientations::SOUTHFACE;

	// >>>>>>>>>>>>>>>>>>>>> Y AXIS, NORTH EAST
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].borderLimits.x = 1;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].borderLimits.y = 0;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].borderLimits.z = -1;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[0] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[1] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[2] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[3] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[4] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedLines[5] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].diagonalLine = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedFaces[0] = ECBPPOrientations::EASTFACE;
	bdLinesMap[ECBPPOrientations::YAXIS_NORTHEAST].linkedFaces[1] = ECBPPOrientations::NORTHFACE;


	// ------------------------------------------------------------------------------------------------BEGIN X AXIS initializations

	// >>>>>>>>>>>>>>>>>>>>> X AXIS, LOWER NORTH
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].borderLimits.x = 0; // not applicable here
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].borderLimits.y = -1;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].borderLimits.z = -1;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[0] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[1] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[3] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[4] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedLines[5] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].diagonalLine = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedFaces[0] = ECBPPOrientations::NORTHFACE;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERNORTH].linkedFaces[1] = ECBPPOrientations::BOTTOMFACE;

	// >>>>>>>>>>>>>>>>>>>>> X AXIS, UPPER NORTH
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].borderLimits.x = 0;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].borderLimits.y = 1;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].borderLimits.z = -1;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[0] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[1] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[2] = ECBPPOrientations::YAXIS_NORTHWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[3] = ECBPPOrientations::YAXIS_NORTHEAST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[4] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedLines[5] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].diagonalLine = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedFaces[0] = ECBPPOrientations::NORTHFACE;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERNORTH].linkedFaces[1] = ECBPPOrientations::TOPFACE;

	// >>>>>>>>>>>>>>>>>>>>> X AXIS, UPPER SOUTH
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].borderLimits.x = 0;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].borderLimits.y = 1;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].borderLimits.z = 1;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[0] = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[1] = ECBPPOrientations::XAXIS_LOWERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[2] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[4] = ECBPPOrientations::ZAXIS_UPPERWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedLines[5] = ECBPPOrientations::ZAXIS_UPPEREAST;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].diagonalLine = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedFaces[0] = ECBPPOrientations::SOUTHFACE;
	bdLinesMap[ECBPPOrientations::XAXIS_UPPERSOUTH].linkedFaces[1] = ECBPPOrientations::TOPFACE;

	// >>>>>>>>>>>>>>>>>>>>> X AXIS, LOWER SOUTH
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].borderLimits.x = 0;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].borderLimits.y = -1;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].borderLimits.z = 1;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[0] = ECBPPOrientations::XAXIS_UPPERSOUTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[1] = ECBPPOrientations::XAXIS_LOWERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[2] = ECBPPOrientations::YAXIS_SOUTHWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[3] = ECBPPOrientations::YAXIS_SOUTHEAST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[4] = ECBPPOrientations::ZAXIS_LOWERWEST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedLines[5] = ECBPPOrientations::ZAXIS_LOWEREAST;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].diagonalLine = ECBPPOrientations::XAXIS_UPPERNORTH;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedFaces[0] = ECBPPOrientations::SOUTHFACE;
	bdLinesMap[ECBPPOrientations::XAXIS_LOWERSOUTH].linkedFaces[1] = ECBPPOrientations::BOTTOMFACE;

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FACE MAP SET UP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// >>>>>>>>>>>>>>>>>>>>>>> WEST FACE
	faceMap[ECBPPOrientations::WESTFACE].borderLimits.x = -1;
	faceMap[ECBPPOrientations::WESTFACE].borderLimits.y = 0;
	faceMap[ECBPPOrientations::WESTFACE].borderLimits.z = 0;

	// >>>>>>>>>>>>>>>>>>>>>>> NORTH FACE
	faceMap[ECBPPOrientations::NORTHFACE].borderLimits.x = 0;
	faceMap[ECBPPOrientations::NORTHFACE].borderLimits.y = 0;
	faceMap[ECBPPOrientations::NORTHFACE].borderLimits.z = -1;

	// >>>>>>>>>>>>>>>>>>>>>>> EAST FACE
	faceMap[ECBPPOrientations::EASTFACE].borderLimits.x = 1;
	faceMap[ECBPPOrientations::EASTFACE].borderLimits.y = 0;
	faceMap[ECBPPOrientations::EASTFACE].borderLimits.z = 0;

	// >>>>>>>>>>>>>>>>>>>>>>> SOUTH FACE
	faceMap[ECBPPOrientations::SOUTHFACE].borderLimits.x = 0;
	faceMap[ECBPPOrientations::SOUTHFACE].borderLimits.y = 0;
	faceMap[ECBPPOrientations::SOUTHFACE].borderLimits.z = 1;

	// >>>>>>>>>>>>>>>>>>>>>>> TOP FACE
	faceMap[ECBPPOrientations::TOPFACE].borderLimits.x = 0;
	faceMap[ECBPPOrientations::TOPFACE].borderLimits.y = 1;
	faceMap[ECBPPOrientations::TOPFACE].borderLimits.z = 0;

	// >>>>>>>>>>>>>>>>>>>>>>> BOTTOM FACE
	faceMap[ECBPPOrientations::BOTTOMFACE].borderLimits.x = 0;
	faceMap[ECBPPOrientations::BOTTOMFACE].borderLimits.y = -1;
	faceMap[ECBPPOrientations::BOTTOMFACE].borderLimits.z = 0;

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FACE INTERCEPT MAP SET UP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	faceInterceptMap[ECBPPOrientations::WESTFACE] = ECBPPOrientations::X_INT;
	faceInterceptMap[ECBPPOrientations::EASTFACE] = ECBPPOrientations::X_INT;
	faceInterceptMap[ECBPPOrientations::NORTHFACE] = ECBPPOrientations::Z_INT;
	faceInterceptMap[ECBPPOrientations::SOUTHFACE] = ECBPPOrientations::Z_INT;
	faceInterceptMap[ECBPPOrientations::TOPFACE] = ECBPPOrientations::Y_INT;
	faceInterceptMap[ECBPPOrientations::BOTTOMFACE] = ECBPPOrientations::Y_INT;



}