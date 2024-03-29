#pragma once

#ifndef EXCEPTIONRECORDTYPE_H
#define EXCEPTIONRECORDTYPE_H

/*

Description: Used to give more context to an instance of ExceptionRecord, to allow for easy debugging/parsing/switch statements.

*/

enum class ExceptionRecordType
{
	NOVAL,

	// EXCEPTION enums (that is, otherwise fatal errors to the program that would crash it or cause undefined behavior)
	EXCEPTION_INVALID_TYPICAL_SEQUENCE,	// used by: CleaveSequenceFactory::handleScenarioTypical. Indicates an invalid cleave sequence 
										//			was detected when attempting to use the typical method.
										// REQUIRES: a Message containing multiple categorized lines.

	EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED,	// used by: WeldedTriangleGroupBuilder::handleFinalObservation(). Indicates that the number of shifts
													//			(aka calls to WeldedTriangle::shiftLines() in this function) exceeded the max
													//			amount of 3, as there should never be more than 3 shifts occurring. 
													// REQUIRES: a Message containing two context strings.
	EXCEPTION_TERMINATION_ATTEMPTS_EXCEEDED	// used by: WeldedTriangleGroupBuilder::runTracingObservers(). Indicates that the function may have been stuck
											// in an infinite loop, which would have been flagged by too many instances where a TracingObserver had a 
											// currentObserverState value of TracingObserverState::TERMINATED. See that function for more details on usage.
											// This exception code should have a corresponding error message, containing the original WeldedLines, and
											// the remaining ones.
};

#endif
