#pragma once

#ifndef PRMATOMIC2D_H
#define PRMATOMIC2D_H

#include "PointResolutionMethod.h"

// headers exclusive to PRMAtomic2D
#include <mutex>
#include "PRMA2DResolverBase.h"

class PRMAtomic2D : public PointResolutionMethod
{
	public:
		void setupMethod(Message in_message);
		void run();
		PRResult getResult();
	private:
		std::shared_ptr<PRMA2DResolverBase> resolverPtr = nullptr;	// type of resolver base will be determined by the value of the Message
};

#endif
