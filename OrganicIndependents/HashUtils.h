#pragma once

#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <openssl/ssl.h>
#include <iomanip>
#include <iostream>
#include <sstream>

/*

Description: A simple utility class that can contain various hashing functions.

*/

class HashUtils
{
	public:
		static std::string sha256(const std::string str);

	private:
};

#endif
