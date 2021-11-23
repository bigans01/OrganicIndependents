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
		resolverLinks.links[linkID].initializeMassPtrAndFirstAtom(PAtomDimType::TWO_D);	// initialize its mass, and an initial unmaterialized atom.
		resolverLinks.links[linkID].materializeInitialAtom(linkID, linkPoint, .1f);	// initialize the first atom

		pMassPtrMap[linkID] = resolverLinks.links[linkID].getPMassPtr();			// store a reference to the PMass, in the pMassPtrMap; 
																					// we will be iterating through the pMassPtrMap when doing comparisons
																					// of one mass to another.
		std::cout << "Number of shared_ptr counts for " << linkID << ": " << pMassPtrMap[linkID].use_count() << std::endl;
	}

	// in this derivative of the PRMAtomic2D, the Z is constant; so just get the value of Z from the first point.
	staticDimensionValue = resolverLinks.links.begin()->second.originalPoint.z;
}

ECBPolyPoint PRMA2DResolverXY::generateExpandedPoint(float in_dimA, float in_dimB)
{
	return ECBPolyPoint(in_dimA, in_dimB, staticDimensionValue);
}

void PRMA2DResolverXY::runResolutionAttempt()
{
	generateComparisonCount();	// generate the initial comparison count
	while (areComparisonsDone() == false) // continue until comparisons are not done; i.e, 
		                                  // while the counter is not 0 and the resolutionAcquired flag is false
	{
		std::cout << "Running comparison loop iteration..." << std::endl;
		bool collisionDetected = compareUnbondedMasses();
		if (collisionDetected == true)	// if a collision was detected, it means that a new PMass was created, and at least one PAtom is no longer bonded.
										// we must now check the status of all atoms to see if they are ALL bonded. If this is true, 
										// the comparison run is complete.
		{
			checkIfResolutionAchieved();	// if there are no unbonded masses, we are done, and can exit the loop.
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
	// If a collision was detected, we would then set the value of collisionDetected to true. 
	produceAndRunComparisonSets(unbondedMassSet, allMassSet);

	std::cout << ">> Number of unbonded masses is: " << numberOfUnbondedMasses << std::endl;

	return collisionDetected;
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
	auto comparisonsBegin = comparisonSetMap.begin();
	auto comparisonsEnd = comparisonSetMap.end();
	for (; comparisonsBegin != comparisonsEnd; comparisonsBegin++)
	{
		//std::cout << "PMass with ID " << comparisonsBegin->first << " will be compared to these values: ";
		auto currentComparisonSetBegin = comparisonsBegin->second.begin();
		auto currentComparisonSetEnd = comparisonsBegin->second.end();
		for (; currentComparisonSetBegin != currentComparisonSetEnd; currentComparisonSetBegin++)
		{
			//std::cout << " " << *currentComparisonSetBegin;

			int selectedUnbonded = comparisonsBegin->first;
			int indexToCompareTo = *currentComparisonSetBegin;

			// run the comparison, by temporarily adding the atom into the mass;
			// if returning struct contains true bool value, do appropriate logic and return out of
			// this call to produceAndRonComparisonSets.
			pMassPtrMap[selectedUnbonded]->checkForCollisionAgainstOtherMass(pMassPtrMap[indexToCompareTo]);
		}
		//std::cout << std::endl;
	}

	return wasCollisionDetected;
}