/*

Initial creation:        7/7/2017

Description: 

The EnclaveKeyDef class contains a 3D key (EnclaveKey) and a 2D key (Enclave2DKey)
it allows for using hash maps (such as unordered_map, unordered_set) that can store
three-dimensional values. It is used extensively in many libraries.

maps and unordered_maps must specify the KeyHasher hashing function, to make the map valid.



*/

#pragma once

#ifndef ENCLAVEKEY_H
#define ENCLAVEKEY_H

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

		EnclaveKey operator+(const EnclaveKey& enclaveKey_b)
		{
			EnclaveKey returnKey;
			returnKey.x = x + enclaveKey_b.x;
			returnKey.y = y + enclaveKey_b.y;
			returnKey.z = z + enclaveKey_b.z;
			return returnKey;
		}

		EnclaveKey operator-(const EnclaveKey& enclaveKey_b)
		{
			EnclaveKey returnKey;
			returnKey.x = x - enclaveKey_b.x;
			returnKey.y = y - enclaveKey_b.y;
			returnKey.z = z - enclaveKey_b.z;
			return returnKey;
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

		// simply prints the key to std::cout.
		void printKey()
		{
			std::cout << " (" << x << ", " << y << ", " << z << ") ";
		}

		// simply return the inverted key.
		EnclaveKey getInvertedKey()
		{
			return EnclaveKey(x*-1, y*-1, z*-1);
		}

		// returns an std::string that contains the key coordinates.
		std::string getKeyString()
		{
			return std::string("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
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

		void printKey()
		{
			std::cout << " (" << a << ", " << b << ") ";
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