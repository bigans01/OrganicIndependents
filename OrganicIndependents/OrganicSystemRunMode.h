#pragma once

#ifndef ORGANICSYSTEMRUNMODE_H
#define ORGANICSYSTEMRUNMODE_H

enum class OrganicSystemRunMode
{
	SERVER,		// the server will run by itself; no OpenGL or GUI support/interaction
	COMBINED	// runs a server + client, interactive through OpenGL/GUI.
};

#endif
