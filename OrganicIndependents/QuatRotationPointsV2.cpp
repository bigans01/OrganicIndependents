#include "stdafx.h"
#include "QuatRotationPointsV2.h"


void QuatRotationPointsV2::applyQuaternion(glm::quat in_quat)
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		currentProxyValue->applyQuat(in_quat);
	}
}

void QuatRotationPointsV2::applyTranslation(glm::vec3 in_translation)
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		// Get the current vec3 version, then apply the translation, and then send that copy back.
		glm::vec3 currentProxyGlmVec3 = currentProxyValue->getVec3();
		currentProxyGlmVec3 += in_translation;
		currentProxyValue->setVec3(currentProxyGlmVec3);
	}
}

void QuatRotationPointsV2::applyTranslationToIndexRange(glm::vec3 in_translation, int in_indexStart, int in_indexEnd)
{
	auto currentProxyValue = pointProxiesVector.begin() + in_indexStart;
	int numberOfIterations = (in_indexEnd - in_indexStart) + 1; // number of iterations = to the difference between the index range, +1 (to include the beginning of the range)
	for (int x = 0; x < numberOfIterations; x++)
	{
		glm::vec3 currentProxyGlmVec3 = currentProxyValue->get()->getVec3();
		currentProxyGlmVec3 += in_translation;
		currentProxyValue->get()->setVec3(currentProxyGlmVec3);
		currentProxyValue++;
	}
}

void QuatRotationPointsV2::printTrianglePoints()
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		glm::vec3 currentVec3 = currentProxyValue->getVec3();
		std::cout << "##-> Point: " << currentVec3.x << ", " << currentVec3.y << ", " << currentVec3.z << std::endl;
	}
}

void QuatRotationPointsV2::applyNormalization(float in_normalizationValue)
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		glm::vec3 currentVec3 = currentProxyValue->getVec3();
		currentVec3.x = currentVec3.x / in_normalizationValue;
		currentVec3.y = currentVec3.y / in_normalizationValue;
		currentProxyValue->setVec3(currentVec3);
	}
}

void QuatRotationPointsV2::applyDimensionCorrections()
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		glm::vec3 currentVec3 = currentProxyValue->getVec3();

		// check the x value
		if (currentVec3.x > 1.0f)
		{
			currentVec3.x = 1.0f;
		}
		else if (currentVec3.x < 0.0f)
		{
			currentVec3.x = 0.0f;
		}

		// check the y value
		if (currentVec3.y > 1.0f)
		{
			currentVec3.y = 1.0f;
		}
		else if (currentVec3.y < 0.0f)
		{
			currentVec3.y = 0.0f;
		}

		currentProxyValue->setVec3(currentVec3);
	}
}

void QuatRotationPointsV2::printPoints()
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		std::cout << "Found point: ";
		glm::vec3 currentVec3 = currentProxyValue->getVec3();
		std::cout << currentVec3.x << ", " << currentVec3.y << ", " << currentVec3.z << std::endl;
	}
}

void QuatRotationPointsV2::clearPoints()
{
	pointProxiesVector.clear();
}

void QuatRotationPointsV2::roundAllPointsToHundredths()
{
	for (auto& currentProxyValue : pointProxiesVector)
	{
		glm::vec3 currentVec3 = currentProxyValue->getVec3();
		currentVec3 = IndependentUtils::roundVec3ToHundredths(currentVec3);
		currentProxyValue->setVec3(currentVec3);
	}
}

int QuatRotationPointsV2::getSize()
{
	return int(pointProxiesVector.size());
}

void QuatRotationPointsV2::eraseLastElement()
{
	pointProxiesVector.pop_back();
}

glm::vec3 QuatRotationPointsV2::getFirstPoint()
{
	auto pointsStart = pointProxiesVector.begin();
	return pointsStart->get()->getVec3();
}

glm::vec3 QuatRotationPointsV2::getSecondPoint()
{
	auto pointsStart = pointProxiesVector.begin();
	pointsStart++;
	return pointsStart->get()->getVec3();
}

glm::vec3 QuatRotationPointsV2::getThirdPoint()
{
	auto pointsStart = pointProxiesVector.begin();
	pointsStart++;
	pointsStart++;
	return pointsStart->get()->getVec3();
}

glm::vec3 QuatRotationPointsV2::getPointByIndex(int in_index)
{
	auto pointsStart = pointProxiesVector.begin();
	if (in_index == 0)	// get the first point
	{
		return pointsStart->get()->getVec3();
	}
	else if (in_index != 0)
	{
		for (int x = 0; x < in_index; x++)	// iterate a number of times equal to in_index, if it isn't 0
		{
			pointsStart++;	// iterate
		}
		return pointsStart->get()->getVec3();	// return whatever returns after in_index number of iterations
	}
}