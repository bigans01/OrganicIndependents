#pragma once

#ifndef BORDERDATAMAP_H
#define BORDERDATAMAP_H

#include "ECBPPOrientations.h"
#include "BlockBorderMDCorner.h"
#include "BlockBorderMDLine.h"
#include "BlockBorderMDFace.h"

class BorderDataMap
{
public:
	BorderDataMap();
	struct OrientationHasher
	{
		std::size_t operator()(const ECBPPOrientations& k) const
		{
			return static_cast<std::size_t>(k);
		}
	};

	std::unordered_map<ECBPPOrientations, BlockBorderMDCorner, OrientationHasher> cornerMap;	// unordered map for storing corner meta data
	std::unordered_map<ECBPPOrientations, BlockBorderMDLine, OrientationHasher> bdLinesMap;		// " for storing line meta data
	std::unordered_map<ECBPPOrientations, BlockBorderMDFace, OrientationHasher> faceMap;
	std::unordered_map<ECBPPOrientations, ECBPPOrientations, OrientationHasher> faceInterceptMap;
};

#endif
