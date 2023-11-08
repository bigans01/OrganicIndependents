#pragma once

#ifndef FINTERSECTMETA_H
#define FINTERSECTMETA_H

#include "FTrianglePoint.h"


class FIntersectMeta
{
	public:
		FTrianglePoint originPoint;
		FTrianglePoint intersectedPoint;
		EnclaveKeyDef::EnclaveKey incrementingKey;
};

#endif
