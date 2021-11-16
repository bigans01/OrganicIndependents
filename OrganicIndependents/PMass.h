#pragma once

#ifndef PMASS_H
#define PMASS_H

#include <map>
#include "PAtomBase.h"
#include <mutex>
#include <set>
#include <unordered_map>
#include "EnclaveKeyDef.h"
#include <iostream>

class PMass
{
	public:
		void insertAtom(std::shared_ptr<PAtomBase> in_atom);
		std::map<int, PAtomBase*> fetchAtomPointers();
		void printFusedAreas();
		std::shared_ptr<PAtomBase> getFirstAtomPtr();	// needed because all PMass starts 
														// with exactly 1 atom; this makes our life easier.
	private:
		std::map<int, std::shared_ptr<PAtomBase>> atoms;
		std::map<int, FusableArea> fusedAreas;

		void collideAtomIntoExistingMass(int in_idToCollide);
		void insertNewFusedArea(FusableArea in_fusableArea);
};

#endif
