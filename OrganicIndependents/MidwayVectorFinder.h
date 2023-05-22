#pragma once

#ifndef MIDWAYVECTORFINDER_H
#define MIDWAYVECTORFINDER_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class MidwayVectorFinder
{
	public:
		MidwayVectorFinder(glm::vec3 in_vectorA, glm::vec3 in_vectorB);
		glm::vec3 getProducedMidwayUnitVector();
	private:
		glm::vec3 vectorA;
		glm::vec3 vectorB;
		glm::vec3 midwayUnitVector;

		float getRadiansBetweenUnitVectors(glm::vec3 in_vectorA, glm::vec3 in_vectorB);
		glm::quat produceQuaternion(float in_radians, glm::vec3 in_quaternionAxisUnitVector);
		bool testIfHalfwayQuatProducesMidwayVector(float in_halfwayQuatValue, 
												   glm::quat in_quatForCheck,
												   float in_trueRadianValue);
};

#endif
