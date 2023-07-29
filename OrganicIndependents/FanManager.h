#pragma once

#ifndef FANMANAGER_H
#define FANMANAGER_H

#include "EnclaveBlockVertex.h"
#include "ThinFan.h"
#include "FatFan.h"
#include "TertiaryTriangleContainer.h"
#include "PointSearchData.h"
#include "OrganicWrappedBBFan.h"
#include "BlockSearchMeta.h"

class IndependentUtils;
class FanManager
{
public:
	FanManager() {};

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// -------------------------------------------------------------------------------> Constructor Overload #1
	// This overloaded constructor was added on 6/26/2022, as a requirement for EnclaveBlock = operator to work.
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	FanManager(const FanManager& fanManager_a)
	{
		totalFans = fanManager_a.totalFans;
		currentFanStorageMode = fanManager_a.currentFanStorageMode;
		totalPoints = fanManager_a.totalPoints;
		currentPointStorageMode = fanManager_a.currentPointStorageMode;


		for (int x = 0; x < 16; x++)
		{
			localFanArray[x] = fanManager_a.localFanArray[x];
		}

		// Fan Array checks and copies
		isExpandedFanArrayActive = fanManager_a.isExpandedFanArrayActive;
		expandedFanArraySize = fanManager_a.expandedFanArraySize;
		if (isExpandedFanArrayActive == true)
		{
			if (currentFanStorageMode == FanArrayMode::THIN)
			{
				expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedThinFanArray[x] = fanManager_a.expandedThinFanArray[x];
				}
			}
			else if (currentFanStorageMode == FanArrayMode::FAT)
			{
				expandedFatFanArray.reset(new FatFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedFatFanArray[x] = fanManager_a.expandedFatFanArray[x];
				}
			}
		}

		// EnclaveBlockVertex array checks and copies
		expandedVertexArraySize = fanManager_a.expandedVertexArraySize;
		if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS)
		{
			for (int x = 0; x < totalPoints; x++)		// only need to copy over an amount of points equal to totalPoints
			{
				localVertexArray[x] = fanManager_a.localVertexArray[x];
			}
		}
		else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS)
		{
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < totalPoints; x++)
			{
				expandedVertexArray[x] = fanManager_a.expandedVertexArray[x];		// copy over the contents
			}
		}
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// -------------------------------------------------------------------------------> Operator Overload #1
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	FanManager& operator=(FanManager&& fanManager_a)
	{
		totalFans = fanManager_a.totalFans;
		currentFanStorageMode = fanManager_a.currentFanStorageMode;
		totalPoints = fanManager_a.totalPoints;
		currentPointStorageMode = fanManager_a.currentPointStorageMode;

		for (int x = 0; x < 16; x++)
		{
			localFanArray[x] = fanManager_a.localFanArray[x];
		}

		// Fan Array checks and copies
		isExpandedFanArrayActive = fanManager_a.isExpandedFanArrayActive;
		expandedFanArraySize = fanManager_a.expandedFanArraySize;


		if (isExpandedFanArrayActive == true)
		{
			if (currentFanStorageMode == FanArrayMode::THIN)
			{
				expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedThinFanArray[x] = fanManager_a.expandedThinFanArray[x];
				}
			}
			else if (currentFanStorageMode == FanArrayMode::FAT)
			{
				expandedFatFanArray.reset(new FatFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedFatFanArray[x] = fanManager_a.expandedFatFanArray[x];
				}
			}
		}

		// EnclaveBlockVertex array checks and copies
		expandedVertexArraySize = fanManager_a.expandedVertexArraySize;
		if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS)
		{
			for (int x = 0; x < totalPoints; x++)		// only need to copy over an amount of points equal to totalPoints
			{
				localVertexArray[x] = fanManager_a.localVertexArray[x];
			}
		}
		else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS)
		{
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < totalPoints; x++)
			{
				expandedVertexArray[x] = fanManager_a.expandedVertexArray[x];		// copy over the contents
			}
		}

		return *this;
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// -------------------------------------------------------------------------------> Operator Overload #2 (direct copy, FanManager to FanManager)
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	FanManager& operator=(const FanManager& fanManager_a)
	{
		totalFans = fanManager_a.totalFans;
		currentFanStorageMode = fanManager_a.currentFanStorageMode;
		totalPoints = fanManager_a.totalPoints;
		currentPointStorageMode = fanManager_a.currentPointStorageMode;


		for (int x = 0; x < 16; x++)
		{
			localFanArray[x] = fanManager_a.localFanArray[x];
		}

		// Fan Array checks and copies
		isExpandedFanArrayActive = fanManager_a.isExpandedFanArrayActive;
		expandedFanArraySize = fanManager_a.expandedFanArraySize;
		if (isExpandedFanArrayActive == true)
		{
			if (currentFanStorageMode == FanArrayMode::THIN)
			{
				expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedThinFanArray[x] = fanManager_a.expandedThinFanArray[x];
				}
			}
			else if (currentFanStorageMode == FanArrayMode::FAT)
			{
				expandedFatFanArray.reset(new FatFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedFatFanArray[x] = fanManager_a.expandedFatFanArray[x];
				}
			}
		}

		// EnclaveBlockVertex array checks and copies
		expandedVertexArraySize = fanManager_a.expandedVertexArraySize;
		if (currentPointStorageMode == PointArrayMode::LOCAL_POINTS)
		{
			for (int x = 0; x < totalPoints; x++)		// only need to copy over an amount of points equal to totalPoints
			{
				localVertexArray[x] = fanManager_a.localVertexArray[x];
			}
		}
		else if (currentPointStorageMode == PointArrayMode::NONLOCAL_POINTS)
		{
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < totalPoints; x++)
			{
				expandedVertexArray[x] = fanManager_a.expandedVertexArray[x];		// copy over the contents
			}
		}

		return *this;
	}

	void processTertiaryData(TertiaryTriangleContainer in_polyMetaData, TriangleMaterial in_materialID);									// used by EnclaveBlock::processTertiaryData
	void insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan);															// used by EnclaveBlock::insertBBFanFromRawEnclave
	PointSearchData checkIfPointExists(EnclaveBlockVertex in_blockVertex);														// used by EnclaveBlock::checkIfPointExists
	PointSearchData checkIfNearbyPointExists(EnclaveBlockVertex in_blockVertex);												// used by EnclaveBlock::checkIfNearbyPointExists
	BlockSearchMeta checkForExactPoint(ECBPolyPoint in_point, int in_debugFlag);												// used by EnclaveBlock::checkForExactPoint
	BlockSearchMeta checkIfNearbyPointExistsOnLine(ECBPolyPoint in_point, EnclaveKeyDef::EnclaveKey in_key, int in_debugFlag);	// used by EnclaveBlock::checkIfNearbyPointExistsOnLine

	EnclaveBlockVertex fetchPoint(int in_pointIndex);			// *should* be used by both EnclaveBlock::fetchPoint functions.
	int getTotalNumberOfTertiariesInBlock();					// used by EnclaveBlock::getTotalNumberOfTertiariesInBlock()
	void listSecondaries();										// used by EnclaveBlock::listSecondaries() -- NOT CRITICAL, for debugging.
	void listPoints();											// used by EnclaveBlock::listPoints() -- NOT CRITICAL, for debugging.
	int getNumberOfTotalFans();									// used by EnclaveBlock::getNumberOfBBFans(); returns the number of total fans.
	int getNumberOfTotalTriangles();							
	FanBase* retrieveSecondaryFromIndex(int in_index);			// retrieves a secondary from a particular index; dependent on FanArrayMode to determine where to get the data from.
	int getNumberOfTertiariesInTriangleAtIndex(int in_index);	// used by EnclaveBlock::getNumberOfTertiariesInTriangleAtIndex.
	ECBPolyPoint getEmptyNormalFromTriangle(int in_index);		// used by EnclaveBlock::getEmptyNormalFromTriangle

	Message writeFanManagerToBDMFormat();	// attempts to store the data of the FanManager that called this function to a Message;
											// the MessageType of the Message needs to be BDM_BLOCK_FANMANAGER.

	void constructManagerFromMessage(Message in_managerDataMessage);		// attempts to build the FanManager from a Message.

private:

	// Structs for fan array and point management.
	enum class PointArrayMode
	{
		LOCAL_POINTS,
		NONLOCAL_POINTS
	};

	enum class FanArrayMode
	{
		LOCALIZED,	// still uses the default array, not the smart point, for coherency.
		THIN,		// uses the expandedFanArray (ThinFan), and up to 256 points in expandedVertexArray; 
		FAT,		// uses the expandedFanArray, and up to "int" points in expandedVertexArray; 
	};

	// |||||||||||||||||||||||||||||||||||||||||||||||| BEGIN: Point management members |||||||||||||||||||||||||||||||||||||||||||||||||||||
	PointArrayMode currentPointStorageMode = PointArrayMode::LOCAL_POINTS;
	EnclaveBlockVertex localVertexArray[32];	// where the points are stored when the currentPointStorageMode is PointArrayMode::LOCAL_POINTS.
	unsigned short totalPoints = 0;					// the total number of points the manager is using.
	unsigned short expandedVertexArraySize = 16;	// the current size of the point array; this value may get incremented by +16 when calling checkForVertexExpansion().
	std::unique_ptr<EnclaveBlockVertex[]> expandedVertexArray;	// for when we have > 32 points; this gets expanded by 32 points each.
	// |||||||||||||||||||||||||||||||||||||||||||||||| END: Point management members |||||||||||||||||||||||||||||||||||||||||||||||||||||



	// |||||||||||||||||||||||||||||||||||||||||||||||| BEGIN: Fan management members |||||||||||||||||||||||||||||||||||||||||||||||||||||
	// for LOCALIZED fan array mode; used by default, unless we need more space.
	FanArrayMode currentFanStorageMode = FanArrayMode::LOCALIZED;		// default is always localized; it's unnecessary to use FAT fans unless we're in FAT mode; LOCALIZED/THIN = THIN fans.
	ThinFan localFanArray[16];	// stores up to 16 ThinFans; this is where ThinFans go initially, 
								// when the currentFanStorageMode is set to FanArrayMode::LOCALIZED.

	unsigned short totalFans = 0;				// the total number of triangles the manager has; the equivalent of numBBFanByte in EnclaveBlock.
	unsigned short expandedFanArraySize = 16;	// When the value of currentMode is FanArrayMode::THIN or FanArrayMode::FAT, this value is
												// used to represent the current size of the expandedThinFanArray or expandedFatFanArray, respectively.
												// This value gets incremented when the expandTriangleArray() function gets called, to indicate the current size of the triangle array.
												// When the FanManager writes itself to a Message, this value needs to be preserved somehow.

	std::unique_ptr<ThinFan[]> expandedThinFanArray;	// for THIN array mode
	std::unique_ptr<FatFan[]> expandedFatFanArray;		// for FAT array mode
	bool isExpandedFanArrayActive = false;
	// |||||||||||||||||||||||||||||||||||||||||||||||| END: Fan management members |||||||||||||||||||||||||||||||||||||||||||||||||||||











	// |||||||||||||| state checking/manipulation functions.
	FanBase* retrieveNextAvailableSecondary();

	// triangle expansion functions.
	void checkForTriangleExpansion();
	void expandTriangleArray();

	void checkForVertexExpansion();		// Determines whether or not the number of vertices stored would need to be increased, before calling the
										// processTertiaryData function, which attempts to insert new points if it finds them. 
										//
										// The order of expansion is as follows:
										//
										//	1.	If the value of totalPoints is 16, which is the max limit for PointArrayMode::LOCAL_POINTS,
										//		then the expandedVertexArray gets reset/instantiated with a value of 32. The vertices from localVertexArray then get loaded into
										//		the expandedVertexArray. The currentPointMdoe then becomes PointArrayMode::NONLOCALPOINTS, to indicate that the vertices
										//		are stored in a dynamically allocated array, and NOt the localVertexArray member.
										//
										//  2.	else, if the currentPointMode is PointArrayMode::NONLOCAL_POINTS, and the totalPoints value is < 256,
										//		all current points in the expandVertexArray get copied into a temporary, dynamically allocated array. Then,
										//		the expandedVertexArray is reset, with its current size +16. At that point, the values in the dynamically allocated array
										//		go into the newly reset/allocated expandedVertexArray.
										//
										//	3.	<<<< solution for when going beyond 256 points still needs to be developed >>>>

							
	int addNewPoint(EnclaveBlockVertex in_blockVertex);

	ECBPolyPoint convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex);
};

#endif
