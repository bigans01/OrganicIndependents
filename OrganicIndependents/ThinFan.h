#pragma once

#ifndef THINFAN_H
#define THINFAN_H

#include "FanBase.h"

class ThinFan : public FanBase
{
	public:
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// Serialize common fan base members
			ar& materialID;
			ar& numberOfTertiaries;
			ar& faceAlignment;
			ar& usesTileCoords;

			// Empty normal should not need to be saved, as that should be able to calculated immediately based on point ordering
			// ...does this need special logic?

			// Copy the thin style thinArray; number that exists is based on value of numberOfTertiaries + 2.
			ar& boost::serialization::make_array(thinArray, numberOfTertiaries + 2);

		}


		int getPointAtIndex(int in_pointArrayIndex);
		void fillPointIndex(int in_pointArrayIndex, int in_pointID);
		FanData getFanData();
		void printPoints();
		void buildFromFanData(FanData in_fanData);
		bool setFanEmptyNormalDebug(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices);
		std::vector<unsigned int> fetchExpandedFanIndices();

		unsigned char thinArray[8] = { 0 };	// "Thin" means we use unsigned chars to index the points.

};

#endif
