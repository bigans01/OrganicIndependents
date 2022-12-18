#pragma once

#ifndef FTRACEORIENTATIONRESULTS_H
#define FTRACEORIENTATIONRESULTS_H

#include "ECBPPOrientations.h"

class FTraceOrientationResults
{
	public:
		ECBPPOrientations otype = ECBPPOrientations::NOVAL;		// would be corner, line, face; given a dummy value for initialization
		ECBPPOrientations osubtype = ECBPPOrientations::NOVAL;		// upperSW line, corner etc
};

#endif
