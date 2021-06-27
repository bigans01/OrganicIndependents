#pragma once

#ifndef ECBINTERSECTMETA_H
#define ECBINTERSECTMETA_H

#include "EnclaveKeyDef.h"
#include "ECBPolyPoint.h"

class ECBIntersectMeta
{
public:
	ECBPolyPoint originPoint;
	ECBPolyPoint intersectedPoint;
	EnclaveKeyDef::EnclaveKey incrementingKey;
};

#endif#
