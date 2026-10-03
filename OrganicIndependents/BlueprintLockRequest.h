#pragma once

#ifndef BLUEPRINTLOCKREQUEST_H
#define BLUEPRINTLOCKREQUEST_H

class BlueprintLockRequest
{
public:
	BlueprintLockRequest() {};
	BlueprintLockRequest(int in_approvedRequestID)
	{
		isRequestValid = true;
		requestAssignedTicket = in_approvedRequestID;
	}
	bool isRequestValid = false;
	int requestAssignedTicket = 0;
};

#endif
