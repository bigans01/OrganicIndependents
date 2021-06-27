#pragma once

#ifndef PRIMARYLINET1LINK_H
#define PRIMARYLINET1LINK_H

#include "PrimaryLineT1.h"

class PrimaryLineT1Link
{
public:
	PrimaryLineT1 primaryLineData;		// primary T1 line data
	int indexOfNextLink = 100;			// will have a value of 0, 1, or 2; initialized to 100 (no apparent reason other than that it shouldn't be initialized with 0 1 or 2.
};

#endif