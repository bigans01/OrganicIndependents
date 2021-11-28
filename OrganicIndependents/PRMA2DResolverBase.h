#pragma once

#ifndef PRM2ARESOLVERBASE_H
#define PRM2ARESOLVERBASE_H

// eg, an example class names that derives this base would be: PRMA2DResolverXY, PRMA2DResolverYZ, PRMA2DREsolverXZ
#include "ECBPolyPoint.h"
#include "Message.h"

class PRMA2DResolverBase
{
	public:
		virtual void initializeFromMessage(Message in_messageToInitializeFrom) = 0;
		virtual void runResolutionAttempt() = 0;
	protected:
		float staticDimensionValue = 0.0f;	// the single dimension value that isn't affected by 2d operations (i.e, this would be  the value of "Z" when doing XY)

};

#endif
