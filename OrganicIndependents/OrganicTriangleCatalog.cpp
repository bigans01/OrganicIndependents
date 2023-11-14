#include "stdafx.h"
#include "OrganicTriangleCatalog.h"

void OrganicTriangleCatalog::rebuildCatalog(std::map<int, ECBPoly>* in_polyMapRef, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	// clear out the catalog.
	catalog.clear();

	auto blueprintPolysBegin = (*in_polyMapRef).begin();
	auto blueprintPolysEnd = (*in_polyMapRef).end();
	for (; blueprintPolysBegin != blueprintPolysEnd; blueprintPolysBegin++)
	{

		// ECBPOLY_FIX
		//ECBPoly translatedPoly = blueprintPolysBegin->second;

		//std::map<int, ECBPoly> dummyMap;
		//dummyMap[blueprintPolysBegin->first] = translatedPoly;
		//auto translatedPolyIter = dummyMap.find(blueprintPolysBegin->first);

		OrganicTriangle testTriangle(&blueprintPolysBegin->second, blueprintPolysBegin->first);

		// finally, insert each OrganicTriangle into the catalog.
		catalog[blueprintPolysBegin->first] = testTriangle;
	}

	std::cout << "(OrganicTriangleCatalog): Catalog rebuild successful; built " << catalog.size() << " OrganicTriangles for catalog. " << std::endl;

}

void OrganicTriangleCatalog::insertOrganicTriangleIntoCatalog(std::map<int, ECBPoly>* in_polyMapRef,
	EnclaveKeyDef::EnclaveKey in_blueprintKey,
	int in_newOrganicTriangleCatalogID)
{

	// ECBPOLY_FIX
	auto foundECBPoly = (*in_polyMapRef).find(in_newOrganicTriangleCatalogID);	// the ECBPoly to build from should already exist in the ECBPoly map.
	//ECBPoly translatedPoly = foundECBPoly->second;

	//std::map<int, ECBPoly> dummyMap;
	//dummyMap[foundECBPoly->first] = translatedPoly;
	//auto translatedPolyIter = dummyMap.find(foundECBPoly->first);

	OrganicTriangle testTriangle(&foundECBPoly->second, foundECBPoly->first);
	catalog[foundECBPoly->first] = testTriangle;

}

void OrganicTriangleCatalog::deleteOrganicTriangleFromCatalog(int in_organicTriangleCatalogIDToDelete)
{
	catalog.erase(in_organicTriangleCatalogIDToDelete);
}

std::set<int> OrganicTriangleCatalog::getMorphLodToBlockAffectedOrganicTriangles(EnclaveKeyDef::EnclaveKey in_oreKey)
{
	std::set<int> returnSet;
	auto otCatalogBegin = catalog.begin();
	auto otCatalogEnd = catalog.end();
	for (; otCatalogBegin != otCatalogEnd; otCatalogBegin++)
	{
		auto checkIfOreExists = otCatalogBegin->second.polyMap.allTracedEnclaves.find(in_oreKey);
		if (checkIfOreExists != otCatalogBegin->second.polyMap.allTracedEnclaves.end())
		{
			returnSet.insert(otCatalogBegin->first);
		}
	}
	return returnSet;
}