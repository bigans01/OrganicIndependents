#pragma once

#ifndef TERMINATINGSETCONTAINER_H
#define TERMINATINGSETCONTAINER_H

#include <map>
#include "TerminatingSet.h"

class TerminatingSetContainer
{
public:
	std::map<int, TerminatingSet> tracingSetMap;
	std::map<int, TerminatingSet> terminatingSetMap;
	void insertKeyIntoTracingSet(int in_lineID, EnclaveKeyDef::EnclaveKey in_blockKey);
	void buildTerminatingSets();
	std::map<int, TerminatingSet> constructSpecialSet(int in_lineID, int in_lineToExclude);
};

#endif
