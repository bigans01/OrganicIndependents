#pragma once

#ifndef RECONMANAGERWORKPIPE_H
#define RECONMANAGERWORKPIPE_H

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

/*
* 
* Description: The ReconManagerWorkPipe is essentially a specialized queue, that uses a std::vector under the hood. Certain tickets to 
* be inserted will take precedence over others, allowing them to be inserted into the middle of the "queue" by alloing a line "cut-in"; 
* the purpose of this, when applied to blueprint processing and exporting, is to allow for a thread to insert a exporting/pickup ticket that
* will be processed in a reasonable time, even if there is a large number of blueprints to process. This would guarantee that the 
* thread waiting on blueprints will have the ability to routinely pickup on blueprints that are ready for exporting/have been processed,
* even if there 100 blueprint processing tickets back-to-back.
* 
* This class is exclusively used by ReconManagerV2, which is the next iteration of the ReconstitutionManager class.

*/

enum class RMWorkPipeTicketEnum
{
	PROCESS_BLUEPRINTS,
	MOVE_BLUEPRINTS_FROM_DOCK
};

class ReconManagerWorkPipe
{
	public:
		// Below: insert either a RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS or RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK ticket;
		// There should only ever be one RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK in the pipe at a given time, and it has precedence
		// over the RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS, if the contiguous amount of PROCESS_BLUEPRINT tickets exceeds the processBlueprintCongestionLimit.
		// When this limit is exceeded, the next attempt to insert a MOVE_BLUEPRINTS_FROM_DOCK ticket will be inserted at position X in the rmWorkPipe vector,
		// where X is equal to the value of processBlueprintCongestionLimit + 1. This will guarantee that it the MOVE_BLUEPRINTS_FROM_DOCK is processed
		// sooner, rather than later towards the end of the vector.
		void insertTicket(RMWorkPipeTicketEnum in_ticket)
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			if (in_ticket == RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS)
			{
				rmWorkPipe.push_back(in_ticket);
			}

			else if 
			(
				(in_ticket == RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK)
				&&
				(hasMoveTicket == false)
			)
			{
				// check if there's a congestion limit hit...if there is, allow the 
				// MOVE_BLUEPRINTS_FROM_DOCK to cut in line, and then set hasMoveTicket to true.

				if (scanForCongestionLimit())
				{
					allowMoveCutIn(in_ticket);
				}
				else
				{
					rmWorkPipe.push_back(in_ticket);
				}

				hasMoveTicket = true;

				// ...otherwise, there's no congestion, just insert the ticket.
			}
		}

		// Below: mainly for debugging, print whatever tickets are in the queue to std::cout.
		void printTickets()
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			int currentTicketNumber = 0;
			for (auto& currentTicket : rmWorkPipe)
			{
				switch (currentTicket)
				{
					case RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS:
					{
						std::cout << currentTicketNumber << " -> " << "PROCESS_BLUEPRINTS" << std::endl;
						break;
					}

					case RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK:
					{
						std::cout << currentTicketNumber << " -> " << "MOVE_BLUEPRINTS_FROM_DOCK" << std::endl;
						break;
					}
				}

				currentTicketNumber++;
			}
		}

		// Below: get the value at the front of the "queue" 
		RMWorkPipeTicketEnum front()
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			return *rmWorkPipe.begin();
		}

		// Below: "pop" the "front" of the "queue"
		void pop()
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			if (*rmWorkPipe.begin() == RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK)
			{
				hasMoveTicket = false;
			}
			rmWorkPipe.erase(rmWorkPipe.begin());
		}

		// Below: check if the vecotr contains a certain ticket value.
		bool contains_value(RMWorkPipeTicketEnum in_valueToSearch)
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			bool foundValue = false;
			for (auto& currentTicket : rmWorkPipe)
			{
				if (currentTicket == in_valueToSearch)
				{
					foundValue = true;
					break;
				}
			}
			return foundValue;
		}

		// Below: return true if the vector is empty.
		bool empty()
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			return rmWorkPipe.empty();
		}

		// Below: return the number of tickets in the pipe.
		int size()
		{
			std::lock_guard<std::mutex> pipeGuard(pipeCheckMutex);
			return rmWorkPipe.size();
		}

	private:
		std::mutex pipeCheckMutex;	// for thread-safe operations (reading, writing, checking, etc)
		std::vector<RMWorkPipeTicketEnum> rmWorkPipe;	// vector "queue" that contains the tickets to process.
		int processBlueprintCongestionLimit = 4;	// if the number of consecutive PROCESS_BLUEPRINTS tickets meets or exceeds this number
													// when attempting to insert a MOVE_BLUEPRINTS_FROM_DOCK ticket, the MOVE_BLUEPRINTS_FROM_DOCK
													// will need to "cut" in line.
		
		bool hasMoveTicket = false;		// should be set to true if a MOVE_BLUEPRINTS_FROM_DOCK ticket is detected in the rmWorkPipe.

		bool scanForCongestionLimit()	// Scan the contents of the rmWorkPipe until the congestion limit is reached; if the limit is reached,
										// we will have to allow a cut-in for the MOVE_BLUEPRINTS_FROM_DOCK ticket (see usage in insertTicket function of this class)
		{ 
			bool isCongested = false;
			if (!rmWorkPipe.empty())
			{
				int consecutiveCounter = 0;
				for (auto& currentTicket : rmWorkPipe)
				{
					if (currentTicket == RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS)
					{
						consecutiveCounter++;
						if (consecutiveCounter >= processBlueprintCongestionLimit)
						{
							isCongested = true;
							break;
						}
					}
				}
			}
			return isCongested;
		}

		void allowMoveCutIn(RMWorkPipeTicketEnum in_ticketToCut)	// allow the cut in for the MOVE_BLUEPRINTS_FROM_DOCK ticket.
		{
			rmWorkPipe.insert(rmWorkPipe.begin() + processBlueprintCongestionLimit, in_ticketToCut);
		}
		                                
		


};

#endif