#pragma once

#ifndef OPERABLE3DENCLAVEKEYSET_H
#define OPERABLE3DENCLAVEKEYSET_H

class Operable3DEnclaveKeySet
{
	public:
		Operable3DEnclaveKeySet() {};
		Operable3DEnclaveKeySet(std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_rawSet) : keySet(in_rawSet) {};

		// operator for raw set copy
		Operable3DEnclaveKeySet& operator=(const std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_rawSet)
		{
			keySet = in_rawSet;
			return *this;
		}

		Operable3DEnclaveKeySet& operator+=(const EnclaveKeyDef::EnclaveKey& in_keytoAdd)
		{
			keySet.insert(in_keytoAdd);
			return *this;
		}

		Operable3DEnclaveKeySet& operator+=(const Operable3DEnclaveKeySet& set_b)
		{
			auto otherSetBegin = set_b.keySet.begin();
			auto otherSetEnd = set_b.keySet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				keySet.insert(*otherSetBegin);
			}
			return *this;
		}

		Operable3DEnclaveKeySet& operator-=(const EnclaveKeyDef::EnclaveKey& in_keytoSubtract)
		{
			keySet.erase(in_keytoSubtract);
			return *this;
		}

		Operable3DEnclaveKeySet& operator-=(const Operable3DEnclaveKeySet& set_b)
		{
			auto otherSetBegin = set_b.keySet.begin();
			auto otherSetEnd = set_b.keySet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				keySet.erase(*otherSetBegin);
			}
			return *this;
		}


		void clear()
		{
			keySet.clear();
		}

		int size()
		{
			return int(keySet.size());
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>::iterator find(EnclaveKeyDef::EnclaveKey in_keyToFind)
		{
			return keySet.find(in_keyToFind);
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>::iterator begin()
		{
			return keySet.begin();
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher>::iterator end()
		{
			return keySet.end();
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> getSet()
		{
			return keySet;
		}

		void printSet()
		{
			auto setBegin = keySet.begin();
			auto setEnd = keySet.end();
			for (; setBegin != setEnd; setBegin++)
			{
				auto ptr = *setBegin;
				std::cout << "(";
				ptr.printKey();
				std::cout << ")" << std::endl;
			}
		}

	private:
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> keySet;
};

#endif