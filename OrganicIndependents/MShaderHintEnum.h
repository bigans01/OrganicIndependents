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
	TRANSIT_CLEAR_COLOR,	// cleanly transit between the current value of the clear color found in the MShaderController's controllerValueRegistry,
							// and the shader we are transiting to
};

#endif
