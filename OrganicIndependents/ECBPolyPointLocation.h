#pragma once


#ifndef ECBPOLYPOINTLOCATION_H
#define ECBPOLYPOINTLOCATION_H



class ECBPolyPointLocation
{
public:
	EnclaveKeyDef::EnclaveKey enclaveKey;
	EnclaveKeyDef::EnclaveKey blockKey;
	ECBPolyPoint preciseCoord;
};

#endif
