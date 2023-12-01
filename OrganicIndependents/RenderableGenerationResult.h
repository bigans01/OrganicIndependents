#pragma once

#ifndef RENDERABLEGENERATIONRESULT_H
#define RENDERABLEGENERATIONRESULT_H

#include "Operable3DEnclaveKeySet.h"
#include "OrganicFanGroup.h"
#include <map>

/*

Description: Currently designed only for TILE based classes that are derived from RenderableTriangleBase,
this simple container class is meant to store any fans produced by a call to RenderableTriangleBase::generate(),
so that it can be accessed by other functions. See it's usage in the function, RenderableTriangleHandler::generateBlockTrianglesFromSecondaries.

*/

class RenderableGenerationResult
{
	public:
		RenderableGenerationResult() {};	// default constructor, minimal usage.
		RenderableGenerationResult(Operable3DEnclaveKeySet in_invalidBlockSet,
			std::map<int, OrganicFanGroup> in_producedFans) :
			invalidBlockSet(in_invalidBlockSet),
			producedFans(in_producedFans)
		{};

		Operable3DEnclaveKeySet invalidBlockSet;	// invalid blocks produced by the generate() call should go here.
		std::map<int, OrganicFanGroup> producedFans;	// the actual fans produced by the call to generate(); the int key value represents the block-key-to-integer value.
	private:
};

#endif
