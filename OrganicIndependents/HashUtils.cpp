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