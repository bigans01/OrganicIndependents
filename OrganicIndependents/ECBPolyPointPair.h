#pragma once

#ifndef ECBPOLYPOINTPAIR_H
#define ECBPOLYPOINTPAIR_H



class ECBPolyPointPair
{
public:
	ECBPolyPointPair() {};
	ECBPolyPointPair(ECBPolyPoint in_pointA,
		ECBPolyPoint in_pointB) :
		pointA(in_pointA),
		pointB(in_pointB)
	{};

	ECBPolyPoint pointA;
	ECBPolyPoint pointB;
};

#endif
