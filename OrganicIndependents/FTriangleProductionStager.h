#pragma once

#ifndef FTRIANGLEPRODUCTIONSTAGER_H
#define FTRIANGLEPRODUCTIONSTAGER_H

#include <vector>
#include "FTriangleLine.h"

class FTriangleProductionStager
{
	public:
		void insertLine(FTriangleLine in_lineToInsert);
	private:
		std::vector<FTriangleLine> stagerLines;

};

#endif
