/*------------------------------------------------------------------------------------------

--OrganicVtxColorFaceMeta.h (Last Update: 9/4/2017)

Description: Contains meta data for a cube-based texture that will be used in the Organic system.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORFACEMETA_H
#define ORGANICVTXCOLORFACEMETA_H

#include "OrganicVtxColorRGB.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicVtxColorFaceMeta
{
	private:
		friend class OrganicSystem;
		friend class EnclaveManifestFactoryT1;
		friend class EnclaveManifest;
		friend class OrganicRawManifest;
		OrganicVtxColorRGB FaceMeta[6];			// an array for storing 6 RGB float triplet values (one block face has 6 of these)
};

#endif