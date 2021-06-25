#pragma once

#ifndef ENCLAVEBLOCKVERTEX_H
#define ENCLAVEBLOCKVERTEX_H

class EnclaveBlockVertex
{
public:
	unsigned char x = 0;
	unsigned char y = 0;
	unsigned char z = 0;

	// operator overloading (required)
	bool operator==(const EnclaveBlockVertex &other) const
	{
		return (x == other.x
			&&
			y == other.y
			&&
			z == other.z);
	}
};

#endif