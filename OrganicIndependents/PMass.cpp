#include "stdafx.h"
#include "PMass.h"

void PMass::insertAtom(std::shared_ptr<PAtomBase> in_atom)
{
	int newAtomID = in_atom->originalMassID;
	atoms[newAtomID] = in_atom;

	std::cout << "!!! number of fusable areas in inserted atom: " << atoms[newAtomID]->fusableAreaMap.size() << std::endl;

	if (atoms.size() > 1)	// if there is more than one atom, they must be appropriately collided.
	{
		std::cout << "!!! Multiple atoms detected; executing collision tests..." << std::endl;
		collideAtomIntoExistingMass(newAtomID);
	};
}

std::map<int, PAtomBase*> PMass::fetchAtomPointers()
{
	std::map<int, PAtomBase*> returnMap;
	auto atomsBegin = atoms.begin();
	auto atomsEnd = atoms.end();
	for (; atomsBegin != atomsEnd; atomsBegin++)
	{
		int currentAtomID = atomsBegin->second->originalMassID;
		returnMap[currentAtomID] = returnMap[currentAtomID];
	}
	return returnMap;
}

void PMass::collideAtomIntoExistingMass(int in_idToCollide)
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

	std::cout << "Printing values of existing atomIDs: " << std::endl;
	existingAtomIDs.printSet();

	// compare each FusableArea having a FusableAreaState::UNFUSED in the colldiingAtom (the atom having an id of in_idToCollide) against the fusableAreaMap of each other atom, to see if they collide...
	// --AND--
	// then check if each FusableArea having a FusableAreaState::UNFUSED in the colldiingAtom, has a match against anything in the fusedAreas.
	std::vector<FusableArea> fusableAreastoRemove;
	auto collidingAtomMapBegin = atoms[in_idToCollide]->fusableAreaMap.begin();
	auto collidingAtomMapEnd = atoms[in_idToCollide]->fusableAreaMap.end();
	for (; collidingAtomMapBegin != collidingAtomMapEnd; collidingAtomMapBegin++)
	{
		std::cout << "!!! Running comparison. " << std::endl;

		// compare against other atoms.
		auto otherAtomsBegin = existingAtomIDs.begin();
		auto otherAtomsEnd = existingAtomIDs.end();
		bool collisionDetected = false;			// gets set to true as soon as a collision is detected.

		for (; otherAtomsBegin != otherAtomsEnd; otherAtomsBegin++)
		{
			std::cout << "!!! Running against other atom..." << std::endl;
			auto currentOtherAtomMapBegin = atoms[*otherAtomsBegin]->fusableAreaMap.begin();
			auto currentOtherAtomMapEnd = atoms[*otherAtomsBegin]->fusableAreaMap.end();

			
			for (; currentOtherAtomMapBegin != currentOtherAtomMapEnd; currentOtherAtomMapBegin++)
			{
				if (collidingAtomMapBegin->second == currentOtherAtomMapBegin->second)
				{
					// the agent's match; we need to make a new FusableArea that is the combination of the two, put that result into fusedAreas,
					// and then remove the respective areas from each atom. 

					std::cout << "!!! Found matching FusableArea!" << std::endl;

					FusableArea newArea = collidingAtomMapBegin->second;	// copy newArea, by assigning it the value of the colliding atom's current FusableArea that we are looking at.
					newArea += currentOtherAtomMapBegin->second;			// combine the collding FusableArea with the other one.
					insertNewFusedArea(newArea);
					fusableAreastoRemove.push_back(newArea);				// use this vector after we exit these nested loops, 
																			// to appropriately erase each existing FusableArea with a FusedAreaState::FUSED value from each involved atom.
					collisionDetected = true;								// signal that a collision was detected; we won't need to check fused areas.
				}
			}
		}

		std::cout << "!!! Sized of fused areas BEFORE compare: " << fusedAreas.size() << std::endl;

		// compare against fusedAreas only when a collision hasn't been detected yet.
		if (collisionDetected == false)
		{
			auto existingFusedAreasBegin = fusedAreas.begin();
			auto existingFusedAreasEnd = fusedAreas.end();
			for (; existingFusedAreasBegin != existingFusedAreasEnd; existingFusedAreasBegin++)
			{
				if (collidingAtomMapBegin->second == existingFusedAreasBegin->second)
				{
					std::cout << "!! Found match against existing fused area! " << std::endl;
					existingFusedAreasBegin->second += collidingAtomMapBegin->second;	// append the collding FusableArea into the one that 
																						// already exists in fusedAreas.
					//insertNewFusedArea(collidingAtomMapBegin->second);
					fusableAreastoRemove.push_back(collidingAtomMapBegin->second);
				}
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
			std::cout << "!! Size of fusable areas in erased atom having ID " << eraseAttemptsBegin->first << ": " << eraseAttemptsBegin->second->fusableAreaMap.size() << std::endl;
		}
	}

	std::cout << "!! Size of exiting fusable areas: " << fusedAreas.size() << std::endl;
}

void PMass::insertNewFusedArea(FusableArea in_fusableArea)
{
	int currentFusedAreaSize = fusedAreas.size();
	fusedAreas[currentFusedAreaSize] = in_fusableArea;
};