#pragma once

#ifndef PRRESULT_H
#define PRRESULT_H

#include "PMassResult.h"

class PRResult
{
	public:
		void insertResult(PMassResult in_result)
		{
			acquiredResults[int(acquiredResults.size())] = in_result;
		}
		std::map<int, PMassResult> acquiredResults;
};

#endif
