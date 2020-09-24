#include "stdafx.h"
#include "MessageInterpreter.h"


void MessageInterpreter::moveResponseToCompleted(int in_responseKeyInt)
{
	completedResponses[in_responseKeyInt] = pendingResponses[in_responseKeyInt];
	pendingResponses.erase(in_responseKeyInt);
}

void MessageInterpreter::moveRequestToCompleted(int in_responseKeyInt)
{
	completedRequests[in_responseKeyInt] = pendingRequests[in_responseKeyInt];
	pendingRequests.erase(in_responseKeyInt);
}

void MessageInterpreter::insertRequestToPending(Message in_message)		
{
	pendingRequests[in_message.messageID] = in_message;
}

void MessageInterpreter::insertResponseToPending(Message in_message)
{
	pendingResponses[in_message.messageID] = in_message;
}

int MessageInterpreter::fetchRequestIndexAndIncrement()
{
	int returnVal = requestID;	// get the value to return
	requestID++;					// increment the request ID
	return returnVal;
}