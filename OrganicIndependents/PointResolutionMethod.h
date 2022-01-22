#pragma once

#ifndef POINTRESOLUTIONMETHOD_H
#define POINTRESOLUTIONMETHOD_H

#include "Message.h"
#include "PRResult.h"
#include "ResolverLinkMap.h"
#include "PRMTypeEnum.h"

class PointResolutionMethod
{
	public:
		virtual void setupMethod(Message in_message) = 0;
		virtual void run() = 0;
		virtual ResolverLinkMap getResult() = 0;
	protected:
		PRMTypeEnum resolverType = PRMTypeEnum::NOTYPE;	// must be configured in the derivative class' setupMethod function

};

#endif
