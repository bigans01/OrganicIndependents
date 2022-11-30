#pragma once

#ifndef TRIANGLEMATERIAL_H
#define TRIANGLEMATERIAL_H

/*

Description:

This enum class is meant to represent what type of material any "Organic" style Triangle consists of. One triangle, one material, for any 
Organic-style triangle.

*/

enum class TriangleMaterial
{
	NOVAL,
	DIRT,
	GRASS
};

// The inline keyword is required below, due to the fact that the overload operator below is included in two or more compilation units.
// A description of this understanding can be found here: https://stackoverflow.com/questions/4013703/cant-overload-for-enum-in-c
//
// This will simplify the usage of the enum std::cout, or a file output stream, etc.
//
// If there are strange output errors related to this in the linker, may have to do int(...enum value for this class) to convert cleanly.
inline std::ostream& operator<<(std::ostream& os, const TriangleMaterial& in_material)
{
	os << int(in_material);
	return os;
}


#endif
