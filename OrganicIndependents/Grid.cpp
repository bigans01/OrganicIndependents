#include "stdafx.h"
#include "Grid.h"

void NoiseGridScanner::start(EnclaveKeyDef::Enclave2DKey in_startSectorKey)
{
	// Check if the sector exists.
	if (!gridPtr->doesSectorExist(in_startSectorKey))
	{
		std::cout << "! Sector does not exist, will generate members of sector before continuing..." << std::endl;

		gridPtr->generateSector(in_startSectorKey.a, in_startSectorKey.b);
		gridPtr->generateSectorTiles(in_startSectorKey.a, in_startSectorKey.b);




		gridPtr->generateSamplesForSector(in_startSectorKey.a, in_startSectorKey.b, gridPtr->getSeedValue());

		// One island:  run a clip value of 0.50 against a coord at -3054, -1030
		// Two islands: run a clip value of 0.43 against a coord at 1024, -6.

		// NOTES: 0.43 was too liberal -- constantly hit tile limit.

		gridPtr->runClippingOnExistingSector(in_startSectorKey.a, in_startSectorKey.b, gridPtr->getThresholdValue());
		gridPtr->generateSectorTilesets(in_startSectorKey.a, in_startSectorKey.b);

		std::cout << "::::::::::::::::::: NEW TEST START :::::::::::::::::::::::::::::: " << std::endl;

		std::cout << "Testing on sector with key: ";
		in_startSectorKey.printKey();
		std::cout << std::endl;

		
		auto fetchedTileMapPtr = gridPtr->getSectorTileMapPtr(in_startSectorKey.a, in_startSectorKey.b);

		NoiseGridSectorGroupBuilderV2 newBuilder(fetchedTileMapPtr);
		newBuilder.start();

		std::cout << "::::::::::::::::::: NEW TEST END :::::::::::::::::::::::::::::: " << std::endl;
		int testPass = 3;
		std::cin >> testPass;

		gridPtr->generateSectorGroupings(in_startSectorKey.a, in_startSectorKey.b);
		gridPtr->printSectorCoutArt(in_startSectorKey.a, in_startSectorKey.b);
		gridPtr->printNeighborMetadataForTilesInSector(in_startSectorKey.a, in_startSectorKey.b);


		// Next, check for any NoiseGridSectorGrouping instances in the current newly generated sector. Only continue with creating an instance of 
		// NoiseGridScanAttempt if there are groupings to check in the current sector.

		auto targetSectorPtr = gridPtr->getSectorRef(in_startSectorKey.a, in_startSectorKey.b);
		if (targetSectorPtr->doesSectorHaveGroupings())
		{
			std::cout << "--------------------------------------------" << std::endl;
			std::cout << "Found data in sector, printing grouping data. " << std::endl;
			std::cout << "--------------------------------------------" << std::endl;
			targetSectorPtr->printNeighboringLinksPerGrouping();

			// TODO: while fetching first grouping is fine initially, we will need to cycle
			// through each grouping
			//auto targetGrouping = targetSectorPtr->fetchFirstGrouping();


			int targetGroupingIndex = 0;


			// ++++++++++++++++++++++++++++++++ Optional, non critical struct for debugging purposes ++++++++++++++++++++++++++++++++++++++
			struct ScanGroupingStats
			{
				ScanGroupingStats() {};
				ScanGroupingStats(int in_scanAttemptId, int in_finalTileCount, bool in_succeeded):
					scanAttemptId(in_scanAttemptId),
					finalTileCount(in_finalTileCount),
					wasSuccessful(in_succeeded)
				{}

				void printGroupingStats()
				{
					std::cout << "scanAttemptId: " << scanAttemptId << " | finalTileCount: " << finalTileCount;
					if (wasSuccessful)
					{
						std::cout << " | success: yes" << std::endl;
					}
					else
					{
						std::cout << " | success: no" << std::endl;
					}
				}

				int scanAttemptId = -1;
				int finalTileCount = 0;
				bool wasSuccessful = false;
			};
			std::vector<ScanGroupingStats> grpingStatsVector;
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


			auto fetchedGroupings = targetSectorPtr->fetchGroupingVector();
			for (auto& targetGrouping : fetchedGroupings)
			{
				NoiseGridScanAttempt initialScanAttempt(gridPtr, targetSectorPtr, targetGrouping);
				initialScanAttempt.runScan();

				// If the run was successful, extract the 
				if (initialScanAttempt.getFinalRunState() == NGSClusterRunState::STOP_WORK_DONE)
				{
					generatedClusterMetaVector.push_back(initialScanAttempt.fetchGeneratedMeta());
					ScanGroupingStats newStats(targetGroupingIndex, initialScanAttempt.getFinalTileCount(), true);
					grpingStatsVector.push_back(newStats);
				}
				else
				{
					ScanGroupingStats newStats(targetGroupingIndex, initialScanAttempt.getFinalTileCount(), false);
					grpingStatsVector.push_back(newStats);
				}

				initialScanAttempt.resetAttemptClusterTileCount();

				targetGroupingIndex++;
			}

			// class that absorbs the output of a NoiseGridScanAttempt will need to get initialized here.
			// 
			// 
			// Optional: print out grouping stats from the debug struct above.
			std::cout << "++++++++++++++++++ Printing out grpingStatsVector +++++++++++++++++++++++" << std::endl;
			for (auto& currentStats : grpingStatsVector)
			{
				currentStats.printGroupingStats();
			}

		}
		
		

		// Check the neighboringSectorLinks of the selected NoiseGridSectorGrouping, to determine if there are any checks that have to be done
		// in a neighboring sector. If there are no neighboringSectorLinks, this scan is complete, as there is nothing to link to.
	}
}

std::vector<PerlinClusterMeta> NoiseGridScanner::fetchClusterMetaVector()
{
	return generatedClusterMetaVector;
}

void NoiseGridScanAttempt::runScan()
{
	std::cout << "!! Running scan..." << std::endl;

	currentScanSectorPtr->printGroupingTileCounts();






	// TEST: print out the neighboring tiles to check for in the current grouping.
	currentScanSectorFetchedGrouping.groupingPtr->scanForTilesWithRemainingLocalNeighbors();

	currentScanSectorFetchedGrouping.groupingPtr->printNeighboringLinks();






	NGSClusterRunState loopRunState = NGSClusterRunState::SHOULD_CONTINUE;
	

	
	while (loopRunState == NGSClusterRunState::SHOULD_CONTINUE)
	{
		

		// Step 1: put the initial sector we're working with, into the attemptCluster, as being SCANNED.
		attemptCluster.insertSectorAsScanned(currentScanSectorPtr->fetchSectorRootCoord(),
			currentScanSectorFetchedGrouping.groupingId, currentScanSectorFetchedGrouping.groupingPtr->getNumberOfTilesInGrouping());


		// Step 2: Put current sector key, plus curernt grouping key, into a map.

		// Step 3: Get the copy of neighboringSectorLinks from the current sector grouping. Iterate through each entry,
		// checking if the sector exists. If the sector doesn't exist, create it. Then, check all the groupings of that target 
		// sector and check if any of their members contain any of the members of any of the links.
		auto fetchedLinksMap = currentScanSectorFetchedGrouping.groupingPtr->fetchNeighboringSectorLinks();

		// Step 4a: HARDCODED TEST: just check that the desired logic works on the first sector.
		//if (!fetchedLinksMap.empty())
		for (auto& currentLink : fetchedLinksMap)
		{
			auto firstHit = currentLink;
			EnclaveKeyDef::Enclave2DKey firstHitKey = firstHit.first;
			NoiseGridSectorGroupingLink firstHitLink = firstHit.second;

			// Check if the desired key exists in the grid.
			if (!scanAttemptGridPtr->doesSectorExist(firstHitKey))
			{
				std::cout << "NoiseGridScanAttempt: Neighboring sector ";
				firstHitKey.printKey();
				std::cout << " does not exist; will create sector." << std::endl;

				scanAttemptGridPtr->generateSectorContents(firstHitKey, true);
			}


			currentNeighboringScanSectorPtr = scanAttemptGridPtr->getSectorRef(firstHitKey.a, firstHitKey.b);

			NoiseGridLinkSearchResult searchResult = currentNeighboringScanSectorPtr->runLinkScan(firstHitLink.fetchLinkTiles());

			// If the result was found, put it into the scanning map.
			if (searchResult.wasLinkFound)
			{
				std::cout << "!! Link found in grouping " << searchResult.foundLinkGroupingID << " in sector ";
				searchResult.foundLinkSector.printKey();
				std::cout << std::endl;

				// Attempt to insert the NGSCClusterEntry as referenced.
				attemptCluster.attemptToInsertSectorAsReferenced(searchResult.foundLinkSector,
					searchResult.foundLinkGroupingID,
					currentNeighboringScanSectorPtr->getNumberOfTilesInGrouping(searchResult.foundLinkGroupingID));

				attemptCluster.printClusterData();
			}


		}

		// As long as the cluster limit hasn't been reached, get the next sector to scan.
		loopRunState = attemptCluster.fetchRunState();
		if (loopRunState == NGSClusterRunState::SHOULD_CONTINUE)
		{
			std::cout << "!! Run state set as NGSClusterRunState::SHOULD_CONTINUE...will iterate again. " << std::endl;

			// Below: the function call updates the following values:
			// -currentScanSectorPtr
			// -currentScanSectorFetchedGrouping
			setupNextScanIteration();
		}
	}

	// when finally done, print the final stats in the cluster.
	attemptCluster.printRunAttemptData();

	// Was the run complete? Time to create a PerlinCluster and store it.
	if (loopRunState == NGSClusterRunState::STOP_WORK_DONE)
	{
		// fetch the PerlinClusterMeta value from the attemptCluster, put it into the generatedClusterMeta.
		generatedClusterMeta = attemptCluster.fetchProducedGroupings();
	}

	finalTileCount = attemptCluster.fetchCurrentTileCount();

	// Set the final runState.
	finalRunState = loopRunState;
}

void NoiseGridScanAttempt::resetAttemptClusterTileCount()
{
	attemptCluster.resetCurrentTileCount();
}

NGSClusterRunState NoiseGridScanAttempt::getFinalRunState()
{
	return finalRunState;
}

PerlinClusterMeta NoiseGridScanAttempt::fetchGeneratedMeta()
{
	return generatedClusterMeta;
}

int NoiseGridScanAttempt::getFinalTileCount()
{
	return finalTileCount;
}

void NoiseGridScanAttempt::setupNextScanIteration()
{
	NGSCScanTargetGrouping nextGrouping = attemptCluster.fetchNextTargetGroupingFromReferenced();

	// Set the next sector to be scanned.
	currentScanSectorPtr = scanAttemptGridPtr->getSectorRef(nextGrouping.targetGroupingSectorKey.a, nextGrouping.targetGroupingSectorKey.b);

	currentScanSectorFetchedGrouping.groupingId = nextGrouping.targetGroupingId;
	currentScanSectorFetchedGrouping.groupingPtr = scanAttemptGridPtr->getSectorRef(nextGrouping.targetGroupingSectorKey.a, nextGrouping.targetGroupingSectorKey.b)->fetchSpecificGroupingById(nextGrouping.targetGroupingId);

	std::cout << "!! Cout art of the next iteration's sector: " << std::endl;
	scanAttemptGridPtr->printSectorCoutArt(nextGrouping.targetGroupingSectorKey.a, nextGrouping.targetGroupingSectorKey.b);
}