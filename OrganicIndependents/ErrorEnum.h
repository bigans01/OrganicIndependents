#pragma once

#ifndef ERRORENUM_H
#define ERRORENUM_H

enum class ErrorEnum
{
	NONE,
	TEST,
	CUTLINEWELDER_BAD_TYPICAL_RUN,	// indicates that CutLineWelder::handleTypicalRun encountered an infinite while loop.
	CUTLINEWELDER_BAD_SLICE_RUN,		// indicates that CutLineWelder::handleSliceRun encountered an infinite while loop.
	CUTTIRANGLEGROUPBUILDER_BAD_BUILD	// indicates that CutTriangleGroupBuilder::runCutTraceObserver() failed to extract lines

};

#endif
