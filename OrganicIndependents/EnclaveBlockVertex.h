#pragma once

#ifndef ENCLAVEBLOCKVERTEX_H
#define ENCLAVEBLOCKVERTEX_H

#include "EnclaveBlockVertexTextureModeEnum.h"
#include "Message.h"

/*
* 
* Description: this class stores metadata about an individual point that is designed to be used in an EnclaveBlock.
* Members are kept private, and retrieved via varios get* style functions; constructors remain public for obvious reasons,
* and makes it easier to track where instances of EnclaveBlockVertex are being created.
* 
* The usesUV char value is a byte that stores flags on how the vertex should be used, and translates enum values
* from the enum EnclaveBlockVertexTextureModeEnum to their basic enum representation.
*
* The two UV values are used when needing to store metadata from an FTrianglePoint. Currently, as of 6/19/2025,
* not all points will contain FTrianglePoint data.

*/

class EnclaveBlockVertex
{
	public:
		EnclaveBlockVertex() {};
		EnclaveBlockVertex(unsigned char in_x, 
						   unsigned char in_y, 
						   unsigned char in_z,
						   EnclaveBlockVertexTextureModeEnum in_texturingModeValue,
			               float in_vertU,
			               float in_vertV) : 
			x(in_x), 
			y(in_y), 
			z(in_z),
			vertU(in_vertU),
			vertV(in_vertV)
		{
			switch (in_texturingModeValue)
			{
				case EnclaveBlockVertexTextureModeEnum::TILED_COORDS:
				{
					usesUV = 0;
					break;
				}

				case EnclaveBlockVertexTextureModeEnum::TEXTURE_COORDS:
				{
					usesUV = 1;
					break;
				}

				case EnclaveBlockVertexTextureModeEnum::UNDEFINED_COORDS:
				{
					usesUV = 2;
					break;
				}
			}
		}

		// operator overloading (required)
		bool operator==(const EnclaveBlockVertex &other) const
		{
			return (x == other.x
				&&
				y == other.y
				&&
				z == other.z);
		}

		EnclaveBlockVertexTextureModeEnum texturingModeValue = EnclaveBlockVertexTextureModeEnum::TILED_COORDS;	// default will be tiled coords.

		Message convertBlockVertexToMessage()	// converts the EnclaveBlockVertex to a Message.
		{
			Message convertedMessage;
			convertedMessage.insertInt(int(x));
			convertedMessage.insertInt(int(y));
			convertedMessage.insertInt(int(z));
			convertedMessage.insertInt(int(usesUV));
			convertedMessage.insertFloat(vertU);
			convertedMessage.insertFloat(vertV);
			return convertedMessage;
		}

		// increment/decrement functions
		void incrementX(unsigned char in_valueToIncrment) { x += in_valueToIncrment; }
		void decrementX(unsigned char in_valueToIncrment) { x -= in_valueToIncrment; }

		void incrementY(unsigned char in_valueToIncrment) { y += in_valueToIncrment; }
		void decrementY(unsigned char in_valueToIncrment) { y -= in_valueToIncrment; }

		void incrementZ(unsigned char in_valueToIncrment) { z += in_valueToIncrment; }
		void decrementZ(unsigned char in_valueToIncrment) { z -= in_valueToIncrment; }

		// x/y/z value return functions.
		unsigned char getVertexX() { return x; }
		unsigned char getVertexY() { return y; }
		unsigned char getVertexZ() { return z; }

	private:
		unsigned char x = 0;
		unsigned char y = 0;
		unsigned char z = 0;
		unsigned char usesUV = 0;	// used to store the UV usage mode; must be defined in constructor and not publically accessible.
		float vertU = 0.0f;	// if usesUV = 1, this would be the U coordinate in a 2d texture.
		float vertV = 0.0f; // if usesUV = 1, this would be the V coordinate in a 2d texture.

};

#endif