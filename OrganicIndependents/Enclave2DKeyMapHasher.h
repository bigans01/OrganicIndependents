#pragma once

#ifndef ENCLAVEKEYMAPHASHER_H
#define ENCLAVEKEYMAPHASHER_H

#include <unordered_map>
#include <map>
#include <set>
#include "EnclaveKeyDef.h"
#include <iostream>
#include <string>
#include <type_traits>
#include "HashUtils.h"

class Enlave2DKeyMapHasher
{
	public:
        // 1. Primary template: assume it's NOT the correct unordered_map
        template <typename T>
        struct is_specific_unordered_map : std::false_type {};

        // 2. Partial specialization: matches if T is an unordered_map with the exact K, H, and generic V
        template <typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
        struct is_specific_unordered_map<std::unordered_map<Key, Value, Hash, Pred, Alloc>> {

            // Check if Key matches AND Hash matches
            //static constexpr bool value = std::is_same_v<Key, std::string>&&
             //   std::is_same_v<Hash, std::hash<std::string>>;

            static constexpr bool value = std::is_same_v<Key, EnclaveKeyDef::Enclave2DKey>;
        };

        // processMap: pass the referenced map to this function to start.
        template <typename MapType> void processMap(const MapType&& mapInstance) 
        {
            // attempt a decay and check if is a reference to the specific map
            if (is_specific_unordered_map<std::decay_t<std::remove_pointer_t<MapType>>>::value)
            {
                valid = true;
                runHashCalculation(std::move(mapInstance));
            }
        }

        bool isValid() { return valid; }
        std::string getMapHash() { return calculatedHash;  }

    private:
        template <typename MapType> void runHashCalculation(const MapType&& mapInstance)
        {
            std::cout << "Hooray. " << std::endl;
            
            // Step 1: populate yByXMap.
            for (auto& currentKey : *mapInstance)
            {
                EnclaveKeyDef::Enclave2DKey currentKeyCopy = currentKey.first;
                std::cout << "++ Current key to hash is: ";
                currentKeyCopy.printKey();
                std::cout << std::endl;

                // Remember: each y (b), from least to greatest, get's a set of x (a) values from least to greatest.
                yByXMap[currentKeyCopy.b].insert(currentKeyCopy.a);
            }

            // Step 2: create SHA256 by reading via the specified order.
            for (auto& currentY : yByXMap)
            {
                for (auto& currentX : currentY.second)
                {
                    EnclaveKeyDef::Enclave2DKey currentKey(currentX, currentY.first);
                    calculatedHash = HashUtils::sha256(calculatedHash + currentKey.getKeyString());
                }
            }
            
            printHashOrder();
        }

        void printHashOrder()
        {
            for (auto& currentY : yByXMap)
            {
                std::cout << "Current Y value: " << currentY.first << std::endl;
                for (auto& currentX : currentY.second)
                {
                    std::cout << currentX << ", ";
                }
                std::cout << std::endl;
            }
        }

        std::string calculatedHash = "0";
        std::map<int, std::set<int>> yByXMap;
        bool valid = false;
};


#endif
