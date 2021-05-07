#pragma once

#ifndef BORDERMDFACELIST_H
#define BORDERMDFACELIST_H

#include "ECBPPOrientations.h"

class BorderMDFaceList
{
public:
	BorderMDFaceList();	// default constructor
	ECBPPOrientations faceList[3];
};

#endif