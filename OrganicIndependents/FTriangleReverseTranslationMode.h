#pragma once

#ifndef FTRIANGLEREVERSETRANSLATIONMODE_H
#define FTRIANGLEREVERSETRANSLATIONMODE_H

/*

Description:

 an enum class that determines how to translate all of the points in the stagerMap. LOCALIZED will mean
 that all the points will fall between 0 and 32.0f; ABSOLUTE indicates that the points should be oriented relative
 to the original FTriangle's position, before fracturing was done

*/

enum class FTriangleReverseTranslationMode
{
	ABSOLUTE,
	LOCALIZED
};

#endif
