#pragma once

#ifndef PMASS_H
#define PMASS_H

#include "PAtomBase.h"
#include "AtomicBondingResult.h"
#include "PMassState.h"
#include "PMassResult.h"

class PMass
{
	public:
		AtomicBondingResult checkForCollisionAgainstOtherMass(std::shared_ptr<PMass> in_otherMassPtr);
		void insertAtom(std::shared_ptr<PAtomBase> in_atom);
		std::map<int, PAtomBase*> fetchAtomPointers();
		void printFusedAreas();
		std::shared_ptr<PAtomBase> getFirstAtomPtr();	// needed because all PMass starts 
														// with exactly 1 atom; this makes our life easier.
		PAtomState returnAtomStates();	// all atoms in a PMass should have the same PAtomState, at the end of a bonding pass (BONDED, UNBONDED)
		PMassState currentMassState = PMassState::UNDETERMINED;	// used to determine what should be returned as the resulting point; 
																// see enum definitions for details.
		bool collideAtomIntoExistingMass(int in_idToCollide);
		void insertNewFusedArea(FusableArea in_fusableArea);
		void printAtomIDs();
		std::set<int> getAtomIds();
		void expandAllAtoms();
		PMassResult determineAndReturnResult();

	private:
		
		std::map<int, std::shared_ptr<PAtomBase>> atoms;
		std::map<int, FusableArea> fusedAreas;

};

#endif
