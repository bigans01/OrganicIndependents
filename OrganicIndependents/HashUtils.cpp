#include  "stdafx.h"
#include  "HashUtils.h"
#pragma warning(disable : 4996)

std::string HashUtils::sha256(const std::string str)
{
	// This function is a copy/pasta from: https://terminalroot.com/how-to-generate-sha256-hash-with-cpp-and-openssl/
	//
	// Use that URL for a model, in the future, when needing to edit this function; this SHA256 code seems to work fine so far (11/7/2023)

	unsigned char hash[SHA256_DIGEST_LENGTH];

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	std::stringstream ss;

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return ss.str();
}

std::string HashUtils::convertVec3ToHash(glm::vec3 in_pointToConvert)
{
	// Step 1: multiply each float by 100 (to get rid of hundredths deciaml), then convert to int.
	int xConverted = static_cast<int>(in_pointToConvert.x * 100);
	int yConverted = static_cast<int>(in_pointToConvert.y * 100);
	int zConverted = static_cast<int>(in_pointToConvert.z * 100);

	// Step 2: convert each int to string; concatenate the string together.
	std::string xString = std::to_string(xConverted);
	std::string yString = std::to_string(yConverted);
	std::string zString = std::to_string(zConverted);

	//std::cout << "xString is: " << xString << std::endl;
	//std::cout << "yString is: " << yString << std::endl;
	//std::cout << "zString is: " << zString << std::endl;

	std::string combinedString = xString + yString + zString;
	//std::cout << "combinedString is: " << combinedString << std::endl;

	// Step 3: setup the hashing
	unsigned char hash[SHA256_DIGEST_LENGTH];

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, combinedString.c_str(), combinedString.size());
	SHA256_Final(hash, &sha256);

	std::stringstream ss;

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return ss.str();
}