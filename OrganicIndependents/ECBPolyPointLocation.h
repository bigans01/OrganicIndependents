#pragma once


#ifndef ECBPOLYPOINTLOCATION_H
#define ECBPOLYPOINTLOCATION_H

#include "EnclaveKeyDef.h"
#include "ECBPolyPoint.h"

class ECBPolyPointLocation
{
public:
	EnclaveKeyDef::EnclaveKey enclaveKey;
	EnclaveKeyDef::EnclaveKey blockKey;
	ECBPolyPoint preciseCoord;
};

#endif
