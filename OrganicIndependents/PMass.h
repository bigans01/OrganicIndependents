#pragma once

#ifndef PMASS_H
#define PMASS_H

#include <map>
#include "PAtomBase.h"
#include <mutex>
#include <set>
#include <unordered_map>
#include "EnclaveKeyDef.h"

class PMass
{
	public:
		void insertAtom(std::shared_ptr<PAtomBase> in_atom)
		{
			int newAtomID = in_atom->originalMassID;
			atoms[newAtomID] = in_atom;
			if (atoms.size() > 1)	// if there is more than one atom, they must be appropriately collided.
			{
				collideAtomIntoExistingMass(newAtomID);
			};
		};

		std::map<int, PAtomBase*> fetchAtomPointers()
		{
			std::map<int, PAtomBase*> returnMap;
			auto atomsBegin = atoms.begin();
			auto atomsEnd = atoms.end();
			for (; atomsBegin != atomsEnd; atomsBegin++)
			{
				int currentAtomID = atomsBegin->second->originalMassID;
				returnMap[currentAtomID] = returnMap[currentAtomID];
			}
		}
	private:
		std::map<int, std::shared_ptr<PAtomBase>> atoms;
		std::map<int, FusableArea> fusedAreas;

		void collideAtomIntoExistingMass(int in_idToCollide)
		{
			// populate a set that contains the existing atom IDs, and then subtract the idToCollide from it; 
			// this will be the set of IDs that the id to collide will be compared against.
			OperableIntSet existingAtomIDs;
			auto atomIDsBegin = atoms.begin();
			auto atomIDsEnd = atoms.end();
			for (; atomIDsBegin != atomIDsEnd; atomIDsBegin++)
			{
				existingAtomIDs += atomIDsBegin->first;
			}
			existingAtomIDs -= in_idToCollide;

			// compare each FusableArea having a FusableAreaState::UNFUSED in the colldiingAtom (the atom having an id of in_idToCollide) against the fusableAreaMap of each other atom, to see if they collide...
			// --AND--
			// then check if each FusableArea having a FusableAreaState::UNFUSED in the colldiingAtom, has a match against anything in the fusedAreas.
			std::vector<FusableArea> fusableAreastoRemove;
			auto collidingAtomMapBegin = atoms[in_idToCollide]->fusableAreaMap.begin();
			auto collidingAtomMapEnd = atoms[in_idToCollide]->fusableAreaMap.end();
			for (; collidingAtomMapBegin != collidingAtomMapEnd; collidingAtomMapBegin++)
			{
				// compare against other atoms.
				auto otherAtomsBegin = existingAtomIDs.getSet().begin();
				auto otherAtomsEnd = existingAtomIDs.getSet().end();
				for (; otherAtomsBegin != otherAtomsEnd; otherAtomsBegin++)
				{
					auto currentOtherAtomMapBegin = atoms[*otherAtomsBegin]->fusableAreaMap.begin();
					auto currentOtherAtomMapEnd = atoms[*otherAtomsBegin]->fusableAreaMap.end();
					for (; currentOtherAtomMapBegin != currentOtherAtomMapEnd; currentOtherAtomMapBegin++)
					{
						if (collidingAtomMapBegin->second == currentOtherAtomMapBegin->second)
						{
							// the agent's match; we need to make a new FusableArea that is the combination of the two, put that result into fusedAreas,
							// and then remove the respective areas from each atom. 
							FusableArea newArea = collidingAtomMapBegin->second;	// copy newArea, by assigning it the value of the colliding atom's current FusableArea that we are looking at.
							newArea += currentOtherAtomMapBegin->second;			// combine the collding FusableArea with the other one.
							insertNewFusedArea(newArea);
							fusableAreastoRemove.push_back(newArea);				// use this vector after we exit these nested loops, 
																					// to appropriately erase each existing FusableArea with a FusedAreaState::FUSED value from each involved atom.
						}
					}
				}

				// compare against fusedAreas.
				auto existingFusedAreasBegin = fusedAreas.begin();
				auto existingFusedAreasEnd = fusedAreas.end();
				for (; existingFusedAreasBegin != existingFusedAreasEnd; existingFusedAreasBegin++)
				{
					if (collidingAtomMapBegin->second == existingFusedAreasBegin->second)
					{
						existingFusedAreasBegin->second += collidingAtomMapBegin->second;	// append the collding FusableArea into the one that 
																							// already exists in fusedAreas.
						fusableAreastoRemove.push_back(collidingAtomMapBegin->second);
					}
				}
			}

			// finally, erase the respective FusableAreas in each atom.
			auto fusableAreasToRemoveBegin = fusableAreastoRemove.begin();
			auto fusableAreasToRemoveEnd = fusableAreastoRemove.end();
			for (; fusableAreasToRemoveBegin != fusableAreasToRemoveEnd; fusableAreasToRemoveBegin++)
			{
				auto eraseAttemptsBegin = atoms.begin();
				auto eraseAttemptsEnd = atoms.end();
				for (; eraseAttemptsBegin != eraseAttemptsEnd; eraseAttemptsBegin++)
				{
					eraseAttemptsBegin->second->detachFusableArea(*fusableAreasToRemoveBegin);	// for each FusableArea to remove, attempt a detachment in each existing atom.
				}
			}
		}

		void insertNewFusedArea(FusableArea in_fusableArea)
		{
			int currentFusedAreaSize = fusedAreas.size();
			fusedAreas[currentFusedAreaSize] = in_fusableArea;
		}
};

#endif
