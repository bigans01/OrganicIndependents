#pragma once

#ifndef ENCLAVEBLOCKVERTEX_H
#define ENCLAVEBLOCKVERTEX_H

class EnclaveBlockVertex
{
public:
	EnclaveBlockVertex() {};
	EnclaveBlockVertex(unsigned char in_x, unsigned char in_y, unsigned char in_z) : x(in_x), y(in_y), z(in_z) {}

	// operator overloading (required)
	bool operator==(const EnclaveBlockVertex &other) const
	{
		return (x == other.x
			&&
			y == other.y
			&&
			z == other.z);
	}

	unsigned char x = 0;
	unsigned char y = 0;
	unsigned char z = 0;

	Message convertBlockVertexToMessage()
	{
		Message convertedMessage;
		convertedMessage.insertInt(int(x));
		convertedMessage.insertInt(int(y));
		convertedMessage.insertInt(int(z));
		return convertedMessage;
	}
};

#endif