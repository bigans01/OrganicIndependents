#pragma once

#ifndef ORGANICTRIANGLECATALOG_H
#define ORGANICTRIANGLECATALOG_H

#include "EnclaveKeyDef.h"
#include <unordered_map>
#include <map>
#include "ECBPoly.h"
#include "OrganicTriangle.h"
#include <iostream>
#include <set>

class OrganicTriangleCatalog
{
public:
	void rebuildCatalog(std::map<int, ECBPoly>* in_polyMapRef, EnclaveKeyDef::EnclaveKey in_blueprintKey);	// pass in a pointer to the blueprint's ECBPoly map to build it.
	void insertOrganicTriangleIntoCatalog(std::map<int, ECBPoly>* in_polyMapRef,
		EnclaveKeyDef::EnclaveKey in_blueprintKey,
		int in_newOrganicTriangleCatalogID);
	void deleteOrganicTriangleFromCatalog(int in_organicTriangleCatalogIDToDelete);
	std::set<int> getMorphLodToBlockAffectedOrganicTriangles(EnclaveKeyDef::EnclaveKey in_oreKey);
private:
	std::map<int, OrganicTriangle> catalog;
};

#endif
