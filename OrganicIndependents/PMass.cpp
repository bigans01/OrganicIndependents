#include "stdafx.h"
#include "PMass.h"

void PMass::insertAtom(std::shared_ptr<PAtomBase> in_atom)
{
	/*
	std::cout << "!!! Size of atoms before insert: " << atoms.size() << std::endl;
	std::cout << "IDs of atoms before insert: " << std::endl;
	auto printAtomsBegin = atoms.begin();
	auto printAtomsEnd = atoms.end();
	for (; printAtomsBegin != printAtomsEnd; printAtomsBegin++)
	{
		std::cout << printAtomsBegin->first << std::endl;
	}
	*/
	int newAtomID = in_atom->originalMassID;
	//std::cout << "!!-> New Atom ID is: " << newAtomID << std::endl;
	atoms[newAtomID] = in_atom;

	//std::cout << "!!! ID of atom to fuse: " << newAtomID << std::endl;
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

void PMass::printFusedAreas()
{
	if (fusedAreas.empty() == false)
	{
		std::cout << "(PMass): printing fused areas: " << std::endl;
		auto areasBegin = fusedAreas.begin();
		auto areasEnd = fusedAreas.end();
		for (; areasBegin != areasEnd; areasBegin++)
		{
			areasBegin->second.pointAgent.printPointCoords();
			areasBegin->second.printAtomicIDRegister();
		}
	}
}

PAtomState PMass::returnAtomStates()
{
	PAtomState returnState = PAtomState::UNBONDED;
	auto atomsBegin = atoms.begin();
	auto atomsEnd = atoms.end();
	for (; atomsBegin != atomsEnd; atomsBegin++)
	{
		if (atomsBegin->second->atomState == PAtomState::BONDED)
		{
			returnState = PAtomState::BONDED;
			break;
		}
	}
	return returnState;
}

std::shared_ptr<PAtomBase> PMass::getFirstAtomPtr()
{
	// NOTE: this function assumes that the PAtom already exists.
	return atoms.begin()->second;
}

AtomicBondingResult PMass::checkForCollisionAgainstOtherMass(std::shared_ptr<PMass> in_otherMassPtr)
{
	AtomicBondingResult collisionReturnResult;	// if there is no match, the value of bondingResultExists is false.
	// should only be done if all the atoms in this PMass have a return value of
	// UNBONDED when returnAtomStates() is called; we will get the only existing atom in the mass for the "left" comparison.
	auto leftAtom = atoms.begin();
	auto leftAtomID = leftAtom->second->originalMassID;

	bool coreCollisionDetected = false;

	// Pass 1: Core collision test
	auto otherMassAtomsBegin = in_otherMassPtr->atoms.begin();
	auto otherMassAtomsEnd = in_otherMassPtr->atoms.end();
	for (; otherMassAtomsBegin != otherMassAtomsEnd; otherMassAtomsBegin++)
	{
		if (leftAtom->second->atomCorePoint == otherMassAtomsBegin->second->atomCorePoint)
		{
			coreCollisionDetected = true;
			std::cout << "!! Core point collision detected between two atoms..." << std::endl;
			collisionReturnResult = AtomicBondingResult(leftAtom->second);

			// because the cores are the same, both involved atoms should have a state of bonded, if it wasn't set already.
			leftAtom->second->atomState = PAtomState::BONDED;
			otherMassAtomsBegin->second->atomState = PAtomState::BONDED;
			currentMassState = PMassState::POINT_AT_CORE;	
		}
	}

	// Pass 2: fusable area testing; done if nothing is matched in pass 1.
	if (coreCollisionDetected == false)
	{
		auto leftFusableAreasBegin = leftAtom->second->fusableAreaMap.begin();
		auto leftFusableAreasEnd = leftAtom->second->fusableAreaMap.end();
		for (; leftFusableAreasBegin != leftFusableAreasEnd; leftFusableAreasBegin++)
		{
			// each of the current right atom's FusableArea entrants need to be compared against the left atom's FusableArea entrants.  
			auto currentRightAtomsBegin = in_otherMassPtr->atoms.begin();
			auto currentRightAtomsEnd = in_otherMassPtr->atoms.end();
			for (; currentRightAtomsBegin != currentRightAtomsEnd; currentRightAtomsBegin++)
			{
				auto currentRightAtomFusableAreasBegin = currentRightAtomsBegin->second->fusableAreaMap.begin();
				auto currentRightAtomFusableAreasEnd = currentRightAtomsBegin->second->fusableAreaMap.end();
				for (; currentRightAtomFusableAreasBegin != currentRightAtomFusableAreasEnd; currentRightAtomFusableAreasBegin++)
				{
					/*
					std::cout << "Comparing left fusable point (" << leftFusableAreasBegin->second.pointAgent.x << ", "
						<< leftFusableAreasBegin->second.pointAgent.y << ", "
						<< leftFusableAreasBegin->second.pointAgent.z << ") to Right atom fusable point ("

						<< currentRightAtomFusableAreasBegin->second.pointAgent.x << ", "
						<< currentRightAtomFusableAreasBegin->second.pointAgent.y << ", "
						<< currentRightAtomFusableAreasBegin->second.pointAgent.z << ") " << std::endl;
					*/

					if (leftFusableAreasBegin->second == currentRightAtomFusableAreasBegin->second)	// the fusable areas match; do the logic.
					{
						collisionReturnResult = AtomicBondingResult(leftAtom->second);	// return the pointer to the left atom, to signifiy it collided.
						std::cout << "!! Left atom collision detected. " << std::endl;
						goto quickEnd;	// use goto to exit this nested loop set.
					}
				}
			}
		}

	}
	quickEnd:	// for quick exit.

	return collisionReturnResult;
}

bool PMass::collideAtomIntoExistingMass(int in_idToCollide)
{
	bool collisionReturnResult = false;

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

	std::cout << "ID to collide is: " << in_idToCollide << std::endl;

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
					collisionReturnResult = true;
					currentMassState = PMassState::POINT_IN_FUSABLE_AREA;			// whenever two atoms collide in a fusable area, 
				}															// the point to return will be somewhere in there.
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
				if (collidingAtomMapBegin->second == existingFusedAreasBegin->second)	// if the colliding atoms' fused area merges with this one, do the following
				{
					std::cout << "!! ################################################### Found match against existing fused area! " << std::endl;
					existingFusedAreasBegin->second += collidingAtomMapBegin->second;	// append the collding FusableArea into the one that 
																						// already exists in fusedAreas.
					//insertNewFusedArea(collidingAtomMapBegin->second);
					fusableAreastoRemove.push_back(collidingAtomMapBegin->second);

					collisionReturnResult = true;
					currentMassState = PMassState::POINT_IN_FUSABLE_AREA;				// whenever two atoms collide in a fusable area, 
																						// the point to return will be somewhere in there.
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

	std::cout << "!! Size of existing fusable areas: " << fusedAreas.size() << std::endl;

	return collisionReturnResult;
}

void PMass::insertNewFusedArea(FusableArea in_fusableArea)
{
	int currentFusedAreaSize = fusedAreas.size();
	fusedAreas[currentFusedAreaSize] = in_fusableArea;
};

void PMass::printAtomIDs()
{
	std::cout << "Unique atomic for this PMass are: " << std::endl;
	auto atomsBegin = atoms.begin();
	auto atomsEnd = atoms.end();
	for (; atomsBegin != atomsEnd; atomsBegin++)
	{
		std::cout << "First key: " << atomsBegin->first << " ";
		std::cout << "Original mass ID: " << atomsBegin->second->originalMassID << " | point: " 
			<< atomsBegin->second->atomCorePoint.x 
			<< ", " << atomsBegin->second->atomCorePoint.y
			<< ", " << atomsBegin->second->atomCorePoint.z << "| expansion interval: " << atomsBegin->second->expansionInterval << std::endl;
	}
	std::cout << std::endl;
}


std::set<int> PMass::getAtomIds()
{
	std::set<int> returnSet;
	auto atomsBegin = atoms.begin();
	auto atomsEnd = atoms.end();
	for (; atomsBegin != atomsEnd; atomsBegin++)
	{
		returnSet.insert(atomsBegin->first);
	}
	return returnSet;
}

void PMass::expandAllAtoms()
{
	auto atomsBegin = atoms.begin();
	auto atomsEnd = atoms.end();
	for (; atomsBegin != atomsEnd; atomsBegin++)
	{
		atomsBegin->second->expand();
	}
}

PMassResult PMass::determineAndReturnResult()
{
	PMassResult returnResult;	// remember, by default the bool value is false.
	std::cout << "::-> START: Calling determine result..." << std::endl;
	switch (currentMassState)
	{
		case PMassState::POINT_AT_CORE:
		{
			std::cout << "::-> found point at core!" << std::endl;
			PMassResult foundResult(getFirstAtomPtr()->atomCorePoint, true, getAtomIds());
			returnResult = foundResult;
			break;
		}
		case PMassState::POINT_IN_FUSABLE_AREA:
		{
			// for now (1/21/2022), we'll just use the point of the first fusable area we find.
			auto firstFusableArea = fusedAreas.begin();
			PMassResult foundResult(firstFusableArea->second.pointAgent, true, getAtomIds());
			returnResult = foundResult;
			break;
		}
	}
	std::cout << "::-> FINISHED: Calling determine result..." << std::endl;
	return returnResult;
}