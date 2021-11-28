#pragma once

#ifndef RESOLVERLINK_H
#define RESOLVERLINK_H

#include "ECBPolyPoint.h"
#include "PMass.h"
#include <mutex>
#include "PAtomDimType.h"
#include "PAtom2DXY.h"

class ResolverLink
{
	public:
		ResolverLink() {};
		ResolverLink(ECBPolyPoint in_originalPoint) : originalPoint(in_originalPoint), resolvedPoint(in_originalPoint) {};
		
		void initializeMassPtrAndFirstAtom(PAtomDimType in_pAtomDimType, int in_uniqueAtomID, ECBPolyPoint in_atomCorePoint, float in_expansionInterval)
		{
			pMassPtr = std::shared_ptr<PMass>(new PMass());		// create a new mass
			switch (in_pAtomDimType)	// use a switch to etermine the type (1D, 2D, 3D)
			{
				case PAtomDimType::TWO_D_XY:
				{
					std::shared_ptr<PAtomBase> twoDAtom(new (PAtom2DXY));
					twoDAtom->materialize(in_uniqueAtomID, in_atomCorePoint, in_expansionInterval);
					pMassPtr->insertAtom(twoDAtom);	// insert a single unmaterialized atom into the mass
				}
			}	
		}

		void materializeInitialAtom(int in_uniqueAtomID, ECBPolyPoint in_atomCorePoint, float in_expansionInterval)
		{
			pMassPtr->getFirstAtomPtr()->materialize(in_uniqueAtomID, in_atomCorePoint, in_expansionInterval);
		}

		std::shared_ptr<PMass> getPMassPtr()
		{
			return pMassPtr;
		}

		void setPMassPtr(std::shared_ptr<PMass> in_pMassPtr)
		{
			pMassPtr = in_pMassPtr;
		}

		ECBPolyPoint originalPoint;
		ECBPolyPoint resolvedPoint;
		std::shared_ptr<PMass> pMassPtr;
};

#endif
