#pragma once

#ifndef MESSAGELOCALITY_H
#define MESSAGELOCALITY_H

enum class MessageLocality
{
	LOCAL,			// local to the OS, as in it's destined for an OrganicServer that is on this system
	REMOTE			// it is destined for another OS; could be on this system, or not...
};

#endif
