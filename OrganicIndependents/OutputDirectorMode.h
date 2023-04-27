#pragma once

#ifndef OUTPUTDIRECTORMODE_H
#define OUTPUTDIRECTORMODE_H

/*

Description: This enum class is used in the OutputDirector class, to signify what to do with output that is being logged to it.

*/

enum class OutputDirectorMode
{
	OUTPUT_NONE,		// won't do anything with output.
	OUTPUT_STDCOUT,		// write output directly to console.
	OUTPUT_SAVE			// save the output somewhere (i.e., a vector of strings)
};

#endif
