#pragma once

#ifndef ECBPPORIENTATIONRESULTS_H
#define ECBPPORIENTATIONRESULTS_H

#include "ECBPPOrientations.h"

class ECBPPOrientationResults
{
public:
	ECBPPOrientations otype = ECBPPOrientations::NOVAL;		// would be corner, line, face; given a dummy value for initialization
	ECBPPOrientations osubtype = ECBPPOrientations::NOVAL;		// upperSW line, corner etc
};

#endif
