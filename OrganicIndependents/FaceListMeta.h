#pragma once

#ifndef FACELISTMETA_H
#define FACELISTMETA_H

#include "BorderMDFaceList.h"

class FaceListMeta
{
public:
	BorderMDFaceList faceList;
	int numberOfIntercepts = 0;
};

#endif
