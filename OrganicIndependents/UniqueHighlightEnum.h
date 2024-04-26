#pragma once

#ifndef UNIQUEHIGHLIGHTENUM_H
#define UNIQUEHIGHLIGHTENUM_H

/*

Description:

This enum class is intended as a way to represent a certain type of unique highlighting; 
it is used in function calls that contain unique highlight data, to signify the type of
highlighting being used. See example usages in the following classes:

	-OrganicGLManager (OrganicCoreLib library)
	-LocalizedHighlighterGeraT1 (OrganicGLWinLib)

*/

enum class UniqueHighlightEnum
{
	CAMERA_COLLECTION,	// signifies that the data is for Blueprint corner highlighting
	TARGETED_BLOCK		// ""	for individual block highlighting (white corners of a block area)
};

#endif
