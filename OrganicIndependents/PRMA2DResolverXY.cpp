#include "stdafx.h"
#include "PRMA2DResolverXY.h"

void PRMA2DResolverXY::initializeFromMessage(Message in_messageToInitializeFrom)
{
	// firstly, (always) open the message.
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
		ECBPolyPoint linkPoint = in_messageToInitializeFrom.readPoint();
		ResolverLink newLink(linkPoint);
		resolverLinks.links[linkID] = newLink;	// create the link
		resolverLinks.links[linkID].initializeMassPtrAndFirstAtom(PAtomDimType::TWO_D);	// initialize its mass, and an initial unmaterialized atom.
		resolverLinks.links[linkID].materializeInitialAtom(linkID, linkPoint, .1f);	// initialize the first atom
	}
}

ECBPolyPoint PRMA2DResolverXY::generateExpandedPoint(float in_dimA, float in_dimB)
{
	return ECBPolyPoint(in_dimA, in_dimB, staticDimensionValue);
}

void PRMA2DResolverXY::runResolutionAttempt()
{

}