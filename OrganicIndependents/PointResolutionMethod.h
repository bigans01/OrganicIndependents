#pragma once

#ifndef POINTRESOLUTIONMETHOD_H
#define POINTRESOLUTIONMETHOD_H

#include "Message.h"
#include "PRResult.h"
#include "ResolverLinkMap.h"

class PointResolutionMethod
{
	public:
		virtual void setupMethod(Message in_message, ResolverLinkMap in_linkMap) = 0;
		virtual void run() = 0;
		virtual PRResult getResult() = 0;
	private:

};

#endif
