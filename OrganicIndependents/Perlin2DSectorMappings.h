#pragma once

#ifndef PERLIN2DSECTORMAPPINGS_H
#define PERLIN2DSECTORMAPPINGS_H

#include <string>
#include <iostream>
#include <vector>
#include "Perlin2DKeyedState.h"
#include <boost/archive/basic_binary_oarchive.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>
#include "EnclaveKeyDef.h"

/*
* 
* Description: PerlinClusterSectorStateEnum is used to indicate the relationship of a PerlinCluster to the sectors it affects.
* A value of "PROCESSED" indicates that the PerlinCluster was either A.) newly created from scratch, and originated in the given sector
* file, or B.) it existed already as a "REFERENCED" value in the sector file, but was updated to "PROCESSED" once a PerlinFactory
* object found that it was previously involved in that sector. 
* 
* In other words, all sectors involved in a perlin cluster during it's initial creation that are NOT the origin sector, should be marked as "REFERENCED" when the 
* corresponding OSector files are updated; the origin sector should be updated as "PROCESSED." As the PerlinFactory processes an individual sector
* that is already labelled as "REFERENCED" when in the context of the same PerlinCluster, the "deployment" of the cluster will be skipped --as it was already
* created -- and the corresponding PerlinClusterHashMeta value(s) in the looked-at OSector file should get updated to "PROCESSED"
* 
* 


*/
enum class PerlinClusterSectorStateEnum
{
	NOVAL,
	PROCESSED,
	REFERENCED
};

/*
* 
* Description: PerlinClusterHashMeta maps a SHA256 string to a corresponding PerlinClusterSectorStateEnum value;
* the data in this class is directly written-to and read-from OSector files, hence the usage of the Boost serialization
  functionality. (see OSector in OrganicWindowsAdapter library)
*/
class PerlinClusterHashMeta
{
	public:
		PerlinClusterHashMeta() {};
		PerlinClusterHashMeta(std::string in_clusterHash, PerlinClusterSectorStateEnum in_clusterStatusInSector, std::string in_parentGridName):
			clusterHash(in_clusterHash),
			clusterStatusInSector(in_clusterStatusInSector),
			parentGridName(in_parentGridName)
		{}

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & clusterHash;
			ar & clusterStatusInSector;
			ar & parentGridName;
		}

		void printHashMetaData()
		{
			std::cout << "!! clusterHash: " << clusterHash << " | clusterStatusInSector: ";
			switch (clusterStatusInSector)
			{
				case PerlinClusterSectorStateEnum::NOVAL:
				{
					std::cout << "PerlinClusterSectorStateEnum::NOVAL";
					break;
				}

				case PerlinClusterSectorStateEnum::PROCESSED:
				{
					std::cout << "PerlinClusterSectorStateEnum::PROCESSED";
					break;
				}

				case PerlinClusterSectorStateEnum::REFERENCED:
				{
					std::cout << "PerlinClusterSectorStateEnum::REFERENCED";
					break;
				}
			}
			std::cout << " | parentGridName: " << parentGridName << std::endl;
		}

		std::string clusterHash = "";
		PerlinClusterSectorStateEnum clusterStatusInSector = PerlinClusterSectorStateEnum::NOVAL;
		std::string parentGridName = "";

};

/* Description: PerlinClusterSectorState maps the EnclaveKey2D of sector, to the data needed to produce a 
*  PerlinClusterHashMeta; the generateClusterHashMeta() is called to produce the corresponding PerlinClusterHashMeta value.
*/
class PerlinClusterSectorState
{
	public:
		PerlinClusterSectorState() {};
		PerlinClusterSectorState(std::string in_currentHash, EnclaveKeyDef::Enclave2DKey in_currentKey, PerlinClusterSectorStateEnum in_currentClusterSectorState) :
			currentHash(in_currentHash),
			currentKey(in_currentKey),
			currentClusterSectorState(in_currentClusterSectorState)
		{}

		std::string currentHash = "";
		EnclaveKeyDef::Enclave2DKey currentKey;
		PerlinClusterSectorStateEnum currentClusterSectorState = PerlinClusterSectorStateEnum::NOVAL;

		PerlinClusterHashMeta generateClusterHashMeta(std::string in_parentGridName)
		{
			return PerlinClusterHashMeta(currentHash, currentClusterSectorState, in_parentGridName);
		}


};

/*
* Description: Perlin2DSectorMappingContainer is designed to contain a std::vector of PerlinClusterSectorState;
* the non-default constructor takes in a origin key that is used to update the PerlinClusterSectorState corresponding to 
* that key to use a PerlinClusterSectorStateEnum::PROCESSED value; this allows the calling function to have all the metadata
* needed to update the corresponding PerlinClusterSectorState entries in each OSector file appropriately. The primary method of 
* generating this is via PerlinCluster::generateMappingContainer()
*/
class Perlin2DSectorMappingContainer
{
	public:
		Perlin2DSectorMappingContainer() {}
		Perlin2DSectorMappingContainer(std::string in_commonHash, EnclaveKeyDef::Enclave2DKey in_containerOriginKey) :
			commonHash(in_commonHash),
			containerOriginKey(in_containerOriginKey)
		{}

		// Below: used by PerlinCluster::generateMappingContainer() to create new PerlinClusterSectorState objects.
		void insertKeyedStates(std::vector<Perlin2DKeyedState> in_keyedStateVector)
		{
			for (auto& currentKeyedItem : in_keyedStateVector)
			{
				// Create a new PerlinClusterSectorState, where the state is based on whether or not the key we're looking at
				// is the origin key.
				if (currentKeyedItem.twoDKey == containerOriginKey)
				{
					PerlinClusterSectorState processedState(commonHash, currentKeyedItem.twoDKey, PerlinClusterSectorStateEnum::PROCESSED);
					stateMappings.push_back(processedState);
				}
				else
				{
					PerlinClusterSectorState processedState(commonHash, currentKeyedItem.twoDKey, PerlinClusterSectorStateEnum::REFERENCED);
					stateMappings.push_back(processedState);
				}
			}
		}

		
		void printOutMappingContainerContents()
		{
			std::cout << "!!! Printing out mapping container contents..." << std::endl;
			for (auto& currentKeyedItem : stateMappings)
			{
				std::cout << "Hash: " << currentKeyedItem.currentHash << " | Key: ";
				currentKeyedItem.currentKey.printKey();
				std::cout << "Status: ";

				switch (currentKeyedItem.currentClusterSectorState)
				{
					case PerlinClusterSectorStateEnum::NOVAL:
					{
						std::cout << "PerlinClusterSectorStateEnum::NOVAL" << std::endl;
						break;
					}

					case PerlinClusterSectorStateEnum::PROCESSED:
					{
						std::cout << "PerlinClusterSectorStateEnum::PROCESSED" << std::endl;
						break;
					}

					case PerlinClusterSectorStateEnum::REFERENCED:
					{
						std::cout << "PerlinClusterSectorStateEnum::REFERENCED" << std::endl;
						break;
					}
				}
			}
		}
		
		// Below: these two functions are used by OSectorManager::checkProcessingColumn
		std::vector<PerlinClusterSectorState> fetchContainerSectorStates()
		{
			return stateMappings;
		}

		EnclaveKeyDef::Enclave2DKey fetchContainerOriginKey() { return containerOriginKey; }
		

	private:
		std::string commonHash = "";
		EnclaveKeyDef::Enclave2DKey containerOriginKey;
		std::vector<PerlinClusterSectorState> stateMappings;
};

#endif
