#pragma once

#ifndef PERLINCLUSTERGENERATORENUM_H
#define PERLINCLUSTERGENERATORENUM_H

/* Description: PerlinClusterGeneratorEnum is meant to be used to identify the type of blueprint data generation that a PerlinCluster
* will ultimately produce when a PerlinCluster calls it's generate() function.

*/

enum class PerlinClusterGeneratorEnum
{
	PERLIN_NOGENVAL,
	PERLIN_MOUNTAIN
};

#endif
