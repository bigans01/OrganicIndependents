#pragma once

#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <openssl/ssl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "FTrianglePoint.h"

/*

Description: A simple utility class that can contain various hashing functions.

*/

class HashUtils
{
	public:
		static std::string sha256(const std::string str);	// converts a string to a corresponding SHA-256 value.
		static std::string convertVec3ToHash(glm::vec3 in_pointToConvert); // converts a glm::vec3
		static std::string convertFTrianglePointToHash(FTrianglePoint in_pointToConvert); // converts a glm::vec3

	private:
};

#endif
