#pragma once

#ifndef DIMENSIONALLINESCANNERBASE_H
#define DIMENSIONALLINESCANNERBASE_H

#include "FTriangleProductionStager.h"
#include "TrackedMap.h"
#include "FTriangleUtils.h"

class DimensionalLineScannerBase
{
	public:
		void initializeLineScanner(std::set<float> in_fixedScanPoints,
								UniquePointContainer* in_dimensionalUniquePointsRef,
								std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* in_dimensionalStagerRef,
								int in_dimensionalScanBackwardKey,
								int in_dimensionalScanForwardKey,
								float in_fixedDimensionalInterval);
		virtual void runScan() = 0;	// runs N line-scanning planes, where N is the number of points in fixedScanPoints; 
									// logic is uniquely defined by each class that is derived from this base class.
	protected:
		std::set<float> fixedScanPoints;				// contains each X,Y, OR Z coordinate for the scanning plane we will be using.
		UniquePointContainer* dimensionalUniquePointsRef = nullptr;	// a pointer to the points we will check against during line scanning;
																	// also analyzed to determine endpoints and non-endpoints.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleProductionStager, EnclaveKeyDef::KeyHasher>* dimensionalStagerRef = nullptr;

		// the dimensional keys below must be incremented once, during every iteration of the loop that goes through the fixedScanPoints, 
		// in the function runScan().
		int dimensionalScanBackwardKey = 0;		// the current "backward" dimensional value (for the key vaue that exists "behind" the plane we are scanning)
		int dimensionalScanForwardKey = 0;		// the current "forward" dimensional value (for the key vaue that exists in front of the plane we are scanning)
		float fixedDimensionalInterval = 0.0f;	// the constant amount of space between each grid cell that we're working with (i.e, when producing blueprints this would be 32.0f)

		// now, for the exterior points, use a map of a temporary struct that stores three values:
		// -the ID of the first compared point
		// -the ID of the second compared point
		// -the calculated distance between the compared points
		struct PointComparison
		{
			PointComparison() {};
			PointComparison(int in_pointAid,
				glm::vec3 in_pointA,
				int in_pointBid,
				glm::vec3 in_pointB)
			{
				pointAid = in_pointAid;
				pointBid = in_pointBid;
				pointA = in_pointA;
				pointB = in_pointB;
				distanceBetweenPoints = glm::distance(in_pointA, in_pointB);
			}

			bool operator==(const PointComparison& in_otherPoint) const	// const is required at end here for C++_20
			{
				return
				(
					// check for matching IDs, but swapped:
					(pointAid == in_otherPoint.pointBid)
					&&
					(pointBid == in_otherPoint.pointAid)
				);
			}

			void printComparison()
			{
				std::cout << "Comparison result: pointA -> ID: ";
				std::cout << pointAid << " point: " << pointA.x << ", " << pointA.y << ", " << pointA.z;
				std::cout << " | pointB -> ID: ";
				std::cout << pointBid << " point: " << pointB.x << ", " << pointB.y << ", " << pointB.z;
				std::cout << " | distance: " << distanceBetweenPoints;
				std::cout << std::endl;
			}

			int pointAid = 0;
			int pointBid = 0;
			glm::vec3 pointA;
			glm::vec3 pointB;
			float distanceBetweenPoints = 0.0f;
		};

		// below: a struct designed to return two values: a vector containing the two endpoints, and a map containing the non-endpoints.
		struct ReorganizedPoints
		{
			ReorganizedPoints() {};
			ReorganizedPoints(std::vector<FTrianglePoint> in_endpoints,
							std::map<int, FTrianglePoint> in_nonEndpoints) :
				endpoints(in_endpoints),
				nonEndpoints(in_nonEndpoints)
			{};

			std::vector<FTrianglePoint> endpoints;
			std::map<int, FTrianglePoint> nonEndpoints;
		};

		// below: a function used to reorganize points, when more than two exterior points are found in a scanning plane.
		ReorganizedPoints reorganizePoints(std::vector<FTrianglePoint> in_endpoints,
									std::map<int, FTrianglePoint> in_nonEndpoints);

		int findClosestPointIndex(FTrianglePoint in_pointA, std::map<int, FTrianglePoint>* in_searchablePointsMapRef);
};

#endif
