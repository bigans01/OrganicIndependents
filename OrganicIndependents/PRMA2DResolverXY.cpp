#include "stdafx.h"
#include "PRMA2DResolverXY.h"

void PRMA2DResolverXY::initializeFromMessage(Message in_messageToInitializeFrom)
{
	// firstly, (always) open the message. This class expects the message is properly formatted
	// before it is analyzed.
	// 
	// Remember: 
	//
	// First int of message is the number of links.
	// Subsequent ints/ECBPolyPoints of message are the IDs and ECBPolyPoints for each new link.
	//
	in_messageToInitializeFrom.open();
	int numberOfLinks = in_messageToInitializeFrom.readInt();
	for (int x = 0; x < numberOfLinks; x++)
	{
		int linkID = in_messageToInitializeFrom.readInt();
		std::cout << "!! generating resolverLink having ID: " << linkID << std::endl;

		ECBPolyPoint linkPoint = in_messageToInitializeFrom.readPoint();
		ResolverLink newLink(linkPoint);
		resolverLinks.links[linkID] = newLink;	// create the link
		resolverLinks.links[linkID].initializeMassPtrAndFirstAtom(PAtomDimType::TWO_D_XY, linkID, linkPoint, .1f);	// initialize its mass, and an initial unmaterialized atom.
		pMassPtrMap[linkID] = resolverLinks.links[linkID].getPMassPtr();			// store a reference to the PMass, in the pMassPtrMap; 
																					// we will be iterating through the pMassPtrMap when doing comparisons
																					// of one mass to another.
		std::cout << "++++ printing initial atom IDs in the PMass with ID " << linkID << std::endl;
		resolverLinks.links[linkID].getPMassPtr()->printAtomIDs();
		pMassPtrMap[linkID]->printAtomIDs();
		std::cout << "++++ done printing." << std::endl;

		std::cout << "Number of shared_ptr counts for " << linkID << ": " << pMassPtrMap[linkID].use_count() << std::endl;
	}

	// in this derivative of the PRMAtomic2D, the Z is constant; so just get the value of Z from the first point.
	staticDimensionValue = resolverLinks.links.begin()->second.originalPoint.z;
}

void PRMA2DResolverXY::runResolutionAttempt()
{
	generateComparisonCount();	// generate the initial comparison count
	while (areComparisonsDone() == false) // continue until comparisons are not done; i.e, 
		                                  // while the counter is not 0 and the resolutionAcquired flag is false
	{
		std::cout << "Running comparison loop iteration..." << std::endl;
		bool massCollisionDetected = compareUnbondedMasses();
		if (massCollisionDetected == true)	// if a collision was detected, it means that a new PMass was created, and at least one PAtom is no longer bonded.
										// we must now check the status of all atoms to see if they are ALL bonded. If this is true, 
										// the comparison run is complete.
		{
			checkIfResolutionAchieved();	// if there are no unbonded masses, we are done, and can exit the loop.
		}
		else if (massCollisionDetected == false)
		{
			std::cout << "!!! No collision detected; will now expand existing UNBONDED atoms..." << std::endl;

		}
		comparisonsRemaining--;	// comparisons remaining should always be decremented at end of loop, if we didn't break out of it.
	}
}

bool PRMA2DResolverXY::areComparisonsDone()
{
	bool comparisonsDone = false;
	if
	(
		(comparisonsRemaining == 0)		// if there are no more loop iterations, we're done.
		||
		(resolutionAcquired == true)	// if the resolution was acquired, we're done.
	)
	{
		comparisonsDone = true;
		std::cout << "!!!>>> Comparisons are done, number of PMasses is: " << pMassPtrMap.size() << std::endl;
	}

	return comparisonsDone;
}

void PRMA2DResolverXY::generateComparisonCount()
{
	comparisonsRemaining = pMassPtrMap.size();	// the number of comparisons is always equal to the initial number of pMass ptrs.
}


bool PRMA2DResolverXY::compareUnbondedMasses()
{
	// bool value that indicates if collision was detected.
	bool collisionDetected = false;

	// First, find all PMass that has isn't bound yet; put these into an OperableIntSet. Additionally, create another OperableIntSet of all 
	// masses, regardless of the value of returnAtomStates for that PMass.
	int numberOfUnbondedMasses = 0;
	OperableIntSet unbondedMassSet;
	OperableIntSet allMassSet;
	auto massesBegin = pMassPtrMap.begin();
	auto massesEnd = pMassPtrMap.end();
	for (; massesBegin != massesEnd; massesBegin++)
	{
		if (massesBegin->second->returnAtomStates() == PAtomState::UNBONDED)
		{
			unbondedMassSet += massesBegin->first;
			numberOfUnbondedMasses++;
		}
		allMassSet += massesBegin->first;
	}

	// Next, pass in the OperableIntSets to a new function that compares each PMass with the ID found in the unbondedMassSet, 
	// against all values in allMassSet (minus the ID found). If there is ANY collision, return a class/struct that has a bool flag of TRUE 
	// set to indicate that there was a collision. If a collision wasn't detected, run X number of attempts, until either:
	// 
	// A.) the number of attempts is reached
	// --OR
	// B.) a collision was detected.
	//
	// The logic would be as follows; run x attempts, and call produceAndRunComparisonSets each time. If collisionFound if true,
	// stop looping via break. If false, expand all atoms in each PMass (unbonded only), at the end of that iteration, then attempt again.
	// 
	//bool collisionFound = produceAndRunComparisonSets(unbondedMassSet, allMassSet);
	bool collisionFound = false;
	for (int x = 0; x < 5; x++)	
	{
		collisionFound = produceAndRunComparisonSets(unbondedMassSet, allMassSet);
		if (collisionFound == true)
		{
			std::cout << "!! Valid mass collision detected; breaking out and returning true." << std::endl;
			collisionDetected = true;
			break;	// break out of this unbonded mass comparison as soon as we detect a collision
		}
		else if (collisionFound == false)	// if a collision wasn't found, expand all remaining unbonded atoms.
		{
			std::cout << "!! No valid mass collision detected; expanding all UNBONDED masses..." << std::endl;
			expandedUnbondedMasses();
			int expandWait = 3;
			std::cin >> expandWait;
		}
	}

	std::cout << ">> Number of unbonded masses is: " << numberOfUnbondedMasses << std::endl;

	return collisionDetected;
}

void PRMA2DResolverXY::expandedUnbondedMasses()
{
	auto unbondedMassScanBegin = pMassPtrMap.begin();
	auto unbondedMassScanEnd = pMassPtrMap.end();
	for (; unbondedMassScanBegin != unbondedMassScanEnd; unbondedMassScanBegin++)
	{
		if (unbondedMassScanBegin->second->returnAtomStates() == PAtomState::UNBONDED)
		{
			std::cout << "5 for the PMass having ID: " << unbondedMassScanBegin->first << std::endl;
			unbondedMassScanBegin->second->expandAllAtoms();
		}
	}
}

void PRMA2DResolverXY::checkIfResolutionAchieved()
{
	bool unbondedDetected = false;
	auto massCheckBegin = pMassPtrMap.begin();
	auto massCheckEnd = pMassPtrMap.end();
	for (; massCheckBegin != massCheckEnd; massCheckBegin++)
	{
		// break out of this loop on detection of any unbonded atoms.
		if (massCheckBegin->second->returnAtomStates() == PAtomState::UNBONDED)
		{
			unbondedDetected = true;
			break;
		}
	}

	// if no unbonded atoms were detected, we are done. Set the completion flag to indicate there should be no more comparisons done.
	if (unbondedDetected == false)
	{
		std::cout << "!!! Notice, no UNBONDED detected; resolution has been acquired. " << std::endl;
		resolutionAcquired = true;
	}
}

bool PRMA2DResolverXY::produceAndRunComparisonSets(OperableIntSet in_unbondedMassSet, OperableIntSet in_allMassSet)
{
	bool wasCollisionDetected = false;

	// create each comparison set, by iterating through unbonded mass set, and creating a new set that is 
	// equal to the allMassSet minus the current value we are at in the unbonded mass set.
	std::map<int, OperableIntSet> comparisonSetMap;
	auto unbondedBegin = in_unbondedMassSet.begin();
	auto unbondedEnd = in_unbondedMassSet.end();
	for (; unbondedBegin != unbondedEnd; unbondedBegin++)
	{
		OperableIntSet newSetBase = in_allMassSet;
		newSetBase -= *unbondedBegin;
		comparisonSetMap[*unbondedBegin] = newSetBase;
	}

	// testing only: print out the points to compare against.
	int selectedUnbonded = 0;
	int indexToCompareTo = 0;
	PMass rightPMassCopy;

	auto comparisonsBegin = comparisonSetMap.begin();
	auto comparisonsEnd = comparisonSetMap.end();
	for (; comparisonsBegin != comparisonsEnd; comparisonsBegin++)
	{
		//std::cout << "|| PMass with ID " << comparisonsBegin->first << " will be compared to these values: ";
		auto currentComparisonSetBegin = comparisonsBegin->second.begin();
		auto currentComparisonSetEnd = comparisonsBegin->second.end();
		for (; currentComparisonSetBegin != currentComparisonSetEnd; currentComparisonSetBegin++)
		{
			//std::cout << " " << *currentComparisonSetBegin << std::endl;

			selectedUnbonded = comparisonsBegin->first;
			indexToCompareTo = *currentComparisonSetBegin;

			std::cout << "Comparing unbonded PMass with ID " << selectedUnbonded << " to PMass with ID " << indexToCompareTo << std::endl;
			pMassPtrMap[indexToCompareTo]->printAtomIDs();
			std::cout << "----" << std::endl;

			// run the comparison, by temporarily adding the atom into the mass;
			// if returning struct contains true bool value, do appropriate logic and return out of
			// this call to produceAndRonComparisonSets.
			auto collisionResult = pMassPtrMap[selectedUnbonded]->checkForCollisionAgainstOtherMass(pMassPtrMap[indexToCompareTo]);
			if (collisionResult.bondingResultExists == true)
			{
				std::cout << ">>>>> !! Collision result found as true; left PMass atom will be collided into a copy of right-hand PMass. " << std::endl;
				//std::cout << ">>>> Printing right mass IDs one final time..." << std::endl;
				//pMassPtrMap[indexToCompareTo]->printAtomIDs();
				//std::cout << ">>>> Done printing final time..." << std::endl;
				
				rightPMassCopy = *pMassPtrMap[indexToCompareTo];
				auto leftAtomPtr = pMassPtrMap[selectedUnbonded]->getFirstAtomPtr();
				//rightPMassCopy.printAtomIDs();
				//std::cout << "!! >> Done with first print call..." << std::endl;
				rightPMassCopy.insertAtom(leftAtomPtr);
				rightPMassCopy.printAtomIDs();
				std::cout << ">>>> Finished print of atom ids for PMass copy..." << std::endl;

				int waitVal = 3;
				std::cin >> waitVal;

				wasCollisionDetected = true;
				goto quickEnd;
			}

			
		}
		std::cout << std::endl;
	}

	quickEnd:
	// if a collision was detected, insert the new PMass into pMassPtrMap, and update the resolverLinks (having IDs equal to selectedUnbonded and indexToCompareTo)
	// to have their PMass ptrs point to this new map.
	if (wasCollisionDetected == true)
	{
		int newPMassKeyValue = getHighestMassPtrMapKeyPlusOne();
		//std::cout << ">>> new PMass key value is: " << std::endl;

		// first, insert the new PMass.
		std::shared_ptr<PMass> newMassPtr(new PMass());
		*newMassPtr = rightPMassCopy;
		pMassPtrMap[newPMassKeyValue] = newMassPtr;
		//std::cout << ">>> new Mass at key " << newPMassKeyValue << " has the following atom data: " << std::endl;
		pMassPtrMap[newPMassKeyValue]->printAtomIDs();

		// remove the existing pMassPtrs at keys having selectedUnbonded and indexToCompareTo.
		pMassPtrMap.erase(selectedUnbonded);
		pMassPtrMap.erase(indexToCompareTo);

		// get the IDs of the atoms from the new pMass; use these to update the links.
		auto newPMassAtomIDs = pMassPtrMap[newPMassKeyValue]->getAtomIds();
		auto newIDsBegin = newPMassAtomIDs.begin();
		auto newIDsEnd = newPMassAtomIDs.end();
		for (; newIDsBegin != newIDsEnd; newIDsBegin++)
		{
			if (resolverLinks.links.find(*newIDsBegin) != resolverLinks.links.end())
			{
				//std::cout << ">> Updating link with ID " << *newIDsBegin << std::endl;
				resolverLinks.links[*newIDsBegin].setPMassPtr(pMassPtrMap[newPMassKeyValue]);
			}
		}

		
		std::cout << "!! Size of resolver links: " << resolverLinks.links.size() << std::endl;
		std::cout << "!! Size of pMassMptr map: " << pMassPtrMap.size() << std::endl;

		// quick debug check: let's print the IDs of each atom in the PMass that is pointed to by the links:
		auto linksBegin = resolverLinks.links.begin();
		auto linksEnd = resolverLinks.links.end();
		for (; linksBegin != linksEnd; linksBegin++)
		{
			std::cout << "Link ID " << linksBegin->first << ": " << std::endl;
			linksBegin->second.getPMassPtr()->printAtomIDs();
			auto currentState = linksBegin->second.getPMassPtr()->returnAtomStates();
			if (currentState == PAtomState::BONDED)
			{
				std::cout << "State is BONDED. " << std::endl;
			}
			if (currentState == PAtomState::UNBONDED)
			{
				std::cout << "State is UNBONDED. " << std::endl;
			}
		}
		
		
		std::cout << ">>> Collision logic complete; Press any number to continue... " << std::endl;
		int continueVal = 3;
		std::cin >> continueVal;
	}

	return wasCollisionDetected;
}

int PRMA2DResolverXY::getHighestMassPtrMapKeyPlusOne()
{
	// get the value of the highest key in the map; this + 1 will be equal to the key value that represents the newest PMass that is inserted.
	return pMassPtrMap.rbegin()->first + 1;
}