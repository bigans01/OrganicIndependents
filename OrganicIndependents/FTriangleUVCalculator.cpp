#include "stdafx.h"
#include "FTriangleUVCalculator.h"

void FTriangleUVCalculator::buildShadowPoints()
{
	for (auto& currentPoint : targetPointsForCalculations->points)
	{
		FTrianglePointShadow newShadowPoint(&currentPoint);
		shadowPointVector.push_back(newShadowPoint);
	}
}

void FTriangleUVCalculator::calculateShadowTriangleCentroid()
{
	shadowTriangleCentroid = IndependentUtils::determineTriangleCentroid(FTriangleUtils::convertDoubleToECBPolyPoint(barycentricBasisPoints[0].point),
		                                                                    FTriangleUtils::convertDoubleToECBPolyPoint(barycentricBasisPoints[1].point),
			                                                                FTriangleUtils::convertDoubleToECBPolyPoint(barycentricBasisPoints[2].point));
}

void FTriangleUVCalculator::calculateShadowTriangleNormal()
{
	DoublePoint doubleU = barycentricBasisPoints[1].point - barycentricBasisPoints[0].point;
	DoublePoint doubleV = barycentricBasisPoints[2].point - barycentricBasisPoints[0].point;
	glm::vec3 u(doubleU.x, doubleU.y, doubleU.z);
	glm::vec3 v(doubleV.x, doubleV.y, doubleV.z);

	shadowTriangleNormal = glm::normalize(cross(u, v));	// the normal of the triangle, represented as a unit vector.

}

void FTriangleUVCalculator::applyCentroidTranslationToShadowPoints()
{
	// get the inverse, and add it to all points (including the barycentric basis points)
	DoublePoint invertedCentroid = shadowTriangleCentroid;
	invertedCentroid *= -1.0f;

	// apply the centroid to all points.
	for (auto& currentShadowPoint : shadowPointVector)
	{
		currentShadowPoint.fetchShadowPoint()->point += invertedCentroid;
	}

	// apply to the barycentric basis coords.
	for (int x = 0; x < 3; x++)
	{
		barycentricBasisPoints[x].point += invertedCentroid;
	}
}

void FTriangleUVCalculator::calculateAndApplyQuaternion()
{
	// The axis that the quaternion will rotate on, should be equal to the unit vector of the cross product of
	// the shadowTriangleNormal and anchoringNormal.
	glm::vec3 quatRotationAxis = glm::normalize(cross(shadowTriangleNormal, anchoringNormal));

	// calculate the radians between the shadowTriangleNormal and the anchoringNormal. Do this
	// by getting the dot product of the two unit vectors for shadowTriangleNormal and anchoringNormal
	float quatDot = glm::dot(shadowTriangleNormal, anchoringNormal);
	double quatCos = std::acos(quatDot);

	std::cout << "Shadow triangle normal: " << shadowTriangleNormal.x << ", " << shadowTriangleNormal.y << ", " << shadowTriangleNormal.z << std::endl;
	std::cout << "Anchoring normal: " << anchoringNormal.x << ", " << anchoringNormal.y << ", " << anchoringNormal.z << std::endl;
	std::cout << "Quaternion cosine value is: " << quatCos << std::endl;

	xyQuaternion = glm::quat(cos(quatCos / 2),
		sin(quatCos / 2) * quatRotationAxis.x,
		sin(quatCos / 2) * quatRotationAxis.y,
		sin(quatCos / 2) * quatRotationAxis.z);

	// apply the quaternion to all shadow points
	for (auto& currentShadowPoint : shadowPointVector)
	{
		//currentShadowPoint.fetchShadowPoint()->point *= xyQuaternion;
		//convertDoublePointToVec3
		
		glm::vec3 currentPointToVec3 = FTriangleUtils::convertDoublePointToVec3(currentShadowPoint.fetchShadowPoint()->point);
		glm::vec3 quatAffectedPoint = xyQuaternion * currentPointToVec3;
		DoublePoint convertVec3Back(quatAffectedPoint);
		currentShadowPoint.fetchShadowPoint()->point = convertVec3Back;
	}

	// apply the quaternion to the barycentric basis points.
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 currentPointToVec3 = FTriangleUtils::convertDoublePointToVec3(barycentricBasisPoints[x].point);
		glm::vec3 quatAffectedPoint = xyQuaternion * currentPointToVec3;
		DoublePoint convertVec3Back(quatAffectedPoint);
		barycentricBasisPoints[x].point = convertVec3Back;
	}

	// print the shadow points
	std::cout << "Printing point data, after quaternion application: " << std::endl;
	for (auto& currentShadowPoint : shadowPointVector)
	{
		currentShadowPoint.fetchShadowPoint()->printPointData();
		std::cout << std::endl;

		std::cout << "barycentric coords for point are: " << std::endl;
		glm::vec3 currentBaryCoords = calculateBarycentricCoords(barycentricBasisPoints[0],
															     barycentricBasisPoints[1],
																 barycentricBasisPoints[2],
			                                                     *(currentShadowPoint.fetchShadowPoint()));
		std::cout << currentBaryCoords.x << ", " << currentBaryCoords.y << ", " << currentBaryCoords.z << std::endl;
		glm::vec2 currentUV = getUV(barycentricBasisPoints[0],
									barycentricBasisPoints[1],
									barycentricBasisPoints[2], 
									currentBaryCoords);

		std::cout << "UV value of point is: U-> " << currentUV.x << " | V-> " << currentUV.y << std::endl;

		// lastly, set the UV for the shadow point, and then transfer it back.
		currentShadowPoint.fetchShadowPoint()->fTextureU = currentUV.x;
		currentShadowPoint.fetchShadowPoint()->fTextureV = currentUV.y;

		currentShadowPoint.loadShadowedUVIntoOrigin();
	}

}

glm::vec3 FTriangleUVCalculator::calculateBarycentricCoords(FTrianglePoint in_point0,
	FTrianglePoint in_point1,
	FTrianglePoint in_point2,
	FTrianglePoint in_pointToSolveFor)
{
	// Formula for area = magnitude of the cross product of 0->1 and 0->2 vectors, divided by 2. 

	// area_A = for point A, at index 0 (alpha): 
	// 
	// baryAreaAU = in_point1 - in_pointToSolveFor
	// baryAreaAV = in_point2 - in_pointToSolveFor
	glm::vec3 baryAreaAU = FTriangleUtils::convertDoublePointToVec3(in_point1.point - in_pointToSolveFor.point);
	glm::vec3 baryAreaAV = FTriangleUtils::convertDoublePointToVec3(in_point2.point - in_pointToSolveFor.point);
	glm::vec3 crossedAUAV = glm::cross(baryAreaAU, baryAreaAV);
	float baryAreaA = sqrt(pow(crossedAUAV.x, 2.0) + pow(crossedAUAV.y, 2.0) + pow(crossedAUAV.z, 2.0)) / 2;

	// area_B = for point B, at index 1 (beta):
	//
	// baryAreaBU = in_point0 - in_pointToSolveFor
	// baryAreaBV = in_point2 - in_pointToSolveFor
	glm::vec3 baryAreaBU = FTriangleUtils::convertDoublePointToVec3(in_point0.point - in_pointToSolveFor.point);
	glm::vec3 baryAreaBV = FTriangleUtils::convertDoublePointToVec3(in_point2.point - in_pointToSolveFor.point);
	glm::vec3 crossedBUBV = glm::cross(baryAreaBU, baryAreaBV);
	float baryAreaB = sqrt(pow(crossedBUBV.x, 2.0) + pow(crossedBUBV.y, 2.0) + pow(crossedBUBV.z, 2.0)) / 2;

	// area_C = for point C, at index 2 (gamma):
	//
	// baryAreaGU = in_point0 - in_pointToSolveFor
	// baryAreaGV = in_point1 - in_pointToSolveFor
	glm::vec3 baryAreaGU = FTriangleUtils::convertDoublePointToVec3(in_point0.point - in_pointToSolveFor.point);
	glm::vec3 baryAreaGV = FTriangleUtils::convertDoublePointToVec3(in_point1.point - in_pointToSolveFor.point);
	glm::vec3 crossedGUGV = glm::cross(baryAreaGU, baryAreaGV);
	float baryAreaG = sqrt(pow(crossedGUGV.x, 2.0) + pow(crossedGUGV.y, 2.0) + pow(crossedGUGV.z, 2.0)) / 2;

	float overallArea = baryAreaA + baryAreaB + baryAreaG;
	
	glm::vec3 baryCoords;
	
	baryCoords.x = baryAreaA / overallArea;		// alpha (point 0)
	baryCoords.y = baryAreaB / overallArea;		// beta (point 1)
	baryCoords.z = baryAreaG / overallArea;		// gamma (point 2)

	return baryCoords;
}

glm::vec2 FTriangleUVCalculator::getUV(FTrianglePoint in_point0,
	FTrianglePoint in_point1,
	FTrianglePoint in_point2,
	glm::vec3 in_barycentricCoords)
{
	glm::vec2 uv;

	// Remember, alpha is for point0, beta for point 1, and gamma for point 2. 
	// 
	// I.E, point0 is compared against in_barycentricCoords.x (which is the alpha).
	//
	// So,
	//
	// in_barycentricCoords.x = alpha value
	// in_barycentricCoords.y = beta value
	// in_barycentricCoords.z = gamma value
	uv.x = (in_point0.fTextureU * in_barycentricCoords.x) + (in_point1.fTextureU * in_barycentricCoords.y) + (in_point2.fTextureU * in_barycentricCoords.z);
	uv.y = (in_point0.fTextureV * in_barycentricCoords.x) + (in_point1.fTextureV * in_barycentricCoords.y) + (in_point2.fTextureV * in_barycentricCoords.z);

	return uv;
}