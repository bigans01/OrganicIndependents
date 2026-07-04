#pragma once

#ifndef FANBASE_H
#define FANBASE_H

#include "FanData.h"
#include "EnclaveBlockVertexTri.h"

/*

Description: base class that stores common functions that would be used between "Fan" type objects, such as ThinFan and FatFan.
No boost logic is used here; rather, that logic is defined in the ThinFan and FatFan children of this class.

*/

class FanBase
{
	public:

		// virtual functions
		virtual int getPointAtIndex(int in_pointArrayIndex) = 0;
		virtual void fillPointIndex(int in_pointArrayIndex, int in_pointID) = 0;
		virtual FanData getFanData() = 0;
		virtual void printPoints() = 0;
		virtual void buildFromFanData(FanData in_fanData) = 0;
		virtual bool setFanEmptyNormalDebug(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices) = 0;
		virtual std::vector<unsigned int> fetchExpandedFanIndices() = 0;

		// base class functions
		int getNumberOfTrianglesInFan();

		TriangleMaterial materialID = TriangleMaterial::NOVAL;	// the rendering material of the triangle (dirt, stone, wood, etc)
		unsigned char numberOfTertiaries = 0;	// this value represents the number of individual triangles that make up the fan.
		BoundaryPolyIndicator faceAlignment;	// used to indicate if the individual triangles of the fan are aligned to a border (such as POS_X),
												// or are scab children / parents.

		bool usesTileCoords = true;		// this bool determines whether or not this fan uses tile coords in a texture atlas,
										// or if the EnclaveBlockVertex objects that make up the fan need to use their UV float values
										// for texturing. Default value is true, which should indicate it's going to use tiling.

		void setFanEmptyNormal(ECBPolyPoint in_emptyNormal) { emptyNormal = in_emptyNormal; }

		ECBPolyPoint getFanEmptyNormal() { return emptyNormal; }

	protected:
		ECBPolyPoint emptyNormal;	// empty normal that stores the direction of "empty space"; when writing to a VBOL, this is not stored on disk,
									// as it is not necessary to do when some simple math on the points of the FanBase can be used to recreate this value.
};

#endif