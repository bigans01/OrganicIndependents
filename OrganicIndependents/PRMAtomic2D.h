#pragma once

#ifndef PRMATOMIC2D_H
#define PRMATOMIC2D_H

#include "PointResolutionMethod.h"

// headers exclusive to PRMAtomic2D
#include <mutex>
#include "PRMA2DResolverBase.h"
#include "PRMA2DResolverXY.h"
#include "ResolverLinkMap.h"

class PRMAtomic2D : public PointResolutionMethod
{
	public:
		void setupMethod(Message in_message);		// 1. Call this first, so that the resolverPtr can be set appropriately.
		void run();																// 2. Once all the links are loaded into the underlying resolverPtr's instance, go ahead and run it.
		PRResult getResult();
	private:
		std::unique_ptr<PRMA2DResolverBase> resolverPtr = nullptr;	// type of resolver base will be determined by the value of the Message
};

#endif
