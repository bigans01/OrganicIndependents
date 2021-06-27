#include "stdafx.h"
#include "TerminatingSetContainer.h"

void TerminatingSetContainer::insertKeyIntoTracingSet(int in_lineID, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	tracingSetMap[in_lineID].set.insert(in_blockKey);
}

void TerminatingSetContainer::buildTerminatingSets()
{
	// build terminating set for line 0
	auto line_0_term_1_begin = tracingSetMap[1].set.begin();
	auto line_0_term_1_end = tracingSetMap[1].set.end();
	for (line_0_term_1_begin; line_0_term_1_begin != line_0_term_1_end; line_0_term_1_begin++)
	{
		terminatingSetMap[0].set.insert(*line_0_term_1_begin);
	}

	auto line_0_term_2_begin = tracingSetMap[2].set.begin();
	auto line_0_term_2_end = tracingSetMap[2].set.end();
	for (line_0_term_2_begin; line_0_term_2_begin != line_0_term_2_end; line_0_term_2_begin++)
	{
		terminatingSetMap[0].set.insert(*line_0_term_2_begin);
	}






	// build terminating set for line 1
	auto line_1_term_2_begin = tracingSetMap[2].set.begin();
	auto line_1_term_2_end = tracingSetMap[2].set.end();
	for (line_1_term_2_begin; line_1_term_2_begin != line_1_term_2_end; line_1_term_2_begin++)
	{
		terminatingSetMap[1].set.insert(*line_1_term_2_begin);
	}

	auto line_1_term_0_begin = tracingSetMap[0].set.begin();
	auto line_1_term_0_end = tracingSetMap[0].set.end();
	for (line_1_term_0_begin; line_1_term_0_begin != line_1_term_0_end; line_1_term_0_begin++)
	{
		terminatingSetMap[1].set.insert(*line_1_term_0_begin);
	}




	// build terminating set for line 2
	auto line_2_term_0_begin = tracingSetMap[0].set.begin();
	auto line_2_term_0_end = tracingSetMap[0].set.end();
	for (line_2_term_0_begin; line_2_term_0_begin != line_2_term_0_end; line_2_term_0_begin++)
	{
		terminatingSetMap[2].set.insert(*line_2_term_0_begin);
	}

	auto line_2_term_1_begin = tracingSetMap[1].set.begin();
	auto line_2_term_1_end = tracingSetMap[1].set.end();
	for (line_2_term_1_begin; line_2_term_1_begin != line_2_term_1_end; line_2_term_1_begin++)
	{
		terminatingSetMap[2].set.insert(*line_2_term_1_begin);
	}
}

std::map<int, TerminatingSet> TerminatingSetContainer::constructSpecialSet(int in_lineID, int in_lineToExclude)
{
	std::map<int, TerminatingSet> returnSet;
	//returnSet[in_lineID] = terminatingSetMap[in_lineID];		// transfer the set map.
	//returnSet[in_lineID].set.erase
	if (in_lineID == 0)
	{
		if (in_lineToExclude == 1)
		{
			returnSet[0] = tracingSetMap[2];
		}
		else if (in_lineToExclude == 2)
		{
			returnSet[0] = tracingSetMap[1];
		}
	}

	if (in_lineID == 1)
	{
		if (in_lineToExclude == 0)
		{
			returnSet[1] = tracingSetMap[2];
		}
		else if (in_lineToExclude == 2)
		{
			returnSet[1] = tracingSetMap[0];
		}
	}

	if (in_lineID == 2)
	{
		if (in_lineToExclude == 0)
		{
			returnSet[2] = tracingSetMap[1];
		}
		else if (in_lineToExclude == 1)
		{
			returnSet[2] = tracingSetMap[0];
		}
	}

	return returnSet;
}