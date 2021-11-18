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
	compareMasses();

	// loop until each PMass that is pointed-to by the entries in pMassPtrMap contain > 1 atom;
	// this indicates that the pointed-to PMass has collided with something.
	int numberOfAttempts = 10;
	bool isRunComplete = false;
	for (int x = 0; x < numberOfAttempts; x++)
	{
		// do logic here...
		// ...
		// ...


		if (isRunComplete == true)
		{
			break;	// break out of the loop if complete.
		}
	}
	
}

void PRMA2DResolverXY::compareMasses()
{
	// first, find all PMass that has isn't bound yet.
	int numberOfUnbondedMasses = 0;
	auto massesBegin = pMassPtrMap.begin();
	auto massesEnd = pMassPtrMap.end();
	for (; massesBegin != massesEnd; massesBegin++)
	{
		if (massesBegin->second->returnAtomStates() == PAtomState::UNBONDED)
		{
			numberOfUnbondedMasses++;
		}
	}
	std::cout << ">> Number of unbonded masses is: " << numberOfUnbondedMasses << std::endl;
}