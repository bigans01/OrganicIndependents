#pragma once

#ifndef FTRIANGLEPOINTUVCALCULATOR_H
#define FTRIANGLEPOINTUVCALCULATOR_H

#include "FTrianglePoint.h"

class FTrianglePointUVCalculator
{
	public:
		FTrianglePointUVCalculator(FTrianglePoint in_candidatePoint0,
			FTrianglePoint in_candidatePoint1,
			FTrianglePoint in_candidatePoint2)
		{
			candidatePoints[0] = in_candidatePoint0;
			candidatePoints[1] = in_candidatePoint1;
			candidatePoints[2] = in_candidatePoint2;

			determineLinkedVelocities();
		}

		void calculateUVForPoint(FTrianglePoint* in_pointRef);
	private:
		FTrianglePoint candidatePoints[3];

		enum class LinkedVelocityType
		{
			LVELOCITY_POS_X,	// linked to U
			LVELOCITY_NEG_X,	// ""
			LVELOCITY_POS_Y,	// linked to V
			LVELOCITY_NEG_Y	// ""
		};

		struct LinkedVelocity
		{
			LinkedVelocity() {};
			LinkedVelocity(float in_fTriangleDimVelocity, 
							float in_textureCoordDimVelocity,
							int in_utilizedPointIndex) :
				fTriangleDimVelocity(in_fTriangleDimVelocity),
				textureCoordDimVelocity(in_textureCoordDimVelocity),
				utilizedPointIndex(in_utilizedPointIndex)
			{};

			float fTriangleDimVelocity = 0.0f;
			float textureCoordDimVelocity = 0.0f;
			int utilizedPointIndex = 0;
		};

		std::map<LinkedVelocityType, LinkedVelocity> xuVelocities;
		std::map<LinkedVelocityType, LinkedVelocity> yvVelocities;

		void determineLinkedVelocities();
		void printLinkedVelocities();
};

#endif
