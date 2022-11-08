#pragma once

#ifndef VEC3PAIR_H
#define VEC3PAIR_H


class Vec3Pair
{
	public:
		Vec3Pair() {};
		Vec3Pair(glm::vec3 in_vecA, glm::vec3 in_vecB) :
			vecA(in_vecA),
			vecB(in_vecB)
		{};

		Vec3Pair& operator=(const ECBPolyPointPair& in_pairToCopy)
		{
			vecA.x = in_pairToCopy.pointA.x;
			vecA.y = in_pairToCopy.pointA.y;
			vecA.z = in_pairToCopy.pointA.z;

			vecB.x = in_pairToCopy.pointB.x;
			vecB.y = in_pairToCopy.pointB.y;
			vecB.z = in_pairToCopy.pointB.z;

			return *this;
		}


		glm::vec3 vecA;
		glm::vec3 vecB;
};

#endif


