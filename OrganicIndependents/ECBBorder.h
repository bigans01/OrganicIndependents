#pragma once


// migrated over from OrganicCoreLib on 11/11/2019


#ifndef ECBBORDER_H
#define ECBBORDER_H

class ECBBorder
{
public:
	float move_x = 0.0f;	// if 0, indicates no changes at all
	float move_y = 0.0f;
	float move_z = 0.0f;
	EnclaveKeyDef::EnclaveKey getKey();
};

#endif