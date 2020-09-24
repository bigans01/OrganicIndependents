#pragma once

#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H

#include <map>
#include "Message.h"

class MessageInterpreter
{
	public:
		

		// request maps
		int requestIndex = 0;
		std::map<int, Message> pendingRequests;
		std::map<int, Message> completedRequests;

		// response maps
		int responseIndex = 0;
		std::map<int, Message> pendingResponses;
		std::map<int, Message> completedResponses;

		void moveResponseToCompleted(int in_responseKeyInt);
		void moveRequestToCompleted(int in_responseKeyInt);

		void insertRequestToPending(Message in_message);
		void insertResponseToPending(Message in_message);

};

#endif
