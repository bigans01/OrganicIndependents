#pragma once

#ifndef MESSAGECONTAINERTYPE_H
#define MESSAGECONTAINERTYPE_H

/*

Summary: a simple enum class used to describe what a MessageContainer instance should be used for.

*/

enum class MessageContainerType
{
	MC_UNDEFINED,	// default value; should almost always be set when using a MessageContainer.
	MC_BDM			// used to indcate that the data inside the MessageContainer pertains to Blueprint Data Messaging (BDM)
};

#endif
