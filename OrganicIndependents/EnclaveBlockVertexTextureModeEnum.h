#pragma once

#ifndef ENCLAVEBLOCKVERTEXTEXTUREMODEENUM_H
#define ENCLAVEBLOCKVERTEXTEXTUREMODEENUM_H

/*
* 
* Description: this enum class stores values that are meant to signal
* intended use of texturing data.

*/

enum class EnclaveBlockVertexTextureModeEnum
{
	TILED_COORDS,		// -> this means, use a tiled texture atlas.
	TEXTURE_COORDS,		// -> this means, use U/V values instead of textured atlas coordinates.
	UNDEFINED_COORDS	// -> this means what you think it means (nothing has been defined yet)
};

#endif
