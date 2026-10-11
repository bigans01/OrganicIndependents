#pragma once

#ifndef ENCLAVEBLOCKSKELETON_H
#define ENCLAVEBLOCKSKELETON_H

class EnclaveBlockSkeleton
{
	public:
		TriangleMaterial materialID = TriangleMaterial::NOVAL;		// the materialID

		// Boost serialization function, for reading/writing (primarily for usage by OrganicRawEnclave)
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& materialID;
		}
};

#endif

