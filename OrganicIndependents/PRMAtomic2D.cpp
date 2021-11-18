#include "stdafx.h"
#include "PRMAtomic2D.h"

void PRMAtomic2D::setupMethod(Message in_message)
{
	switch (in_message.messageType)
	{
		case MessageType::PRMA2DRESOLVER_XY:
		{
			std::cout << "!! Found message for PRMA2DRESOLVER_XY. " << std::endl;
			resolverPtr = std::unique_ptr<PRMA2DResolverBase>(new PRMA2DResolverXY());
			resolverPtr->initializeFromMessage(std::move(in_message)); // For PRMA2DResolverXY, this will create one ResolverLink for each point,
																	   // initialize the PMass in said link, 
																	   // and then initialize AND instantiate the initial atom of said PMass.
			resolverType = PRMTypeEnum::ATOMIC2DXY;		// set this value, so that any query to this the enum value of the exact resolver we used we used,
														// which would be PRMA2DResolverXY
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