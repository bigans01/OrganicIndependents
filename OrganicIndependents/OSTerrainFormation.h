#pragma once

#ifndef OSTERRAINFORMATION_H
#define OSTERRAINFORMATION_H

// This class is used by OrganicServerLib and OrganicCoreLib to recognize the different types of terrain formations; the OrganicCoreLib needs to use it
// to specify a particular formation request in a Message that is then sent, and interperted by, the OrganicServer it is interfacing with (local or remote)

enum class OSTerrainFormation
{
	MOUNTAIN,
	NOVAL
};

#endif
