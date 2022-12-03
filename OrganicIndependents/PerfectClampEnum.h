#pragma once

#ifndef PERFECTCLAMPENUM_H
#define PERFECTCLAMPENUM_H

/*

Description:

This enum class is meant to represent the perfect clamp status of any "Organic" style triangle. 

*/

enum class PerfectClampEnum
{
	NONE,
	CLAMPED_TO_X,
	CLAMPED_TO_Y,
	CLAMPED_TO_Z
};

// The inline keyword is required below, due to the fact that the overload operator below is included in two or more compilation units.
// A description of this understanding can be found here: https://stackoverflow.com/questions/4013703/cant-overload-for-enum-in-c
//
// This will simplify the usage of the enum std::cout, or a file output stream, etc.
//
// If there are strange output errors related to this in the linker, may have to do int(...enum value for this class) to convert cleanly.
inline std::ostream& operator<<(std::ostream& os, const PerfectClampEnum& in_perfectClampValue)
{
	os << int(in_perfectClampValue);
	return os;
}

#endif
