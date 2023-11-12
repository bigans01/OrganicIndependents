#include "stdafx.h"
#include "FTrianglePointUVCalculator.h"

void FTrianglePointUVCalculator::determineLinkedVelocities()
{

	// ||||||||||||||||||||||||||||||||||||||||||| Check for POS_X
	// use a float-keyed map; this will automatically store the pairing that has the greatest distance as the
	// rbegin element.
	std::map<float, int> posXCandidates;

	for (int x = 1; x < 3; x++)
	{
		// the current analyzed point X is greater than point 0. 
		if (candidatePoints[x].point.x > candidatePoints[0].point.x)
		{
			std::cout << "(LVELOCITY_POS_X candidate) Found point at index " << x << " as having X value greater than that at 0" << std::endl;

			float candidateDistance = glm::distance(candidatePoints[x].point.x, candidatePoints[0].point.x);
			posXCandidates[candidateDistance] = x;
		}
	}

	// Get the rbegin in posXCandidates, if there is anything in it.
	if (!posXCandidates.empty())
	{
		int targetCandidateForPosX = posXCandidates.rbegin()->second;

		float calculatedXDiff = candidatePoints[targetCandidateForPosX].point.x - candidatePoints[0].point.x;
		float calculatedUDiff = candidatePoints[targetCandidateForPosX].fTextureU - candidatePoints[0].fTextureU;

		LinkedVelocity newVelocity(calculatedXDiff, calculatedUDiff, targetCandidateForPosX);

		xuVelocities[LinkedVelocityType::LVELOCITY_POS_X] = newVelocity;
	}



	// ||||||||||||||||||||||||||||||||||||||||||| Check for NEG_X
	// use a float-keyed map; this will automatically store the pairing that has the greatest distance as the
	// rbegin element.
	std::map<float, int> negXCandidates;

	for (int x = 1; x < 3; x++)
	{
		// the current analyzed point X is greater than point 0. 
		if (candidatePoints[x].point.x < candidatePoints[0].point.x)
		{
			std::cout << "(LVELOCITY_NEG_X candidate) Found point at index " << x << " as having X value less than that at 0" << std::endl;

			float candidateDistance = glm::distance(candidatePoints[x].point.x, candidatePoints[0].point.x);
			negXCandidates[candidateDistance] = x;
		}
	}

	// Get the rbegin in negXCandidates, if there is anything in it.
	if (!negXCandidates.empty())
	{
		int targetCandidateForNegX = negXCandidates.rbegin()->second;

		float calculatedXDiff = candidatePoints[targetCandidateForNegX].point.x - candidatePoints[0].point.x;
		float calculatedUDiff = candidatePoints[targetCandidateForNegX].fTextureU - candidatePoints[0].fTextureU;

		LinkedVelocity newVelocity(calculatedXDiff, calculatedUDiff, targetCandidateForNegX);

		xuVelocities[LinkedVelocityType::LVELOCITY_NEG_X] = newVelocity;
	}

	// ||||||||||||||||||||||||||||||||||||||||||| Check for POS_Y
	std::map<float, int> posYCandidates;

	for (int x = 1; x < 3; x++)
	{
		// the current analyzed point y is greater than point 0. 
		if (candidatePoints[x].point.y > candidatePoints[0].point.y)
		{
			std::cout << "(LVELOCITY_POS_Y candidate) Found point at index " << x << " as having Y value greater than that at 0" << std::endl;

			float candidateDistance = glm::distance(candidatePoints[x].point.y, candidatePoints[0].point.y);
			posYCandidates[candidateDistance] = x;
		}
	}

	if (!posYCandidates.empty())
	{
		int targetCandidateForPosY = posYCandidates.rbegin()->second;

		float calculatedXDiff = candidatePoints[targetCandidateForPosY].point.y - candidatePoints[0].point.y;
		float calculatedUDiff = candidatePoints[targetCandidateForPosY].fTextureV - candidatePoints[0].fTextureV;

		LinkedVelocity newVelocity(calculatedXDiff, calculatedUDiff, targetCandidateForPosY);

		yvVelocities[LinkedVelocityType::LVELOCITY_POS_Y] = newVelocity;
	}

	// ||||||||||||||||||||||||||||||||||||||||||| Check for NEG_Y
	std::map<float, int> negYCandidates;

	for (int x = 1; x < 3; x++)
	{
		// the current analyzed point y is less than point 0.
		if (candidatePoints[x].point.y < candidatePoints[0].point.y)
		{
			std::cout << "(LVELOCITY_NEG_Y candidate) Found point at index " << x << " as having Y value greater than that at 0" << std::endl;

			float candidateDistance = glm::distance(candidatePoints[x].point.y, candidatePoints[0].point.y);
			posYCandidates[candidateDistance] = x;
		}
	}

	if (!negYCandidates.empty())
	{
		int targetCandidateForPosY = posYCandidates.rbegin()->second;

		float calculatedXDiff = candidatePoints[targetCandidateForPosY].point.y - candidatePoints[0].point.y;
		float calculatedUDiff = candidatePoints[targetCandidateForPosY].fTextureV - candidatePoints[0].fTextureV;

		LinkedVelocity newVelocity(calculatedXDiff, calculatedUDiff, targetCandidateForPosY);

		yvVelocities[LinkedVelocityType::LVELOCITY_NEG_Y] = newVelocity;
	}

	printLinkedVelocities();
}

void FTrianglePointUVCalculator::printLinkedVelocities()
{
	std::cout << "Printing calculated X velocities: " << std::endl;
	for (auto& currentVelocity : xuVelocities)
	{
		switch (currentVelocity.first)
		{
			case LinkedVelocityType::LVELOCITY_NEG_X:
			{
				std::cout << "LVELOCITY_NEG_X: negXVelocity -> " << currentVelocity.second.fTriangleDimVelocity << " | negUVelocity -> " << currentVelocity.second.textureCoordDimVelocity << " | Utilized point: ";
				std::cout << candidatePoints[currentVelocity.second.utilizedPointIndex].point.x << ", " 
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.y << ", " 
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.z << std::endl;
				break;
			};

			case LinkedVelocityType::LVELOCITY_POS_X:
			{
				std::cout << "LVELOCITY_POS_X: posXVelocity -> " << currentVelocity.second.fTriangleDimVelocity << " | posUVelocity -> " << currentVelocity.second.textureCoordDimVelocity << " | Utilized point: ";
				std::cout << candidatePoints[currentVelocity.second.utilizedPointIndex].point.x << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.y << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.z << std::endl;
				break;
			};
		}
	}

	std::cout << "Printing calculated Y velocities: " << std::endl;
	for (auto& currentVelocity : yvVelocities)
	{
		switch (currentVelocity.first)
		{
			case LinkedVelocityType::LVELOCITY_NEG_Y:
			{
				std::cout << "LVELOCITY_NEG_Y: negYVelocity -> " << currentVelocity.second.fTriangleDimVelocity << " | negVVelocity -> " << currentVelocity.second.textureCoordDimVelocity << " | Utilized point: ";
				std::cout << candidatePoints[currentVelocity.second.utilizedPointIndex].point.x << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.y << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.z << std::endl;
				break;
			}

			case LinkedVelocityType::LVELOCITY_POS_Y:
			{
				std::cout << "LVELOCITY_POS_Y: posYVelocity -> " << currentVelocity.second.fTriangleDimVelocity << " | posVVelocity -> " << currentVelocity.second.textureCoordDimVelocity << " | Utilized point: ";
				std::cout << candidatePoints[currentVelocity.second.utilizedPointIndex].point.x << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.y << ", "
					<< candidatePoints[currentVelocity.second.utilizedPointIndex].point.z << std::endl;
				break;
			}
		}
	}
}

void FTrianglePointUVCalculator::calculateUVForPoint(FTrianglePoint* in_pointRef)
{
	FTrianglePoint anchorPoint = candidatePoints[0];

	
	
	// Calculate for U
	std::cout << "+++Calculating for U. " << std::endl;
	float calculatedU = 0.0f;

	float sX_final = 0.0f;
	if (in_pointRef->point.x > anchorPoint.point.x)
	{
		std::cout << "Selecting LVELOCITY_POS_X velocity. " << std::endl;
		LinkedVelocity posXVelocity = xuVelocities[LinkedVelocityType::LVELOCITY_POS_X];
		float diffX = in_pointRef->point.x - anchorPoint.point.x;
		float sX = (posXVelocity.textureCoordDimVelocity * diffX) / posXVelocity.fTriangleDimVelocity;
		sX_final = anchorPoint.fTextureU + sX;
	}

	else if (in_pointRef->point.x < anchorPoint.point.x)
	{
		std::cout << "Selecting LVELOCITY_NEG_X velocity. " << std::endl;
		LinkedVelocity negXVelocity = xuVelocities[LinkedVelocityType::LVELOCITY_NEG_X];
		float diffX = in_pointRef->point.x - anchorPoint.point.x;
		float sX = (negXVelocity.textureCoordDimVelocity * diffX) / negXVelocity.fTriangleDimVelocity;
		sX_final = anchorPoint.fTextureU + sX;
	}
	else
	{
		sX_final = anchorPoint.fTextureU;
	}



	// Calculate for V
	std::cout << "+++calculating for V. " << std::endl;
	float calculatedV = 0.0f;
	
	float sY_final = 0.0f;
	if (in_pointRef->point.y > anchorPoint.point.y)
	{
		std::cout << "Selecting LVELOCITY_POS_Y velocity. " << std::endl;
		LinkedVelocity posYVelocity = yvVelocities[LinkedVelocityType::LVELOCITY_POS_Y];

		std::cout << "Anchor point: "; anchorPoint.point.printPointCoords(); std::cout << std::endl;
		std::cout << "Referenced point: "; in_pointRef->point.printPointCoords(); std::cout << std::endl;



		float diffY = in_pointRef->point.y - anchorPoint.point.y;
		float sY = (posYVelocity.textureCoordDimVelocity * diffY) / posYVelocity.fTriangleDimVelocity;
		sY_final = anchorPoint.fTextureV + sY;
	}

	else if (in_pointRef->point.y < anchorPoint.point.y)
	{
		std::cout << "Selecting LVELOCITY_NEG_Y velocity. " << std::endl;
		LinkedVelocity negYVelocity = yvVelocities[LinkedVelocityType::LVELOCITY_NEG_Y];
		float diffY = in_pointRef->point.y - anchorPoint.point.y;
		float sY = (negYVelocity.textureCoordDimVelocity * diffY) / negYVelocity.fTriangleDimVelocity;
		sY_final = anchorPoint.fTextureV + sY;
	}
	else
	{
		sY_final = anchorPoint.fTextureV;
	}

	std::cout << "--> Calculated UV for point " << in_pointRef->point.x << ", " << in_pointRef->point.y << ", " << in_pointRef->point.z << " is U -> " << sX_final << " | V -> " << sY_final << std::endl;
}