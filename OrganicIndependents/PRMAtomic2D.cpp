#include "stdafx.h"
#include "PRMAtomic2D.h"

void PRMAtomic2D::setupMethod(Message in_message, ResolverLinkMap in_linkMap)
{
	switch (in_message.messageType)
	{
		case MessageType::PRMA2DRESOLVER_XY:
		{
			resolverPtr = std::unique_ptr<PRMA2DResolverBase>(new PRMA2DResolverXY());
			resolverPtr->transferLinksAndSetStaticDimValue(in_linkMap);
			break;
		}
	}
}

void PRMAtomic2D::run()
{
	resolverPtr->runResolutionAttempt();
}

PRResult PRMAtomic2D::getResult()
{
	PRResult returnResult;
	return returnResult;
}