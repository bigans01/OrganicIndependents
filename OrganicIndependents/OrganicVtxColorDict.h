/*------------------------------------------------------------------------------------------

--OrganicVtxColorDict.h (Last Update: 9/4/2017)

Description: contains one or more entries of OrganicVtxColorMetaArrays (?)

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORDICTIONARY_H
#define ORGANICVTXCOLORDICTIONARY_H

#include "OrganicVtxColorMetaArray.h"
#include <unordered_map>

class OrganicVtxColorDict
{
	private:
		friend class OrganicSystem;
		friend class EnclaveManifestFactoryT1;
		friend class EnclaveManifest;
		friend class OrganicJobManager;
		friend class OrganicRawManifest;
		std::unordered_map<std::string, OrganicVtxColorMetaArray> dictionary;	// unordered map that actually contains the dictionary objects
		OrganicVtxColorDict();											// required so that te dictionary can be added into the unordered map
};

#endif
