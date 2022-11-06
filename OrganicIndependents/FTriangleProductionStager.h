#pragma once

#ifndef FTRIANGLEPRODUCTIONSTAGER_H
#define FTRIANGLEPRODUCTIONSTAGER_H

#include "FTriangleLine.h"

class FTriangleProductionStager
{
	public:
		void insertLine(FTriangleLine in_lineToInsert);
		void printLines();
	private:
		std::vector<FTriangleLine> stagerLines;

		// flags to indicate certain things happened (duplicates, bad triangles, etc)
		bool duplicatesFound = false;

};

#endif
