#include "stdafx.h"
#include "MidwayVectorFinder.h"

MidwayVectorFinder::MidwayVectorFinder(glm::vec3 in_vectorA, glm::vec3 in_vectorB)
{
	// Step one: normalize the two involved vectors.
	vectorA = glm::normalize(in_vectorA);
	vectorB = glm::normalize(in_vectorB);

	// Step 2: calculate the true radians between the vectors.
	float trueRadians = getRadiansBetweenUnitVectors(in_vectorA, in_vectorB);

	std::cout << "++++++++++++++++++++ Start, midway vector finding. " << std::endl;
	std::cout << "(MidwayVectorFinder::MidwayVectorFinder): trueRadians value is: " << trueRadians << std::endl;

	// Step 3: create the quaternion we will use as an "axis" to rotate the degrees on.
	// To do this, we need to use the vector that is a cross product of the input vectors (this resulting vector is the axis of the quaternion)
	// It must then be normalized to a unit vector.
	glm::vec3 quatAxisBase = cross(in_vectorA, in_vectorB);
	float quatUnitVectorDivisor = sqrt(pow(quatAxisBase.x, 2.0f) + pow(quatAxisBase.y, 2.0f) + pow(quatAxisBase.z, 2.0f));
	glm::vec3 quatAxisUnitVectored = quatAxisBase / quatUnitVectorDivisor;
	std::cout << "Value of quaternion axis unit vector: " << quatAxisUnitVectored.x << ", " << quatAxisUnitVectored.y << ", " << quatAxisUnitVectored.z << std::endl;
	
	// Store a value of the trueRadians /2.
	float halfRadians = trueRadians / 2;

	// Create a quat that uses a radian value equal to half trueRadians value. We will need this for the next test.
	glm::quat halfTrueRadianQuat = produceQuaternion(halfRadians, quatAxisUnitVectored);

	bool isRotateDirectionAccurate = testIfHalfwayQuatProducesMidwayVector(halfRadians, halfTrueRadianQuat, trueRadians);
	if (isRotateDirectionAccurate == true)
	{
		midwayUnitVector = in_vectorB * halfTrueRadianQuat;
		std::cout << "Value of original rotated quat is now: " << midwayUnitVector.x << ", " << midwayUnitVector.y << ", " << midwayUnitVector.z << std::endl;
	}

	else if (isRotateDirectionAccurate == false)
	{
		std::cout << "NOTICE: original radian application value of " << halfRadians << " produced an out-of-bounds vector. Will now use opposite value. " << std::endl;
		float invertedHalfRadians = halfRadians *= -1.0f;

		glm::quat invertedHalfTrueRadianQuat = produceQuaternion(invertedHalfRadians, quatAxisUnitVectored);
		midwayUnitVector = in_vectorB * invertedHalfTrueRadianQuat;
		std::cout << "Value of reverse rotated quat is now: " << midwayUnitVector.x << ", " << midwayUnitVector.y << ", " << midwayUnitVector.z << std::endl;
	}
}

glm::vec3 MidwayVectorFinder::getProducedMidwayUnitVector()
{
	return midwayUnitVector;
}

float MidwayVectorFinder::getRadiansBetweenUnitVectors(glm::vec3 in_vectorA, glm::vec3 in_vectorB)
{
	float radians = 0.0f;

	// We will assume the input vectors are actually normalized unit vectors here.
	// Reference: https://www.omnicalculator.com/math/angle-between-two-vectors.

	// We need to divide the dot prodcut of the input vectors, by the product of the magnitutes of both input vectors.

	// Get the dot (for the numerator)
	float numeratorDot = glm::dot(in_vectorA, in_vectorB);

	// Get the product of the magnitudes (for the denominator)
	float lowerMagA = sqrt(pow(in_vectorA.x, 2.0f) + pow(in_vectorA.y, 2.0f) + pow(in_vectorA.z, 2.0f));
	float lowerMagB = sqrt(pow(in_vectorB.x, 2.0f) + pow(in_vectorB.y, 2.0f) + pow(in_vectorB.z, 2.0f));
	float lowerCombo = lowerMagA * lowerMagB;

	radians = acos(numeratorDot / lowerCombo);

	return radians;
}

glm::quat MidwayVectorFinder::produceQuaternion(float in_radians, glm::vec3 in_quaternionAxisUnitVector)
{
	return glm::quat(cos(in_radians / 2), 
					sin(in_radians / 2)*in_quaternionAxisUnitVector.x, 
					sin(in_radians / 2)*in_quaternionAxisUnitVector.y, 
					sin(in_radians / 2)*in_quaternionAxisUnitVector.z);
}

bool MidwayVectorFinder::testIfHalfwayQuatProducesMidwayVector(float in_halfwayQuatValue, 
															   glm::quat in_quatForCheck,
															   float in_trueRadianValue)
{
	bool halfwayFound = true;

	// Step 1: Create a new vector produced by the quaternion, using the value of vectorB.
	glm::vec3 rotatedVectorB = vectorB * in_quatForCheck;

	// Step 2: take the new rotatedVector, and do two measurements: 
	// -the radians between rotatedVectorB and vectorB
	// -the radians between rotatedVectorB and vectorA
	float radiansFromNewVecToB = getRadiansBetweenUnitVectors(vectorB, rotatedVectorB);
	float radiansFromNewVecToA = getRadiansBetweenUnitVectors(vectorA, rotatedVectorB);

	// Step 3: If the values of radiansFromNewVecToB or radiansFromNewVecToA are greater than 
	// 
	if
	(
		(radiansFromNewVecToB > in_trueRadianValue)
		||
		(radiansFromNewVecToA > in_trueRadianValue)
	)
	{
		std::cout << "(MidwayVectorFinder::testIfHalfwayQuatProducesMidwayVector): rotatedVectorB is outside the bounds of the two vectors. " << std::endl;
		std::cout << "trueRadianValue: " << in_trueRadianValue << std::endl;
		std::cout << "radiansFromNewVecToB value: " << radiansFromNewVecToB << std::endl;
		std::cout << "radiansFromNewVecToA value: " << radiansFromNewVecToA << std::endl;
		halfwayFound = false;
	}
	std::cout << "(MidwayVectorFinder::testIfHalfwayQuatProducesMidwayVector): rotatedVectorB point is: " << rotatedVectorB.x << ",  " << rotatedVectorB.y << ", " << rotatedVectorB.z << std::endl;

	return halfwayFound;
}