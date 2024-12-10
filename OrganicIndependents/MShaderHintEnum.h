#pragma once

#ifndef MSHADERHINTENUM_H
#define MSHADERHINTENUM_H

/*
* 
* Description: A simple enum class designed to be used 
* with MShaderController, to represent hints that will go into it's hint index.

*/

enum class MShaderHintEnum
{
	UNSET,					// default value
	TRANSIT_CLEAR_COLOR,	// if the previous MShaderController being used, and the current one have clear color values, cleanly transit between the two.
};

#endif
