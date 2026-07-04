#include "stdafx.h"
#include "FatFan.h"

int FatFan::getPointAtIndex(int in_pointArrayIndex)
{
	return thinArray[in_pointArrayIndex];
}

void FatFan::fillPointIndex(int in_pointArrayIndex, int in_pointID)
{
	thinArray[in_pointArrayIndex] = in_pointID;
}

FanData FatFan::getFanData()
{
	FanData returnData(
		thinArray,
		materialID,
		numberOfTertiaries,
		faceAlignment,
		emptyNormal);
	return returnData;
}

void FatFan::printPoints()
{
	{
		if (numberOfTertiaries != 0)
		{
			int numberOfPoints = 2 + numberOfTertiaries;
			for (int x = 0; x < numberOfPoints; x++)
			{
				std::cout << "Fan point index at [" << x << "]: " << thinArray[x] << std::endl;
			}
		}
	}
}

void FatFan::buildFromFanData(FanData in_fanData)
{
	numberOfTertiaries = in_fanData.numberOfTertiaries;

	// Remember: the number of points is equal to the number of tertiaries + 2.
	for (int x = 0; x < numberOfTertiaries + 2; x++)
	{
		thinArray[x] = in_fanData.pointArray[x];
	}

	materialID = in_fanData.materialID;
	faceAlignment = in_fanData.faceAlignment;
	emptyNormal = in_fanData.emptyNormal;
}

bool FatFan::setFanEmptyNormalDebug(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices)
{
	bool flipRequired = false;

	// Step 1: Get the unit vector of the parent empty normal.
	glm::vec3 parentUnitVector = glm::normalize(glm::vec3(in_emptyNormal.x, in_emptyNormal.y, in_emptyNormal.z));

	// Step 2: the 3 points input during a call to this function (see in_determiningVertices above) will be the 
	// basis for determining if the current fan is in the correct point order. Two calculation must be made:
	// 
	// original unit vector = cross product of vectors from points 0 -> 1, 0 -> 2, and then normalized.
	// reversed unit vector = cross product of vectors from points 0 -> 2, 0 -> 1, and then normalized.

	// generate common points
	ECBPolyPoint determiningPoint0 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointA);
	ECBPolyPoint determiningPoint1 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointB);
	ECBPolyPoint determiningPoint2 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointC);

	// calculate the vectors.
	ECBPolyPoint vectorA = determiningPoint1 - determiningPoint0;
	ECBPolyPoint vectorB = determiningPoint2 - determiningPoint0;

	glm::vec3 u(vectorA.x, vectorA.y, vectorA.z);
	glm::vec3 v(vectorB.x, vectorB.y, vectorB.z);

	// calculate the original unit vector - cross(u,v)
	glm::vec3 originalNormal = normalize(cross(u, v));

	// calculate the reverse unit vector - cross(v,u)
	glm::vec3 reverseNormal = normalize(cross(v, u));


	std::cout << "input value empty normal: " << in_emptyNormal.x << ", " << in_emptyNormal.y << ", " << in_emptyNormal.z << std::endl;
	std::cout << "originalNormal: " << originalNormal.x << ", " << originalNormal.y << ", " << originalNormal.z << std::endl;
	std::cout << "reverseNormal: " << reverseNormal.x << ", " << reverseNormal.y << ", " << reverseNormal.z << std::endl;

	float distToOriginal = glm::distance(parentUnitVector, originalNormal);
	float distToReverse = glm::distance(parentUnitVector, reverseNormal);

	std::cout << "distToOriginal:" << distToOriginal << std::endl;
	std::cout << "distToReverse:" << distToReverse << std::endl;

	// If the reverse distance is closest to the parent unit vectorm, we must swap the points 
	bool reverseRequired = false;
	if (distToReverse < distToOriginal)
	{
		std::cout << "!! Reversing required! " << std::endl;

		reverseRequired = true;
		emptyNormal = reverseNormal;
		flipRequired = true;
	}
	else
	{
		emptyNormal = originalNormal;
	}

	std::cout << "!! emptyNormal is now: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;

	return flipRequired;
}

bool FatFan::setEmptyNormalAndCheckAlignment(ECBPolyPoint in_emptyNormal, EnclaveBlockVertexTri in_determiningVertices)
{
	bool flipRequired = false;

	// Step 1: Get the unit vector of the parent empty normal.
	glm::vec3 parentUnitVector = glm::normalize(glm::vec3(in_emptyNormal.x, in_emptyNormal.y, in_emptyNormal.z));

	// Step 2: the 3 points input during a call to this function (see in_determiningVertices above) will be the 
	// basis for determining if the current fan is in the correct point order. Two calculation must be made:
	// 
	// original unit vector = cross product of vectors from points 0 -> 1, 0 -> 2, and then normalized.
	// reversed unit vector = cross product of vectors from points 0 -> 2, 0 -> 1, and then normalized.

	// generate common points
	ECBPolyPoint determiningPoint0 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointA);
	ECBPolyPoint determiningPoint1 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointB);
	ECBPolyPoint determiningPoint2 = IndependentUtils::convertEnclaveBlockVertexToFloats(in_determiningVertices.pointC);

	// calculate the vectors.
	ECBPolyPoint vectorA = determiningPoint1 - determiningPoint0;
	ECBPolyPoint vectorB = determiningPoint2 - determiningPoint0;

	glm::vec3 u(vectorA.x, vectorA.y, vectorA.z);
	glm::vec3 v(vectorB.x, vectorB.y, vectorB.z);

	// calculate the original unit vector - cross(u,v)
	glm::vec3 originalNormal = normalize(cross(u, v));

	// calculate the reverse unit vector - cross(v,u)
	glm::vec3 reverseNormal = normalize(cross(v, u));

	float distToOriginal = glm::distance(parentUnitVector, originalNormal);
	float distToReverse = glm::distance(parentUnitVector, reverseNormal);

	// If the reverse distance is closest to the parent unit vectorm, we must swap the points 
	bool reverseRequired = false;
	if (distToReverse < distToOriginal)
	{
		reverseRequired = true;
		emptyNormal = reverseNormal;
		flipRequired = true;
	}
	else
	{
		emptyNormal = originalNormal;
	}

	return flipRequired;
}

std::vector<unsigned int> FatFan::fetchExpandedFanIndices()
{
	// The fan should be generated in the following manner:
	// use two vectors:
	//  vector 1 is from point at to index 1 - point at index 0
	//  vector 2 is from the last point in the index - point at index 0

	std::vector<unsigned int> returnVector;
	for (int x = 0; x < 2; x++)
	{
		returnVector.push_back(thinArray[x]);
	}

	// For 3rd point, it should be the last point in the fan that is not 0. So, number of tertiaries + 1.
	returnVector.push_back(thinArray[numberOfTertiaries + 1]);
	

	return returnVector;
}