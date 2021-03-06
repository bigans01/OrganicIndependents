/*------------------------------------------------------------------------------------------


Initial creation:        7/7/2017
--EnclaveKeyDef.h		(Last update 11/11/2019) (Moved to OrganicPolyOperationsLib)

Description: Header file for EnclaveKeyDef.cpp.

Summary: EnclaveKeyDef.h supplies the definition that will be used for the key values in any type of collection. The Key is made up of a x/y/z coordinate, and is meant to be put
into an unordered map. A hash for this is then constructed, in accordance with the rules for an unordered map. Examples of classes that use this are:

-EnclaveCollectionMap (member variable: EnclaveCollectionMap)
-ManifestCollectionMap (member variable: ManiCollectionMap)
-RenderCollectionMap (member variable: RenderMatrix)

Dependents: none


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEKEY_H
#define ENCLAVEKEY_H

#include <iostream>

class EnclaveKeyDef {

public:
	// forms the structure of the Key itself.
	struct EnclaveKey
	{
		int x = 0;
		int y = 0;
		int z = 0;

		EnclaveKey() {};
		EnclaveKey(int in_x, int in_y, int in_z) : x(in_x), y(in_y), z(in_z) {}

		// addition operator overload
		EnclaveKey& operator+=(const EnclaveKey& enclaveKey_b)
		{
			x += enclaveKey_b.x;
			y += enclaveKey_b.y;
			z += enclaveKey_b.z;
			return *this;
		}

		// subtraction operator overload
		EnclaveKey& operator-=(const EnclaveKey& enclaveKey_b)
		{
			x -= enclaveKey_b.x;
			y -= enclaveKey_b.y;
			z -= enclaveKey_b.z;
			return *this;
		}

		// operator overloading (required)
		bool operator==(const EnclaveKey &other) const
		{
			return (x == other.x
				&&
				y == other.y
				&&
				z == other.z);
		}
		bool operator!=(const EnclaveKey &other) const
		{
			return
			(x != other.x
			||
			y != other.y
			||
			z != other.z);
		}
		
		// checks if the key equals this exact x/y/z
		bool equals(int in_x, int in_y, int in_z)
		{
			bool equalsValue = false;
			if
			(
				(x == in_x)
				&&
				(y == in_y)
				&&
				(z == in_z)
			)
			{
				equalsValue = true;
			}
			return equalsValue;
		}
	};

	struct Enclave2DKey			// for use with EnclaveUnveilMeta only (subject to change later)
	{
		int a = 0;
		int b = 0;

		Enclave2DKey() {};
		Enclave2DKey(int in_a, int in_b) : a(in_a), b(in_b) {};

		bool equals(int in_a, int in_b)
		{
			bool equalsValue = false;
			if
			(
				(a == in_a)
				&&
				(b == in_b)
			)
			{
				equalsValue = true;
			}
			return equalsValue;
		}

		bool operator==(const Enclave2DKey &other) const
		{
			return (a == other.a
				&&
				b == other.b
				);
		}
	};


	struct KeyHasher
	{
		// hash generation function, required to be specified when put into an unordered map. 
		std::size_t operator()(const EnclaveKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			return (
				(hash<int>()(k.x)
					^ (hash<int>()(k.y) << 1)
					) >> 1)
				^ (hash<int>()(k.z) << 1);
		}

		std::size_t operator()(const Enclave2DKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			return (hash<int>()(k.a)
				^ (hash<int>()(k.b) << 1)
				) >> 1;
		}
	};


};

#endif