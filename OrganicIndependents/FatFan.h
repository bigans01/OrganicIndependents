#pragma once

#ifndef FATFAN_H
#define FATFAN_H

#include "FanBase.h"

/*

Description: this derivative of FanBase has the values of it's thinArray be that of of an int, instead of unsigned char, 
hence the term "fat." This is situations in which there may be more than 256 points in the block to work with. An extreme edge
case to be sure, but better to be safe than sorry.

The FatFan derivative of FanBase is the version used in OrganicWrappedBBFan, when it is arranging its data. Because OrganicWrappedBBFan
eventually gets passed to the FanManager class where it gets processed, only the FatFan needs functions that allow its empty normal to be set;
The contents of a FatFan will very likely ultimately end up in a ThinFan in most scenarios -- but giving the OrganicWrappedBBFan the FatFan 
(the largest possible FanBase derivative) seemed fine.

The function setEmptyNormalAndCheckAlignment must be called by OrganicWrappedBBFan, to check if the empty normal
currently produced by the points of the poly aligns to it; if the inverse normal of the triangle is better aligned to the empty normal,
the points of the poly must be swapped. Afterwards, the normal is set. 

*/

class FatFan : public FanBase
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

			// Copy the fat style thinArray; number that exists is based on value of numberOfTertiaries + 2.
			ar& boost::serialization::make_array(thinArray, numberOfTertiaries + 2);
		}

		int getPointAtIndex(int in_pointArrayIndex);
		void fillPointIndex(int in_pointArrayIndex, int in_pointID);
		FanData getFanData();
		void printPoints();
		void buildFromFanData(FanData in_fanData);
		bool setEmptyNormalAndCheckAlignment(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices);	// same logic as debug/testing below, without std::cout output.
																															// This function is called by OrganicWrappedBBFan, in order to determine
																															// if the points need to be swapped to align the triangle normal with the passed-in
																															// value of the empty normal. The reason this function is not found in ThinFan is
																															// because the OrganicWrappedBBFan contains a FatFan; the OrganicWrappedBBFan is responsible for
																															// re-organizing the point data of the FatFan, and nowhere else does a ThinFan get its points
																															// analyzed for re-organization. Thus, there was no need to define it as a virtual function in FanBase.
																															//
																															// The empty normal to use is calculated by converting the value of in_emptyNormal, transforming it to a unit vector,
																															// and then comparing its cross value to each of the two possible unit vectors
																															// a triangle could produce. The cross value of these two new unit vectors is compared against the normalized input empty normal;
																															// Whichever of the two triangle normals is "closest" to the unit-vector input empty normal (calculated via cross), will become the new 
																															// normal. 
																															// 
																															// If the points of the triangle need to be swapped to align closer to the passed-in empty normal, this function returns true.																																		

		bool setFanEmptyNormalDebug(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices);		// debug/testing: return true if the empty normal given by the 3 points of
																												    // the triangle is NOT closest to the given in_emptyNormal, in which case we need to flip.
		std::vector<unsigned int> fetchExpandedFanIndices();

		unsigned int thinArray[8] = { 0 };	// "Fat" means we use unsigned ints to reference the points.
};

#endif
