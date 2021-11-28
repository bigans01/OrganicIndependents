#pragma once

#ifndef PATOMSTATE_H
#define PATOMSTATE_H

enum class PAtomState
{
	UNBONDED,		// means either A.) the core of the atom doesn't match another atom's core
					// or B.) none of the fusable areas in this atom are shared with another atom

	BONDED			// means either A.) the core of the atom DOES match another atom's core, meaning that the PAtom has a currentExpansionIteration of 1
	                // or B.) at least one fusable area of the atom is missing, due to it being erased since it bonded with the 
					// FusableArea of another atom (the merged fusable area would go to the parent PMass)
					
};

#endif
