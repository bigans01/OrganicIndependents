#pragma once

#ifndef FINTERSECTMETA_H
#define FINTERSECTMETA_H


class FIntersectMeta
{
	public:
		ECBPolyPoint originPoint;
		ECBPolyPoint intersectedPoint;
		EnclaveKeyDef::EnclaveKey incrementingKey;
};

#endif
