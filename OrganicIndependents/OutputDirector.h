#pragma once

#ifndef OUTPUTDIRECTOR_H
#define OUTPUTDIRECTOR_H

#include "OutputDirectorMode.h"
#include <iomanip>
#include <sstream>

/*

Description: meant to "direct" output to a specific area, such as console, or to save it.
By "saving" it (via outputMode == OutputDirectorMode::OUTPUT_SAVE), all output is written 
to the recordedOutput string vector. This can then be retrieved by getRecordedOutput().

See this article, for some information on trying to understand lvale/rvalue types in C++.
https://stackoverflow.com/questions/42145453/type-not-deduced-to-be-r-value-reference-why-not

See this article, for why we need to use std::decay:
https://stackoverflow.com/questions/42416958/template-argument-deduction-from-string-literal

*/



class OutputDirector
{
	public:
		OutputDirector() {};
		OutputDirector(OutputDirectorMode in_initialMode) :
			outputMode(in_initialMode)
		{};


		// Below: the series of "log" functions, will keep appending to currentStringToWrite, until
		// it finds a newline character (/n)
		template<typename First, typename ...Rest> void log(First && first, Rest && ...rest)
		{
			switch (outputMode)
			{
				// If we are writing to std::cout, it will go directly there.
				case OutputDirectorMode::OUTPUT_STDCOUT:
				{
					std::cout << std::forward<First>(first);
					log(std::forward<Rest>(rest)...);
					break;
				}

				// If we are saving the output to be recorded, it must go in 
				// recordedOutput.
				case OutputDirectorMode::OUTPUT_SAVE:
				{
					// A refresher/in-depth understanding of lvalue vs rvalue references is needed,
					// in order to know why the is_same function call below works.

					
					// First check: check for actual std::string passed as argument.
					//if constexpr (std::is_same<First, std::string&>::value)	// check if First is an lvalue of type string.
					if constexpr (std::is_same<std::decay_t<First>, std::string>::value)	// check if First is an lvalue of type string.
					{
						//std::cout << "match to std::string! (rvalue, &)" << std::endl;
						analyzedAndInsertString(std::forward<First>(first));
					}
					
					
					// Second check: check if a const char[] array was passed as an argument; Because arrays are not copied
					// but instead referenced, this shows up as const char(&)[N], where N is the number of elements in the array.
					// We must decay the value of T, and check if it's const char. If it is, we can making a string out of it.
					// See article #2 that is linked above for more info on std::decay_t.
					else if constexpr (std::is_same<std::decay_t<First>, const char *>::value)
					{
						//std::cout << "match to const char*, (decay)" << std::endl;
						std::string convertedString(std::forward<First>(first));
						analyzedAndInsertString(convertedString);
					}


					// Third check: if we're inserting a float or double, make sure it's formatted appropriately
					// (precision, fixed, etc)
					else if constexpr
					(
						(std::is_same<std::decay_t<First>, float>::value)
						||
						(std::is_same<std::decay_t<First>, double>::value)
					)
					{
						std::stringstream stream;
						stream << std::fixed << std::setprecision(2) << std::forward<First>(first);
						currentStringToWrite += stream.str();
					}
					

					log(std::forward<Rest>(rest)...);
					break;
				}
			}
		}
		void log() {};

		// Below: the logLine set of functions does what you think: it can log multiple entries on the same line,
		// then at the end it will call a push_back (which is visually the same as ending the line, without a newline character)
		template<typename First, typename ...Rest> void logLine(First && first, Rest && ...rest)
		{
			// If we are writing to std::cout, it will go directly there.
			switch (outputMode)
			{
				case OutputDirectorMode::OUTPUT_STDCOUT:
				{
					std::cout << std::forward<First>(first);
					logLine(std::forward<Rest>(rest)...);
					break;
				}

				// If we are saving the output to be recorded, it must go in 
				// recordedOutput.
				case OutputDirectorMode::OUTPUT_SAVE:
				{
					// A refresher/in-depth understanding of lvalue vs rvalue references is needed,
					// in order to know why the is_same function call below works.


					// First check: check for actual std::string passed as argument.
					//if constexpr (std::is_same<First, std::string&>::value)	// check if First is an lvalue of type string.
					if constexpr (std::is_same<std::decay_t<First>, std::string>::value)	// check if First is an lvalue of type string.
					{
						currentStringToWrite += std::forward<First>(first);
					}

					// Second check: check if a const char[] array was passed as an argument; Because arrays are not copied
					// but instead referenced, this shows up as const char(&)[N], where N is the number of elements in the array.
					// We must decay the value of T, and check if it's const char. If it is, we can making a string out of it.
					// See article #2 that is linked above for more info on std::decay_t.
					else if constexpr (std::is_same<std::decay_t<First>, const char *>::value)
					{
						std::string convertedString(std::forward<First>(first));
						currentStringToWrite += convertedString;
					}

					// Third check: if we're inserting a float or double, make sure it's formatted appropriately
					// (precision, fixed, etc)
					else if constexpr
					(
						(std::is_same<std::decay_t<First>, float>::value)
						||
						(std::is_same<std::decay_t<First>, double>::value)
					)
					{
						std::stringstream stream;
						stream << std::fixed << std::setprecision(2) << std::forward<First>(first);
						currentStringToWrite += stream.str();
					}


					// Fourth check: anything else, ints; anything that could be translated via
					// std::to_string. 
					else
					{
						std::string convertedString = std::to_string(std::forward<First>(first));
						currentStringToWrite += convertedString;
					}

					logLine(std::forward<Rest>(rest)...);
					break;
				}
			}
		}
		void logLine() {

			// The pushback simulates a new line, in this case. Be sure
			// to append anything else that is left in currentStringToWrite.
			if (currentStringToWrite != "")
			{
				recordedOutput.push_back(currentStringToWrite);
				currentStringToWrite = "";
			}

			switch (outputMode)
			{
				case OutputDirectorMode::OUTPUT_STDCOUT:
				{
					std::cout << std::endl;
					break;
				}
			}
		};

		void analyzedAndInsertString(std::string in_stringToAnalyze);	// checks the passed-in string for newline characters, and pushes it back into the recordedOutput.
		std::vector<std::string> getRecordedOutput();					// return a copy of recordedOutput (remember, it will only be populated when outputMode is OUTPUT_SAVE.)
		void setOutputMode(OutputDirectorMode in_outputMode);

	private:
		OutputDirectorMode outputMode = OutputDirectorMode::OUTPUT_NONE;	// determines what happens when either the log or logLine 
																			// functions are called:
																			//		-if the value is OUTPUT_NONE, nothing happens.
																			//		-if the value is OUTPUT_STDCOUT, the output is written to std::cout.
																			//		-if the value is OUTPUT_SAVE, the outputs are saved to recordedOutput, where
																			//		 they can be retrieved by the call to getRecordedOutput().

		std::string currentStringToWrite = "";		// this variable contains the string that will eventually 
													// be pushed back into the recordedOutput vector.

		std::vector<std::string> recordedOutput;	// when the outputMode is set to OUTPUT_SAVE, the output will go here.
};

#endif