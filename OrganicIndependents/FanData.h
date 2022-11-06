#pragma once

#ifndef FANDATA_H
#define FANDATA_H



class FanData
{
public:
	FanData() {};
	FanData(unsigned char in_intArrayPtr[],
		unsigned short in_materialID,
		unsigned char in_numberOfTertiaries,
		//unsigned char in_faceAlignment,
		BoundaryPolyIndicator in_faceAlignment,
		ECBPolyPoint in_emptyNormal)
	{
		for (int x = 0; x < 8; x++)
		{
			pointArray[x] = in_intArrayPtr[x];
		}
		materialID = in_materialID;
		numberOfTertiaries = in_numberOfTertiaries;
		faceAlignment = in_faceAlignment;
		emptyNormal = in_emptyNormal;
	}

	FanData(unsigned int in_intArrayPtr[],
		short in_materialID,
		unsigned char in_numberOfTertiaries,
		//unsigned char in_faceAlignment,
		BoundaryPolyIndicator in_faceAlignment,
		ECBPolyPoint in_emptyNormal)
	{
		for (int x = 0; x < 8; x++)
		{
			pointArray[x] = in_intArrayPtr[x];
		}
		materialID = in_materialID;
		numberOfTertiaries = in_numberOfTertiaries;
		faceAlignment = in_faceAlignment;
		emptyNormal = in_emptyNormal;
	}

	unsigned int pointArray[8] = { 0 };
	unsigned short materialID = 0;
	unsigned char numberOfTertiaries = 0;
	//unsigned char faceAlignment = 0;
	BoundaryPolyIndicator faceAlignment;

	// for the normal
	ECBPolyPoint emptyNormal;

};

#endif
