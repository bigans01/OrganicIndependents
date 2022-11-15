#include "stdafx.h"
#include "DimensionalLineScannerBase.h"

void DimensionalLineScannerBase::initializeLineScanner(std::set<float> in_fixedScanPoints,
	UniquePointContainer* in_dimensionalUniquePointsRef,
	std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* in_dimensionalStagerRef,
	int in_dimensionalScanBackwardKey,
	int in_dimensionalScanForwardKey,
	float in_fixedDimensionalInterval)
{
	fixedScanPoints = in_fixedScanPoints;
	dimensionalUniquePointsRef = in_dimensionalUniquePointsRef;
	dimensionalStagerRef = in_dimensionalStagerRef;
	dimensionalScanBackwardKey = in_dimensionalScanBackwardKey;
	dimensionalScanForwardKey = in_dimensionalScanForwardKey;
	fixedDimensionalInterval = in_fixedDimensionalInterval;
}


DimensionalLineScannerBase::ReorganizedPoints DimensionalLineScannerBase::reorganizePoints(std::vector<FTrianglePoint> in_endpoints,
	std::map<int, FTrianglePoint> in_nonEndpoints)
{
	// a map to store all generated comparisons
	std::map<int, PointComparison> comparisonTracker;

	// organize the data to use for the comparisons, by loading the extra points into their own map
	std::map<int, FTrianglePoint> exteriorPointComparisonMap;
	for (auto& pointToCopy : in_endpoints)
	{
		exteriorPointComparisonMap[exteriorPointComparisonMap.size()] = pointToCopy;
	}

	// run the comparisons
	for (auto& comparablePoint : exteriorPointComparisonMap)
	{
		int currentPointIndex = comparablePoint.first;	// the current point we're going to use as point A
		int startIndex = exteriorPointComparisonMap.begin()->first;	// the index of the element at the start of the map

		for (auto& currentComparedPoint : exteriorPointComparisonMap)
		{
			if (currentPointIndex != currentComparedPoint.first)
			{
				std::cout << "Generating comparison..." << std::endl;
				PointComparison newComparison(comparablePoint.first,
					comparablePoint.second.point,
					currentComparedPoint.first,
					currentComparedPoint.second.point);

				// do a scan to ensure that the comparison doesn't match anything that already exists.
				bool doesComparisonAlreadyExist = false;
				for (auto& currentComparisonToScan : comparisonTracker)
				{
					if (newComparison == currentComparisonToScan.second)
					{
						doesComparisonAlreadyExist = true;
						break;
					}
				}

				if (doesComparisonAlreadyExist == false)
				{
					std::cout << "Inserted new comparison: " << std::endl;
					newComparison.printComparison();
					comparisonTracker[comparisonTracker.size()] = newComparison;
				}
			}
		}
	}

	// find the index of the comparison that has the greatest distance.
	int currentMaxDistanceIndexID = 0;
	float currentMaxDistance = 0.0f;
	for (auto& generatedComparison : comparisonTracker)
	{
		float candidateMaxDistance = std::max(generatedComparison.second.distanceBetweenPoints, currentMaxDistance);
		if (candidateMaxDistance != currentMaxDistance)	// in this case, the candidateMaxDistance would be the new distance.
		{
			currentMaxDistanceIndexID = generatedComparison.first;
			currentMaxDistance = candidateMaxDistance;
		}
	}

	// now that we have the max distance, find that corresponding comparison, and extract the points with those two IDs 
	std::map<int, FTrianglePoint> endpointsPointMap;
	std::map<int, FTrianglePoint> nonEndpointPointsMap;

	// Step 1: load up the nonEndpointPointsMap with all of the interior points


	endpointsPointMap = exteriorPointComparisonMap;
	nonEndpointPointsMap = in_nonEndpoints;	// any 

	// Fetch the comparison that had the max distance; this will contain the keyed ID of the EXTERIOR 
	// points that serve as endpoints. All other EXTERIOR points need to be moved into the nonEndpointPointsMap 
	// in the step that follows.
	auto targetComparison = comparisonTracker[currentMaxDistanceIndexID];

	// move the corresponding exterior points in the endpointsPointMap over to the nonEndpointPointsMap,
	// if they are not a matching ID from the comparison; these are the EXTERIOR points that are not endpoints, 
	// so they should be moved into the same group as the interior points.
	std::set<int> removalIndices;
	for (auto& currentPointToTryAndMove : endpointsPointMap)
	{
		if
		(
			!((currentPointToTryAndMove.first == targetComparison.pointAid)
			||
			(currentPointToTryAndMove.first == targetComparison.pointBid))
		)
		{
			std::cout << "Moving point over: ";
			currentPointToTryAndMove.second.printPointData();
			std::cout << std::endl;

			nonEndpointPointsMap[nonEndpointPointsMap.size()] = currentPointToTryAndMove.second;
			removalIndices.insert(currentPointToTryAndMove.first);
		}
	}

	/*
	std::cout << "->>>> Printing out endpoints, first pass." << std::endl;
	for (auto& passOneExterior : endpointsPointMap)
	{
		passOneExterior.second.printPointData();
		std::cout << std::endl;
	}
	*/

	// erase the EXTERIOR points we moved into the interior map above.
	for (auto& currentExteriorIDToRemove : removalIndices)
	{
		std::cout << "!!! Erasing element at: " << currentExteriorIDToRemove << std::endl;
		endpointsPointMap.erase(currentExteriorIDToRemove);
	}

	/*
	std::cout << "->>>> Printing out endpoints, second pass." << std::endl;
	for (auto& passOneExterior : endpointsPointMap)
	{
		passOneExterior.second.printPointData();
		std::cout << std::endl;
	}
	*/

	std::cout << "Number of endpoints: " << endpointsPointMap.size() << std::endl;
	std::cout << "Number of non-endpoints: " << nonEndpointPointsMap.size() << std::endl;

	std::vector<FTrianglePoint> finalEndpointsVector;
	finalEndpointsVector.push_back(endpointsPointMap.begin()->second);
	finalEndpointsVector.push_back(endpointsPointMap.rbegin()->second);

	/*
	std::cout << "Done re-organizing points..." << std::endl;
	int comparisonDone = 3;
	std::cin >> comparisonDone;
	*/

	return ReorganizedPoints(finalEndpointsVector, nonEndpointPointsMap);
}

int DimensionalLineScannerBase::findClosestPointIndex(FTrianglePoint in_pointA, std::map<int, FTrianglePoint>* in_searchablePointsMapRef)
{
	std::map<int, float> distances;

	std::cout << "(DimensionalLineScannerBase::findClosestPointIndex): size of points to search is: " << in_searchablePointsMapRef->size() << std::endl;

	for (auto& currentSearchablePoint : *in_searchablePointsMapRef)
	{
		float currentDistance = glm::distance(in_pointA.point, currentSearchablePoint.second.point);
		distances[currentSearchablePoint.first] = currentDistance;
	}

	float currentMinDistance = 100000.0f;	// set this high, so it continually goes low
	int currentMinIndex = 0;
	for (auto& currentDistance : distances)
	{
		float currentMin = std::min(currentMinDistance, currentDistance.second);
		if (currentMin == currentDistance.second)
		{
			currentMinDistance = currentDistance.second;
			currentMinIndex = currentDistance.first;
		}
	}

	std::cout << "Original point to find min distance was: ";
	in_pointA.printPointData();
	std::cout << std::endl;

	std::cout << "Index of minimum point was: " << currentMinIndex << " | distance was: " << currentMinDistance << " | point was: ";
	(*in_searchablePointsMapRef)[currentMinIndex].printPointData();
	std::cout << std::endl;

	return currentMinIndex;
}