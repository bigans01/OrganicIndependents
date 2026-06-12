#pragma once

#ifndef NGSCLUSTERENTRY_H
#define NGSCLUSTERENTRY_H



/* NGSClusterEntry: designed to track each individual grouping utilized by a PerlinCluster, within a scanned sector. Instances of this
*  class will be found as part of the metaMap member in the PerlinClusterMeta class, which will be mapped by sector key. The 
*  "NGS" prefix stands for "NoiseGridScanner."
* 
* This class has two corresponding enum types:
* 
* NGSCSectorSuperStatus: used to determine the usage state of groupings associated with a PerlinCluster, per sector; for instance, 
* if the number of groupings used by the PerlinCluster is 2, but only 1 is used, an enum state of NGSC_SS_NOT_ALL_USED should exist
* (this is a rare case). The more common case is that there is a single group being used. So if 1 of 1 groups is used, the state would be
* NGSC_SS_ALL_USED.
* 
* NGSCGroupingStatus: used to indicate if the grouping was part of a sector that was the origin sector specified, during a call to
* NoiseGridScanner::start. A value of GROUPING_USED indicates that the grouping was part of the start sector. GROUPING_REFERENCED
* means the grouping was used in a sector that was not the start sector. The GROUPING_USED values are used later to determine the group IDs 
* that should be updated/written to the sector file of the origin sector, in a later step.

*/

enum class NGSCSectorSuperStatus
{
	NGSC_SS_ALL_USED,
	NGSC_SS_NOT_ALL_USED,
	NGSC_SS_UNDEFINED
};

enum class NGSCGroupingStatus
{
	GROUPING_USED,
	GROUPING_REFERENCED,
	GROUPING_NOVAL
};



class NGSClusterEntry
{
	public:
		NGSClusterEntry() {};

		struct ClusterGroupingStatus
		{

			ClusterGroupingStatus() {};
			ClusterGroupingStatus(int in_clusterGroupingTileCount, bool in_clusterGroupingIsUsed) :
				clusterGroupingTileCount(in_clusterGroupingTileCount),
				clusterGroupingIsUsed(in_clusterGroupingIsUsed)
			{
			}

			int clusterGroupingTileCount;
			bool clusterGroupingIsUsed = false;
		};

		void printEntry()
		{
			std::string enumedValue = "";
			getState();
			switch (currentState)
			{
				case NGSCSectorSuperStatus::NGSC_SS_UNDEFINED: { enumedValue = "NGSC_SS_UNDEFINED"; break; }
				case NGSCSectorSuperStatus::NGSC_SS_NOT_ALL_USED: { enumedValue = "NGSC_SS_NOT_ALL_USED"; break; }
				case NGSCSectorSuperStatus::NGSC_SS_ALL_USED: { enumedValue = "NGSC_SS_ALL_USED"; break; }
			}

			std::cout << " NGSCSectorSuperStatus::" << enumedValue << "|| Groupings: ";

			for (auto& currentClusterEntry : clusterGroupingMap)
			{
				std::cout << "Grouping ID: " << currentClusterEntry.first;

				switch (currentClusterEntry.second.clusterGroupingIsUsed)
				{
				case (false):
				{
					std::cout << " | clusterGroupingTileCount: " << currentClusterEntry.second.clusterGroupingTileCount << " | clusterGroupingIsUsed: false";
					break;
				}

				case (true):
				{
					std::cout << " | clusterGroupingTileCount: " << currentClusterEntry.second.clusterGroupingTileCount << " | clusterGroupingIsUsed: true";
					break;
				}
				}

				std::cout << std::endl;
			}
		}

		// Below: return the NGSCGroupingStatus of this NGSClusterEntry; can use any value from a populated clusteGroupingMap,
		// as they will all be the same (i.e, if there are 3 groupings inserted from a call to NoiseGridScanCluster::insertSectorAsScanned,
		// they would all be a contain a value of NGSCGroupingStatus::GROUPING_USED). If there's nothing in the map or it doesnt exist,
		// return GROUPING_NOVAL.
		NGSCGroupingStatus getGroupingStatus()
		{
			NGSCGroupingStatus returnStatus = NGSCGroupingStatus::GROUPING_NOVAL;

			// if the map is not empty, get the first value.
			if (!clusterGroupingMap.empty())
			{
				if (clusterGroupingMap.begin()->second.clusterGroupingIsUsed == false)
				{
					returnStatus = NGSCGroupingStatus::GROUPING_REFERENCED;
				}
				else
				{
					returnStatus = NGSCGroupingStatus::GROUPING_USED;
				}
			}

			return returnStatus;
		}

		NGSCSectorSuperStatus getState()
		{
			updateState();
			return currentState;
		}

		int getNextReferencedClusterGroupingId()
		{
			int foundGroupingId = -1;

			// Should return the next ClusterGroupingStatus that is referenced and not used.
			for (auto& currentClusterEntry : clusterGroupingMap)
			{
				// a bool of false indicates that it should be NGSCGroupingStatus::GROUPING_REFERENCED.
				if (!currentClusterEntry.second.clusterGroupingIsUsed)
				{
					foundGroupingId = currentClusterEntry.first;
				}
			}

			return foundGroupingId;
		}

		void insertClusterEntry(NGSCGroupingStatus in_currentState, int in_groupingId, int in_numberOftilesInGroup)
		{
			switch (in_currentState)
			{
				case NGSCGroupingStatus::GROUPING_REFERENCED:
				{
					ClusterGroupingStatus referencedStatus(in_numberOftilesInGroup, false);

					// Check 1: do a normal insert, if it doesnt exist. A specific grouping in a sector
					// which is inserted into a NGSClusterEntry cannot be updated if it exists and the desired status to
					// insert is NGSCGroupingStatus::GROUPING_REFERENCED. The bool value is set to false, to represent
					// NGSCGroupingStatus::GROUPING_REFERENCED
					auto groupFinder = clusterGroupingMap.find(in_groupingId);
					if (groupFinder == clusterGroupingMap.end())
					{
						clusterGroupingMap[in_groupingId] = referencedStatus;
					}

					break;
				}

				case NGSCGroupingStatus::GROUPING_USED:
				{
					// bool gets set to true, to represent NGSCGroupingStatus::GROUPING_USED.
					ClusterGroupingStatus usedStatus(in_numberOftilesInGroup, true);
					clusterGroupingMap[in_groupingId] = usedStatus;
					break;
				}
			}
		}

		void updateState()
		{
			// Must cycle through each grouping to get the state;
			// also re-calculate the value of the number of tiles.
			int numberOfUseds = 0;
			totalTilesInGroupCluster = 0;
			for (auto& currentGroupingToCheck : clusterGroupingMap)
			{
				if (currentGroupingToCheck.second.clusterGroupingIsUsed)
				{
					numberOfUseds++;
					totalTilesInGroupCluster += currentGroupingToCheck.second.clusterGroupingTileCount;
				}
			}



			// If the number of useds IS equal to the size of the map, this should be
			// NGSC_SS_ALL_USED.
			if (numberOfUseds == clusterGroupingMap.size())
			{
				currentState = NGSCSectorSuperStatus::NGSC_SS_ALL_USED;
			}


			// If the number of useds IS NOT equal to the size of the map, this should be
			// NGSC_SS_NOT_ALL_USED
			else
			{
				currentState = NGSCSectorSuperStatus::NGSC_SS_NOT_ALL_USED;
			}
		}

		int getTotalTiles()
		{
			return totalTilesInGroupCluster;
		}

		std::vector<int> fetchClusterEntryGroupingIds()
		{
			std::vector<int> fetchedIds;
			for (auto& currentGrouping : clusterGroupingMap)
			{
				fetchedIds.push_back(currentGrouping.first);
			}
			return fetchedIds;
		}

		std::map<int, ClusterGroupingStatus> fetchClusterGroupingCopy()
		{
			return clusterGroupingMap;
		}

	private:
		NGSCSectorSuperStatus currentState = NGSCSectorSuperStatus::NGSC_SS_UNDEFINED;
		//int groupingId = -1;
		int totalTilesInGroupCluster = -1;

		

		std::map<int, ClusterGroupingStatus> clusterGroupingMap;



};

#endif