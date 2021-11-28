#pragma once

#ifndef PATOM2DXY_H
#define PATOM2DXY_H

#include "PAtomBase.h"


class PAtom2DXY : public PAtomBase
{
	public:
		void expand();
	private:
		EnclaveKeyDef::EnclaveKey generateExpansionKey(int in_x, int in_y);
		ECBPolyPoint generateExpansionPoint(int in_x, int in_y);
		void insertFusableAreaFor2DXY(int in_x, int in_y);

};

#endif

